
# Compilation script for gcc 3.0+
mkdir debug 2> /dev/null
cd debug

rm -rf *.o 2> /dev/null

CC=g++

# Flags needed for compilation
#
# NOTE - These flags are provided in a particular order. This order must be
#        maintained for the code to be correctly built.
#
#  -mthread = Support multi-threaded code
#  -c       = Compile only
#  -Wall    = All warnings
#  -fPIC    = Create relocateable code
#
COMPILER_FLAGS=" -ansi -g -c -Wall "

# The directory containg the JDK header files.
#
JDK_INCLUDE=$JAVA_HOME/include

# The directory containing the operating system specific JDK header files.
#
JDK_OS_INCLUDE=$JDK_INCLUDE/linux

JACE_ROOT=../../../../../..
LOCAL_SOURCE=../../../../source
LOCAL_PROXY_SOURCE=../../../../proxies/source

echo "Compiling..."

$CC \
 $COMPILER_FLAGS \
 -I$JACE_ROOT/include \
 -I../../../../include \
 -I../../../../proxies/include \
 -I$JDK_INCLUDE \
 -I$JDK_OS_INCLUDE \
 $LOCAL_SOURCE/*.cpp \
 $LOCAL_PROXY_SOURCE/jace/proxy/java/io/*.cpp \
 $LOCAL_PROXY_SOURCE/jace/proxy/java/lang/*.cpp \
 $LOCAL_PROXY_SOURCE/jace/proxy/java/util/*.cpp

