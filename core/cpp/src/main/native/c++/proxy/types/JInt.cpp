#include "jace/proxy/types/JInt.h"

#include "jace/JClassImpl.h"

#include <iostream>
using std::ostream;

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, proxy, types)


JInt::JInt(jvalue value)
{
  setJavaJniValue(value);
}

JInt::JInt(const JNIType _int)
{
  jvalue value;
  value.i = _int;
  setJavaJniValue(value);
}

JInt::JInt(const JByte& _byte)
{
  jvalue value;
  value.i = static_cast<jbyte>(_byte);
  setJavaJniValue(value);
}


JInt::~JInt()
{}

JInt::operator JNIType() const
{ 
  return static_cast<jvalue>(*this).i; 
}

bool JInt::operator==(const JInt& _int) const
{
  return static_cast<JNIType>(_int) == static_cast<JNIType>(*this);
}

bool JInt::operator!=(const JInt& _int) const
{
  return !(*this == _int);
}

bool JInt::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JInt::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JInt::staticGetJavaJniClass() {
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("int", "I"));
	return *result;
}

const JClass& JInt::getJavaJniClass() const {
  return JInt::staticGetJavaJniClass();
}

ostream& operator<<(ostream& stream, const JInt& val)
{
  return stream << static_cast<JInt::JNIType>(val);
}

const char* JInt::ClassName = "Integer";

END_NAMESPACE_3(jace, proxy, types)
