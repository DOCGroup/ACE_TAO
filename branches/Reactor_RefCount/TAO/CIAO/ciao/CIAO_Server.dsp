# Microsoft Developer Studio Project File - Name="CIAO_Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CIAO_Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CIAO_Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CIAO_Server.mak" CFG="CIAO_Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CIAO_Server - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CIAO_Server - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CIAO_Server - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_SERVER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I ".." /I "..\..\.." /I "..\.." /I "..\..\orbsvcs\orbsvcs" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_SERVER_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib tao.lib TAO_IFR_Client.lib CIAO_Client.lib CIAO_Container.lib TAO_Security.lib TAO_PortableServer.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\CIAO_Server.dll" /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\orbsvcs\orbsvcs" /libpath:"..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "CIAO_Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CIAO_Server___Win32_Debug"
# PROP BASE Intermediate_Dir "CIAO_Server___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_SERVER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I ".." /I "..\..\.." /I "..\.." /I "..\..\orbsvcs\orbsvcs" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CIAO_SERVER_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib taod.lib TAO_IFR_Clientd.lib CIAO_Clientd.lib CIAO_Containerd.lib TAO_Securityd.lib TAO_PortableServerd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\CIAO_Serverd.dll" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:"..\..\tao" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\orbsvcs\orbsvcs" /libpath:"..\..\tao\PortableServer"

!ENDIF 

# Begin Target

# Name "CIAO_Server - Win32 Release"
# Name "CIAO_Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CCM_DeploymentC.cpp
# End Source File
# Begin Source File

SOURCE=.\CCM_DeploymentS.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersC.cpp
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersS.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentInstallation_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentServer_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Container_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Container_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_init.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerActivator_Impl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\CCM_DeploymentC.h
# End Source File
# Begin Source File

SOURCE=.\CCM_DeploymentS.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersC.h
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersS.h
# End Source File
# Begin Source File

SOURCE=.\ComponentInstallation_Impl.h
# End Source File
# Begin Source File

SOURCE=.\ComponentServer_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Container_Base.h
# End Source File
# Begin Source File

SOURCE=.\Container_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Server_init.h
# End Source File
# Begin Source File

SOURCE=.\ServerActivator_Impl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCM_Deployment.idl

!IF  "$(CFG)" == "CIAO_Server - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CCM_Deployment.idl
InputName=CCM_Deployment

BuildCmds= \
	..\..\..\bin\release\tao_idl -Ge 1 -Sc -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_SERVER_Export -Wb,export_include=CIAO_SERVER_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CIAO_Server - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CCM_Deployment.idl
InputName=CCM_Deployment

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -Sc -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_SERVER_Export -Wb,export_include=CIAO_SERVER_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).idl

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

SOURCE=.\CIAO_Servers.idl

!IF  "$(CFG)" == "CIAO_Server - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Servers.idl
InputName=CIAO_Servers

BuildCmds= \
	..\..\..\bin\release\tao_idl -Ge 1 -Sc -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_SERVER_Export -Wb,export_include=CIAO_SERVER_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).idl

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

!ELSEIF  "$(CFG)" == "CIAO_Server - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputName)
InputPath=.\CIAO_Servers.idl
InputName=CIAO_Servers

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -Sc -I ../.. -I ../../orbsvcs/orbsvcs -Wb,export_macro=CIAO_SERVER_Export -Wb,export_include=CIAO_SERVER_export.h -Wb,pre_include="ace/pre.h" -Wb,post_include="ace/post.h" $(InputName).idl

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

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\CCM_DeploymentC.i
# End Source File
# Begin Source File

SOURCE=.\CCM_DeploymentS.i
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersC.i
# End Source File
# Begin Source File

SOURCE=.\CIAO_ServersS.i
# End Source File
# Begin Source File

SOURCE=.\ComponentInstallation_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\ComponentServer_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\Container_Base.inl
# End Source File
# Begin Source File

SOURCE=.\Container_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\ServerActivator_Impl.inl
# End Source File
# End Group
# End Target
# End Project
