# Microsoft Developer Studio Project File - Name="Compass DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Compass DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Compass.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Compass.mak" CFG="Compass DLL - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Compass DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Compass DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Compass DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /Zi /Ob2 /GR /I "..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D COMPASS_BUILD=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Compass.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXML.lib ACEXML_Parser.lib zzip.lib z.lib ACE.lib /libpath:"..\..\ace" /nologo /version:5.2.4 /subsystem:windows /dll  /machine:I386 /out:"..\..\bin\Compass.dll"

!ELSEIF  "$(CFG)" == "Compass DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D COMPASS_BUILD=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Compass.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEXMLd.lib ACEXML_Parserd.lib zzipd.lib zd.lib ACEd.lib /libpath:"..\..\ace" /nologo /version:5.2.4 /subsystem:windows /dll /debug /pdb:Compassd.pdb /machine:I386 /out:"..\..\bin\Compassd.dll"

!ENDIF

# Begin Target

# Name "Compass DLL - Win32 Release"
# Name "Compass DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Assembly.cpp
# End Source File
# Begin Source File

SOURCE=.\AssemblyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentServer.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigValue.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyFile_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerActivator.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Assembly.h
# End Source File
# Begin Source File

SOURCE=.\AssemblyFactory.h
# End Source File
# Begin Source File

SOURCE=.\CCMHome.h
# End Source File
# Begin Source File

SOURCE=.\Compass_Export.h
# End Source File
# Begin Source File

SOURCE=.\CompassTypes.h
# End Source File
# Begin Source File

SOURCE=.\ComponentInstallation.h
# End Source File
# Begin Source File

SOURCE=.\ComponentServer.h
# End Source File
# Begin Source File

SOURCE=.\ConfigValue.h
# End Source File
# Begin Source File

SOURCE=.\Container.h
# End Source File
# Begin Source File

SOURCE=.\Deployment.h
# End Source File
# Begin Source File

SOURCE=.\PropertyFile_Handler.h
# End Source File
# Begin Source File

SOURCE=.\ServerActivator.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Assembly.inl
# End Source File
# Begin Source File

SOURCE=.\CCMHome.inl
# End Source File
# Begin Source File

SOURCE=.\ComponentInstallation.inl
# End Source File
# Begin Source File

SOURCE=.\ComponentServer.inl
# End Source File
# Begin Source File

SOURCE=.\Container.inl
# End Source File
# Begin Source File

SOURCE=.\PropertyFile_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\ServerActivator.inl
# End Source File
# End Group
# End Target
# End Project
