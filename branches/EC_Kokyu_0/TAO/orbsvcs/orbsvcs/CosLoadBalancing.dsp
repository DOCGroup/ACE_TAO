# Microsoft Developer Studio Project File - Name="CosLoadBalancing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CosLoadBalancing - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CosLoadBalancing.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CosLoadBalancing.mak" CFG="CosLoadBalancing - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CosLoadBalancing - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosLoadBalancing - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosLoadBalancing - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosLoadBalancing - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CosLoadBalancing - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\CosLoadBalancing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CosLoadBalancing_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_LOADBALANCING_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_CosNaming.lib TAO_PortableGroup.lib TAO_Messaging.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancing.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\CosLoadBalancing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CosLoadBalancing_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_LOADBALANCING_BUILD_DLL" /D "TAO_ORBSVCS_HAS_LOADBALANCING" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib TAO_PortableServerd.lib TAO_CosNamingd.lib TAO_PortableGroupd.lib TAO_Messagingd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancingd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CosLoadBalancing___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "CosLoadBalancing___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\CosLoadBalancing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_LOADBALANCING_BUILD_DLL" /D "TAO_ORBSVCS_HAS_LOADBALANCING" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_LOADBALANCING_BUILD_DLL" /D "TAO_ORBSVCS_HAS_LOADBALANCING" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancingd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 TAOd.lib aced.lib TAO_PortableServermfcd.lib TAO_CosNamingmfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancingmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CosLoadBalancing___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "CosLoadBalancing___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\CosLoadBalancingMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_LOADBALANCING_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_LOADBALANCING_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancing.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib TAO_PortableServermfc.lib TAO_CosNamingmfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_CosLoadBalancingmfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "CosLoadBalancing - Win32 Release"
# Name "CosLoadBalancing - Win32 Debug"
# Name "CosLoadBalancing - Win32 MFC Debug"
# Name "CosLoadBalancing - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CosLoadBalancingC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientRequestInterceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Component.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_CPU_Monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_IORInterceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LeastLoaded.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlertInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlertMap.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadListMap.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadMap.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Location_Index_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_MemberLocator.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_MonitorMap.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ObjectReferenceFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\LB_ORTC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Pull_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Random.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_RoundRobin.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ServerRequestInterceptor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\CosLoadBalancingC.h
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS.h
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS_T.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientComponent.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ClientRequestInterceptor.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Component.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_conf.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_CPU_Monitor.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_IORInterceptor.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LeastLoaded.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert_Handler.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlertInfo.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlertMap.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadListMap.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadManager.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadMap.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Location_Index_Map.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_MemberLocator.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_MonitorMap.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ObjectReferenceFactory.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\LB_ORTC.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Pull_Handler.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_Random.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_RoundRobin.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_ServerRequestInterceptor.h
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancing\LoadBalancing_export.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\CosLoadBalancing.idl

!IF  "$(CFG)" == "CosLoadBalancing - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSLO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/Loadbalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/Loadbalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LB_ORT.idl

!IF  "$(CFG)" == "CosLoadBalancing - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__LB_OR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/Loadbalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/Loadbalancing_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\CosLoadBalancingC.i
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS.i
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS_T.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LeastLoaded.inl
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert.inl
# End Source File
# Begin Source File

SOURCE=.\LB_ORTC.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosLoadBalancingS_T.cpp

!IF  "$(CFG)" == "CosLoadBalancing - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CosLoadBalancing - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosLoadBalancing.rc
# End Source File
# End Group
# End Target
# End Project
