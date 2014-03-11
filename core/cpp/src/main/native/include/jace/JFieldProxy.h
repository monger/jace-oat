#ifndef JACE_JFIELD_PROXY_H
#define JACE_JFIELD_PROXY_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "JFieldProxyHelper.h"
#include "jace/proxy/types/JBoolean.h"
#include "jace/proxy/types/JByte.h"
#include "jace/proxy/types/JChar.h"
#include "jace/proxy/types/JDouble.h"
#include "jace/proxy/types/JFloat.h"
#include "jace/proxy/types/JInt.h"
#include "jace/proxy/types/JLong.h"
#include "jace/proxy/types/JShort.h"

BEGIN_NAMESPACE(jace)

/**
 * A JFieldProxy is a wrapper around a JField.
 *
 * A JFieldProxy makes sure that assignments can happen to class
 * and instance fields. For example,
 *
 *
 * // Java class
 * public class Foo
 * {
 *   public String bar;
 * }
 *
 * // C++ proxy class
 * class Foo: public Object
 * {
 * public:
 *   JFieldProxy<String> bar();
 * }
 *
 * // C++ code.
 * Foo.bar() = String("Hello!");
 *
 * @author Toby Reyelts
 *
 */
template <class FieldType> class JFieldProxy: public FieldType
{
public:
	/**
	 * Creates a new JFieldProxy that belongs to the given object,
	 * and represents the given value.
	 *
	 * This constructor should always be specialized away by subclasses.
	 */
	JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
		FieldType(value), fieldID(_fieldID)
	{
		if (_parent) {
			parent = newGlobalRef(_parent);
		} else {
			parent = _parent;
        }

		parentClass = 0;
	}


	/**
	 * Creates a new JFieldProxy that belongs to the given class,
	 * and represents the given value. (The field is a static one).
	 *
	 * This constructor should always be specialized away by subclasses.
	 */
	JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
		FieldType(value), fieldID(_fieldID)
	{
		parent = 0;
		parentClass = newGlobalRef(_parentClass);
	}


	/**
	 * Creates a new JFieldProxy that belongs to the given object,
	 * and represents the given value.
	 *
	 * This copy constructor should always be specialized away by subclasses.
	 */
	JFieldProxy(const JFieldProxy& object):
		FieldType(object.getJavaJniValue())
	{
		if (object.parent) {
			parent = newGlobalRef(object.parent);
		} else {
			parent = 0;
        }

		if (object.parentClass) {
			parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
		} else {
			parentClass = 0;
        }
	}


	virtual ~JFieldProxy() throw () {
		if (parent)
		{
			deleteGlobalRef(parent), parent = 0;
		}

		if (parentClass)
		{
			deleteGlobalRef(parentClass), parentClass = 0;
		}
	}

	/**
	 * If someone assigns to this proxy, they're really assigning
	 * to the field.
	 */
	FieldType& operator=(const FieldType& field)
	{
		if (parent)
			setJavaJniObject(JFieldProxyHelper::assign(field, parent, fieldID));
		else
			setJavaJniObject(JFieldProxyHelper::assign(field, parentClass, fieldID));
		return *this;
	}

private:
	jobject parent;
	jclass parentClass;
	jfieldID fieldID;
};

template <> inline
JFieldProxy< ::jace::proxy::types::JBoolean >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JBoolean(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JBoolean >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JBoolean(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JBoolean >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JBoolean >& object):
  ::jace::proxy::types::JBoolean(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  }
  else
    parent = 0;

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  }
  else
    parentClass = 0;
}

template <> inline
jace::proxy::types::JBoolean& JFieldProxy< ::jace::proxy::types::JBoolean >::operator=(const ::jace::proxy::types::JBoolean& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticBooleanField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetBooleanField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JByte >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JByte(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JByte >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JByte(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JByte >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JByte >& object):
  ::jace::proxy::types::JByte(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  }
  else
    parent = 0;

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  }
  else
    parentClass = 0;
}

template <> inline
jace::proxy::types::JByte& JFieldProxy< ::jace::proxy::types::JByte >::operator=(const ::jace::proxy::types::JByte& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticByteField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetByteField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JChar >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent)
:
  ::jace::proxy::types::JChar(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  }
  else
    parent = _parent;
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JChar >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JChar(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JChar >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JChar >& object) :
  ::jace::proxy::types::JChar(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  }
  else
    parent = 0;

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  }
  else
    parentClass = 0;
}

template <> inline
jace::proxy::types::JChar& JFieldProxy< ::jace::proxy::types::JChar >::operator=(const ::jace::proxy::types::JChar& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticCharField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetCharField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JShort >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JShort(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JShort >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JShort(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JShort >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JShort >& object):
  ::jace::proxy::types::JShort(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  } else {
    parent = 0;
  }

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  } else {
    parentClass = 0;
  }
}

template <> inline
jace::proxy::types::JShort& JFieldProxy< ::jace::proxy::types::JShort >::operator=(const ::jace::proxy::types::JShort& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticShortField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetShortField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JInt >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JInt(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JInt >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JInt(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JInt >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JInt >& object):
  ::jace::proxy::types::JInt(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  } else {
    parent = 0;
  }

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  } else {
    parentClass = 0;
  }
}

template <> inline
jace::proxy::types::JInt& JFieldProxy< ::jace::proxy::types::JInt >::operator=(const ::jace::proxy::types::JInt& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticIntField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetIntField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JLong >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JLong(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JLong >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JLong(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JLong >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JLong >& object):
  ::jace::proxy::types::JLong(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  } else {
    parent = 0;
  }

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  } else {
    parentClass = 0;
  }
}

template <> inline
jace::proxy::types::JLong& JFieldProxy< ::jace::proxy::types::JLong >::operator=(const ::jace::proxy::types::JLong& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent) {
    jclass fieldClass = parentClass;
    env->SetStaticLongField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetLongField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JFloat >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JFloat(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JFloat >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JFloat(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JFloat >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JFloat >& object):
  ::jace::proxy::types::JFloat(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  } else {
    parent = 0;
  }

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  } else {
    parentClass = 0;
  }
}

template <> inline
jace::proxy::types::JFloat& JFieldProxy< ::jace::proxy::types::JFloat >::operator=(const ::jace::proxy::types::JFloat& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticFloatField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetFloatField(parent, fieldID, type);

  return *this;
}


template <> inline
JFieldProxy< ::jace::proxy::types::JDouble >::JFieldProxy(jfieldID _fieldID, jvalue value, jobject _parent):
  ::jace::proxy::types::JDouble(value), fieldID(_fieldID)
{
  if (_parent)
  {
    parent = newGlobalRef(_parent); 
  } else {
    parent = _parent;
  }
  
  parentClass = 0;
}

template <> inline
JFieldProxy< ::jace::proxy::types::JDouble >::JFieldProxy(jfieldID _fieldID, jvalue value, jclass _parentClass):
  ::jace::proxy::types::JDouble(value), fieldID(_fieldID)
{
  parent = 0;
  parentClass = static_cast<jclass>(newGlobalRef(_parentClass)); 
}

template <> inline
JFieldProxy< ::jace::proxy::types::JDouble >::JFieldProxy(const JFieldProxy< ::jace::proxy::types::JDouble >& object):
  ::jace::proxy::types::JDouble(static_cast<jvalue>(object)), fieldID(object.fieldID)
{
  if (object.parent)
  {
    parent = newGlobalRef(object.parent); 
  } else {
    parent = 0;
  }

  if (object.parentClass)
  {
    parentClass = static_cast<jclass>(newGlobalRef(object.parentClass));
  } else {
    parentClass = 0;
  }
}

template <> inline
jace::proxy::types::JDouble& JFieldProxy< ::jace::proxy::types::JDouble >::operator=(const ::jace::proxy::types::JDouble& type)
{
  JNIEnv* env = attach();
  jvalue value = static_cast<jvalue>(type);
  setJavaJniValue(value);

  // If we don't have a parent object, we are a static field member.
  if (!parent)
  {
    jclass fieldClass = parentClass;
    env->SetStaticDoubleField(fieldClass, fieldID, type);
  }
  // Otherwise, we are an instance member.
  else
    env->SetDoubleField(parent, fieldID, type);

  return *this;
}

END_NAMESPACE(jace)

#endif // #ifndef JACE_JFIELD_PROXY_H
