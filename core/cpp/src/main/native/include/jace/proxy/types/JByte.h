#ifndef JACE_TYPES_JBYTE_H
#define JACE_TYPES_JBYTE_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"


BEGIN_NAMESPACE_3(jace, proxy, types)

/**
 * A representation of the java primitive byte.
 *
 * @author Toby Reyelts
 */
class JByte : public ::jace::proxy::JValue
{
public:
    /** Typedefs for this object */
    typedef jbyte       JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new JByte with the given value.
	 */
	JByte(jvalue value);

	/**
	 * Creates a new JByte with the given value.
	 */
	JByte(JNIType byte);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JByte();

	/**
	 * Returns the byte value of this java byte.
	 */
	operator JNIType() const;

	/**
	 * Compares this JByte to another.
	 */
	bool operator==(const JByte& _byte) const;

	/**
	 * Compares this JByte to another.
	 */
	bool operator!=(const JByte& _byte) const;


	/**
	 * Compares this JByte to a JNIType.
	 */
	bool operator==(JNIType val) const;

	/**
	 * Compares this JByte to a JNIType.
	 */
	bool operator!=(JNIType val) const;

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

#endif // #ifndef JACE_TYPES_JBYTE_H

