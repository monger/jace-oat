#ifndef JACE_RUNTIME_NATIVEPROXY_H
#define JACE_RUNTIME_NATIVEPROXY_H

#include "jace/OsDep.h"
#include "jace/Namespace.h"
#include "jace/JNIException.h"

#include "jace/Jace.h"
#include "jace/proxy/JValue.h"
#include "jace/proxy/JObject.h"
#include "jace/JClassImpl.h"

#include <string>

#include <jni.h>

/**
 * Native Proxy registration
 */

BEGIN_NAMESPACE_3(jace, runtime, NativeProxy)

class Builder {
public:
    /** Function typedef for invocation functions */
    typedef jobject(*Fx)(JNIEnv*, jclass, jobject, jobjectArray);

    Builder(const std::string& className) throw (JNIException);
    ~Builder();
    void registerCallback(const std::string& name, const Fx callback);
    template <typename T> T build() throw (JNIException) {
        ::jace::proxy::JObject obj = instantiate();
        if (::jace::instanceof<T>(obj)) {
            return ::jace::java_cast<T>(obj);
        }
        std::string msg = "Proxy not instance of " + T::staticGetJavaJniClass().getInternalName();
        throw JNIException(msg);
    }
    
private:
    ::jace::proxy::JObject instantiate();
    
    /* Member variables */
    jclass      m_classRef;
    jobject     m_instance;
    jmethodID   m_registerCallbackMethod;
    jmethodID   m_createProxyMethod;
};

END_NAMESPACE_3(jace, runtime, NativeProxy)

#endif
