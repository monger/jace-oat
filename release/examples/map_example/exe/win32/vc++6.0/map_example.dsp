# Microsoft Developer Studio Project File - Name="map_example" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=map_example - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "map_example.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "map_example.mak" CFG="map_example - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "map_example - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "map_example - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "map_example - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "c:\java\jdk1.4\include\win32" /I "..\..\..\include" /I "..\..\..\proxies\include" /I "..\..\..\..\..\include" /I "c:\java\jdk1.4\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib jvm.lib jace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\lib\win32\vc++6.0" /libpath:"c:\java\jdk1.4\lib"

!ELSEIF  "$(CFG)" == "map_example - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "c:\java\jdk1.4\include\win32" /I "..\..\..\include" /I "..\..\..\proxies\include" /I "..\..\..\..\..\include" /I "c:\java\jdk1.4\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib jvm.lib jaced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\lib\win32\vc++6.0" /libpath:"c:\java\jdk1.4\lib"

!ENDIF 

# Begin Target

# Name "map_example - Win32 Release"
# Name "map_example - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "proxies"

# PROP Default_Filter ""
# Begin Group "java"

# PROP Default_Filter ""
# Begin Group "io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\io\Serializable.cpp
# End Source File
# End Group
# Begin Group "lang"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\CharSequence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\Cloneable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\Comparable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\Integer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\Number.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\Object.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\lang\System.cpp
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\AbstractMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\Collection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\HashMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\Iterator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\Map.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\Map.Entry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\proxies\source\jace\proxy\java\util\Set.cpp
# End Source File
# End Group
# End Group
# End Group
# End Target
# End Project