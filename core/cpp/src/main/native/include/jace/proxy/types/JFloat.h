#ifndef JACE_TYPES_JFLOAT_H
#define JACE_TYPES_JFLOAT_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"


BEGIN_NAMESPACE_3(jace, proxy, types)

/**
 * A representation of the java primitive float.
 *
 * @author Toby Reyelts
 */
class JFloat : public JValue
{
public:
    /** Typedefs for this object */
    typedef jfloat      JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new instance with the given value.
	 */
	JFloat(jvalue value);

	/**
	 * Creates a new instance with the given value.
	 */
	JFloat(JNIType value);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JFloat();

	/**
	 * Returns the value of this instance.
	 */
	operator JNIType() const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(const JFloat& value) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator!=(const JFloat& value) const;

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
	static const ::jace::JClass& staticGetJavaJniClass() throw (::jace::JNIException);

	/**
	 * Returns the JClass for this instance.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const throw (::jace::JNIException);
};


END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JFLOAT_H
