#ifndef JACE_TYPES_JINT_H
#define JACE_TYPES_JINT_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"
#include "jace/proxy/types/JByte.h"

#include <iostream>


BEGIN_NAMESPACE_3(jace, proxy, types)
class JByte;

/**
 * A representation of the java primitive int.
 *
 * @author Toby Reyelts
 */
class JInt: public JValue
{
public:
    /** Typedefs for this object */
    typedef jint        JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new instance with the given value.
	 */
	JInt(jvalue value);

	/**
	 * Creates a new instance with the given value.
	 */
	JInt(const JNIType _int);

	/**
	 * Creates a new instance with the given value.
	 */
	JInt(const ::jace::proxy::types::JByte& _byte);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JInt();

	/**
	 * Returns the value of this instance.
	 */
	operator JNIType() const;

	/**
	 * Compares this JInt to another.
	 */
	bool operator==(const JInt& _int) const;

	/**
	 * Compares this JInt to another.
	 */
	bool operator!=(const JInt& _int) const;

	/**
	 * Compares this JInt to a JNIType.
	 */
	bool operator==(JNIType val) const;

	/**
	 * Compares this JInt to a JNIType.
	 */
	bool operator!=(JNIType val) const;

	/**
	 * Returns the JClass for this class.
	 */
	static const ::jace::JClass& staticGetJavaJniClass();

	/**
	 * Retrieves the JavaClass for this JObject.
	 *
	 * @throw JNIException if an error occurs while trying to retrieve the class.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const;

	friend std::ostream& operator<<(std::ostream& stream, const JInt& val);
};


END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JINT_H
