#include "jace/Jace.h"
using jace::JFactory;
using jace::VmLoader;
using jace::VirtualMachineShutdownError;
using jace::VirtualMachineRunningError;
using jace::proxy::JObject;
using jace::proxy::JValue;

#include <cstdarg>
#include <stdlib.h>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <algorithm>
using std::copy;
using std::replace;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <string>
using std::string;

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>

BEGIN_NAMESPACE(jace)

// A reference to the java virtual machine.
// We're under the assumption that there will always only be one of these.
JavaVM* jvm = 0;
jint jniVersion = 0;
boost::thread::id mainThreadId;

/**
 * Synchronizes access to "jvm" and "jniVersion" variables.
 */
boost::shared_mutex jvmMtx;
typedef boost::shared_lock<boost::shared_mutex>             auto_read_lock;
typedef boost::unique_lock<boost::shared_mutex>             auto_write_lock;
typedef boost::upgrade_lock<boost::shared_mutex>            auto_upgrade_lock;
typedef boost::upgrade_to_unique_lock<boost::shared_mutex>  auto_upgrade_unique_lock;

enum Daemon {
	/** Attach as non-daemon */
	NON_DAEMON,
	/** Attach as daemon */
	DAEMON,
	/** Attach as daemon if this is not the main thread */
	AUTO
};

/* The map of all of the java class factories. */
typedef map<string,JFactory*> FactoryMap;
FactoryMap* getFactoryMap() {
	static FactoryMap factoryMap;
	return &factoryMap;
}

/* A helper function to get a character string from a java string */
std::string asString(JNIEnv* env, jstring str) {
	const char* utfString = env->GetStringUTFChars(str, 0);
	if (!utfString) {
		std::string msg = "Unable to retrieve the character string for an exception message.";
		throw JNIException(msg);
	}
	std::string stdString = utfString;
	env->ReleaseStringUTFChars(str, utfString);
	return stdString;
}

/* Automatically-detaching pointer */
void threadDetacher(JNIEnv**) { detach(); }
boost::thread_specific_ptr<JNIEnv*> attachedJni(threadDetacher);
/**
 * Attaches the current thread to the virtual machine and returns the appropriate
 * JNIEnv for the thread. If the thread is already attached, this method method
 * does nothing.
 *
 * PRECONDITION: jvm is not null, jniVersion is not 0, and jvmMtx is read-locked
 *
 * @param jvm the java virtual machine to attach the thread to
 * @param jniVersion the version of the vm that we want to attach
 * @param daemon true if the thread should be attached as a daemon thread
 * @throws JNIException if an error occurs while trying to attach the current thread.
 * @see AttachCurrentThread
 * @see AttachCurrentThreadAsDaemon
 */
JNIEnv* attachImpl(JavaVM* jvm, jint jniVersion, const Daemon daemon) /* throw (JNIException) */ {
	JNIEnv* env;
	if (jvm->GetEnv((void**) &env, jniVersion) == JNI_OK)
	{
		// Already attached
		return env;
	}

	JavaVMAttachArgs args;
	args.version = jniVersion;
	string temp("NativeThread-");
	temp += toString(boost::this_thread::get_id());
#ifdef __ANDROID__
#define _JACE_ENV_CAST
    args.name = temp.c_str();
#else
#define _JACE_ENV_CAST (void**)
    args.name = new char[temp.length() + 1];
    strcpy(args.name, temp.c_str());
#endif

	args.group = 0;
	jint result;
	if (daemon == NON_DAEMON || 
        (daemon == AUTO && (mainThreadId == boost::thread::id() || mainThreadId == boost::this_thread::get_id()))) {
		result = jvm->AttachCurrentThread(_JACE_ENV_CAST &env, &args);
	} else {
		result = jvm->AttachCurrentThreadAsDaemon(_JACE_ENV_CAST &env, &args);
    }
    if (mainThreadId != boost::this_thread::get_id()) {
        /* Attach our env to this thread so that we can auto-detach */
        attachedJni.reset(&env);
    }

#ifndef __ANDROID__
    delete[] args.name;
#endif
#undef _JACE_ENV_CAST
    
	if (result != 0)
	{
		string msg = string("Jace::attach\n") +
			"Unable to attach the current thread. The specific JNI error code is " +
			toString(result);
		throw JNIException(msg);
	}
	return env;
}

/**
 * Allows createVm() and setJavaVm() to share code without recursive mutexes.
 *
 * PRECONDITION: _jvm is not null, _jniVersion is not 0, and jvmMtx is locked with the upgrade lock
 */
void setJavaVmImpl(JavaVM* _jvm, jint _jniVersion, auto_upgrade_lock& upgradeLock) {
	assert(_jvm != 0);
	JNIEnv* env = attachImpl(_jvm, _jniVersion, NON_DAEMON);
    
    {
        auto_upgrade_unique_lock writeLock(upgradeLock);
	    jvm = _jvm;
	    jniVersion = env->GetVersion();
        mainThreadId = boost::this_thread::get_id();
    }
}

/** Global storage for our loader and options - so that we make sure and keep them around until we are done */
Loader  g_loader;
bool    g_created = false;
/** Implementation of createVm */
void createJavaVm(Loader loader,
                  const OptionList& options) {
    g_loader = loader;

	JavaVM* jvm;
	JNIEnv* env;
	JavaVMInitArgs vm_args;
	JavaVMOption* jniOptions = options.createJniOptions();

	vm_args.version = g_loader->getJniVersion();
	vm_args.options = jniOptions;
	vm_args.nOptions = jint(options.size());
	vm_args.ignoreUnrecognized = true;

    {
        auto_upgrade_lock upgradeLock(jvmMtx);
    	jint rc = g_loader->createJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
    	options.destroyJniOptions(jniOptions);

    	if (rc != 0) {
    		string msg = "Unable to create the virtual machine. The error was " + toString(rc);
    		throw JNIException(msg);
    	}
    	setJavaVmImpl(jvm, vm_args.version, upgradeLock);
        g_created = true;
    }
}

/** Implementation of setJavaVm */
void setJavaVm(JavaVM* _jvm, jint _jniVersion) {
	if (_jvm == 0) {
		throw new JNIException("jvm may not be null");
    }
    if (_jniVersion == 0) {
        throw new JNIException("jniVersion may not be 0");
    }
    {
        auto_upgrade_lock upgradeLock(jvmMtx);
	    if (jvm != 0 || jniVersion != 0) {
		    throw VirtualMachineRunningError("The virtual machine is already running");
        }
	    setJavaVmImpl(_jvm, _jniVersion, upgradeLock);
    }
}

/** Implementation of resetJavaVm() */
void resetJavaVm() {
	auto_upgrade_lock upgradeLock(jvmMtx);
    if (jvm == 0) {
        // JVM already shut down
        return;
    }
    if (g_created) {
    	jint jniVersionBeforeShutdown = jniVersion;
    	JavaVM* jvmBeforeShutdown = jvm;
        {
            auto_upgrade_unique_lock writeLock(upgradeLock);
            jvm = 0;
            jniVersion = 0;
            mainThreadId = boost::thread::id();
        }

    	// DestroyJavaVM()'s return value is only reliable under JDK 1.6 or newer; older versions always
    	// return failure.
    	jint result = jvmBeforeShutdown->DestroyJavaVM();
    	if (jniVersionBeforeShutdown >= JNI_VERSION_1_6 && result != JNI_OK) {
    		throw JNIException("DestroyJavaVM() returned " + toString(result));
        }
    } else {
        auto_upgrade_unique_lock writeLock(upgradeLock);
        jvm = 0;
        jniVersion = 0;
        mainThreadId = boost::thread::id();
    }
    /* And reset the loader so things get cleaned up */
    g_loader.reset();
}

JavaVM* getJavaVm() { return jvm; }

/** Implementation of attach() */
JNIEnv* attach() {
    auto_read_lock readLock(jvmMtx);
	if (jvm == 0 || jniVersion == 0 || mainThreadId == boost::thread::id()) {
		throw VirtualMachineShutdownError("The virtual machine is shut down");
    }
	return attachImpl(jvm, jniVersion, AUTO);
}

/** Implementation of detach() */
void detach() {
    auto_read_lock readLock(jvmMtx);
	if (jvm == 0) {
		// The JVM is already shut down
		return;
	}
	jvm->DetachCurrentThread();
    /* Release instead of reset so we don't keep getting called */
    attachedJni.release();
}

/** Implementation of newLocalRef() */
jobject newLocalRef(jobject ref) {
    JNIEnv* env = attach();
	jobject localRef = env->NewLocalRef(ref);
	if (!localRef) {
		throw JNIException(string("Jace::newLocalRef\n") +
			                      "Unable to create a new local reference.\n" +
			                      "It is likely that you have exceeded the maximum local reference count.\n" +
			                      "You can increase the maximum count with a call to EnsureLocalCapacity().");
	}
	return localRef;
}

/** Implementation of deleteLocalRef() */
void deleteLocalRef(jobject localRef) {
    try {
        JNIEnv* env = attach();
    	env->DeleteLocalRef(localRef);
    } catch (...) {}
}

/** Implementation of newGlobalRef() */
jobject newGlobalRef(jobject ref) {
    JNIEnv* env = attach();
	jobject globalRef = env->NewGlobalRef(ref);
	if (!globalRef) {
		throw JNIException(string("Jace::newGlobalRef\n") +
			                      "Unable to create a new global reference.\n" +
			                      "It is likely that you have exceeded the max heap size of your virtual machine.");
	}
	return globalRef;
}

/** Implementation of deleteGlobalRef() */
void deleteGlobalRef(jobject globalRef) {
    try {
        JNIEnv* env = attach();
	    env->DeleteGlobalRef(globalRef);
    } catch (...) {}
}

/** Implementation of enlist() */
void enlist(JFactory* factory) {
	string name = factory->getClass().getInternalName();
	replace(name.begin(), name.end(), '/', '.');
	getFactoryMap()->insert(FactoryMap::value_type(name, factory));
}

/** Implementation of catchAndThrow() */
void catchAndThrow() {
    JNIEnv* env = attach();
	if (!env->ExceptionCheck()) {
        /* No exception */
		return;
    }

	jthrowable jexception = env->ExceptionOccurred();
	env->ExceptionClear();

	// Find the fully qualified name for the exception type, so
	// we can find a matching C++ proxy exception.
	//
	// In java, this looks like:
	//   String typeName = exception.getClass().getName();
	jclass throwableClass = env->FindClass("java/lang/Throwable");
	if (!throwableClass) {
		string msg = "Assert failed: Unable to find the class, java.lang.Throwable.";
		throw JNIException(msg);
	}

	jclass classClass = env->FindClass("java/lang/Class");
	if (!classClass) {
		string msg = "Assert failed: Unable to find the class, java.lang.Class.";
		throw JNIException(msg);
	}

	jmethodID throwableGetClass = env->GetMethodID(throwableClass, "getClass", "()Ljava/lang/Class;");
	if (!throwableGetClass) {
		string msg = "Assert failed: Unable to find the method, Throwable.getClass().";
		throw JNIException(msg);
	}

	env->DeleteLocalRef(throwableClass);
	jmethodID classGetName = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
	if (!classGetName) {
		string msg = "Assert failed: Unable to find the method, Class.getName().";
		throw JNIException(msg);
	}

	jmethodID classGetSuperclass = env->GetMethodID(classClass, "getSuperclass", "()Ljava/lang/Class;");
	if (!classGetSuperclass) {
		string msg = "Assert failed: Unable to find the method, Class.getSuperclass().";
		throw JNIException(msg);
	}

	env->DeleteLocalRef(classClass);
	jobject exceptionClass = env->CallObjectMethod(jexception, throwableGetClass);
	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
		string msg = string("jace::catchAndThrow()\n") +
			                "An error occurred while trying to call getClass() on the thrown exception.";
		throw JNIException(msg);
	}

	jstring exceptionType = static_cast<jstring>(env->CallObjectMethod(exceptionClass, classGetName));
	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
		string msg = string("jace::catchAndThrow()\n") +
			                "An error occurred while trying to call getName() on the class of the thrown exception.";
		throw JNIException(msg);
	}

	string exceptionTypeString = asString(env, exceptionType);

	// Now, find the matching factory for this exception type.
	while (true) {
		FactoryMap::iterator it = getFactoryMap()->find(exceptionTypeString);

		// If we couldn't find a match, try to find the parent exception type.
		if (it == getFactoryMap()->end())
		{
			jobject superClass = env->CallObjectMethod(exceptionClass, classGetSuperclass);

			if (env->ExceptionOccurred()) {
				env->ExceptionDescribe();
				string msg = string("jace::catchAndThrow()\n") +
					                "An error occurred while trying to call getSuperclass() on the thrown exception.";
				throw JNIException(msg);
			}

			// We get NULL if we've already reached java.lang.Object, in which case,
			// we couldn't find any match at all.
			if (!superClass) {
				break;
            }

			env->DeleteLocalRef(exceptionClass);
			env->DeleteLocalRef(exceptionType);
			exceptionClass = superClass;

			exceptionType = static_cast<jstring>(env->CallObjectMethod(exceptionClass, classGetName));

			if (env->ExceptionOccurred()) {
				env->ExceptionDescribe();
				throw JNIException("jace::catchAndThrow()\nAn error occurred while trying to call "
					               "getName() on the superclass of the thrown exception.");
			}

			exceptionTypeString = asString(env, exceptionType);
			if (exceptionTypeString == "java.lang.Object") {
				// Couldn't find a matching exception. Abort!
				break;
			}
			continue;
		}

		// Ask the factory to throw the exception.
		jvalue value;
		value.l = jexception;
		it->second->throwInstance(value);
	}

	exceptionClass = env->CallObjectMethod(jexception, throwableGetClass);

	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
		string msg = string("jace::catchAndThrow()\n") +
			                "An error occurred while trying to call getClass() on the thrown exception.";
		throw JNIException(msg);
	}

	exceptionType = static_cast<jstring>(env->CallObjectMethod(exceptionClass, classGetName));

	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
		string msg = string("jace::catchAndThrow()\n") +
			                "An error occurred while trying to call getName() on the class of the thrown exception.";
		throw JNIException(msg);
	}

	exceptionTypeString = asString(env, exceptionType);
	string msg = string("Can't find any linked in parent exception for ") + exceptionTypeString + "\n";
	throw JNIException(msg);
}

/** Implementation of toString() */
string toString(jobject obj) {
	JNIEnv* env = attach();
	jclass objectClass = env->FindClass("java/lang/Object");

	if (!objectClass) {
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, java.lang.Object.");
	}

	jmethodID toString = env->GetMethodID(objectClass, "toString", "()Ljava/lang/String;");
	if (!toString) {
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, Object.toString().");
	}

	jstring javaStr = static_cast<jstring>(env->CallObjectMethod(obj, toString));
	const char* strBuf = env->GetStringUTFChars(javaStr, 0);
	string value = string(strBuf);

	env->ReleaseStringUTFChars(javaStr, strBuf);

	env->DeleteLocalRef(javaStr);
	env->DeleteLocalRef(objectClass);

	return value;
}

/** Implementation of java_throw() */
void java_throw(const std::string& internalName, const std::string& message) {
    JNIEnv* env = attach();
    jclass exClass = env->FindClass(internalName.c_str());
    if (!exClass) {
        THROW_JNI_EXCEPTION("Could not find class " + internalName);
    }
    env->ThrowNew(exClass, message.c_str());
    env->DeleteLocalRef(exClass);
}


END_NAMESPACE(jace)
