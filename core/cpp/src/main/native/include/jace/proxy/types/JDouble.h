#ifndef JACE_TYPES_JDOUBLE_H
#define JACE_TYPES_JDOUBLE_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"


BEGIN_NAMESPACE_3(jace, proxy, types)

/**
 * A representation of the java primitive double.
 *
 * @author Toby Reyelts
 */
class JDouble : public JValue
{
public:
    /** Typedefs for this object */
    typedef jdouble     JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new JDouble with the given value.
	 */
	JDouble(jvalue value);

	/**
	 * Creates a new JDouble with the given value.
	 */
	JDouble(JNIType value);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JDouble();

	/**
	 * Returns the value of this instance.
	 */
	operator JNIType() const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(const JDouble& value) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator!=(const JDouble& value) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(JNIType value) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator!=(JNIType value) const;

	/**
	 * Returns the JClass for this class.
	 */
	static const ::jace::JClass& staticGetJavaJniClass();

	/**
	 * Returns the JClass for this instance.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const;
};


END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JDOUBLE_H
