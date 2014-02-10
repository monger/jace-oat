#ifndef JACE_OsDep_H
#include "jace/OsDep.h"
#endif

#ifdef _WIN32

#ifndef JACE_WIN32_VM_LOADER
#define JACE_WIN32_VM_LOADER

#include "jace/Namespace.h"
#include "jace/JNIException.h"
#include "jace/VmLoader.h"

#include <jni.h>
#include <string>

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
#endif


BEGIN_NAMESPACE(jace)

/**
 * A generic Virtual Machine loader for Windows.
 *
 * This simple loader should work fine on any Windows that supports Win32
 * including Win95, Win98, WinNT*, Win2K, and WinXP.
 *
 * @author Toby Reyelts
 */
class Win32VmLoader : public ::jace::VmLoader
{
public:
	/**
	 * The JVM Vendors supported by this loader.
	 */
	JACE_API enum JVMVendor
	{
		JVMV_SUN,
		JVMV_IBM
	};

	/**
	 * JVM types supported by this loader.
	 */
	JACE_API enum JVMType
	{
		JVMT_DEFAULT,
		JVMT_CLASSIC, // Up to 1.3
		JVMT_HOTSPOT, // Up to 1.3
		JVMT_SERVER, 	// Only in JDKs not JREs
		JVMT_CLIENT, 	// New in 1.4
		JVMT_DEBUG    // Only IBM
	};

	/**
	 * Creates a new VM loader for the specified VM.
	 * The VM to be loaded is selected by vendor, type and version parameters.
	 *
	 * @param jvmVendor - Vendor of the VM to be loaded. Valid values are JVMV_IBM and JVMV_SUN
	 *        The default value is JVMV_SUN.
	 *
	 * @param jvmType - Type of the VM to be loaded. Valid values are
	 *        JVMT_DEFAULT, JVMT_CLASSIC, JVMT_HOTSPOT, JVMT_SERVER, JVMT_CLIENT, JVMT_DEBUG.
	 *        The default value is JVMT_DEFAULT, which means that the lastest Sun VM will be used.
	 *        Note, that not every combination with jvmVendor is possible.
	 *
	 * @param version - Version of the VM as String (e.g. "1.3.1", "1.4")
	 *        The default value is the version of the lastest Sun VM installed.
	 *
	 * @param jniVersion - The version of JNI to use. For example,
	 * JNI_VERSION_1_2 or JNI_VERSION_1_4. The default is DEFAULT_JNI_VERSION.
	 *
	 * @throws JNIException if an error occurs while loading the JVM library.
	 */
	JACE_API Win32VmLoader(JVMVendor jvmVendor = JVMV_SUN,
		JVMType jvmType = JVMT_DEFAULT,
		std::string version = "",
		jint jniVersion = DEFAULT_JNI_VERSION) throw (JNIException);

	/**
	 * Creates a new VM loader for the specified VM.
	 * The VM to be loaded is specified by the path to the shared library.
	 *
	 * @param path - The path to the shared library implementing the VM. Specifying
	 *               a filename with no path will result in the use of the standard
	 *               library search path (see LoadLibrary() for more information).
	 *
	 * @param version - The version of JNI to use. For example,
	 * JNI_VERSION_1_2 or JNI_VERSION_1_4. The default is DEFAULT_JNI_VERSION
	 *
	 * @throws JNIException if an error occurs while loading the JVM library.
	 */
	JACE_API Win32VmLoader(std::string path, jint jniVersion = DEFAULT_JNI_VERSION) throw (JNIException);

	JACE_API virtual ~Win32VmLoader();

	JACE_API jint getCreatedJavaVMs(JavaVM **vmBuf, jsize bufLen, jsize *nVMs) const;
	JACE_API jint createJavaVM(JavaVM **pvm, void **env, void *args) const;

private:
	typedef jint (JNICALL *GetCreatedJavaVMs_t)(JavaVM **vmBuf, jsize bufLen, jsize *nVMs);
	typedef jint (JNICALL *CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);

	GetCreatedJavaVMs_t getCreatedJavaVMsPtr;
	CreateJavaVM_t createJavaVMPtr;

	std::string path;
	HINSTANCE handle;

	/**
	 * Method to specify a VM and initialize the VM entry points.
	 * The VM to be loaded could be selected by vendor, type and version parameters
	 *
	 * @param jvmVendor - Vendor of the VM to be loaded. Valid values are JVMV_IBM and JVMV_SUN
	 *        The default value is JVMV_SUN.
	 * @param jvmType - Type of the VM to be loaded. Valid values are
	 *        JVMT_DEFAULT, JVMT_CLASSIC, JVMT_HOTSPOT, JVMT_SERVER, JVMT_CLIENT, JVMT_DEBUG.
	 *    		The default value is JVMT_DEFAULT, which means that the lastest Sun VM will be used.
	 *        Note, that not every combination with jvmVendor is possible.
	 * @param version - Version of the VM as String (e.g. "1.3.1", "1.4")
	 *        The default value is the version of the lastest Sun VM installed.
	 *
	 * @throws JNIException if an error occurs while trying to look up the VM.
	 */
	void specifyVm(JVMVendor jvmVendor, JVMType jvmType, std::string version);

	/**
	 * Loads the VM at the specified path.
	 *
	 * @throws JNIException if an error occurs while trying to load the VM.
	 */
	void loadVm(const std::string& path) throw (JNIException);
};

END_NAMESPACE(jace)

#endif // _WIN32

#endif // JACE_WIN32_VM_LOADER
