# Microsoft Developer Studio Project File - Name="RTCORBA_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RTCORBA_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTCORBA_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTCORBA_Static.mak" CFG="RTCORBA_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTCORBA_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RTCORBA_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTCORBA_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\RTCORBA"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "NDEBUG" /D "WIN32" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_RTCORBAs.lib"

!ELSEIF  "$(CFG)" == "RTCORBA_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\RTCORBA"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_RTCORBAsd.lib"

!ENDIF 

# Begin Target

# Name "RTCORBA_Static - Win32 Static Release"
# Name "RTCORBA_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Linear_Network_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.inl
# End Source File
# Begin Source File

SOURCE=.\RT_Current.i
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.i
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.i
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.i
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.i
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Linear_Network_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.h
# End Source File
# Begin Source File

SOURCE=.\RT_Current.h
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Selector_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Utils.h
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.h
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB_Loader.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\RT_Stub.h
# End Source File
# Begin Source File

SOURCE=.\RT_Stub_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBA.h
# End Source File
# Begin Source File

SOURCE=.\rtcorba_export.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Continuous_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Linear_Network_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Network_Priority_Mapping_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Private_Transport_Descriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Current.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Selector_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Endpoint_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Invocation_Endpoint_Selectors.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORB_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Protocols_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Stub.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Stub_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Thread_Lane_Resources_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBA.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Pool.cpp
# End Source File
# End Group
# End Target
# End Project
