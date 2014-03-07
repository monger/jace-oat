#ifndef JACE_JFIELD_HELPER_H
#define JACE_JFIELD_HELPER_H

#include "Namespace.h"
#include "jace/proxy/JObject.h"
#include "jace/JClass.h"

#include "jni.h"
#include <string>

BEGIN_NAMESPACE(jace)

class JFieldHelper
{
public:
  JFieldHelper(const std::string& name, const jace::JClass& typeClass);

  jvalue getField(jace::proxy::JObject& object);
  jvalue getField(const jace::JClass& jClass);
  jfieldID getFieldID(const jace::JClass& parentClass, bool isStatic);
  jfieldID getFieldID();

private:
	/**
	 * Prevent copying.
	 */
	JFieldHelper& operator=(JFieldHelper&);

  jfieldID mFieldID;
  const std::string mName;
  const JClass& mTypeClass;
};

END_NAMESPACE(jace)

#endif
