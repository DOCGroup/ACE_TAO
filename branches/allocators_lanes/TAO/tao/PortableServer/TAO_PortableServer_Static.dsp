# Microsoft Developer Studio Project File - Name="PortableServer_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PortableServer_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableServer_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableServer_Static.mak" CFG="PortableServer_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PortableServer_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PortableServer_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PortableServer_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\PortableServer"
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
# ADD LIB32 /nologo /out:"TAO_PortableServers.lib"

!ELSEIF  "$(CFG)" == "PortableServer_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\PortableServer"
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
# ADD LIB32 /nologo /out:"TAO_PortableServersd.lib"

!ENDIF 

# Begin Target

# Name "PortableServer_Static - Win32 Static Release"
# Name "PortableServer_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImplRepoS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\Active_Object_Map.i
# End Source File
# Begin Source File

SOURCE=.\Collocated_Object.i
# End Source File
# Begin Source File

SOURCE=.\Default_ORTC.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.i
# End Source File
# Begin Source File

SOURCE=.\Key_Adapters.i
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.i
# End Source File
# Begin Source File

SOURCE=.\POA.i
# End Source File
# Begin Source File

SOURCE=.\POAManager.i
# End Source File
# Begin Source File

SOURCE=.\PortableServerC.i
# End Source File
# Begin Source File

SOURCE=.\Servant_Base.i
# End Source File
# Begin Source File

SOURCE=.\ServerInterceptorAdapter.inl
# End Source File
# Begin Source File

SOURCE=.\ServerRequestInfo.inl
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Active_Object_Map.h
# End Source File
# Begin Source File

SOURCE=.\AMH_Response_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Collocated_Object.h
# End Source File
# Begin Source File

SOURCE=.\Default_Acceptor_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Default_ORTC.h
# End Source File
# Begin Source File

SOURCE=.\Default_Policy_Validator.h
# End Source File
# Begin Source File

SOURCE=.\Default_Servant_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Direct_Object_Proxy_Impl.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepo_i.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.h
# End Source File
# Begin Source File

SOURCE=.\IORInfo.h
# End Source File
# Begin Source File

SOURCE=.\Key_Adapters.h
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\ObjectReferenceFactory.h
# End Source File
# Begin Source File

SOURCE=.\ObjectReferenceTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Manager.h
# End Source File
# Begin Source File

SOURCE=.\POA.h
# End Source File
# Begin Source File

SOURCE=.\POA_Cached_Policies.h
# End Source File
# Begin Source File

SOURCE=.\POA_CORBA.h
# End Source File
# Begin Source File

SOURCE=.\poa_macros.h
# End Source File
# Begin Source File

SOURCE=.\POA_Policies.h
# End Source File
# Begin Source File

SOURCE=.\POA_Policy_Set.h
# End Source File
# Begin Source File

SOURCE=.\POAManager.h
# End Source File
# Begin Source File

SOURCE=.\PortableServer.h
# End Source File
# Begin Source File

SOURCE=.\portableserver_export.h
# End Source File
# Begin Source File

SOURCE=.\PortableServerC.h
# End Source File
# Begin Source File

SOURCE=.\Servant_Base.h
# End Source File
# Begin Source File

SOURCE=.\Servant_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\ServerInterceptorAdapter.h
# End Source File
# Begin Source File

SOURCE=.\ServerRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\Strategized_Object_Proxy_Broker.h
# End Source File
# Begin Source File

SOURCE=.\ThruPOA_Object_Proxy_Impl.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Active_Object_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\AMH_Response_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Collocated_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Acceptor_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_ORTC.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Policy_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\Default_Servant_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Direct_Object_Proxy_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplRepo_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.cpp
# End Source File
# Begin Source File

SOURCE=.\IORInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Key_Adapters.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectReferenceFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectReferenceTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\POA.cpp
# End Source File
# Begin Source File

SOURCE=.\POA_Cached_Policies.cpp
# End Source File
# Begin Source File

SOURCE=.\POA_Policies.cpp
# End Source File
# Begin Source File

SOURCE=.\POA_Policy_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\POAManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PolicyS.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServerC.cpp
# End Source File
# Begin Source File

SOURCE=.\Servant_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Servant_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerInterceptorAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerRequestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Strategized_Object_Proxy_Broker.cpp
# End Source File
# Begin Source File

SOURCE=.\ThruPOA_Object_Proxy_Impl.cpp
# End Source File
# End Group
# End Target
# End Project
