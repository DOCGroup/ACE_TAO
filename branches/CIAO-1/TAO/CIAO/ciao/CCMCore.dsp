# Microsoft Developer Studio Project File - Name="CCMCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CCMCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CCMCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CCMCore.mak" CFG="CCMCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CCMCore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CCMCore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CCMCORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "W:\ACE_wrappers" /I "W:\ACE_wrappers\TAO" /I "W:\ACE_wrappers\TAO\orbsvcs\orbsvcs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 aced.lib taod.lib /nologo /dll /machine:I386 /libpath:"W:\ACE_wrappers\ace" /libpath:"W:\ACE_wrappers\TAO\tao"

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CCMCORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "W:\ACE_wrappers" /I "W:\ACE_wrappers\TAO" /I "W:\ACE_wrappers\TAO\orbsvcs\orbsvcs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib taod.lib TAO_IFR_Clientd.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\IFR_Client"

!ENDIF 

# Begin Target

# Name "CCMCore - Win32 Release"
# Name "CCMCore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CIAO_BaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_ComponentC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_EventC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_TransactionC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CIAO_BaseC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_ComponentC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_EventC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_Export.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_TransactionC.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ciao.rc
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\CIAO_BaseC.i
# End Source File
# Begin Source File

SOURCE=.\CIAO_ComponentC.i
# End Source File
# Begin Source File

SOURCE=.\CIAO_EventC.i
# End Source File
# Begin Source File

SOURCE=.\CIAO_TransactionC.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl;pidl"
# Begin Source File

SOURCE=.\CIAO_Base.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Base.pidl
InputName=CIAO_Base

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Base.pidl
InputName=CIAO_Base

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Component.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Component.pidl
InputName=CIAO_Component

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Component.pidl
InputName=CIAO_Component

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Container.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Container.pidl
InputName=CIAO_Container

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Container.pidl
InputName=CIAO_Container

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Container_Ex.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Container_Ex.pidl
InputName=CIAO_Container_Ex

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Container_Ex.pidl
InputName=CIAO_Container_Ex

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Deployment.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Deployment.pidl
InputName=CIAO_Deployment

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Deployment.pidl
InputName=CIAO_Deployment

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Event.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Event.pidl
InputName=CIAO_Event

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Event.pidl
InputName=CIAO_Event

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIAO_Transaction.pidl

!IF  "$(CFG)" == "CCMCore - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Transaction.pidl
InputName=CIAO_Transaction

BuildCmds= \
	..\..\..\bin\release\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CCMCore - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Transaction.pidl
InputName=CIAO_Transaction

BuildCmds= \
	..\..\..\bin\tao_idl -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_Export -Wb,export_include=CIAO_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).pidl \
	del $(InputName)S.* \
	del $(InputName)S_T.* \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
