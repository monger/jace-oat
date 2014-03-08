#ifndef JACE_TYPES_JBOOLEAN_H
#define JACE_TYPES_JBOOLEAN_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"

BEGIN_NAMESPACE_1(jace)
template <class T> class ElementProxy;
template <class T> class JFieldProxy;
template <class T> class JField;
END_NAMESPACE_1(jace)

BEGIN_NAMESPACE_3(jace, proxy, types)


/**
 * A representation of the java primitive boolean.
 *
 * @author Toby Reyelts
 */
class JBoolean: public ::jace::proxy::JValue
{
public:
    /** Typedefs for this object */
    typedef jboolean    JNIType;
    static const char*  ClassName;

	/**
	 * Creates a new instance with the given value.
	 */
	JBoolean(jvalue value);

	/**
	 * Creates a new instance with the given value.
	 */
	JBoolean(JNIType value);

	/**
	 * Destroys the existing java object.
	 */
	virtual ~JBoolean();

	/**
	 * Returns the value of this instance.
	 */
	operator JNIType() const;

	/**
	 * Compares this JBoolean to another.
	 */
	bool operator==(const JBoolean& _boolean) const;

	/**
	 * Compares this JBoolean to another.
	 */
	bool operator!=(const JBoolean& _boolean) const;

	/**
	 * Compares this JBoolean to a JNIType.
	 */
	bool operator==(JNIType val) const;

	/**
	 * Compares this JBoolean to a JNIType.
	 */
	bool operator!=(JNIType val) const;

	/**
	 * Returns the JClass for this class.
	 */
	static const ::jace::JClass& staticGetJavaJniClass();

	/**
	 * Returns the JClass for this instance.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const;

	friend class ::jace::ElementProxy<JBoolean>;
	friend class ::jace::JFieldProxy<JBoolean>;
	friend class ::jace::JField<JBoolean>;
};


END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JBOOLEAN_H

