# Microsoft Developer Studio Project File - Name="NavDisplayGUI_exec DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NavDisplayGUI_exec DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NavDisplayGUI_exec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NavDisplayGUI_exec.mak" CFG="NavDisplayGUI_exec DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NavDisplayGUI_exec DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NavDisplayGUI_exec DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release\NavDisplayGUI_exec"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\NavDisplay" /I "$(QTDIR)\include" /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D NAVDISPLAY_EXEC_BUILD_DLL=1 /D QT_DLL=1 /D QT_THREAD_SUPPORT=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\NavDisplay" /i "$(QTDIR)\include" /i "..\..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib qt-mt230nc.lib NavDisplay_stub.lib NavDisplay_svnt.lib HUDisplay_stub.lib HUDisplay_svnt.lib CIAO_Server.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Security.lib CIAO_Container.lib TAO.lib TAO_IFR_Client.lib TAO_Valuetype.lib CIAO_Client.lib ACE.lib /nologo /subsystem:windows /dll /pdb:"..\..\..\..\..\..\bin\NavDisplayGUI_exec.pdb" /machine:I386 /out:"..\..\..\..\..\..\bin\NavDisplayGUI_exec.dll" /libpath:".." /libpath:"..\NavDisplay" /libpath:"$(QTDIR)\lib" /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\NavDisplayGUI_exec"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\NavDisplay" /I "$(QTDIR)\include" /I "..\..\..\..\..\orbsvcs\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\ciao" /I "..\..\..\..\.." /I "..\..\..\..\..\tao" /I "..\..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D NAVDISPLAY_EXEC_BUILD_DLL=1 /D QT_DLL=1 /D QT_THREAD_SUPPORT=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\NavDisplay" /i "$(QTDIR)\include" /i "..\..\..\..\..\orbsvcs\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\ciao" /i "..\..\..\..\.." /i "..\..\..\..\..\tao" /i "..\..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib qt-mt230nc.lib NavDisplay_stubd.lib NavDisplay_svntd.lib HUDisplay_stubd.lib HUDisplay_svntd.lib CIAO_Serverd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Securityd.lib CIAO_Containerd.lib TAOd.lib TAO_IFR_Clientd.lib TAO_Valuetyped.lib CIAO_Clientd.lib ACEd.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"..\..\..\..\..\..\bin\NavDisplayGUI_execd.pdb" /debug /machine:I386 /out:"..\..\..\..\..\..\bin\NavDisplayGUI_execd.dll" /libpath:".." /libpath:"..\NavDisplay" /libpath:"$(QTDIR)\lib" /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\ciao" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NavDisplayGUI_exec DLL - Win32 Release"
# Name "NavDisplayGUI_exec DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\AddNavUnitCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailView.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailView_moc.cpp
# End Source File
# Begin Source File

SOURCE=.\MapView.cpp
# End Source File
# Begin Source File

SOURCE=.\MapView_moc.cpp
# End Source File
# Begin Source File

SOURCE=.\NavDisplayEIC.cpp
# End Source File
# Begin Source File

SOURCE=.\NavDisplayGUI_exec.cpp
# End Source File
# Begin Source File

SOURCE=.\NavUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeItem_moc.cpp
# End Source File
# Begin Source File

SOURCE=.\QuitCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\RootPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\RootPanel_moc.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdatePositionCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\Worker.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\AddNavUnitCmd.h
# End Source File
# Begin Source File

SOURCE=.\DetailView.h
# End Source File
# Begin Source File

SOURCE=.\MapView.h
# End Source File
# Begin Source File

SOURCE=.\NavDisplayEIC.h
# End Source File
# Begin Source File

SOURCE=.\NavDisplayGUI_exec.h
# End Source File
# Begin Source File

SOURCE=.\NavUnit.h
# End Source File
# Begin Source File

SOURCE=.\NodeItem.h
# End Source File
# Begin Source File

SOURCE=.\QuitCmd.h
# End Source File
# Begin Source File

SOURCE=.\RootPanel.h
# End Source File
# Begin Source File

SOURCE=.\UpdatePositionCmd.h
# End Source File
# Begin Source File

SOURCE=.\Worker.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\NavDisplayEIC.i
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

SOURCE=.\..\NavDisplay\NavDisplayEI.idl

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\..\NavDisplay\NavDisplayEI.idl
InputName=NavDisplayEI

BuildCmds= \
	..\..\..\..\..\..\bin\tao_idl -o .\ -I ..\NavDisplay -Wb,export_macro=NAVDISPLAY_EXEC_Export -Wb,export_include=NavDisplay_exec_export.h -I..\..\..\..\.. -I..\..\..\..\..\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

".\\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=.\..\NavDisplay\NavDisplayEI.idl
InputName=NavDisplayEI

BuildCmds= \
	..\..\..\..\..\..\bin\tao_idl -o .\ -I ..\NavDisplay -Wb,export_macro=NAVDISPLAY_EXEC_Export -Wb,export_include=NavDisplay_exec_export.h -I..\..\..\..\.. -I..\..\..\..\..\CIAO\ciao -Wb,pre_include="ace\pre.h" -Wb,post_include="ace\post.h" -I ..\..\..\.. -I ..\..\..\..\.. -I ..\..\..\..\..\orbsvcs\orbsvcs -Ge 1 -Sc $(InputPath)

".\\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "moc_files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DetailView.h

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\DetailView.h

".\DetailView_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o DetailView_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\DetailView.h

".\DetailView_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o DetailView_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MapView.h

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\MapView.h

".\MapView_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o MapView_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\MapView.h

".\MapView_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o MapView_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NodeItem.h

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\NodeItem.h

".\NodeItem_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o NodeItem_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\NodeItem.h

".\NodeItem_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o NodeItem_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RootPanel.h

!IF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\RootPanel.h

".\RootPanel_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o RootPanel_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "NavDisplayGUI_exec DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking $(QTDIR)\bin\moc on $(InputPath)
InputPath=.\RootPanel.h

".\RootPanel_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc  $(InputPath) -o RootPanel_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
