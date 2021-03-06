#ifndef JACE_JACE_H
#define JACE_JACE_H

#include "jace/Namespace.h"
#include "jace/JNIException.h"
#include "jace/VirtualMachineRunningError.h"
#include "jace/VirtualMachineShutdownError.h"
#include "jace/JFactory.h"
#include "jace/VmLoader.h"
#include "jace/OptionList.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/proxy/JObject.h"

#include <sstream>

#include <boost/shared_ptr.hpp>

#include <jni.h>

/**
 * Jace helper functions.
 *
 * @author Toby Reyelts
 * @author Gili Tzabari
 */

BEGIN_NAMESPACE(jace)

typedef boost::shared_ptr<VmLoader> Loader;

/**
 * Creates a new Java Virtual Machine using the specified loader
 * with the specified options.
 *
 * To link with a virtual machine you may specify any dynamic loading
 * VmLoader (for example, UnixVmLoader or Win32VmLoader).
 *
 * This call results in a call to setVmLoader internally.
 * @throws VirtualMachineRunningError if the virtual machine is already running
 * @throws JNIException if the virtual machine cannot be created
 */
void createJavaVm(Loader loader,
                  const OptionList& options) /* throw (VirtualMachineRunningError, JNIException) */;

/**
 * Sets the current running java virtual machine. This method can be used to implement a custom vm
 * loading policy outside of createVm.
 *
 * @param jvm a running java virtual machine
 * @param jniVersion the desired version of the jvm (defaults DEFAULT_JNI_VERSION)
 * @throws VirtualMachineRunningError if a JVM is already running
 * @throws JNIException if an error occurs while registering the shutdown hook
 */
void setJavaVm(JavaVM* jvm, 
               jint jniVersion = DEFAULT_JNI_VERSION) /* throw (VirtualMachineRunningError, JNIException) */;

/**
 * Resets the current running java virtual machine.  This should be called to free JVM resources if you are never
 * going to call JNI stuff again.  It is automatically called when the process exits, so it is not required
 * to call it directly.
 */
void resetJavaVm();

/**
 * Returns the current java virtual machine.
 *
 * @return null if no virtual machine is running
 */
JavaVM* getJavaVm();

/**
 * Attaches the current thread to the virtual machine and returns the appropriate
 * JNIEnv for the thread. If the thread is already attached, this method method
 * does nothing.
 *
 * This method is equivilent to attach(0, 0, false).
 *
 * @throws JNIException if an error occurs while trying to attach the current thread.
 * @see AttachCurrentThread
 * @see attach(const jobject, const char*, const bool)
 * @throws JNIException if an error occurs while attaching the current thread
 * @throws VirtualMachineShutdownError if the virtual machine is not running
 */
JNIEnv* attach() /* throw (VirtualMachineShutdownError, JNIException) */;

/**
 * Detaches the current thread from the virtual machine.
 *
 * @see DetachCurrentThread
 */
void detach();

/**
 * A central point for allocating new local references.
 * These references must be deallocated by a call to deleteLocalRef.
 *
 * @throws JNIException if the local reference can not be allocated.
 */
jobject newLocalRef(jobject ref) /* throw (VirtualMachineShutdownError, JNIException) */;

/**
 * A central point for deleting local references.
 */
void deleteLocalRef(jobject localRef);


/**
 * A central point for allocating new global references.
 * These references must be deallocated by a call to deleteGlobalRef.
 *
 * @throws VirtualMachineShutdownError if the virtual machine is not running
 * @throws JNIException if the global reference can not be allocated.
 */
jobject newGlobalRef(jobject ref) /* throw (VirtualMachineShutdownError, JNIException)*/;

/**
 * A central point for deleting global references.
 */
void deleteGlobalRef(jobject globalRef);


/**
 * Enlists a new factory for a java class with Jace.
 *
 * All java classes should enlist with Jace on start-up.
 * They can do this by adding a static member variable
 * of type JEnlister to their class definition.
 *
 * For example, java::lang::Object has a static member variable,
 *
 *   static JEnlister<Object> enlister;
 *
 * which is all that is required to register a new factory
 * for itself.
 */
void enlist(JFactory* factory);


/**
 * Checks to see if a java exception has been thrown.
 *
 * If an exception has been thrown, a corresponding C++ proxy
 * exception is constructed and thrown.
 */
void catchAndThrow();

/** Modifies the given exception to include the underlying exception, if there is one.  Returns true if there was one */
inline bool messageException(std::string& msg) {
    try { 
        jace::catchAndThrow();
        return false;
    }
    catch (std::exception& e) { 
        msg.append("\ncaused by:\n");
        msg.append(e.what());
        return true;
    }
    
}
inline std::string unwrapException(const std::string& msg) {
    std::string r(msg);
    messageException(r);
    return r;
}
#define THROW_JNI_EXCEPTION(m) throw JNIException(unwrapException(m))

/**
 * Returns the string representation of any type.
 */
template <class T> std::string toString(T value) {
	std::stringstream stream;
	stream << value;
	return stream.str();
}

/**
 * Returns the result of calling Object.toString() on obj.
 * Useful for low level debugging.
 */
std::string toString(jobject obj);

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>();
 */
template <typename T>
T java_new() {
	return T::Factory::create();
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(42);
 */
template <typename T, typename A0>
T java_new(A0 a0) {
	return T::Factory::create(a0);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1);
 */
template <typename T, typename A0, typename A1>
T java_new(A0 a0, A1 a1) {
	return T::Factory::create(a0, a1);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2);
 */
template <typename T, typename A0, typename A1, typename A2>
T java_new(A0 a0, A1 a1, A2 a2) {
	return T::Factory::create(a0, a1, a2);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3) {
	return T::Factory::create(a0, a1, a2, a3);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
	return T::Factory::create(a0, a1, a2, a3, a4);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4, a5);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
	return T::Factory::create(a0, a1, a2, a3, a4, a5);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4, a5, a6);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5,
          typename A6>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
	return T::Factory::create(a0, a1, a2, a3, a4, a5, a6);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4, a5, a6, a7);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5,
          typename A6, typename A7>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
	return T::Factory::create(a0, a1, a2, a3, a4, a5, a6, a7);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4, a5, a6, a7, a8);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5,
          typename A6, typename A7, typename A8>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
	return T::Factory::create(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

/**
 * Instantiate a new Java object.
 *
 * For example,
 *
 *  Map map = java_new<HashMap>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5,
          typename A6, typename A7, typename A8, typename A9>
T java_new(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
	return T::Factory::create(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

/**
 * Instantiate a new String.
 *
 * For example,
 *
 *  String map = java_new<String>("Hello");
 *
 * The compiler can't figure out how to go from a char* to JValue on its own.
 */
template <typename T>
T java_new(const char* text) {
	return T::Factory::create(text);
}

/**
 * Instantiate a new String.
 *
 * For example,
 *
 *  String map = java_new<String>(::std::string("Hello"));
 *
 * The compiler can't figure out how to go from a char* to JValue on its own.
 */
template <typename T>
T java_new(const ::std::string& text) {
	return T::Factory::create(text);
}

/**
 * Performs a safe cast from one JObject subclass to another.
 *
 * For example,
 *
 *  Object stringAsObject = String("Hello");
 *  String string = java_cast<String>(stringAsObject);
 *
 * @throws JNIException if obj is not convertible to type T.
 */
template <typename T> 
T java_cast(const ::jace::proxy::JObject& obj) {
	JNIEnv* env = attach();
	jclass argClass = env->GetObjectClass(obj);

	if (!argClass)
	{
		std::string msg = "[java_cast] Failed to retrieve the class type for obj.";
		throw JNIException(msg);
	}

	const ::jace::JClass& resultClass = T::staticGetJavaJniClass();

	bool isValid = env->IsAssignableFrom(argClass, resultClass.getClass());
	env->DeleteLocalRef(argClass), argClass = 0;

	if (isValid)
		return T(static_cast<jobject>(obj));

	std::string msg = "Can not cast to " + resultClass.getInternalName();
	throw JNIException(msg);
}

/** Throws a java exception with the given message */
void java_throw(const std::string& internalName, const std::string& message);

template <typename T> void java_throw(const std::string& message) {
    java_throw(T::staticGetJavaJniClass().getInternalName(), message);
}

/**
 * Boxes a value as an object.  T should be one of the jace value types (JInt, JBoolean, etc)
 */
template <typename T> 
jobject java_box(T val) {
    std::string className = std::string("java/lang/") + T::ClassName;
    JNIEnv* env = attach();
    jclass boxClass = env->FindClass(className.c_str());
    if (!boxClass) {
        THROW_JNI_EXCEPTION("Could not find class " + className);
    }
    
    std::string sig = std::string("(") + T::staticGetJavaJniClass().getSignature() + ")L" + className + ";";
    jmethodID valueOf = env->GetStaticMethodID(boxClass, "valueOf", sig.c_str());
    if (!valueOf) {
        env->DeleteLocalRef(boxClass), boxClass = 0;
        THROW_JNI_EXCEPTION("Could not get valueOf function");
    }
    
    jobject ret = env->CallStaticObjectMethod(boxClass, valueOf, static_cast<typename T::JNIType>(val));
    std::string msg = "Exception thrown invoking valueOf()\n";
    if (messageException(msg)) {
    	env->DeleteLocalRef(boxClass), boxClass = 0;
		throw JNIException(msg);
    }
	env->DeleteLocalRef(boxClass), boxClass = 0;
    return ret;
}


/**
 * Equal to Java's instanceof keyword.
 * Returns true if obj is non-null and can be cast to type T.
 *
 * For example,
 *
 *  Object stringAsObject = String("Hello");
 *
 *  if (instanceof<String>(stringAsObject))
 *    String str = java_cast<String>(stringAsObject);
 *
 *
 * @throws JNIException if obj is not convertible to type T.
 */
template <typename T> 
bool instanceof(const ::jace::proxy::JObject& object) {
	if (object.isNull())
		return false;

	JNIEnv* env = attach();
	jclass argClass = env->GetObjectClass(object);

	if (!argClass)
	{
		std::string msg = "[instanceof] Failed to retrieve the dynamic class type for object.";
		throw JNIException(msg);
	}

	const ::jace::JClass& resultClass = T::staticGetJavaJniClass();

	bool isValid = env->IsAssignableFrom(argClass, resultClass.getClass());
	env->DeleteLocalRef(argClass), argClass = 0;

	return isValid;
}

END_NAMESPACE(jace)

#endif
