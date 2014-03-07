#include "jace/Jace.h"
using jace::java_new;

#include "jace/DefaultVmLoader.h"
using jace::DefaultVmLoader;

#include "jace/OptionList.h"
using jace::OptionList;

#include "jace/JNIException.h"
using jace::JNIException;

#include "jace/VirtualMachineShutdownError.h"
using jace::VirtualMachineShutdownError;

#include "jace/runtime/NativeProxy.h"
using namespace jace::runtime;
using jace::runtime::NativeProxy::CallbackArgs;

#include "jace/proxy/JObject.h"
using jace::proxy::JObject;
#include "jace/proxy/types/JBoolean.h"
using jace::proxy::types::JBoolean;

#include "jace/proxy/java/lang/String.h"
#include "jace/proxy/java/lang/Throwable.h"
#include "jace/proxy/java/util/Iterator.h"
#include "jace/proxy/java/io/Closeable.h"
using namespace jace::proxy::java::lang;
using namespace jace::proxy::java::util;
using namespace jace::proxy::java::io;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::endl;


/** You can also use "real" classes as the return values */
// static String doClose(JObject, CallbackArgs) {
//     cout << "Hello, callback" << endl;
//     return String();
// }

static void doClose(JObject, CallbackArgs) {
    cout << "Hello, callback" << endl;
}

/** You can also use the jace::proxy::types classes as the return values */
// static JBoolean doNext(JObject, CallbackArgs) {
//     return true;
// }

static bool doNext(JObject, CallbackArgs) {
    return true;
}

/**
 * A example using hooks to implement shutdown and proxy objects.  You must pass the path to jace-runtime.jar to run.
 */
int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <path/to/jace-runtime.jar>" << endl;
        return 1;
    }
    try {
        DefaultVmLoader loader(JNI_VERSION_1_6);
        OptionList list;
        list.push_back(jace::ClassPath(argv[1]));
        jace::createVm(loader, list, false);
    
        /* We need to do this in our own scope, so that the proxy gets cleaned up before we destroy the VM */
        {
            /* Just implement a proxy with an anonymous interface */
            NativeProxy::Builder cBuilder("java.io.Closeable");
            cBuilder.registerCallback("close", doClose);
            Closeable c = cBuilder.build<Closeable>();
            cout << "Closing...";
            c.close();
            
            /* You don't have to implement all the functions - non-implemented functions will throw */
            NativeProxy::Builder lBuilder("java.util.Iterator");
            lBuilder.registerCallback("hasNext", doNext);
            Iterator i = lBuilder.build<Iterator>();
            cout << "List has next: " << (i.hasNext() ? "yes" : "no") << endl;
        }
            
        jace::destroyVm();
        return 0;
    } catch (VirtualMachineShutdownError&) {
        cout << "The JVM was terminated in mid-execution. " << endl;
        return -2;
    } catch (JNIException& jniException) {
        cout << "An unexpected JNI error has occurred: " << jniException.what() << endl;
        return -2;
    } catch (Throwable& t) {
        t.printStackTrace();
        return -2;
    } catch (exception& e) {
        cout << "An unexpected C++ error has occurred: " << e.what() << endl;
        return -2;
    }
}
