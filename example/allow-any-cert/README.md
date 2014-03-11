allow-any-cert
==============

Utilizes the version 1.2.33 JACE runtime to add callbacks to `X509ExtendedTrustManager`'s `getAcceptedIssues` and
`isServerTrusted` methods.

To run, ensure that you've installed the **jace-oat** project:

    cd ../../
    mvn clean install

Then build this project:

    mvn install

Ensure that you've defined `$LD_LIBRARY_PATH` to point to Java's **server** jvm:

    export LD_LIBRARY_PATH=$JAVA_HOME/jre/lib/server

Then run the app from OSX, passing in a URL with a self-signed certificate and the JACE runtime you installed from the
parent **jace-oat** project:

    target/allow-any-cert-1.0-SNAPSHOT-macosx-x86_64.uexe http://unsafe-url.com /path/to/com/toonetown/maven/jace/core/runtime/version/runtime-version.jar

