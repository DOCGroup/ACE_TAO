# Microsoft Developer Studio Project File - Name="CIAO_Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CIAO_Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CIAO_Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CIAO_Client.mak" CFG="CIAO_Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CIAO_Client - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CIAO_Client - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CIAO_Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_CLIENT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\..\.." /I "..\.." /I "..\..\orbsvcs\orbsvcs" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_CLIENT_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib tao.lib TAO_IFR_Client.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\CIAO_Client.dll" /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\IFR_Client"

!ELSEIF  "$(CFG)" == "CIAO_Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CIAO_Client___Win32_Debug"
# PROP BASE Intermediate_Dir "CIAO_Client___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_CLIENT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\..\.." /I "..\.." /I "..\..\orbsvcs\orbsvcs" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_CLIENT_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib taod.lib TAO_IFR_Clientd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\CIAO_Clientd.dll" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\IFR_Client"

!ENDIF 

# Begin Target

# Name "CIAO_Client - Win32 Release"
# Name "CIAO_Client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CIAO_BaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_Component_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_ComponentC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_EventC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CIAO_BaseC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_CLIENT_Export.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_Component_i.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_ComponentC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_EventC.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ciao.rc
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\CIAO_Base.pidl

!IF  "$(CFG)" == "CIAO_Client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Base.pidl
InputName=CIAO_Base

BuildCmds= \
	..\..\..\bin\release\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ELSEIF  "$(CFG)" == "CIAO_Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Base.pidl
InputName=CIAO_Base

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Component.pidl

!IF  "$(CFG)" == "CIAO_Client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Component.pidl
InputName=CIAO_Component

BuildCmds= \
	..\..\..\bin\release\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ELSEIF  "$(CFG)" == "CIAO_Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Component.pidl
InputName=CIAO_Component

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Event.pidl

!IF  "$(CFG)" == "CIAO_Client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Event.pidl
InputName=CIAO_Event

BuildCmds= \
	..\..\..\bin\release\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ELSEIF  "$(CFG)" == "CIAO_Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Event.pidl
InputName=CIAO_Event

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_CLIENT_Export -Wb,export_include=CIAO_CLIENT_export.h -Wb,pre_include="ace/pre.h" -Wb,skel_export_macro=CIAO_CONTAINER_Export -Wb,skel_export_include=CIAO_CONTAINER_export.h -Wb,post_include="ace/post.h" $(InputName).pidl

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
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
