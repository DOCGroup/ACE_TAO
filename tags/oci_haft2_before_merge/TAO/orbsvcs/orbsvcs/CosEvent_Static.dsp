# Microsoft Developer Studio Project File - Name="CosEvent_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CosEvent_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CosEvent_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CosEvent_Static.mak" CFG="CosEvent_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CosEvent_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CosEvent_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CosEvent_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\CosEvent"
# PROP Target_Dir ""
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
# ADD LIB32 /nologo /out:"TAO_CosEvents.lib"

!ELSEIF  "$(CFG)" == "CosEvent_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\CosEvent"
# PROP Target_Dir ""
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
# ADD LIB32 /nologo /out:"TAO_CosEventsd.lib"

!ENDIF 

# Begin Target

# Name "CosEvent_Static - Win32 Static Release"
# Name "CosEvent_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\CosEventChannelAdmin.idl

!IF  "$(CFG)" == "CosEvent_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosEvent_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent\event_export.h $(InputName).idl

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

SOURCE=.\CosEventComm.idl

!IF  "$(CFG)" == "CosEvent_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CosEvent_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Event_Export -Wb,export_include=CosEvent\event_export.h $(InputName).idl

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

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerAdmin.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerControl.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Default_Factory.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching_Task.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_EventChannel.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Factory.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_MT_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullConsumer.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullSupplier.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushConsumer.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushSupplier.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Pulling_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_ConsumerControl.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_Pulling_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_SupplierControl.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierAdmin.i
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierControl.i
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.i
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.i
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.i
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.i
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerAdmin.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerControl.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Default_Factory.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Defaults.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching_Task.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_EventChannel.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_MT_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullConsumer.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullSupplier.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Pulling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_ConsumerControl.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_Pulling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_SupplierControl.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierAdmin.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierControl.h
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.h
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.h
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.h
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.h
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent\event_export.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ConsumerControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Default_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Dispatching_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_EventChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_MT_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPullSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_ProxyPushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Pulling_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_ConsumerControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_Pulling_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_Reactive_SupplierControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\CEC_SupplierControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.cpp
# End Source File
# End Group
# End Target
# End Project
