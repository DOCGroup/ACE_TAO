# Microsoft Developer Studio Project File - Name="FTORB_Utils" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FTORB_Utils - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "FTORB_Utils.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "FTORB_Utils.mak" CFG="FTORB_Utils - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "FTORB_Utils - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FTORB_Utils - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FTORB_Utils - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\FTORB_Utils"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D TAO_FT_ORB_UTILS_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FTORB_Utils.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Valuetype.lib TAO_IORManip.lib TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:windows /pdb:"..\..\..\lib\TAO_FTORB_Utils.pdb" /dll  /machine:I386 /out:"..\..\..\lib\TAO_FTORB_Utils.dll"

!ELSEIF  "$(CFG)" == "FTORB_Utils - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\FTORB_Utils"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D TAO_FT_ORB_UTILS_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FTORB_Utils.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Valuetyped.lib TAO_IORManipd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:windows /pdb:"..\..\..\lib\TAO_FTORB_Utilsd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\TAO_FTORB_Utilsd.dll"

!ENDIF

# Begin Target

# Name "FTORB_Utils - Win32 Release"
# Name "FTORB_Utils - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.cpp"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.cpp"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\Channel_Clients_T.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.h"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.h"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\Channel_Clients_T.i"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.inl"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.i"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.i"
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Channel_Clients_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=".\FT_CORBA_ORB.idl"

!IF  "$(CFG)" == "FTORB_Utils - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=FaultTolerance\FT_ORB_Utils_export.h -Wb,skel_export_include=tao\PortableServer\PolicyS.h -Gv -I..\.. -I..\..\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == "FTORB_Utils - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB
InputDir=.

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=FaultTolerance\FT_ORB_Utils_export.h -Wb,skel_export_include=tao\PortableServer\PolicyS.h -Gv -I..\.. -I..\..\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath)

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF

# End Source File
# End Group
# End Target
# End Project
