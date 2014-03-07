#ifndef JACE_TYPES_JCHAR_H
#define JACE_TYPES_JCHAR_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"

#include <iostream>

BEGIN_NAMESPACE_3(jace, proxy, types)


/**
 * A representation of the java primitive char.
 *
 * @author Toby Reyelts
 */
class JChar : public JValue
{
public:
    /** Typedefs for this object */
    typedef jchar       JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new JChar with the given value.
	 */
	JChar(jvalue value);

	/**
	 * Creates a new JChar with the given value.
	 */
	JChar(JNIType _char);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JChar();

	/**
	 * Returns the char value of this java char.
	 */
	operator JNIType() const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(const JChar& _char) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator!=(const JChar& _char) const;

	/**
	 * Compares this instance to another.
	 */
	bool operator==(JNIType val) const;

	/**
	 * Compares this instance to another.
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

	/**
	 * Support printing of characters.
	 */
	friend std::ostream& operator<<(std::ostream& stream, const JChar& val);
};

END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JCHAR_H
