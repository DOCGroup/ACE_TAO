# Microsoft Developer Studio Project File - Name="ORBsvcs perf tests EC_Federated_Latency Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\server"
# PROP BASE Intermediate_Dir "Release\server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\server"
# PROP Intermediate_Dir "Release\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /D "NDEBUG" /O2 /MD /I "../../../../" /I "../../../" /I "../../"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /nologo /subsystem:console /machine:I386 ../../../../ace/ACE.lib ../../../tao/TAO.lib ../../../tao/PortableServer/TAO_PortableServer.lib ../../../tao/Strategies/TAO_Strategies.lib ../../../tao/IORTable/TAO_IORTable.lib ../../orbsvcs/TAO_CosNaming.lib ../../orbsvcs/TAO_RTEvent.lib ../../orbsvcs/TAO_Svc_Utils.lib /out:"server.exe" 

!ELSEIF  "$(CFG)" == "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi /D "_DEBUG" /Od /MDd /I "../../../../" /I "../../../" /I "../../"
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /debug /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /machine:I386 /debug /pdbtype:sept ../../../../ace/ACEd.lib ../../../tao/TAOd.lib ../../../tao/PortableServer/TAO_PortableServerd.lib ../../../tao/Strategies/TAO_Strategiesd.lib ../../../tao/IORTable/TAO_IORTabled.lib ../../orbsvcs/TAO_CosNamingd.lib ../../orbsvcs/TAO_RTEventd.lib ../../orbsvcs/TAO_Svc_Utilsd.lib /out:"server.exe" 

!ENDIF

# Begin Target

# Name "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Release"
# Name "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ControlC.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlS.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Coordinator.cpp
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;fi;fd"
# Begin Source File

SOURCE=.\ControlC.h
# End Source File
# Begin Source File

SOURCE=.\ControlS.h
# End Source File
# Begin Source File

SOURCE=.\ControlS_T.h
# End Source File
# Begin Source File

SOURCE=.\Coordinator.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl;pidl"
# Begin Source File

SOURCE=.\Control.idl
!IF  "$(CFG)" == "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Release"

USERDEP__IDL_="..\..\..\..\bin\Release\tao_idl.exe"	

# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputPath)
InputPath=.\Control.idl
InputName=Control

BuildCmds= \
	..\..\..\..\bin\Release\tao_idl.exe -Ge 1 -I../../ -I../../../ $(InputName).idl

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
                        
!ELSEIF  "$(CFG)" == "ORBsvcs perf tests EC_Federated_Latency Server - Win32 Debug"

USERDEP__IDL_="..\..\..\..\bin\tao_idl.exe"	

# Begin Custom Build - Invoking TAO's IDL Compiler on $(InputPath)
InputPath=.\Control.idl
InputName=Control

BuildCmds= \
	..\..\..\..\bin\tao_idl.exe -Ge 1 -I../../ -I../../../ $(InputName).idl

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

# PROP Default_Filter "inl;i"
# Begin Source File

SOURCE=.\ControlC.i
# End Source File
# Begin Source File

SOURCE=.\ControlS.i
# End Source File
# Begin Source File

SOURCE=.\ControlS_T.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
