# Microsoft Developer Studio Project File - Name="TAO_Strategies_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO_Strategies_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Strategies_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Strategies_Static.mak" CFG="TAO_Strategies_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_Strategies_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_Strategies_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_Strategies_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\Strategies"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /D "TAO_STRATEGIES_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Strategiess.lib"

!ELSEIF  "$(CFG)" == "TAO_Strategies_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_Strategies_Static___Win32_Debug"
# PROP BASE Intermediate_Dir "TAO_Strategies_Static___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\Strategies"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Strategiessd.lib"

!ENDIF 

# Begin Target

# Name "TAO_Strategies_Static - Win32 Static Release"
# Name "TAO_Strategies_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\advanced_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_Strategies_Internal.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Lite_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\advanced_resource.h
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Base.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\strategies_export.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Strategies_Internal.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Lite_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\advanced_resource.i
# End Source File
# Begin Source File

SOURCE=.\Direct_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Base.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_NonReactive_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\Linear_Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Reactor_Per_Priority.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.i
# End Source File
# End Group
# End Target
# End Project
