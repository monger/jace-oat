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

#include "jace/runtime/ShutdownHook.h"
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
using namespace jace::proxy::java::lang;
using namespace jace::proxy::java::util;

#include "jace/proxy/java/net/URL.h"
#include "jace/proxy/javax/net/ssl/HostnameVerifier.h"
#include "jace/proxy/javax/net/ssl/HttpsURLConnection.h"
#include "jace/proxy/javax/net/ssl/KeyManager.h"
#include "jace/proxy/javax/net/ssl/SSLSession.h"
#include "jace/proxy/javax/net/ssl/TrustManager.h"
#include "jace/proxy/javax/net/ssl/X509TrustManager.h"
#include "jace/proxy/javax/net/ssl/SSLSocketFactory.h"
#include "jace/proxy/javax/net/ssl/SSLContext.h"
#include "jace/proxy/java/security/cert/X509Certificate.h"
#include "jace/proxy/java/security/SecureRandom.h"
#include "jace/JArray.h"
using jace::JArray;
using namespace jace::proxy::java::net;
using namespace jace::proxy::javax::net::ssl;
using namespace jace::proxy::java::security;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::endl;

// takes the teeth out of a TrustManager
static void doCheckServerTrusted(JObject, CallbackArgs) {
    cout << "doing nothing on purpose\n";
}
static jobject doGetAcceptedIssuers(JObject, CallbackArgs) {
    cout << "returning null\n";
    return 0;
}

// takes the teeth out of a HostnameVerifier
static bool doVerify(JObject, CallbackArgs) {
    return true;
}

/**
 * A example using hooks to implement shutdown and proxy objects.  You must pass the path to jace-runtime.jar to run.
 */
int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <https-url> <path/to/jace-runtime.jar>" << endl;
        return 1;
    }

    try {
        DefaultVmLoader loader(JNI_VERSION_1_6);
        OptionList list;
        list.push_back(jace::ClassPath(argv[2]));
        jace::createVm(loader, list, false);
        /* Try the shutdown hook registration */
        ShutdownHook::registerShutdownHook();
    
        /* We need to do this in our own scope, so that the proxy gets cleaned up before we destroy the VM */
        {
            // Create an emasculated TrustManager
            cout << "Creating the emasculated TrustManager...\n";
            NativeProxy::Builder tmBuilder("javax.net.ssl.X509TrustManager");
            tmBuilder.registerCallback("checkServerTrusted", doCheckServerTrusted);
            tmBuilder.registerCallback("getAcceptedIssuers", doGetAcceptedIssuers);

            // Create an emasculated HostnameVerifier
            cout << "Creating the emasculated HostnameVerifier...\n";
            NativeProxy::Builder hvBuilder("javax.net.ssl.HostnameVerifier");
            hvBuilder.registerCallback("verify", doVerify);

            // Ready the emasculated TrustManager
            cout << "Assembling the X509Certificate array...\n";
            JArray<TrustManager> trustArray(1);
            X509TrustManager tm = tmBuilder.build<X509TrustManager>();
            trustArray[0] = tm;

            // Ready the emasculated HostnameVerifier
            HostnameVerifier verifier = hvBuilder.build<HostnameVerifier>();

            // Initialize the SSLContext
            cout << "Initializing SSLContext...\n";
            SecureRandom random;
            JArray<KeyManager> keyManagers(0);
            SSLContext context = jace::java_cast<SSLContext>(SSLContext::getInstance("SSL"));
            context.init(keyManagers, trustArray, random);
            SSLSocketFactory sFactory = jace::java_cast<SSLSocketFactory>(context.getSocketFactory());

            // Initialize the connection, applying our emasculated TrustManager and HostnameVerifier
            cout << "Setting default SSLSocketFactory...\n";
            HttpsURLConnection::setDefaultSSLSocketFactory(sFactory);
            cout << "Setting default HostnameVerifier...\n";
            HttpsURLConnection::setDefaultHostnameVerifier(verifier);
            cout << "Creating the URL object...\n";
            URL url = URL::Factory::create(argv[1]);
            cout << "Creating the HttpsURLConnection...\n";
            HttpsURLConnection https = jace::java_cast<HttpsURLConnection>(url.openConnection());

            // Connect to the URL
            cout << "Connecting to " << argv[1] << endl;
            https.connect();
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
