# Microsoft Developer Studio Project File - Name="DsLogAdmin_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DsLogAdmin_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DsLogAdmin_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DsLogAdmin_Static.mak" CFG="DsLogAdmin_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DsLogAdmin_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DsLogAdmin_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\DsLogAdmin"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D ACE_OS_HAS_DLL=0 /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_DsLogAdmins.lib"

!ELSEIF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\DsLogAdmin"
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
# ADD LIB32 /nologo /out:"TAO_DsLogAdminsd.lib"

!ENDIF 

# Begin Target

# Name "DsLogAdmin_Static - Win32 Static Release"
# Name "DsLogAdmin_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DsLogAdminS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS_T.cpp
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\DsLogAdmin.idl

!IF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\DsLogAdmin.idl
InputName=DsLogAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log\log_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__DSLOG="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\DsLogAdmin.idl
InputName=DsLogAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log\log_export.h $(InputName).idl

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

SOURCE=.\DsLogNotification.idl

!IF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Release"

# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\DsLogNotification.idl
InputName=DsLogNotification

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log\log_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "DsLogAdmin_Static - Win32 Static Debug"

# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\DsLogNotification.idl
InputName=DsLogNotification

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Log_Export -Wb,export_include=Log\log_export.h $(InputName).idl

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

SOURCE=.\DsLogAdminC.i
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.i
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS_T.i
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationC.i
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS.i
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS_T.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Log\BasicLog_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\BasicLogFactory_i.h
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminC.h
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.h
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationC.h
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS.h
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS_T.h
# End Source File
# Begin Source File

SOURCE=.\Log\Iterator_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\Log\log_export.h
# End Source File
# Begin Source File

SOURCE=.\Log\Log_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\LogMgr_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\LogNotification.h
# End Source File
# Begin Source File

SOURCE=.\Log\LogRecordStore.h
# End Source File
# Begin Source File

SOURCE=.\Log\PersistStore.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Log\BasicLog_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\BasicLogFactory_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationC.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogNotificationS.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Iterator_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Log_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\LogMgr_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\LogNotification.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\LogRecordStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\PersistStore.cpp
# End Source File
# End Group
# End Target
# End Project
