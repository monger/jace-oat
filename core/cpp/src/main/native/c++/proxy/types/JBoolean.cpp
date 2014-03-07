#include "jace/proxy/types/JBoolean.h"

#include "jace/JClassImpl.h"

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, proxy, types)

JBoolean::JBoolean(jvalue value)
{
  setJavaJniValue(value);
}

JBoolean::JBoolean(JNIType _boolean)
{
  jvalue value;
  value.z = _boolean;
  setJavaJniValue(value);
}

JBoolean::~JBoolean()
{}

JBoolean::operator JNIType() const
{ 
  return static_cast<jvalue>(*this).z;
}

bool JBoolean::operator==(const JBoolean& _boolean) const
{
  return static_cast<JNIType>(_boolean) == static_cast<JNIType>(*this);
}

bool JBoolean::operator!=(const JBoolean& _boolean) const
{
  return !(*this == _boolean);
}

bool JBoolean::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JBoolean::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JBoolean::staticGetJavaJniClass() throw (JNIException)
{
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("boolean", "Z"));
	return *result;
}

const JClass& JBoolean::getJavaJniClass() const throw (JNIException)
{
  return JBoolean::staticGetJavaJniClass();
}

const char* JBoolean::ClassName = "Boolean";

END_NAMESPACE_3(jace, proxy, types)
