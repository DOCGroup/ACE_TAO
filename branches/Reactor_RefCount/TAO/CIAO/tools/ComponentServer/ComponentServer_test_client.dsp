# Microsoft Developer Studio Project File - Name="ComponentServer_test_client EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ComponentServer_test_client EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "ComponentServer_test_client.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "ComponentServer_test_client.mak" CFG="ComponentServer_test_client EXE - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "ComponentServer_test_client EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ComponentServer_test_client EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ComponentServer_test_client EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\.." /I "..\..\.." /I "..\..\ciao" /I "..\..\..\orbsvcs\orbsvcs" /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\..\CIAO" /i "..\..\..\..\CIAO\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib CIAO_Server.lib TAO_PortableServer.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib CIAO_Client.lib ACE.lib /nologo /version:1.3 /subsystem:console /machine:I386 /out:"ComponentServer_test_client.exe" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\ciao" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "ComponentServer_test_client EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\.." /I "..\..\.." /I "..\..\ciao" /I "..\..\..\orbsvcs\orbsvcs" /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\..\CIAO" /i "..\..\..\..\CIAO\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib CIAO_Serverd.lib TAO_PortableServerd.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib CIAO_Clientd.lib ACEd.lib /nologo /version:1.3 /subsystem:console /incremental:no /debug /machine:I386 /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\ciao" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ENDIF

# Begin Target

# Name "ComponentServer_test_client EXE - Win32 Release"
# Name "ComponentServer_test_client EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\ComponentServer_test_client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# End Group
# End Target
# End Project
