#include "jace/proxy/types/JDouble.h"

#include "jace/JClassImpl.h"

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, proxy, types)


JDouble::JDouble(jvalue value)
{
  setJavaJniValue(value);
}

JDouble::JDouble(JNIType _double)
{
  jvalue value;
  value.d = _double;
  setJavaJniValue(value);
}

JDouble::~JDouble()
{}

JDouble::operator JNIType() const
{
  return static_cast<jvalue>(*this).d;
}

bool JDouble::operator==(const JDouble& _double) const
{
  return static_cast<JNIType>(_double) == static_cast<JNIType>(*this);
}

bool JDouble::operator!=(const JDouble& _double) const
{
  return !(*this == _double);
}

bool JDouble::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JDouble::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JDouble::staticGetJavaJniClass() throw (JNIException)
{
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("double", "D"));
	return *result;
}

const JClass& JDouble::getJavaJniClass() const throw (JNIException)
{
  return JDouble::staticGetJavaJniClass();
}

const char* JDouble::ClassName = "Double";

END_NAMESPACE_3(jace, proxy, types)
