#ifndef JACE_JVALUE_H
#define JACE_JVALUE_H

#include "jace/Namespace.h"
#include "jace/JClass.h"
#include "jace/JNIException.h"


BEGIN_NAMESPACE_2(jace, proxy)

/**
 * The base class for all java values.
 *
 * All java values are represented by the JValue class. For example,
 * the java primitive, int, is represented by the class, JInt, which
 * derives directly from JValue. The java object, java.lang.String,
 * is represented by the class, String, which has JValue at the
 * top of its inheritance hierarchy.
 *
 * Classes which derive from JValue need to abide by the following set of rules:
 *
 * - All JValues must be constructable from a JNI jvalue.
 *   For example: JInt::JInt(jvalue value);
 *
 * - All JValues must implement the methods:
 *
 *   static const JClass& staticGetJavaJniClass() and
 *   const JClass& getJavaJniClass() const
 *
 *   staticGetJavaJniClass() must return the same value as getJavaJniClass().
 *   For example, Object implements staticGetJavaJniClass() and getJavaJniClass()
 *   in a preferred fashion:
 *
 *
 *   static boost::mutex javaClassMutex;
 *   const JClass& Object::staticGetJavaJniClass() {
 *     static boost::shared_ptr<JClassImpl> result;
 *     boost::mutex::scoped_lock lock(javaClassMutex);
 *     if (result == 0)
 * 	     result = boost::shared_ptr<JClassImpl>(new JClassImpl("java/lang/Object"));
 *     return *result;
 *   }
 *
 *   const JClass& Object::getJavaJniClass() const {
 *     return Object::staticGetJavaJniClass();
 *   }
 *
 * @author Toby Reyelts
 * @author Gili Tzabari
 */
class JValue
{
public:
	/**
	 * Constructs a new JValue.
	 */
	JValue();

	/**
	 * Destroys the existing JValue.
	 */
	virtual ~JValue();

	/**
	 * Returns the underlying JNI jvalue for this JValue.
	 */
	operator jvalue();

	/**
	 * Returns the underlying JNI jvalue for this JValue.
	 *
	 * Callers of this method should be careful not to call modifying
	 * methods on the returned jvalue.
	 */
	operator jvalue() const;

	/**
	 * Retrieves the JClass for this JValue.
	 *
	 * @throw JNIException if an error occurs while trying to retrieve the class.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const = 0;

protected:
	/**
	 * Sets the jvalue for this JValue.
	 *
	 * This method should only be called once during the lifetime
	 * of this JValue, during the construction of a JValue.
	 *
	 * @param value The jvalue which represents this JValue.
	 *
	 * @throws JNIException if the jobject has already been set,
	 *   or if the JVM runs out of memory while trying to create
	 *   a new global reference.
	 */
	virtual void setJavaJniValue(jvalue value);

private:
	/**
	 * The underlying JNI value.
	 */
	jvalue mValue;
};


END_NAMESPACE_2(jace, proxy)

#endif
