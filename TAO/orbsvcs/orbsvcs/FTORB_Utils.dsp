# Microsoft Developer Studio Project File - Name="FTORB_Utils" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FTORB_Utils - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
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
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FTORB_Utils - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin\Release"
# PROP Intermediate_Dir "Release\FTORB_Utils"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_FT_ORB_UTILS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FTORB_Utils.bsc"
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Valuetype.lib TAO_IORManip.lib TAO.lib ACE.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\lib\TAO_FTORB_Utils.pdb" /machine:I386 /out:"..\..\..\bin\Release\\TAO_FTORB_Utils.dll" /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORManipulation" /version:1.3.5
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FTORB_Utils - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "Debug\FTORB_Utils"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_FT_ORB_UTILS_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FTORB_Utils.bsc"
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Valuetyped.lib TAO_IORManipd.lib TAOd.lib ACEd.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"..\..\..\lib\TAO_FTORB_Utilsd.pdb" /debug /machine:I386 /out:"..\..\..\bin\TAO_FTORB_Utilsd.dll" /libpath:"..\..\tao\IORInterceptor" /libpath:"..\..\tao\ObjRefTemplate" /libpath:"..\..\tao\Valuetype" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\PortableServer" /version:1.3.5
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "FTORB_Utils - Win32 Release"
# Name "FTORB_Utils - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.cpp"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\Channel_Clients_T.h"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.h"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\Channel_Clients_T.i"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBC.i"
# End Source File
# Begin Source File

SOURCE=".\FT_CORBA_ORBS.i"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_IOGR_Property.inl"
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
InputPath=".\FT_CORBA_ORB.idl"
InputName=FT_CORBA_ORB

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=FaultTolerance\FT_ORB_Utils_export.h -Wb,skel_export_include=tao\PortableServer\PolicyS.h -Gv -I..\.. -I..\..\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

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
InputPath=".\FT_CORBA_ORB.idl"
InputName=FT_CORBA_ORB

BuildCmds= \
	PATH=%PATH%;..\..\..\lib \
	..\..\..\bin\tao_idl -o . -Wb,export_macro=TAO_FT_ORB_Utils_Export -Wb,export_include=FaultTolerance\FT_ORB_Utils_export.h -Wb,skel_export_include=tao\PortableServer\PolicyS.h -Gv -I..\.. -I..\..\orbsvcs -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

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
