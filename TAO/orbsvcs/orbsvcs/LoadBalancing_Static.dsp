# Microsoft Developer Studio Project File - Name="LoadBalancing_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LoadBalancing_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LoadBalancing_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LoadBalancing_Static.mak" CFG="LoadBalancing_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LoadBalancing_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LoadBalancing_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LoadBalancing_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\LoadBalancing"
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
# ADD LIB32 /nologo /out:"TAO_LoadBalancings.lib"

!ELSEIF  "$(CFG)" == "LoadBalancing_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\LoadBalancing"
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
# ADD LIB32 /nologo /out:"TAO_LoadBalancingsd.lib"

!ENDIF 

# Begin Target

# Name "LoadBalancing_Static - Win32 Static Release"
# Name "LoadBalancing_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LoadBalancingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\LoadBalancing.idl

!IF  "$(CFG)" == "LoadBalancing_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\LoadBalancing.idl
InputName=LoadBalancing

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing\loadbalancing_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "LoadBalancing_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LOADB="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\LoadBalancing.idl
InputName=LoadBalancing

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_LoadBalancing_Export -Wb,export_include=LoadBalancing/LoadBalancing_export.h $(InputName).idl

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

SOURCE=.\LoadBalancing\LoadBalancer_i.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancing_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingC.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingS.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingS_T.i
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\ReplicaProxy.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancer_i.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancing_export.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancing_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingC.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingS.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingS_T.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\Minimum_Dispersion.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\ReplicaLocator.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\ReplicaProxy.h
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\Round_Robin_Strategy.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancer_i.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\LoadBalancing_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingC.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancingS.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\Minimum_Dispersion.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\ReplicaLocator.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\ReplicaProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadBalancing\Round_Robin_Strategy.cpp
# End Source File
# End Group
# End Target
# End Project
