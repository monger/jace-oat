#include "jace/proxy/types/JFloat.h"

#include "jace/JClassImpl.h"

#include "jace/BoostWarningOff.h"
#include <boost/thread/mutex.hpp>
#include "jace/BoostWarningOff.h"

BEGIN_NAMESPACE_3(jace, proxy, types)


JFloat::JFloat(jvalue value)
{
  setJavaJniValue(value);
}

JFloat::JFloat(JNIType _float)
{
  jvalue value;
  value.f = _float;
  setJavaJniValue(value);
}

JFloat::~JFloat()
{}

JFloat::operator JNIType() const
{
  return static_cast<jvalue>(*this).f;
}

bool JFloat::operator==(const JFloat& _float) const
{
  return static_cast<JNIType>(_float) == static_cast<JNIType>(*this);
}

bool JFloat::operator!=(const JFloat& _float) const
{
  return !(*this == _float);
}

bool JFloat::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JFloat::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JFloat::staticGetJavaJniClass() throw (JNIException)
{
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("float", "F"));
	return *result;
}

const JClass& JFloat::getJavaJniClass() const throw (JNIException)
{
  return JFloat::staticGetJavaJniClass();
}

const char* JFloat::ClassName = "Float";

END_NAMESPACE_3(jace, proxy, types)

