# Microsoft Developer Studio Project File - Name="PortableServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PortableServer - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_PortableServer.mak" CFG="PortableServer - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PortableServer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableServer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableServer - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableServer - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PortableServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableServer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "TAO_PORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_ObjRefTemplate.lib TAO_Valuetype.lib TAO_IORInterceptor.lib ace.lib TAO.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableServer.dll" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableServer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "TAO_PORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_ObjRefTemplated.lib TAO_Valuetyped.lib TAO_IORInterceptord.lib TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableServerd.dll" /pdbtype:sept /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PortableServer___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "PortableServer___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableServerMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTSchedd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableServerd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableServermfcd.dll" /pdbtype:sept /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PortableServer___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "PortableServer___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableServerMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_EVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLESERVER_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTSched.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableServer.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfc.lib acemfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableServermfc.dll" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "PortableServer - Win32 Release"
# Name "PortableServer - Win32 Debug"
# Name "PortableServer - Win32 MFC Debug"
# Name "PortableServer - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

!IF  "$(CFG)" == "PortableServer - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableServer - Win32 Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Release"

!ENDIF 

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
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.cpp
# End Source File
# Begin Source File

SOURCE=.\ImR_ActivatorC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\ImR_ActivatorS.cpp
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorS.cpp
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

SOURCE=.\PortableGroup_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServer_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServer_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableServerC.cpp

!IF  "$(CFG)" == "PortableServer - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableServer - Win32 Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableServer - Win32 MFC Release"

!ENDIF 

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
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
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

SOURCE=.\ImR_ActivatorC.h
# End Source File
# Begin Source File

SOURCE=.\ImR_ActivatorS.h
# End Source File
# Begin Source File

SOURCE=.\ImR_ActivatorS_T.h
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorC.h
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorS.h
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorS_T.h
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

SOURCE=.\PolicyS.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\PortableServer.h
# End Source File
# Begin Source File

SOURCE=.\portableserver_export.h
# End Source File
# Begin Source File

SOURCE=.\PortableServer_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\PortableServer_PolicyFactory.h
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
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
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

SOURCE=.\ImR_ActivatorC.i
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorC.i
# End Source File
# Begin Source File

SOURCE=.\IORInfo.inl
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

SOURCE=.\POA_Cached_Policies.i
# End Source File
# Begin Source File

SOURCE=.\POA_Policies.i
# End Source File
# Begin Source File

SOURCE=.\POA_Policy_Set.i
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
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImplRepoS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ImR_ActivatorS_T.cpp
# End Source File
# Begin Source File

SOURCE=.\ImR_LocatorS_T.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\TAO_PortableServer.rc
# End Source File
# End Group
# End Target
# End Project
