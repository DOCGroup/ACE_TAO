# Microsoft Developer Studio Project File - Name="AV_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AV_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AV_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AV_Static.mak" CFG="AV_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AV_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AV_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AV_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\AV"
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
# ADD LIB32 /nologo /out:"TAO_AVs.lib"

!ELSEIF  "$(CFG)" == "AV_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\AV"
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
# ADD LIB32 /nologo /out:"TAO_AVsd.lib"

!ENDIF 

# Begin Target

# Name "AV_Static - Win32 Static Release"
# Name "AV_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AVStreamsS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AV\Flows_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\AVStreams.idl

!IF  "$(CFG)" == "AV_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

SOURCE=.\Null_MediaCtrl.idl

!IF  "$(CFG)" == "AV_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

SOURCE=.\sfp.idl

!IF  "$(CFG)" == "AV_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

SOURCE=.\AV\AVStreams_i.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.i
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.i
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.i
# End Source File
# Begin Source File

SOURCE=.\sfpC.i
# End Source File
# Begin Source File

SOURCE=.\sfpS.i
# End Source File
# Begin Source File

SOURCE=.\sfpS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\source.i
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.i
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AV\AV_Core.h
# End Source File
# Begin Source File

SOURCE=.\AV\AV_export.h
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\Fill_ACE_QoS.h
# End Source File
# Begin Source File

SOURCE=.\AV\Flows_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.h
# End Source File
# Begin Source File

SOURCE=.\AV\global.h
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.h
# End Source File
# Begin Source File

SOURCE=.\AV\md5.h
# End Source File
# Begin Source File

SOURCE=.\AV\media_timer.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Protocol_Factory.h
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Channel.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Packet.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTP.h
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.h
# End Source File
# Begin Source File

SOURCE=.\sfpC.h
# End Source File
# Begin Source File

SOURCE=.\sfpS.h
# End Source File
# Begin Source File

SOURCE=.\AV\TCP.h
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.h
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AV\AV_Core.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i.cpp
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\default_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Fill_ACE_QoS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\md5c.c
# End Source File
# Begin Source File

SOURCE=.\AV\media_timer.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Nil.cpp
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Protocol_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.cpp
# End Source File
# Begin Source File

SOURCE=.\sfpC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\sfpS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\TCP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.cpp
# End Source File
# End Group
# End Target
# End Project
