# Microsoft Developer Studio Project File - Name="server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=server - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="server - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "server - Win32 MFC Release" (based on "Win32 (x86) Application")
!MESSAGE "server - Win32 MFC Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "server - Win32 MFC Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../" /I "../../" /I "../../orbsvcs" /D "_WINDOWS" /D "_AFXDLL" /D "WIN32" /D "NDEBUG" /D "ACE_HAS_MFC" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 acemfc.lib TAOmfc.lib TAO_CosNamingmfc.lib TAO_PortableServermfc.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../../ace" /libpath:"../../tao" /libpath:"../../orbsvcs/orbsvcs" /libpath:"../../tao/PortableServer"

!ELSEIF  "$(CFG)" == "server - Win32 MFC Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../" /I "../../" /I "../../orbsvcs" /D "_WINDOWS" /D "_AFXDLL" /D "WIN32" /D "_DEBUG" /D "ACE_HAS_MFC" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 acemfcd.lib TAOmfcd.lib TAO_CosNamingmfcd.lib TAO_PortableServermfcd.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../../ace" /libpath:"../../tao" /libpath:"../../orbsvcs/orbsvcs" /libpath:"../../tao/PortableServer"

!ENDIF 

# Begin Target

# Name "server - Win32 MFC Release"
# Name "server - Win32 MFC Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# Begin Source File

SOURCE=.\server.rc
# End Source File
# Begin Source File

SOURCE=.\serverDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\serverView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\w32_test_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\w32_testC.cpp
# End Source File
# Begin Source File

SOURCE=.\w32_testS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\server.h
# End Source File
# Begin Source File

SOURCE=.\serverDoc.h
# End Source File
# Begin Source File

SOURCE=.\serverView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\w32_test_impl.h
# End Source File
# Begin Source File

SOURCE=.\w32_testS.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\server.ico
# End Source File
# Begin Source File

SOURCE=.\res\server.rc2
# End Source File
# Begin Source File

SOURCE=.\res\serverDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\w32_test.idl

!IF  "$(CFG)" == "server - Win32 MFC Release"

# PROP Ignore_Default_Tool 1
USERDEP__W32_T="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Compiling $(InputPath)
InputPath=.\w32_test.idl
InputName=w32_test

BuildCmds= \
	..\..\..\bin\Release\tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "server - Win32 MFC Debug"

# PROP Ignore_Default_Tool 1
USERDEP__W32_T="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Compiling $(InputPath)
InputPath=.\w32_test.idl
InputName=w32_test

BuildCmds= \
	..\..\..\bin\tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
