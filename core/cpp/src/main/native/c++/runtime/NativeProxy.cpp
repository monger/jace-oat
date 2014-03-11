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

/**
 * Invoked by org.jace.util.NativeInvocation.
 */
static jobject native_invokeNative(JNIEnv* env, jclass cls, jlong ref, jint idx, jobject obj, jobjectArray args) {
    Builder*    pBuilder = (Builder*) ref;
    return pBuilder->get(idx)(env, cls, obj, args);
}

/**
 * Registers the native callback hook if it hasn't been registered.
 */
static void registerInvokeNativeHook() {
    auto_lock guard(regMtx);
    if (registered) { return; }
    
    JNIEnv* env = attach();
    jclass hookClass = env->FindClass("org/jace/util/NativeInvocation");
    if (!hookClass) { 
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, org.jace.util.NativeInvocation.");
    }

    jobject(*pf)(JNIEnv*, jclass, jlong, jint, jobject, jobjectArray) = native_invokeNative;
    JNINativeMethod methods[] = { 
        { (char*) "invokeNative", 
          (char*) "(JILjava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;", 
          *(void**)(&pf) } 
    };
    int methods_size = sizeof(methods) / sizeof(methods[0]);
    if (env->RegisterNatives(hookClass, methods, methods_size) != JNI_OK) {
		env->DeleteLocalRef(hookClass), hookClass = 0;
        THROW_JNI_EXCEPTION("Unable to register native callback for invokeNative().");
    }
    registered = true;
    env->DeleteLocalRef(hookClass), hookClass = 0;
}

/**
 * Implementation of our builder
 */
Builder::Builder(const std::string& className) {
    /* Register our hook */
    registerInvokeNativeHook();
    JNIEnv* env = attach();
    jclass instClass = env->FindClass("org/jace/util/NativeInvocation");
    if (!instClass) {
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, org.jace.util.NativeInvocation.");
    }
    
	m_registerCallbackMethod = env->GetMethodID(instClass, "registerNative", "(Ljava/lang/String;JI)V");
	if (!m_registerCallbackMethod) {
		env->DeleteLocalRef(instClass), instClass = 0;
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, NativeInvocation.registerNative().");
	}
    
    m_createProxyMethod = env->GetMethodID(instClass, "createProxy", "()Ljava/lang/Object;");
	if (!m_createProxyMethod) {
		env->DeleteLocalRef(instClass), instClass = 0;
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, NativeInvocation.createProxy().");
	}
    
    jmethodID constructor = env->GetMethodID(instClass, "<init>", "(Ljava/lang/String;)V");
	if (!constructor) {
		env->DeleteLocalRef(instClass), instClass = 0;
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the constructor, NativeInvocation().");
	}
    
    jstring javaString = env->NewStringUTF(className.c_str());
    if (!javaString) {
		env->DeleteLocalRef(instClass), instClass = 0;
        THROW_JNI_EXCEPTION("Assert failed: Error creating java string.");
    }
    
    jobject instance = env->NewObject(instClass, constructor, javaString);
    env->DeleteLocalRef(javaString), javaString = 0;
    if (!instance) {
		env->DeleteLocalRef(instClass), instClass = 0;
        THROW_JNI_EXCEPTION("Assert failed: Error instantiating object.");
    }
    
    m_instance = env->NewGlobalRef(instance);
    env->DeleteLocalRef(instance), instance = 0;
    m_classRef = static_cast<jclass>(env->NewGlobalRef(instClass));
    env->DeleteLocalRef(instClass), instClass = 0;
}
Builder::~Builder() {
    deleteGlobalRef(m_classRef), m_classRef = 0;
    deleteGlobalRef(m_instance), m_instance = 0;
}

void Builder::registerCallback(const std::string& name, const Callback& callback) {
    JNIEnv* env = attach();
    
    jstring javaString = env->NewStringUTF(name.c_str());
    if (!javaString) {
        THROW_JNI_EXCEPTION("Assert failed: Error creating java string.");
    }
    
    env->CallVoidMethod(m_instance, 
                        m_registerCallbackMethod, 
                        javaString, 
                        (jlong) ((intptr_t) this), 
                        (jint) m_callbacks.size());
    string msg = "Exception thrown invoking NativeInvocation.registerNative()\n";
    if (messageException(msg)) {
    	env->DeleteLocalRef(javaString), javaString = 0;
        throw JNIException(msg);
    }
    m_callbacks.push_back(callback);
    env->DeleteLocalRef(javaString), javaString = 0;
}

::jace::proxy::JObject Builder::instantiate() {
    JNIEnv* env = attach();
    jobject obj = env->CallObjectMethod(m_instance, m_createProxyMethod);
    if (!obj) {
        THROW_JNI_EXCEPTION("Exception thrown invoking NativeInvocation.createProxy()");
    }

    ::jace::proxy::JObject returnVal(obj);
    env->DeleteLocalRef(obj), obj = 0;
    return returnVal;
}

END_NAMESPACE_3(jace, runtime, NativeProxy)
