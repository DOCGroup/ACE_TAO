# Microsoft Developer Studio Project File - Name="IOGRTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=IOGRTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "IOGRTest.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "IOGRTest.mak" CFG="IOGRTest - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "IOGRTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "IOGRTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IOGRTest - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\IOGRTest"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_FT_ServerORB.lib TAO_FT_ClientORB.lib TAO_FTORB_Utils.lib TAO_IORManip.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /libpath:"..\..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:"Release\IOGRTest.pdb"  /machine:I386 /out:"Release\IOGRTest.exe"

!ELSEIF  "$(CFG)" == "IOGRTest - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\IOGRTest"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_FT_ServerORBd.lib TAO_FT_ClientORBd.lib TAO_FTORB_Utilsd.lib TAO_IORManipd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /libpath:"..\..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:".\IOGRTest.pdb" /debug /machine:I386 /out:".\IOGRTest.exe"

!ENDIF

# Begin Target

# Name "IOGRTest - Win32 Release"
# Name "IOGRTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\IOGRTest.cpp"
# End Source File
# End Group
# End Target
# End Project
