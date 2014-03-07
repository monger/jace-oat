#include "jace/proxy/types/JChar.h"

#include "jace/JClassImpl.h"

#include <iostream>
using std::ostream;

#include <boost/thread/mutex.hpp>

BEGIN_NAMESPACE_3(jace, proxy, types)


JChar::JChar(jvalue value)
{
  setJavaJniValue(value);
}

JChar::JChar(JNIType _char)
{
  jvalue value;
  value.c = _char;
  setJavaJniValue(value);
}

JChar::~JChar()
{}

JChar::operator JNIType() const
{
  return static_cast<jvalue>(*this).c;
}

bool JChar::operator==(const JChar& _char) const
{
  return static_cast<JNIType>(_char) == static_cast<JNIType>(*this);
}

bool JChar::operator!=(const JChar& _char) const
{
  return !(*this == _char);
}

bool JChar::operator==(JNIType val) const
{
  return val == static_cast<JNIType>(*this);
}

bool JChar::operator!=(JNIType val) const
{
  return !(*this == val);
}

static boost::mutex javaClassMutex;
const JClass& JChar::staticGetJavaJniClass() throw (JNIException)
{
	static boost::shared_ptr<JClassImpl> result;
	boost::mutex::scoped_lock lock(javaClassMutex);
	if (result == 0)
		result = boost::shared_ptr<JClassImpl>(new JClassImpl("char", "C"));
	return *result;
}

const JClass& JChar::getJavaJniClass() const throw (JNIException)
{
  return JChar::staticGetJavaJniClass();
}

ostream& operator<<(ostream& stream, const JChar& val)
{
  return stream << static_cast<char>(val);
}

const char* JChar::ClassName = "Char";

END_NAMESPACE_3(jace, proxy, types)
