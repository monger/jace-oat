#ifndef JACE_TYPES_JLONG_H
#define JACE_TYPES_JLONG_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"
#include "jace/proxy/types/JInt.h"


BEGIN_NAMESPACE_3(jace, proxy, types)
class JInt;

/**
 * A representation of the java primitive long.
 *
 * @author Toby Reyelts
 */
class JLong: public JValue
{
public:
    /** Typedefs for this object */
    typedef jlong       JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new instance with the given value.
	 */
	JLong(jvalue value);

	/**
	 * Creates a new instance with the given value.
	 */
	JLong(JNIType _long);

	/**
	 * Creates a new instance with the given value.
	 */
	JLong(const ::jace::proxy::types::JInt& _int);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JLong();

	/**
	 * Returns the value of this instance.
	 */
	operator JNIType() const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(const JLong& _long) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator!=(const JLong& _long) const;


	/**
	 * Compares this instance to a primitive.
	 */
	bool operator==(JNIType val) const;

	/**
	 * Compares this instance to a primitive.
	 */
	bool operator!=(JNIType val) const;

	/**
	 * Returns the JClass for this class.
	 */
	static const ::jace::JClass& staticGetJavaJniClass() throw (::jace::JNIException);

	/**
	 * Retrieves the JavaClass for this JObject.
	 *
	 * @throw JNIException if an error occurs while trying to retrieve the class.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const throw (::jace::JNIException);
};


END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JLONG_H
