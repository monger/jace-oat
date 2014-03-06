#ifndef JACE_RUNTIME_NATIVEPROXY_H
#define JACE_RUNTIME_NATIVEPROXY_H

#include "jace/OsDep.h"
#include "jace/Namespace.h"
#include "jace/JNIException.h"

#include "jace/Jace.h"
#include "jace/JArray.h"
#include "jace/JClassImpl.h"
#include "jace/proxy/JValue.h"
#include "jace/proxy/JObject.h"
#include "jace/proxy/types/JBoolean.h"
#include "jace/proxy/types/JByte.h"
#include "jace/proxy/types/JChar.h"
#include "jace/proxy/types/JDouble.h"
#include "jace/proxy/types/JFloat.h"
#include "jace/proxy/types/JInt.h"
#include "jace/proxy/types/JLong.h"
#include "jace/proxy/types/JShort.h"
#include "jace/proxy/types/JVoid.h"

#include <string>
#include <vector>
#include <jni.h>

#include <boost/utility.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

/**
 * Native Proxy registration
 */

BEGIN_NAMESPACE_3(jace, runtime, NativeProxy)

typedef ::jace::JArray< ::jace::proxy::JObject> CallbackArgs;

#define JACE_NATIVE_CALLBACK_ARGS ::jace::proxy::JObject obj, ::jace::runtime::NativeProxy::CallbackArgs args

class Callback { 
public:
    typedef boost::function< jobject (JACE_NATIVE_CALLBACK_ARGS) > Fx;
    
    Callback(const Fx& fx) : m_fx(fx) {}
    template <typename R> Callback(R(*fx)(JACE_NATIVE_CALLBACK_ARGS)) : m_fx(fx) {}
    
    /* void (and JVoid) functions */

    /* Functions that return native types */
    #define _JACE_CONSTRUCTOR(t, f)                                             \
    Callback(t(*fx)(JACE_NATIVE_CALLBACK_ARGS)) : m_fx(boost::bind(f, fx, _1, _2)) {}

    /* Void types */
    _JACE_CONSTRUCTOR(void, wrapVoid)
    _JACE_CONSTRUCTOR(::jace::proxy::types::JVoid, wrapVoid)
    
    /* Native, jni, and JValue types */
    #define _JACE_TYPE_CONSTRUCTOR(w)                                           \
    _JACE_CONSTRUCTOR(w, wrap<w>)                                               \
    _JACE_CONSTRUCTOR(w::JNIType, wrap<w>)

    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JBoolean)
    _JACE_CONSTRUCTOR(bool, wrap< ::jace::proxy::types::JBoolean >)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JByte)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JChar)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JDouble)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JFloat)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JInt)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JLong)
    _JACE_TYPE_CONSTRUCTOR(::jace::proxy::types::JShort)
        
    #undef _JACE_TYPE_CONSTRUCTOR
    #undef _JACE_CONSTRUCTOR
    
    jobject operator()(JNIEnv*, jclass, jobject obj, jobjectArray args) const { 
        return m_fx(::jace::proxy::JObject(obj), CallbackArgs(args));
    }

    /* Copy and default constructor and assignment */
    Callback(const Callback& other) : m_fx(other.m_fx) {}
    Callback& operator=(const Callback& rhs) { m_fx = rhs.m_fx; return *this; }
    
private:
    static jobject wrapVoid(const boost::function<void (JACE_NATIVE_CALLBACK_ARGS)>& fx, 
                                           JACE_NATIVE_CALLBACK_ARGS) {
        try {
            fx(obj, args);
        } catch (std::exception& e) {
            ::jace::java_throw("java/lang/RuntimeException", 
                               std::string("Exception during native execution: ") + e.what());
        } catch (...) {
            ::jace::java_throw("java/lang/RuntimeException", "Unknown exception during native execution");
        }
        return 0;
    }
    
    template <typename JaceType>
    static jobject wrap(const boost::function<JaceType (JACE_NATIVE_CALLBACK_ARGS)>& fx, 
                                       JACE_NATIVE_CALLBACK_ARGS) {
        try {
            return ::jace::java_box<JaceType>(fx(obj, args));
        } catch (std::exception& e) {
            ::jace::java_throw("java/lang/RuntimeException", 
                               std::string("Exception during native execution: ") + e.what());
        } catch (...) {
            ::jace::java_throw("java/lang/RuntimeException", "Unknown exception during native execution");
        }
        return 0;
    }
        
private:
    Fx  m_fx;
};

class Builder : public boost::noncopyable {
public:
    Builder(const std::string& className) throw (JNIException);
    ~Builder();
    void registerCallback(const std::string& name, const Callback& callback);
    template <typename T> T build() throw (JNIException) {
        ::jace::proxy::JObject obj = instantiate();
        if (::jace::instanceof<T>(obj)) {
            return ::jace::java_cast<T>(obj);
        }
        std::string msg = "Proxy not instance of " + T::staticGetJavaJniClass().getInternalName();
        throw JNIException(msg);
    }
    const Callback& get(int idx) const { return m_callbacks[idx]; }
    
private:
    ::jace::proxy::JObject instantiate();
    
    /* Member variables */
    jclass                  m_classRef;
    jobject                 m_instance;
    jmethodID               m_registerCallbackMethod;
    jmethodID               m_createProxyMethod;
    
    std::vector<Callback>   m_callbacks;
};

END_NAMESPACE_3(jace, runtime, NativeProxy)

#endif
