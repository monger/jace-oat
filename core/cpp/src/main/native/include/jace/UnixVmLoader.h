#ifndef JACE_UNIX_VM_LOADER
#define JACE_UNIX_VM_LOADER

#ifndef _WIN32

#include "jace/Namespace.h"
#include "jace/VmLoader.h"
#include "jace/JNIException.h"

#include <jni.h>

#include <string>

BEGIN_NAMESPACE(jace)

/**
 * A generic Virtual Machine loader for Unix based operating systems.
 * This simple loader should work fine on most Unices.
 *
 * @author Toby Reyelts
 */
class UnixVmLoader: public ::jace::VmLoader
{
public:
  /**
   * Creates a new VM loader for the specified VM.
   * The VM to be loaded is specified by the path to the shared library.
   *
   * @param path - The path to the shared library implementing the VM.
   *
   * @param jniVersion - The version of JNI to use. For example, JNI_VERSION_1_2 or
   * JNI_VERSION_1_4.  The default is DEFAULT_JNI_VERSION.
   *
	 * @throws JNIException if an error occurs while loading the JVM library
   */
  UnixVmLoader(std::string path, jint jniVersion = DEFAULT_JNI_VERSION);
  virtual ~UnixVmLoader();

  jint createJavaVM(JavaVM **pvm, void **env, void *args) const;
  jint getCreatedJavaVMs(JavaVM **vmBuf, jsize bufLen, jsize *nVMs) const;

private:
  typedef jint (JNICALL *CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);
  typedef jint (JNICALL *GetCreatedJavaVMs_t)(JavaVM **vmBuf, jsize bufLen, jsize *nVMs);

  CreateJavaVM_t createJavaVMPtr;
  GetCreatedJavaVMs_t getCreatedJavaVMsPtr;

  std::string path;
  void* lib;
};

END_NAMESPACE(jace)

#endif // !_WIN32

#endif // JACE_UNIX_VM_LOADER

