#ifndef JACE_JCLASS_IMPL_H
#define JACE_JCLASS_IMPL_H

#include "jace/Namespace.h"
#include "jace/JClass.h"
#include "jace/JNIException.h"

#include <boost/thread/mutex.hpp>

#include <string>

BEGIN_NAMESPACE(jace)


/**
 * The implementation of the JClass interface.
 *
 * @author Toby Reyelts
 */
class JClassImpl: public ::jace::JClass
{
public:
	/**
	 * Creates a new JClassImpl with the given name, and
	 * type name.
	 *
	 * @param internalName the internal name of the class. such as "java/lang/Object".
	 *   The internal name of a class is its fully qualified name, as returned by Class.getName(),
	 *   where '.' is replaced by '/'.
	 *
	 * @param signature the class type signature, such as "Ljava/lang/Object;".
	 *   For more information, see: http://download.oracle.com/javase/6/docs/technotes/guides/jni/spec/types.html#wp16432
	 */
	JClassImpl(const std::string& internalName, const std::string& signature);

	/**
	 * Creates a new JClassImpl with the given name.
	 *
	 * @param internalName the internal name of the class. such as "java/lang/Object".
	 *   The internal name of a class is its fully qualified name, as returned by Class.getName(),
	 *   where '.' is replaced by '/'.
	 *
	 *
	 * The signature for the class is created by prepending
	 * "L" and appending ";" to name.
	 *
	 * For example,
	 *
	 *  JClassImpl("java/lang/String");
	 *
	 * is equivalent to
	 *
	 *  JClassImpl("java/lang/String", "Ljava/lang/String;");
	 */
	JClassImpl(const std::string& internalName);

	/**
	 * Destroys this JClassImpl.
	 */
	virtual ~JClassImpl() throw ();

	/**
	 * Returns the internal name of the class. such as "java/lang/Object".
	 *   The internal name of a class is its fully qualified name, as returned by Class.getName(),
	 *   where '.' is replaced by '/'.
	 */
	virtual const std::string& getInternalName() const;

	/**
	 * Returns the class type signature, such as "Ljava/lang/Object;"
	 */
	virtual const std::string& getSignature() const;

	/**
	 * Returns the JNI representation of this class.
	 */
	virtual jclass getClass() const;

private:
	/**
	 * Prevent copying.
	 */
	JClassImpl(JClassImpl&);
	/**
	 * Prevent assignment.
	 */
	JClassImpl& operator=(JClassImpl&);
	std::string internalName;
	std::string signature;
	mutable jclass theClass;
	mutable boost::mutex mutex;
};


END_NAMESPACE(jace)

#endif
