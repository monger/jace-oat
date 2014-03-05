#include "jace/runtime/NativeProxy.h"

#include <string>
using std::string;

#include "jace/proxy/types/JVoid.h"
#include "jace/JArguments.h"
using jace::JArguments;
#include "jace/JMethod.h"
using jace::JMethod;

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, runtime, NativeProxy)

/** Whether or not we have been registered, and our mutex around that */
bool registered = false;
boost::mutex regMtx;
typedef boost::unique_lock<boost::mutex> auto_lock;

string unwrapException(const string& msg) {
    try { jace::catchAndThrow(); }
    catch (std::exception& e) { return msg + "\ncaused by:\n" + e.what(); }
    return msg;
}
#define THROW_JNI_EXCEPTION(m) throw JNIException(unwrapException(m))

/**
 * Invoked by org.jace.util.NativeInvocation.
 */
static jobject native_invokeNative(JNIEnv* env, jclass cls, jobject obj, jlong ref, jobjectArray args) {
    return ((Builder::Fx) ref)(env, cls, obj, args);
}

/**
 * Registers the native callback hook if it hasn't been registered.
 */
static void registerInvokeNativeHook() throw (JNIException)
{
    auto_lock guard(regMtx);
    if (registered) { return; }
    
    JNIEnv* env = attach();
    jclass hookClass = env->FindClass("org/jace/util/NativeInvocation");
    if (!hookClass) { 
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, org.jace.util.NativeInvocation.");
    }

    jobject(*pf)(JNIEnv*, jclass, jobject, jlong, jobjectArray) = native_invokeNative;
    JNINativeMethod methods[] = { 
        { (char*) "invokeNative", 
          (char*) "(Ljava/lang/Object;J[Ljava/lang/Object;)Ljava/lang/Object;", 
          *(void**)(&pf) } 
    };
    int methods_size = sizeof(methods) / sizeof(methods[0]);
    if (env->RegisterNatives(hookClass, methods, methods_size) != JNI_OK) {
		env->DeleteLocalRef(hookClass);
        THROW_JNI_EXCEPTION("Unable to register native callback for invokeNative().");
    }
    registered = true;
    env->DeleteLocalRef(hookClass);
}

/**
 * Implementation of our builder
 */
Builder::Builder(const std::string& className) throw (JNIException) {
    /* Register our hook */
    registerInvokeNativeHook();
    JNIEnv* env = attach();
    jclass instClass = env->FindClass("org/jace/util/NativeInvocation");
    if (!instClass) {
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, org.jace.util.NativeInvocation.");
    }
    
	m_registerCallbackMethod = env->GetMethodID(instClass, "registerNative", "(Ljava/lang/String;J)V");
	if (!m_registerCallbackMethod) {
		env->DeleteLocalRef(instClass);
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, NativeInvocation.registerNative().");
	}
    
    m_createProxyMethod = env->GetMethodID(instClass, "createProxy", "()Ljava/lang/Object;");
	if (!m_createProxyMethod) {
		env->DeleteLocalRef(instClass);
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, NativeInvocation.createProxy().");
	}
    
    jmethodID constructor = env->GetMethodID(instClass, "<init>", "(Ljava/lang/String;)V");
	if (!constructor) {
		env->DeleteLocalRef(instClass);
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the constructor, NativeInvocation().");
	}
    
    jstring javaString = env->NewStringUTF(className.c_str());
    if (!javaString) {
		env->DeleteLocalRef(instClass);
        THROW_JNI_EXCEPTION("Assert failed: Error creating java string.");
    }
    
    jobject instance = env->NewObject(instClass, constructor, javaString);
    env->DeleteLocalRef(javaString);
    if (!instance) {
		env->DeleteLocalRef(instClass);
        THROW_JNI_EXCEPTION("Assert failed: Error instantiating object.");
    }
    
    m_instance = newGlobalRef(env, instance);
    env->DeleteLocalRef(instance);
    m_classRef = static_cast<jclass>(newGlobalRef(env, instClass));
    env->DeleteLocalRef(instClass);
}
Builder::~Builder() {
    JNIEnv* env = attach();
    deleteGlobalRef(env, m_classRef);
    deleteGlobalRef(env, m_instance);
}

void Builder::registerCallback(const std::string& name, const Fx callback) {
    intptr_t ptr = (intptr_t) callback;
    JNIEnv* env = attach();
    
    jstring javaString = env->NewStringUTF(name.c_str());
    if (!javaString) {
        THROW_JNI_EXCEPTION("Assert failed: Error creating java string.");
    }
    
    env->CallVoidMethod(m_instance, m_registerCallbackMethod, javaString, (jlong) ptr);
    try {
        jace::catchAndThrow();
	} catch (std::exception& e) {
    	env->DeleteLocalRef(javaString);
		string msg = "Exception thrown invoking NativeInvocation.registerNative()\n";
		msg.append("caused by:\n");
		msg.append(e.what());
		throw JNIException(msg);
	}
    
    env->DeleteLocalRef(javaString);
}

::jace::proxy::JObject Builder::instantiate() {
    JNIEnv* env = attach();
    jobject obj = env->CallObjectMethod(m_instance, m_createProxyMethod);
    if (!obj) {
        THROW_JNI_EXCEPTION("Exception thrown invoking NativeInvocation.createProxy()");
    }

    ::jace::proxy::JObject returnVal(obj);
    env->DeleteLocalRef(obj);    
    return returnVal;
}

END_NAMESPACE_3(jace, runtime, ShutdownHook)
