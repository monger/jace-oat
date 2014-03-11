#include "jace/proxy/types/JLong.h"

#include "jace/JClassImpl.h"

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, proxy, types)


JLong::JLong(jvalue value)
{
  setJavaJniValue(value);
}

JLong::JLong(JNIType _long)
{
  jvalue value;
  value.j = _long;
  setJavaJniValue(value);
}

JLong::JLong(const JInt& _int)
{
  jvalue value;
  value.j = static_cast<jint>(_int);
  setJavaJniValue(value);
}


JLong::~JLong()
{}

JLong::operator JNIType() const
{
  return static_cast<jvalue>(*this).j;
}

bool JLong::operator==(const JLong& _long) const
{
  return static_cast<JNIType>(_long) == static_cast<JNIType>(*this);
}

bool JLong::operator!=(const JLong& _long) const
{
  return !(*this == _long);
}

bool JLong::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JLong::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JLong::staticGetJavaJniClass() {
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("long", "J"));
	return *result;
}

const JClass& JLong::getJavaJniClass() const {
  return JLong::staticGetJavaJniClass();
}

const char* JLong::ClassName = "Long";

END_NAMESPACE_3(jace, proxy, types)
