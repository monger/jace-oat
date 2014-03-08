#ifndef JACE_ELEMENT_PROXY_H
#define JACE_ELEMENT_PROXY_H

#include "jace/Namespace.h"
#include "jace/Jace.h"
#include "jace/ElementProxyHelper.h"
#include "jace/proxy/JObject.h"
#include "jace/JClass.h"
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
 * An ElementProxy is a wrapper around a JArray element.
 *
 * An ElementProxy is responsible for pinning and depinning
 * its element as required.
 *
 * @author Toby Reyelts
 */
template <class ElementType> class ElementProxy: public virtual ::jace::proxy::JObject, public ElementType
{
public:
	/**
	 * Creates a new ElementProxy that belongs to the given array.
	 *
	 * This constructor shouldn't be called anymore, as it should be specialized
	 * by every proxy type. Every ElementProxy instance should allocate
	 * a new global ref to its parent array.
	 */
	ElementProxy(jarray array, jvalue element, int _index):
		ElementType(element), parent(array), index(_index)
	{
		// #error "ElementProxy was not properly specialized."

		std::cout << "ElementProxy was not properly specialized for " <<
						 ElementType::staticGetJavaJniClass().getName() << std::endl;
	}


	/**
	 * Copy constructor. This constructor should also never be called. It should be specialized away.
	 */
	ElementProxy(const ElementProxy& proxy):
		ElementType(0), parent(proxy.parent), index(proxy.index)
	{
		std::cout << "ElementProxy was not properly specialized for " <<
						 ElementType::staticGetJavaJniClass().getName() << std::endl;
	}


	/**
	 * If someone assigns to this array element, they're really assigning
	 * to an array, so we need to call SetObjectArrayElement.
	 */
	ElementType& operator=(const ElementType& element)
	{
		::jace::ElementProxyHelper::assign(element, index, parent);
		return *this;
	}


	/**
	 * If someone assigns to this array element, they're really assigning
	 * to an array, so we need to call SetObjectArrayElement.
	 */
	const ElementType& operator=(const ElementType& element) const
	{
		::jace::ElementProxyHelper::assign(element, index, parent);
		return *this;
	}


	~ElementProxy() throw () {
        deleteGlobalRef(parent);
	}

private:
	jarray parent;
	int index;
};

/**
 * Contains the definitions for the template specializations of the template class, ElementProxy.
 *
 * This file is internal to the JACE library.
 */
template <> inline
ElementProxy< ::jace::proxy::types::JBoolean >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JBoolean(element), index(_index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JBoolean >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JBoolean(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JBoolean& ElementProxy< ::jace::proxy::types::JBoolean >::operator=(const ::jace::proxy::types::JBoolean& type)
{
  JNIEnv* env = attach();
  jbooleanArray array = static_cast<jbooleanArray>(parent);
  jboolean val = static_cast<jvalue>(type).z;
  env->SetBooleanArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JByte >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JByte(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JByte >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JByte(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JByte& ElementProxy< ::jace::proxy::types::JByte >::operator=(const ::jace::proxy::types::JByte& type)
{
  JNIEnv* env = attach();
  jbyteArray array = static_cast<jbyteArray>(parent);
  jbyte byte = static_cast<jvalue>(type).b;
  env->SetByteArrayRegion(array, index, 1, &byte);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JChar >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JChar(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JChar >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JChar(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JChar& ElementProxy< ::jace::proxy::types::JChar >::operator=(const ::jace::proxy::types::JChar& type)
{
  JNIEnv* env = attach();
  jcharArray array = static_cast<jcharArray>(parent);
  jchar val = static_cast<jvalue>(type).c;
  env->SetCharArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JDouble >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JDouble(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JDouble >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JDouble(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JDouble& ElementProxy< ::jace::proxy::types::JDouble >::operator=(const ::jace::proxy::types::JDouble& type)
{
  JNIEnv* env = attach();
  jdoubleArray array = static_cast<jdoubleArray>(parent);
  jdouble val = static_cast<jvalue>(type).d;
  env->SetDoubleArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JFloat >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JFloat(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JFloat >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JFloat(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JFloat& ElementProxy< ::jace::proxy::types::JFloat >::operator=(const ::jace::proxy::types::JFloat& type)
{
  JNIEnv* env = attach();
  jfloatArray array = static_cast<jfloatArray>(parent);
  jfloat val = static_cast<jvalue>(type).f;
  env->SetFloatArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JInt >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JInt(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JInt >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JInt(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JInt& ElementProxy< ::jace::proxy::types::JInt >::operator=(const ::jace::proxy::types::JInt& type)
{
  JNIEnv* env = attach();
  jintArray array = static_cast<jintArray>(parent);
  jint val = static_cast<jvalue>(type).i;
  env->SetIntArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JLong >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JLong(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JLong >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JLong(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JLong& ElementProxy< ::jace::proxy::types::JLong >::operator=(const ::jace::proxy::types::JLong& type)
{
  JNIEnv* env = attach();
  jlongArray array = static_cast<jlongArray>(parent);
  jlong val = static_cast<jvalue>(type).j;
  env->SetLongArrayRegion(array, index, 1, &val);
  return *this;
}

template <> inline
ElementProxy< ::jace::proxy::types::JShort >::ElementProxy(jarray array, jvalue element, int _index): 
  ::jace::proxy::types::JShort(element), index(_index)
{
  parent = static_cast<jarray>(newGlobalRef(array));
}

template <> inline
ElementProxy< ::jace::proxy::types::JShort >::ElementProxy(const ElementProxy& proxy): 
  ::jace::proxy::types::JShort(proxy), index(proxy.index)
{
  parent = static_cast<jarray>(::jace::newGlobalRef(proxy.parent));
}

template <> inline
::jace::proxy::types::JShort& ElementProxy< ::jace::proxy::types::JShort >::operator=(const ::jace::proxy::types::JShort& type)
{
  JNIEnv* env = attach();
  jshortArray array = static_cast<jshortArray>(parent);
  jshort val = static_cast<jvalue>(type).s;
  env->SetShortArrayRegion(array, index, 1, &val);
  return *this;
}

END_NAMESPACE(jace)

#endif

