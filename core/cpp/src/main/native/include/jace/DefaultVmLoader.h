#ifndef JACE_DEFAULT_VM_LOADER
#define JACE_DEFAULT_VM_LOADER

#ifdef _WIN32
    #include "jace/Win32VmLoader.h"
#else
    #include "jace/UnixVmLoader.h"
#endif

BEGIN_NAMESPACE(jace)

/**
 * The default loader either extends the Unix or the Windows loader - based upon its platform.
 */
#ifdef _WIN32
    class DefaultVmLoader: public ::jace::Win32VmLoader {
    	public:
            DefaultVmLoader(jint jniVersion = DEFAULT_JNI_VERSION) throw (JNIException) : 
            Win32VmLoader(JVMV_SUN, JVMT_DEFAULT, "", jniVersion) {}
    };
#else
    class DefaultVmLoader: public ::jace::UnixVmLoader {
        public:
        	DefaultVmLoader(jint jniVersion = DEFAULT_JNI_VERSION) throw (JNIException) : 
        #if defined __ANDROID__
            UnixVmLoader("libdvm.so", jniVersion) {}
        #elif defined __APPLE__
            UnixVmLoader("libjvm.dylib", jniVersion) {}
        #else 
            UnixVmLoader("libjvm.so", jniVersion) {}
        #endif
    };
#endif

END_NAMESPACE(jace)

#endif // JACE_DEFAULT_VM_LOADER
