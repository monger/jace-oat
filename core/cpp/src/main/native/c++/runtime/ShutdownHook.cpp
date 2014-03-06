#include "jace/runtime/ShutdownHook.h"
#include "jace/Jace.h"

#include <string>
using std::string;

BEGIN_NAMESPACE_3(jace, runtime, ShutdownHook)

/**
 * Invoked by org.jace.util.ShutdownHook on VM shutdown.
 */
static void native_signalVMShutdown(JNIEnv*, jclass) {
    jace::cleanup();
}

/**
 * Registers the shutdown hook.  Must be called after creating/setting the VM.
 */
void registerShutdownHook() throw (JNIException)
{
    JNIEnv* env = attach();
	jclass hookClass = env->FindClass("org/jace/util/ShutdownHook");
	if (!hookClass) { 
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the class, org.jace.util.ShutdownHook.");
	}

    void(*pf)(JNIEnv*, jclass) = native_signalVMShutdown;
    JNINativeMethod methods[] = { { (char*) "signalVMShutdown", (char*) "()V", *(void**)(&pf) } };
    int methods_size = sizeof(methods) / sizeof(methods[0]);
    if (env->RegisterNatives(hookClass, methods, methods_size) != JNI_OK) {
		env->DeleteLocalRef(hookClass);
        THROW_JNI_EXCEPTION("Unable to register native callback for signalVMShutdown().");
    }

	jmethodID hookRegister = env->GetStaticMethodID(hookClass, "register", "()V");
	if (!hookRegister) {
		env->DeleteLocalRef(hookClass);
        THROW_JNI_EXCEPTION("Assert failed: Unable to find the method, ShutdownHook.register().");
	}
    
	env->CallStaticObjectMethod(hookClass, hookRegister);
    try {
        jace::catchAndThrow();
	} catch (std::exception& e) {
    	env->DeleteLocalRef(hookClass);
		string msg = "Exception thrown invoking ShutdownHook.register()\n";
		msg.append("caused by:\n");
		msg.append(e.what());
		throw JNIException(msg);
	}
    
	env->DeleteLocalRef(hookClass);
}

END_NAMESPACE_3(jace, runtime, ShutdownHook)
