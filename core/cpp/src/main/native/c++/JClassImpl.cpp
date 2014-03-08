#include "jace/JClassImpl.h"

#include "jace/Jace.h"

using std::string;

BEGIN_NAMESPACE(jace)

JClassImpl::JClassImpl(const string& _internalName, const string& _signature):
    internalName(_internalName), signature(_signature), theClass(0) {}


JClassImpl::JClassImpl(const string& _internalName): 
    internalName(_internalName), signature("L" + internalName + ";"), theClass(0) {}
	
/**
 * Destroys this JClassImpl.
 */
JClassImpl::~JClassImpl() throw ()
{
	if (theClass) {
		deleteGlobalRef(theClass);
    }
}


const string& JClassImpl::getInternalName() const
{
  return internalName;
}

const string& JClassImpl::getSignature() const
{
  return signature;
}

/**
 * Returns the JNI representation of this class.
 */
jclass JClassImpl::getClass() const {
	boost::mutex::scoped_lock lock(mutex);
	if (theClass == 0)
	{
		JNIEnv* env = attach();

		jclass localClass = env->FindClass(getInternalName().c_str());

		if (!localClass) {
            THROW_JNI_EXCEPTION(string("JClass::getClass - Unable to find the class <") + getInternalName() + ">");
		}

		theClass = static_cast<jclass>(env->NewGlobalRef(localClass));
		env->DeleteLocalRef(localClass);
	}
	return theClass;
}

END_NAMESPACE(jace)
