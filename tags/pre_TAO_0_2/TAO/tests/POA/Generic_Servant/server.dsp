# Microsoft Developer Studio Project File - Name="POA Generic Servant DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=POA Generic Servant DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="POA Generic Servant DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "POA Generic Servant DLL - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "POA Generic Servant DLL - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "POA Generic Servant DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "server__"
# PROP BASE Intermediate_Dir "server__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\.." /I "..\..\..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D GENERIC_SERVANT_HAS_DLL=1 /D "GENERIC_SERVANT_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib tao.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Generic_Servant.lib" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Desc=Copying DLL to ..\Explicit_Activation and\
      ..\On_Demand_Activation
PostBuild_Cmds=copy Generic_Servant.dll ..\Explicit_Activation	copy\
 Generic_Servant.dll ..\On_Demand_Activation
# End Special Build Tool

!ELSEIF  "$(CFG)" == "POA Generic Servant DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "server_0"
# PROP BASE Intermediate_Dir "server_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /I "..\..\..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D GENERIC_SERVANT_HAS_DLL=1 /D "GENERIC_SERVANT_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Generic_Servantd.dll" /pdbtype:sept /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Desc=Copying DLL to destinations
PostBuild_Cmds=copy Generic_Servantd.dll ..\Explicit_Activation	copy\
 Generic_Servantd.dll ..\On_Demand_Activation
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "POA Generic Servant DLL - Win32 Release"
# Name "POA Generic Servant DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FooC.cpp
# End Source File
# Begin Source File

SOURCE=.\FooS.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFooServant.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\FooC.h
# End Source File
# Begin Source File

SOURCE=.\FooS.h
# End Source File
# Begin Source File

SOURCE=.\FooS_T.h
# End Source File
# Begin Source File

SOURCE=.\MyFooServant.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Foo.idl

!IF  "$(CFG)" == "POA Generic Servant DLL - Win32 Release"

# Begin Custom Build
InputPath=.\Foo.idl
InputName=Foo

BuildCmds= \
	..\..\..\tao_idl\tao_idl -Wb,export_macro=GENERIC_SERVANT_Export\
      -Wb,export_include=generic_servant_export.h $(InputName).idl

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "POA Generic Servant DLL - Win32 Debug"

USERDEP__FOO_I="..\..\..\tao_idl\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Foo.idl
InputName=Foo

BuildCmds= \
	..\..\..\tao_idl\tao_idl -Wb,export_macro=GENERIC_SERVANT_Export\
      -Wb,export_include=generic_servant_export.h $(InputName).idl

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
# End Target
# End Project
