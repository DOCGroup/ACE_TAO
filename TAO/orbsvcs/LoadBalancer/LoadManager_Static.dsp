# Microsoft Developer Studio Project File - Name="LoadManager_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LoadManager_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadManager_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadManager_Static.mak" CFG="LoadManager_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoadManager_Static - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LoadManager_Static - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoadManager_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LIB\Release"
# PROP Intermediate_Dir "LIB\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "_CONSOLE" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_IORInterceptors.lib TAOs.lib TAO_CosLoadBalancings.lib TAO_PortableGroups.lib TAO_Messagings.lib TAO_CosNamings.lib TAO_Svc_Utilss.lib TAO_PortableServers.lib aces.lib TAO_Valuetypes.lib TAO_ObjRefTemplates.lib user32.lib advapi32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\portableserver" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\IORInterceptor"

!ELSEIF  "$(CFG)" == "LoadManager_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "_CONSOLE" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_IORInterceptorsd.lib TAO_ObjRefTemplatesd.lib TAOsd.lib acesd.lib TAO_CosLoadBalancingsd.lib TAO_PortableGroupsd.lib TAO_Messagingsd.lib TAO_CosNamingsd.lib TAO_Svc_Utilssd.lib TAO_PortableServersd.lib TAO_Valuetypesd.lib user32.lib advapi32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\tao\portableserver" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\IORInterceptor"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "LoadManager_Static - Win32 Static Release"
# Name "LoadManager_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\LoadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Signal_Handler.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Signal_Handler.h
# End Source File
# End Group
# End Target
# End Project
