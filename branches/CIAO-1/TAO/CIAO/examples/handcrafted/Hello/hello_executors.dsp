# Microsoft Developer Studio Project File - Name="hello_executors DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hello_executors DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "hello_executors.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "hello_executors.mak" CFG="hello_executors DLL - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "hello_executors DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hello_executors DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hello_executors DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\hello_executors"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /Zi /Ob2 /GR /I "..\..\..\..\orbsvcs\orbsvcs" /I "..\..\.." /I "..\..\..\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D HELLO_EXECUTOR_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\.." /i "..\..\..\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\hello_executors.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO hello_stub.lib hello_servants.lib CIAO_Server.lib TAO_PortableServer.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib CIAO_Client.lib ACE.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\..\..\bin\hello_executors.pdb" /dll  /machine:I386 /out:"..\..\..\..\..\bin\hello_executors.dll"

!ELSEIF  "$(CFG)" == "hello_executors DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\hello_executors"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\..\..\orbsvcs\orbsvcs" /I "..\..\.." /I "..\..\..\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D HELLO_EXECUTOR_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\.." /i "..\..\..\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\hello_executors.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO hello_stubd.lib hello_servantsd.lib CIAO_Serverd.lib TAO_PortableServerd.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib CIAO_Clientd.lib ACEd.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3.1 /subsystem:windows /pdb:"..\..\..\..\..\bin\hello_executorsd.pdb" /dll /debug /machine:I386 /out:"..\..\..\..\..\bin\hello_executorsd.dll"

!ENDIF

# Begin Target

# Name "hello_executors DLL - Win32 Release"
# Name "hello_executors DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\hello_executors.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\hello_executors.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\helloC.i
# End Source File
# Begin Source File

SOURCE=.\helloEC.i
# End Source File
# Begin Source File

SOURCE=.\helloES.i
# End Source File
# Begin Source File

SOURCE=.\helloS.i
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# End Target
# End Project
