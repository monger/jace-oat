
#ifndef JACE_PROXY_JAVA_IO_FILTEROUTPUTSTREAM_H
#define JACE_PROXY_JAVA_IO_FILTEROUTPUTSTREAM_H

#ifndef JACE_OS_DEP_H
#include "jace/os_dep.h"
#endif

#ifndef JACE_NAMESPACE_H
#include "jace/namespace.h"
#endif

#ifndef JACE_JOBJECT_H
#include "jace/proxy/JObject.h"
#endif

#ifndef JACE_JENLISTER_H
#include "jace/JEnlister.h"
#endif

#ifndef JACE_JARRAY_H
#include "jace/JArray.h"
#endif

#ifndef JACE_JFIELD_PROXY_H
#include "jace/JFieldProxy.h"
#endif

/**
 * The super class for this class.
 *
 */
#ifndef JACE_PROXY_JAVA_IO_OUTPUTSTREAM_H
#include "jace/proxy/java/io/OutputStream.h"
#endif

/**
 * Classes which this class is fully dependent upon.
 *
 */
#ifndef JACE_TYPES_JBYTE_H
#include "jace/proxy/types/JByte.h"
#endif

/**
 * Forward declarations for the classes that this class uses.
 *
 */
BEGIN_NAMESPACE_3( jace, proxy, types )
class JVoid;
END_NAMESPACE_3( jace, proxy, types )

BEGIN_NAMESPACE_3( jace, proxy, types )
class JInt;
END_NAMESPACE_3( jace, proxy, types )

BEGIN_NAMESPACE_4( jace, proxy, java, io )
class IOException;
END_NAMESPACE_4( jace, proxy, java, io )

BEGIN_NAMESPACE_4( jace, proxy, java, io )

/**
 * The Jace C++ proxy class for java/io/FilterOutputStream.
 * Please do not edit this class, as any changes you make will be overwritten.
 * For more information, please refer to the Jace Developer's Guide.
 *
 */
class FilterOutputStream : public ::jace::proxy::java::io::OutputStream {

public: 

/**
 * FilterOutputStream
 *
 */
FilterOutputStream( ::jace::proxy::java::io::OutputStream p0 );

/**
 * write
 *
 */
void write( ::jace::proxy::types::JInt p0 );

/**
 * write
 *
 */
void write( ::jace::JArray< ::jace::proxy::types::JByte > p0 );

/**
 * write
 *
 */
void write( ::jace::JArray< ::jace::proxy::types::JByte > p0, ::jace::proxy::types::JInt p1, ::jace::proxy::types::JInt p2 );

/**
 * flush
 *
 */
void flush();

/**
 * close
 *
 */
void close();

/**
 * The following methods are required to integrate this class
 * with the Jace framework.
 *
 */
FilterOutputStream( jvalue value );
FilterOutputStream( jobject object );
FilterOutputStream( const FilterOutputStream& object );
FilterOutputStream( const NoOp& noOp );
virtual const JClass* getJavaJniClass() const throw ( JNIException );
static const JClass* staticGetJavaJniClass() throw ( JNIException );
static JEnlister< FilterOutputStream> enlister;
};

END_NAMESPACE_4( jace, proxy, java, io )

#ifndef PUT_TSDS_IN_HEADER
  template <> ::jace::ElementProxy< ::jace::proxy::java::io::FilterOutputStream>::ElementProxy( jarray array, jvalue element, int index );
  template <> ::jace::ElementProxy< ::jace::proxy::java::io::FilterOutputStream>::ElementProxy( const jace::ElementProxy< ::jace::proxy::java::io::FilterOutputStream>& proxy );
#else
  template <> jace::ElementProxy< jace::proxy::java::io::FilterOutputStream>::ElementProxy( jarray array, jvalue element, int index ) : 
    jace::proxy::java::io::FilterOutputStream( element ), Object( NO_OP ), mIndex( index ) {
    JNIEnv* env = ::jace::helper::attach();
    parent = static_cast<jarray>( ::jace::helper::newGlobalRef( env, array ) );
  }
  template <> jace::ElementProxy< jace::proxy::java::io::FilterOutputStream>::ElementProxy( const jace::ElementProxy< jace::proxy::java::io::FilterOutputStream>& proxy ) : 
    jace::proxy::java::io::FilterOutputStream( proxy.getJavaJniObject() ), Object( NO_OP ), mIndex( proxy.mIndex ) {
    JNIEnv* env = ::jace::helper::attach();
    parent = static_cast<jarray>( ::jace::helper::newGlobalRef( env, proxy.parent ) );
  }
#endif

#ifndef PUT_TSDS_IN_HEADER
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( jfieldID fieldID_, jvalue value, jobject parent_ );
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( jfieldID fieldID_, jvalue value, jclass parentClass_ );
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( const ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>& object );
#else
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( jfieldID fieldID_, jvalue value, jobject parent_ ) : 
    ::jace::proxy::java::io::FilterOutputStream( value ), Object( NO_OP ), fieldID( fieldID_ ) {
    JNIEnv* env = ::jace::helper::attach();

    if ( parent_ ) {
      parent = ::jace::helper::newGlobalRef( env, parent_ );
    }
    else {
      parent = parent_;
    }

    parentClass = 0;
  }
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( jfieldID fieldID_, jvalue value, jclass parentClass_ ) : 
    ::jace::proxy::java::io::FilterOutputStream( value ), Object( NO_OP ), fieldID( fieldID_ ) {
    JNIEnv* env = ::jace::helper::attach();

    parent = 0;
    parentClass = static_cast<jclass>( ::jace::helper::newGlobalRef( env, parentClass_ ) );
  }
  template <> ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>::JFieldProxy( const ::jace::JFieldProxy< ::jace::proxy::java::io::FilterOutputStream>& object ) : 
    ::jace::proxy::java::io::FilterOutputStream( object.getJavaJniValue() ), Object( NO_OP ) {

    fieldID = object.fieldID; 

    if ( object.parent ) {
      JNIEnv* env = ::jace::helper::attach();
      parent = ::jace::helper::newGlobalRef( env, object.parent );
    }
    else {
      parent = 0;
    }

    if ( object.parentClass ) {
      JNIEnv* env = ::jace::helper::attach();
      parentClass = static_cast<jclass>( ::jace::helper::newGlobalRef( env, object.parentClass ) );
    }
    else {
      parentClass = 0;
    }
  }
#endif


#endif // #ifndef JACE_PROXY_JAVA_IO_FILTEROUTPUTSTREAM_H

