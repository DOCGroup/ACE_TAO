# Microsoft Developer Studio Project File - Name="Svc_Utils_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Svc_Utils_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Svc_Utils_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Svc_Utils_Static.mak" CFG="Svc_Utils_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Svc_Utils_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Svc_Utils_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\Svc_Utils"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Svc_Utilss.lib"

!ELSEIF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\Svc_Utils"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Svc_Utilssd.lib"

!ENDIF 

# Begin Target

# Name "Svc_Utils_Static - Win32 Static Release"
# Name "Svc_Utils_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RtecBaseS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.cpp

!IF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Release"

!ELSEIF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\RtecBase.idl

!IF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\RtecBase.idl
InputName=RtecBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECB="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\RtecBase.idl
InputName=RtecBase

BuildCmds= \
	..\..\..\bin\tao_idl_static  -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(InputName).idl

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

SOURCE=.\TimeBase.idl

!IF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "Svc_Utils_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEB="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl_static  -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_Svc_Utils_Export -Wb,export_include=svc_utils_export.h $(InputName).idl

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

SOURCE=.\RtecBaseC.i
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS.i
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS_T.i
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\IOR_Multicast.h
# End Source File
# Begin Source File

SOURCE=.\RtecBaseC.h
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS.h
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS_T.h
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\IOR_Multicast.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp
# End Source File
# End Group
# End Target
# End Project
