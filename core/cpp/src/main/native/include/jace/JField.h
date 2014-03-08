#ifndef JACE_JFIELD_H
#define JACE_JFIELD_H

#include "jace/Namespace.h"
#include "jace/JClass.h"
#include "jace/proxy/JObject.h"
#include "jace/JNIException.h"
#include "jace/JFieldProxy.h"
#include "jace/JFieldHelper.h"
#include "jace/proxy/types/JBoolean.h"
#include "jace/proxy/types/JByte.h"
#include "jace/proxy/types/JChar.h"
#include "jace/proxy/types/JDouble.h"
#include "jace/proxy/types/JFloat.h"
#include "jace/proxy/types/JInt.h"
#include "jace/proxy/types/JLong.h"
#include "jace/proxy/types/JShort.h"

#include <jni.h>

#include <string>

BEGIN_NAMESPACE(jace)

/**
 * Represents a java field.
 *
 * @author Toby Reyelts
 */
template <class Type> class JField
{
public:
	/**
	 * Creates a new JField representing the field with the
	 * given name.
	 */
	JField(const std::string& name): helper(name, Type::staticGetJavaJniClass())
	{
	}

	/**
	 * Retrieves the field belonging to the given object.
	 *
	 * @throws JNIException if an error occurs while trying to retrieve the field.
	 */
	JFieldProxy<Type> get(::jace::proxy::JObject& object)
	{
		jvalue value = helper.getField(object);
		JFieldProxy<Type> fieldProxy(helper.getFieldID(), value, object);
		deleteLocalRef(value.l), value.l = 0;
		return fieldProxy;
	}


	/**
	 * Retrieves the value of the static field belonging to the given class.
	 *
	 * @throws JNIException if an error occurs while trying to retrieve the value.
	 */
	JFieldProxy<Type> get(const ::jace::JClass& jClass)
	{
		jvalue value = helper.getField(jClass);
		JFieldProxy<Type> fieldProxy(helper.getFieldID(), value, jClass.getClass());
		deleteLocalRef(value.l), value.l = 0;
		return fieldProxy;
	}

private:
	::jace::JFieldHelper helper;

	jfieldID getFieldID(const ::jace::JClass& parentClass, bool isStatic = false)
	{
		return helper.getFieldID(parentClass, isStatic);
	}
};

/**
 * Contains the definitions for the template specializations of the template class, JField.
 *
 * This file is internal to the JACE library.
 */

template <> inline
JFieldProxy< ::jace::proxy::types::JBoolean > JField< ::jace::proxy::types::JBoolean >::get(::jace::proxy::JObject& object)
{ 
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jboolean result = env->GetBooleanField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.z = result;

  return ::jace::JFieldProxy< ::jace::proxy::types::JBoolean >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JBoolean > JField< ::jace::proxy::types::JBoolean >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jboolean result = env->GetStaticBooleanField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.z = result;

  return JFieldProxy< ::jace::proxy::types::JBoolean >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JByte > JField< ::jace::proxy::types::JByte >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jbyte result = env->GetByteField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.b = result;

  return JFieldProxy< ::jace::proxy::types::JByte >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JByte > JField< ::jace::proxy::types::JByte >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jbyte result = env->GetStaticByteField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.b = result;

  return JFieldProxy< ::jace::proxy::types::JByte >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JChar > JField< ::jace::proxy::types::JChar >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jchar result = env->GetCharField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.c = result;

  return JFieldProxy< ::jace::proxy::types::JChar >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JChar > JField< ::jace::proxy::types::JChar >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jchar result = env->GetStaticCharField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.c = result;

  return JFieldProxy< ::jace::proxy::types::JChar >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JShort > JField< ::jace::proxy::types::JShort >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jshort result = env->GetShortField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.s = result;

  return JFieldProxy< ::jace::proxy::types::JShort >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JShort > JField< ::jace::proxy::types::JShort >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jshort result = env->GetStaticShortField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.s = result;

  return JFieldProxy< ::jace::proxy::types::JShort >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JInt > JField< ::jace::proxy::types::JInt >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jint result = env->GetIntField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.i = result;

  return JFieldProxy< ::jace::proxy::types::JInt >(fieldID, value, static_cast<jobject>(object));
}

template <> inline
JFieldProxy< ::jace::proxy::types::JInt > JField< ::jace::proxy::types::JInt >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jint result = env->GetStaticIntField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.i = result;

  return JFieldProxy< ::jace::proxy::types::JInt >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JLong > JField< ::jace::proxy::types::JLong >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jlong result = env->GetLongField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.j = result;

  return JFieldProxy< ::jace::proxy::types::JLong >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JLong > JField< ::jace::proxy::types::JLong >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jlong result = env->GetStaticLongField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.j = result;

  return JFieldProxy< ::jace::proxy::types::JLong >(fieldID, value, jClass.getClass());
}



template <> inline
JFieldProxy< ::jace::proxy::types::JFloat > JField< ::jace::proxy::types::JFloat >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jfloat result = env->GetFloatField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.f = result;

  return JFieldProxy< ::jace::proxy::types::JFloat >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JFloat > JField< ::jace::proxy::types::JFloat >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jfloat result = env->GetStaticFloatField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.f = result;

  return JFieldProxy< ::jace::proxy::types::JFloat >(fieldID, value, jClass.getClass());
}


template <> inline
JFieldProxy< ::jace::proxy::types::JDouble > JField< ::jace::proxy::types::JDouble >::get(::jace::proxy::JObject& object)
{
  // Get the fieldID for the field belonging to the given object.
  const JClass& parentClass = object.getJavaJniClass();
  jfieldID fieldID = getFieldID(parentClass);

  // Get the field value.
  JNIEnv* env = attach();
  jdouble result = env->GetDoubleField(static_cast<jobject>(object), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.d = result;

  return JFieldProxy< ::jace::proxy::types::JDouble >(fieldID, value, static_cast<jobject>(object));
}


template <> inline
JFieldProxy< ::jace::proxy::types::JDouble > JField< ::jace::proxy::types::JDouble >::get(const JClass& jClass)
{
  // Get the fieldID for the field belonging to the given class.
  jfieldID fieldID = getFieldID(jClass, true);

  // Get the field value.
  JNIEnv* env = attach();
  jdouble result = env->GetStaticDoubleField(jClass.getClass(), fieldID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();

  jvalue value;
  value.d = result;

  return JFieldProxy< ::jace::proxy::types::JDouble >(fieldID, value, jClass.getClass());
}

END_NAMESPACE(jace)

#endif
