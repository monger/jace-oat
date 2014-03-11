#ifndef JACE_TYPES_JVOID
#define JACE_TYPES_JVOID

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/JClass.h"
#include "jace/proxy/JValue.h"
#include "jace/JNIException.h"


BEGIN_NAMESPACE_3(jace, proxy, types)

/**
 * A class representing the java primitive type, void.
 *
 * @author Toby Reyelts
 */
class JVoid : public JValue
{
public:
	/**
	 * Returns the JClass for the Void type.
	 *
	 * @throw JNIException if an error occurs while trying to retrieve the class.
	 */
	virtual const ::jace::JClass& getJavaJniClass() const;

	/**
	 * Returns the JClass for the Void type.
	 *
	 * @throw JNIException if an error occurs while trying to retrieve the class.
	 */
	static const ::jace::JClass& staticGetJavaJniClass();
};

END_NAMESPACE_3(jace, proxy, types)

#endif // #ifndef JACE_TYPES_JVOID
