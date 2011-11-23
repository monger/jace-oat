#include "jace/BaseException.h"
#include "jace/Jace.h"

BEGIN_NAMESPACE_1(jace)


BaseException::BaseException(const std::string& value) throw (): 
	mValue(value)
{}

BaseException::BaseException(const std::wstring& value) throw ()
{
	mValue = toUTF8(value);
}

BaseException::BaseException(const BaseException& rhs) throw ()
{
  mValue = rhs.mValue;
}

BaseException& BaseException::operator=(const BaseException& rhs) throw ()
{
  if (this == &rhs)
    return *this;
  mValue = rhs.mValue;
  return *this;
}

BaseException::~BaseException() throw ()
{}

const char* BaseException::what() const throw ()
{
  return mValue.c_str();
}

END_NAMESPACE_1(jace)
