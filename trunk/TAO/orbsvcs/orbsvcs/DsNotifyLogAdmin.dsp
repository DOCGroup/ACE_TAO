# Microsoft Developer Studio Project File - Name="DsNotifyLogAdmin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DsNotifyLogAdmin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DsNotifyLogAdmin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DsNotifyLogAdmin.mak" CFG="DsNotifyLogAdmin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DsNotifyLogAdmin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DsNotifyLogAdmin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DsNotifyLogAdmin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\DsNotifyLogAdmin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSNOTIFYLOGADMIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "__MBCS" /D "_USRDLL" /D "TAO_ORBSVCS_HAS_NAMING" /D "_WINDOWS" /D "TAO_NOTIFYLOG_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_CosNaming.lib TAO.lib ace.lib TAO_PortableServer.lib TAO_DsLogAdmin.lib TAO_DsEventLogAdmin.lib TAO_CosNotification.lib TAO_CosEvent.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_DsNotifyLogAdmin.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "DsNotifyLogAdmin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DsNotifyLogAdmin___Win32_Debug0"
# PROP BASE Intermediate_Dir "DsNotifyLogAdmin___Win32_Debug0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\DsNotifyLogAdmin"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DSNOTIFYLOGADMIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR- /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "TAO_ORBSVCS_HAS_NAMING" /D "WIN32" /D "_WINDOWS" /D "TAO_NOTIFYLOG_BUILD_DLL" /FR /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_CosNamingd.lib TAOd.lib aced.lib TAO_PortableServerd.lib TAO_DsLogAdmind.lib TAO_DsEventLogAdmind.lib TAO_CosNotificationd.lib TAO_CosEventd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_DsNotifyLogAdmind.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DsNotifyLogAdmin - Win32 Release"
# Name "DsNotifyLogAdmin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DsNotifyLogAdminC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\DsNotifyLogAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLog_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogFactory_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogNotification.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DsNotifyLogAdminC.h
# End Source File
# Begin Source File

SOURCE=.\DsNotifyLogAdminS.h
# End Source File
# Begin Source File

SOURCE=.\DsNotifyLogAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\Log\notifylog_export.h
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLog_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogFactory_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\NotifyLogNotification.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DsNotifyLogAdmin.idl

!IF  "$(CFG)" == "DsNotifyLogAdmin - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__DSNOT="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\DsNotifyLogAdmin.idl
InputName=DsNotifyLogAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_NotifyLog_Export -Wb,export_include=Log\notifylog_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "DsNotifyLogAdmin - Win32 Debug"

# PROP Intermediate_Dir "DLL\Debug\DsNotifyLogAdmin"
# PROP Ignore_Default_Tool 1
USERDEP__DSNOT="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\DsNotifyLogAdmin.idl
InputName=DsNotifyLogAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_NotifyLog_Export -Wb,export_include=Log\notifylog_export.h $(InputName).idl

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

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DsNotifyLogAdminC.i
# End Source File
# Begin Source File

SOURCE=.\DsNotifyLogAdminS.i
# End Source File
# Begin Source File

SOURCE=.\DsNotifyLogAdminS_T.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DsNotifyLogAdminS_T.cpp
# End Source File
# End Group
# End Target
# End Project
