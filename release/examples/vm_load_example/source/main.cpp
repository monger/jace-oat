
#include "jace/JNIHelper.h"

#include "jace/OptionList.h"
using jace::OptionList;
using jace::Option;
using jace::ClassPath;
using jace::Verbose;
using jace::CustomOption;

#include "jace/StaticVmLoader.h"
using jace::StaticVmLoader;

#ifdef _WIN32
  #include "jace/Win32VmLoader.h"
  using jace::Win32VmLoader;
#else
  #include "jace/UnixVmLoader.h"
  using ::jace::UnixVmLoader;
#endif

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

/**
 * This example demonstrates the new virtual machine API.
 *
 * Starting with Jace 1.1 beta 3, you can now load a virtual
 * machine statically or dynamically. 
 *
 * To statically load a virtual machine, you must
 *   a) statically link with jvm.lib 
 *
 *   b) use StaticVmLoader in your call to createVm(). 
 *
 * To dynamically load a virtual machine, you must
 *
 *   a) NOT statically link with jvm.lib, 
 *
 *   b) globally #define JACE_WANT_DYNAMIC_LOAD. This prevents
 * StaticVmLoader from trying to statically bind with jvm.lib.
 *
 *   c) use an appropriate dynamic VmLoader for your platform - for example,
 * Win32VmLoader or UnixVmLoader. You may also write your own
 * dynamic VmLoader if you so choose.
 *
 * Whether or not you statically or dynamically load your virtual
 * machine, you provide options to it via the same mechanism.
 * You specify the entire set of virtual machine options in an OptionList
 * which you pass in to the call to createVm().
 *
 */
int main( int argc, char* argv[] ) {

  #ifdef JACE_WANT_DYNAMIC_LOAD

    if ( argc != 2 ) {
      cout << "Usage: vm_load_example <path to virtual machine>" << endl;
      return -1;
    }

    string path = argv[ 1 ];

    #ifdef _WIN32
      Win32VmLoader loader( path, JNI_VERSION_1_2 );
    #else
      UnixVmLoader loader( path, JNI_VERSION_1_2 );
    #endif
  #else
    StaticVmLoader loader( JNI_VERSION_1_2 );
  #endif

  OptionList options;

  options.push_back( ClassPath( "." ) );
  options.push_back( Verbose( Verbose::Jni, Verbose::Class ) );
  options.push_back( CustomOption( "-Xmx128M" ) );

  try {
    jace::helper::createVm( loader, options );
  }
  catch ( std::exception& e ) {
    cout << "Unable to create the virtual machine: " << endl;
    cout << e.what();
    return -2;
  }

  cout << "The virtual machine was successfully loaded." << endl;

  return 0;
}

