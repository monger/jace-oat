#ifndef JACE_RUNTIME_SHUTDOWNHOOK_H
#define JACE_RUNTIME_SHUTDOWNHOOK_H

#include "jace/OsDep.h"
#include "jace/Namespace.h"
#include "jace/JNIException.h"

/**
 * Shutdown hook registration
 */

BEGIN_NAMESPACE_3(jace, runtime, ShutdownHook)

/**
 * Registers the shutdown hook
 */
JACE_API void registerShutdownHook() throw (JNIException);

END_NAMESPACE_3(jace, runtime, ShutdownHook)

#endif
