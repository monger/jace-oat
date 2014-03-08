#ifndef JACE_JMETHOD_H
#define JACE_JMETHOD_H

#include "jace/Namespace.h"
#include "jace/JClass.h"
#include "jace/proxy/JObject.h"
#include "jace/proxy/JValue.h"
#include "jace/JArguments.h"
#include "jace/JNIException.h"
#include "jace/JSignature.h"
#include "jace/proxy/types/JBoolean.h"
#include "jace/proxy/types/JByte.h"
#include "jace/proxy/types/JChar.h"
#include "jace/proxy/types/JDouble.h"
#include "jace/proxy/types/JFloat.h"
#include "jace/proxy/types/JInt.h"
#include "jace/proxy/types/JLong.h"
#include "jace/proxy/types/JShort.h"
#include "jace/proxy/types/JVoid.h"

#include <jni.h>

#include <string>
#include <vector>
#include <list>
#include <iostream>


BEGIN_NAMESPACE(jace)


/**
 * Transforms a JArguments to a vector of jvalue's.
 */
std::vector<jvalue> toVector(const JArguments& arguments);

/**
 * Represents a java method.
 *
 * @author Toby Reyelts
 */
template <class ResultType> class JMethod
{
public:
	/**
	 * Creates a new JMethod representing the method with the
	 * given name, belonging to the given class.
	 */
	JMethod(const std::string& name): mName(name), mMethodID(0)
	{}

	/**
	 * Invokes the method with the given arguments.
	 * The method is invoked on the supplied object.
	 *
	 * @throws JNIException if an error occurs while trying to invoke the method.
	 * @throws a matching C++ proxy, if a java exception is thrown by the method.
	 */
	ResultType invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
	{
#ifdef JACE_CHECK_NULLS
		if (object.isNull())
			throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
#endif

		// Get the methodID for the method matching the given arguments.
		jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

		// Call the method.
		JNIEnv* env = attach();
		jobject resultRef;

		if (arguments.asList().size() > 0)
			resultRef = env->CallObjectMethodA(object, methodID, &toVector(arguments)[0]);
		else
			resultRef = env->CallObjectMethod(object, methodID);

		// Catch any java exception that occured during the method call, and throw it as a C++ exception.
		catchAndThrow();

		ResultType result(resultRef);
		env->DeleteLocalRef(resultRef);

		return result;
	}


	/**
	 * Invokes the method with the given arguments.
	 * The method is invoked statically, on the supplied class.
	 *
	 * @throws JNIException if an error occurs while trying to invoke the method.
	 * @throws a matching C++ proxy, if a java exception is thrown by the method.
	 */
	ResultType invoke(const JClass& jClass, const JArguments& arguments)
	{
		// Get the methodID for the method matching the given arguments.
		jmethodID methodID = getMethodID(jClass, arguments, true);

		// Call the method.
		JNIEnv* env = attach();
		jobject resultRef;

		if (arguments.asList().size() > 0)
			resultRef = env->CallStaticObjectMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
		else
			resultRef = env->CallStaticObjectMethod(jClass.getClass(), methodID);

		// Catch any java exception that occured during the method call, and throw it as a C++ exception.
		catchAndThrow();

		ResultType result(resultRef);
		env->DeleteLocalRef(resultRef);

		return result;
	}

protected:
	/**
	 * Returns the jmethodID matching the signature for the given arguments.
	 */
	jmethodID getMethodID(const JClass& jClass, const JArguments& arguments, bool isStatic = false)
	{
		// We cache the jmethodID locally, so if we've already found it, we don't need to go looking for it again.
		if (mMethodID)
			return mMethodID;

		// If we don't already have the jmethodID, we need to determine the signature of this method.
		JSignature signature(ResultType::staticGetJavaJniClass());
		typedef std::list<const ::jace::proxy::JValue*> ValueList;
		ValueList args = arguments.asList();

		ValueList::iterator i = args.begin();
		ValueList::iterator end = args.end();

		for (; i != end; ++i)
		{
			const ::jace::proxy::JValue* value = *i;
			signature << value->getJavaJniClass();
		}

		std::string methodSignature = signature.toString();

		// Now that we have the signature for the method, we could look in a global cache for the
		// jmethodID corresponding to this method, but for now, we'll always find it.
		JNIEnv* env = attach();

		if (isStatic)
			mMethodID = env->GetStaticMethodID(jClass.getClass(), mName.c_str(), methodSignature.c_str());
		else
			mMethodID = env->GetMethodID(jClass.getClass(), mName.c_str(), methodSignature.c_str());

		if (mMethodID == 0)
		{
            THROW_JNI_EXCEPTION(std::string("JMethod::getMethodID\n") +
				                            "Unable to find method <" + mName + 
                                            "> with signature <" + methodSignature + ">");
		}

		//  cout << "JMethod::getMethodID() - Found the method:" << endl;
		//  cout << "  <" << mName << "> with signature <" << methodSignature << "> for " << jClass.getInternalName() << endl;

		return mMethodID;
	}

private:
	std::string mName;
	jmethodID mMethodID;
};

template <> inline
::jace::proxy::types::JBoolean JMethod< ::jace::proxy::types::JBoolean >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jboolean result;
  
  if (arguments.asList().size() > 0)
    result = env->CallStaticBooleanMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticBooleanMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JBoolean(result);
}


template <> inline
::jace::proxy::types::JBoolean JMethod< ::jace::proxy::types::JBoolean >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jboolean result;
  
  if (arguments.asList().size() > 0)
	result = env->CallBooleanMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallBooleanMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JBoolean(result);
}


template <> inline
::jace::proxy::types::JByte JMethod< ::jace::proxy::types::JByte >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jbyte result;
  
  if (arguments.asList().size() > 0)
    result = env->CallStaticByteMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticByteMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JByte(result);
}


template <> inline
::jace::proxy::types::JByte JMethod< ::jace::proxy::types::JByte >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jbyte result;
  
  if (arguments.asList().size() > 0)
    result = env->CallByteMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallByteMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JByte(result);
}


template <> inline
::jace::proxy::types::JChar JMethod< ::jace::proxy::types::JChar >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jchar result;
  
  if (arguments.asList().size() > 0)
	result = env->CallStaticCharMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticCharMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JChar(result);
}


template <> inline
::jace::proxy::types::JChar JMethod< ::jace::proxy::types::JChar >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  /* Get the methodID for the method matching the given arguments.
   */
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jchar result;
  
  if (arguments.asList().size() > 0)
    result = env->CallCharMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallCharMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JChar(result);
}


template <> inline
::jace::proxy::types::JDouble JMethod< ::jace::proxy::types::JDouble >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jdouble result;
  
  if (arguments.asList().size() > 0)
    result = env->CallStaticDoubleMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticDoubleMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JDouble(result);
}


template <> inline
::jace::proxy::types::JDouble JMethod< ::jace::proxy::types::JDouble >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jdouble result;
  
  if (arguments.asList().size() > 0)
	result = env->CallDoubleMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallDoubleMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JDouble(result);
}


template <> inline
::jace::proxy::types::JFloat JMethod< ::jace::proxy::types::JFloat >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jfloat result;
  
  if (arguments.asList().size() > 0)
    result = env->CallStaticFloatMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticFloatMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JFloat(result);
}


template <> inline
::jace::proxy::types::JFloat JMethod< ::jace::proxy::types::JFloat >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jfloat result;
  
  if (arguments.asList().size() > 0)
    result = env->CallFloatMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallFloatMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JFloat(result);
}


template <> inline
::jace::proxy::types::JInt JMethod< ::jace::proxy::types::JInt >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jint result;
  
  if (arguments.asList().size() > 0)
	result = env->CallStaticIntMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticIntMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JInt(result);
}


template <> inline
::jace::proxy::types::JInt JMethod< ::jace::proxy::types::JInt >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jint result;
  
  if (arguments.asList().size() > 0)
	result = env->CallIntMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
	result = env->CallIntMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JInt(result);
}


template <> inline
::jace::proxy::types::JLong JMethod< ::jace::proxy::types::JLong >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jlong result;
  
  if (arguments.asList().size() > 0)
	result = env->CallStaticLongMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
	result = env->CallStaticLongMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JLong(result);
}


template <> inline
::jace::proxy::types::JLong JMethod< ::jace::proxy::types::JLong >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jlong result;
  
  if (arguments.asList().size() > 0)
    result = env->CallLongMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallLongMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JLong(result);
}


template <> inline
::jace::proxy::types::JShort JMethod< ::jace::proxy::types::JShort >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  jshort result;
  
  if (arguments.asList().size() > 0)
    result = env->CallStaticShortMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    result = env->CallStaticShortMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JShort(result);
}


template <> inline
::jace::proxy::types::JShort JMethod< ::jace::proxy::types::JShort >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();
  jshort result;
  
  if (arguments.asList().size() > 0)
    result = env->CallShortMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
    result = env->CallShortMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JShort(result);
}


template <> inline
::jace::proxy::types::JVoid JMethod< ::jace::proxy::types::JVoid >::invoke(const JClass& jClass, const JArguments& arguments)
{
  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(jClass, arguments, true);

  // Call the method.
  JNIEnv* env = attach();
  
  if (arguments.asList().size() > 0)
    env->CallStaticVoidMethodA(jClass.getClass(), methodID, &toVector(arguments)[0]);
  else
    env->CallStaticVoidMethod(jClass.getClass(), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception. 
  catchAndThrow();

  return ::jace::proxy::types::JVoid();
}

template <> inline
::jace::proxy::types::JVoid JMethod< ::jace::proxy::types::JVoid >::invoke(const ::jace::proxy::JObject& object, const JArguments& arguments)
{
  #ifdef JACE_CHECK_NULLS
    if (object.isNull())
      throw JNIException("[JMethod.invoke] Can not invoke an instance method on a null object.");
  #endif

  // Get the methodID for the method matching the given arguments.
  jmethodID methodID = getMethodID(object.getJavaJniClass(), arguments);

  // Call the method.
  JNIEnv* env = attach();

  if (arguments.asList().size() > 0)
	env->CallVoidMethodA(static_cast<jobject>(object), methodID, &toVector(arguments)[0]);
  else
	env->CallVoidMethod(static_cast<jobject>(object), methodID);

  // Catch any java exception that occured during the method call,
  // and throw it as a C++ exception.
  catchAndThrow();
  
  return ::jace::proxy::types::JVoid();
}

END_NAMESPACE(jace)

#endif // #ifndef JACE_JMETHOD_H
