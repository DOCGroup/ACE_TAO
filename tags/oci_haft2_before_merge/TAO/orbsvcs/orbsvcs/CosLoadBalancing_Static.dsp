# Microsoft Developer Studio Project File - Name="CosLoadBalancing_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CosLoadBalancing_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CosLoadBalancing_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CosLoadBalancing_Static.mak" CFG="CosLoadBalancing_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CosLoadBalancing_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CosLoadBalancing_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\CosLoadBalancing"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_CosLoadBalancings.lib"

!ELSEIF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\CosLoadBalancing"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_CosLoadBalancingsd.lib"

!ENDIF 

# Begin Target

# Name "CosLoadBalancing_Static - Win32 Static Release"
# Name "CosLoadBalancing_Static - Win32 Static Debug"
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\CosLoadBalancing.idl

!IF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing\LoadBalancing_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLO="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosLoadBalancing.idl
InputName=CosLoadBalancing

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing\LoadBalancing_export.h $(InputName).idl

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

!IF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Release"

# Begin Custom Build
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -Gv -DCORBA3 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosLoadBalancing_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\LB_ORT.idl
InputName=LB_ORT

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -Gv -DCORBA3 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h $(InputName).idl

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

SOURCE=.\LoadBalancing\LB_LeastLoaded.inl
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert.inl
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadManager.inl
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CosLoadBalancingC.h
# End Source File
# Begin Source File

SOURCE=.\CosLoadBalancingS.h
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

SOURCE=.\LoadBalancing\LB_LoadAlert_Factory.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert_Handler.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LB_LoadAlert_ReplyHandler.h
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
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
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
# End Target
# End Project
