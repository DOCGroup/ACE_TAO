# Microsoft Developer Studio Project File - Name="BMClosedED_exec DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BMClosedED_exec DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BMClosedED_exec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BMClosedED_exec.mak" CFG="BMClosedED_exec DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BMClosedED_exec DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BMClosedED_exec DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BMClosedED_exec DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\BMClosedED_exec"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D BMCLOSEDED_EXEC_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 BMClosedED_stub.lib BMClosedED_svnt.lib BasicSP_stub.lib BasicSP_svnt.lib CIAO_Server.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib TAO_Valuetype.lib CIAO_Client.lib ACE.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\..\..\..\bin\BMClosedED_exec.pdb" /machine:I386 /out:"..\..\..\..\..\..\bin\BMClosedED_exec.dll" /libpath:".." /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BMClosedED_exec DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\BMClosedED_exec"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D BMCLOSEDED_EXEC_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 BMClosedED_stubd.lib BMClosedED_svntd.lib BasicSP_stubd.lib BasicSP_svntd.lib CIAO_Serverd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib TAO_Valuetyped.lib CIAO_Clientd.lib ACEd.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"..\..\..\..\..\..\bin\BMClosedED_execd.pdb" /debug /machine:I386 /out:"..\..\..\..\..\..\bin\BMClosedED_execd.dll" /libpath:".." /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "BMClosedED_exec DLL - Win32 Release"
# Name "BMClosedED_exec DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\BMClosedED_exec.cpp
# End Source File
# Begin Source File

SOURCE=.\BMClosedEDEIC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\BMClosedED_exec.h
# End Source File
# Begin Source File

SOURCE=.\BMClosedEDEIC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\BMClosedEDEIC.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\BMClosedEDEI.idl

!IF  "$(CFG)" == "BMClosedED_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__BMCLO="..\..\..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler on $(InputPath)
InputPath=.\BMClosedEDEI.idl
InputName=BMClosedEDEI

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\..\..\bin\tao_idl -o . -Wb,export_macro=BMCLOSEDED_EXEC_Export -Wb,export_include=BMClosedED_exec_export.h -I..\..\..\..\.. -I..\..\..\..\..\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "BMClosedED_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__BMCLO="..\..\..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler on $(InputPath)
InputPath=.\BMClosedEDEI.idl
InputName=BMClosedEDEI

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\..\..\bin\tao_idl -o . -Wb,export_macro=BMCLOSEDED_EXEC_Export -Wb,export_include=BMClosedED_exec_export.h -I..\..\..\..\.. -I..\..\..\..\..\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
