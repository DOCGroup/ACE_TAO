# Microsoft Developer Studio Project File - Name="Event_Service_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Event_Service_static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Event_Service_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Event_Service_static.mak" CFG="Event_Service_static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Event_Service_static - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Event_Service_static - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Event_Service_static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Static Release"
# PROP BASE Intermediate_Dir "Static Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LIB/Release"
# PROP Intermediate_Dir "LIB/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_HAS_DLL=0 /D ACE_HAS_DLL=0 /D TAO_ORBSVCS_HAS_DLL=1 /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOs.lib aces.lib TAO_CosNamings.lib TAO_RTEvents.lib TAO_RTOLDEvents.lib TAO_RTScheds.lib /nologo /subsystem:console /machine:I386 /out:"..\orbsvcs,..\..\tao,..\..\..\ace"
# ADD LINK32  TAO_ObjRefTemplates.lib TAO_Valuetypes.lib TAO_IORInterceptors.lib TAOs.lib aces.lib TAO_PortableServers.lib TAO_Svc_Utilss.lib TAO_CosNamings.lib TAO_RTEvents.lib TAO_RTOLDEvents.lib TAO_RTScheds.lib user32.lib advapi32.lib TAO_Messagings.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\TAO\PORTABLESERVER" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor"

!ELSEIF  "$(CFG)" == "Event_Service_static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Static Debug"
# PROP BASE Intermediate_Dir "Static Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_HAS_DLL=0 /D ACE_HAS_DLL=0 /D TAO_ORBSVCS_HAS_DLL=1 /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOsd.lib acesd.lib TAO_CosNamingsd.lib TAO_RTEventsd.lib TAO_RTOLDEventsd.lib TAO_RTSchedsd.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\orbsvcs,..\..\tao,..\..\..\ace" /pdbtype:sept
# ADD LINK32 TAO_ObjRefTemplatesd.lib TAO_Valuetypesd.lib TAO_IORInterceptorsd.lib TAOsd.lib acesd.lib TAO_PortableServersd.lib TAO_Svc_Utilssd.lib TAO_CosNamingsd.lib TAO_RTEventsd.lib TAO_RTOLDEventsd.lib TAO_RTSchedsd.lib user32.lib advapi32.lib TAO_Messagingsd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\TAO\PORTABLESERVER" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor"

!ENDIF 

# Begin Target

# Name "Event_Service_static - Win32 Static Release"
# Name "Event_Service_static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Event_Service.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Event_Service.h
# End Source File
# End Group
# End Target
# End Project
