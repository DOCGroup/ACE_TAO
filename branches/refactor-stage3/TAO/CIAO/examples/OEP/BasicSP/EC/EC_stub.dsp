# Microsoft Developer Studio Project File - Name="EC_stub DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EC_stub DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EC_stub.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EC_stub.mak" CFG="EC_stub DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EC_stub DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EC_stub DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EC_stub DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\EC_stub"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D EC_STUB_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 BasicSP_stub.lib TAO.lib TAO_Security.lib TAO_IFR_Client.lib TAO_Valuetype.lib CIAO_Client.lib ACE.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\..\..\..\bin\EC_stub.pdb" /machine:I386 /out:"..\..\..\..\..\..\bin\EC_stub.dll" /libpath:".." /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EC_stub DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\EC_stub"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D EC_STUB_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 BasicSP_stubd.lib TAOd.lib TAO_Securityd.lib TAO_IFR_Clientd.lib TAO_Valuetyped.lib CIAO_Clientd.lib ACEd.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"..\..\..\..\..\..\bin\EC_stubd.pdb" /debug /machine:I386 /out:"..\..\..\..\..\..\bin\EC_stubd.dll" /libpath:".." /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "EC_stub DLL - Win32 Release"
# Name "EC_stub DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\ECC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\ECC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\ECC.i
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\EC.idl

!IF  "$(CFG)" == "EC_stub DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\EC.idl
InputName=EC

BuildCmds= \
	..\..\..\..\..\..\bin\tao_idl -o . -Wb,stub_export_macro=EC_STUB_Export -Wb,stub_export_include=EC_stub_export.h -Wb,skel_export_macro=EC_SVNT_Export -Wb,skel_export_include=EC_svnt_export.h -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\ciao -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

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

!ELSEIF  "$(CFG)" == "EC_stub DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\EC.idl
InputName=EC

BuildCmds= \
	..\..\..\..\..\..\bin\tao_idl -o . -Wb,stub_export_macro=EC_STUB_Export -Wb,stub_export_include=EC_stub_export.h -Wb,skel_export_macro=EC_SVNT_Export -Wb,skel_export_include=EC_svnt_export.h -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\ciao -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

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
