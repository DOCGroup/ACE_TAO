# Microsoft Developer Studio Project File - Name="ACE dynamic library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=ACE dynamic library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll.mak" CFG="ACE dynamic library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE dynamic library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Alpha Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Alpha Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Alpha Unicode Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "ACE dynamic library - Win32 Alpha Unicode Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion ""

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\\" /D ACE_HAS_DLL=1 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 advapi32.lib user32.lib /nologo /version:4.4 /subsystem:windows /dll /pdb:".\ace.pdb" /debug /machine:I386 /out:"..\bin\aced.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D ACE_HAS_DLL=1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\ace-r.pdb" /machine:I386 /out:"..\bin\ace.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\\" /D ACE_HAS_DLL=1 /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\aceu.pdb" /debug /machine:I386 /out:"..\bin\aceud.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D ACE_HAS_DLL=1 /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\aceu-r.pdb" /machine:I386 /out:"..\bin\aceu.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "DLL\Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP nologo Gt0 W3 GX Zi Od Gy I "..\\ /D " WIN32" D "_DEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 FD MTd c
# SUBTRACT BASE CPP YX Yc Yu
# ADD CPP nologo Gt0 W3 GX Zi Od Gy I "..\\ /D " WIN32" D "_DEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 FD MDd c
# SUBTRACT CPP YX Yc Yu
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.4 /subsystem:windows /dll /pdb:".\ace.pdb" /debug /machine:ALPHA /out:"..\bin\aced.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.4 /subsystem:windows /dll /pdb:".\ace.pdb" /debug /machine:ALPHA /out:"..\bin\aced.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "DLL\Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP nologo MT Gt0 W3 GX O2 Ob2 I "..\\ /D " WIN32" D "NDEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 FD c
# SUBTRACT BASE CPP YX
# ADD CPP nologo MD Gt0 W3 GX O2 Ob2 I "..\\ /D " WIN32" D "NDEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 FD c
# SUBTRACT CPP YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\ace-r.pdb" /machine:ALPHA /out:"..\bin\ace.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\ace-r.pdb" /machine:ALPHA /out:"..\bin\ace.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "DLL\Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP nologo Gt0 W3 GX Zi Od Gy I "..\\ /D " WIN32" D "_DEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 D "UNICODE" FD MTd c
# SUBTRACT BASE CPP YX
# ADD CPP nologo Gt0 W3 GX Zi Od Gy I "..\\ /D " WIN32" D "_DEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 D "UNICODE" FD MTd c
# SUBTRACT CPP YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\aceu.pdb" /debug /machine:ALPHA /out:"..\bin\aceud.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\aceu.pdb" /debug /machine:ALPHA /out:"..\bin\aceud.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "DLL\Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP nologo MT Gt0 W3 GX O2 Ob2 I "..\\ /D " WIN32" D "NDEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 D "UNICODE" FD c
# SUBTRACT BASE CPP YX
# ADD CPP nologo MT Gt0 W3 GX O2 Ob2 I "..\\ /D " WIN32" D "NDEBUG" D "_WINDOWS" D ACE_HAS_DLL=1 D "UNICODE" FD c
# SUBTRACT CPP YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /o /win32 "NUL" "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\aceu-r.pdb" /machine:ALPHA /out:"..\bin\aceu.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /pdb:".\aceu-r.pdb" /machine:ALPHA /out:"..\bin\aceu.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ACE dynamic library - Win32 Debug"
# Name "ACE dynamic library - Win32 Release"
# Name "ACE dynamic library - Win32 Unicode Debug"
# Name "ACE dynamic library - Win32 Unicode Release"
# Name "ACE dynamic library - Win32 Alpha Debug"
# Name "ACE dynamic library - Win32 Alpha Release"
# Name "ACE dynamic library - Win32 Alpha Unicode Debug"
# Name "ACE dynamic library - Win32 Alpha Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ACE.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ACE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ACE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ACTIV=\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Activation_Queue.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Method_Request.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ACTIV=\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Activation_Queue.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Method_Request.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Arg_Shifter.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ARG_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Arg_Shifter.h"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ARG_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Arg_Shifter.h"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ARGV.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_ASYNC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_ASYNC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_BASIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Filecache.h"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Tokens.h"\
	".\Remote_Tokens.i"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Template_Instantiations.cpp"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_BASIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Filecache.h"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Tokens.h"\
	".\Remote_Tokens.i"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Template_Instantiations.cpp"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Token.h"\
	".\Token.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Containers.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_CONTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_CONTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_CORBA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\CORBA_Handler.h"\
	".\CORBA_Handler.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_CORBA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\CORBA_Handler.h"\
	".\CORBA_Handler.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_CORBA_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\CORBA_Ref.h"\
	".\CORBA_Ref.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_CORBA_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\CORBA_Ref.h"\
	".\CORBA_Ref.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Date_Time.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DATE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Date_Time.h"\
	".\Date_Time.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DATE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Date_Time.h"\
	".\Date_Time.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DEV_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DEV_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DEV_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DEV_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DEV_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DEV_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DEV_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DEV_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dirent.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DIREN=\
	".\Dirent.i"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DIREN=\
	".\Dirent.i"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DLL.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DLL_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DLL.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DLL_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DLL.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dump.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_DYNAMI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic_Service.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_DYNAMI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic_Service.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Handler.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_EVENT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_EVENT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_EVENT_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Event_Handler_T.h"\
	".\Event_Handler_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_EVENT_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Event_Handler_T.h"\
	".\Event_Handler_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FIFO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FIFO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FIFO_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FIFO_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FIFO_RE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\FIFO_Recv_Msg.h"\
	".\FIFO_Recv_Msg.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FIFO_RE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Recv.h"\
	".\FIFO_Recv.i"\
	".\FIFO_Recv_Msg.h"\
	".\FIFO_Recv_Msg.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FIFO_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FIFO_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FIFO_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\FIFO_Send_Msg.h"\
	".\FIFO_Send_Msg.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FIFO_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FIFO.h"\
	".\FIFO.i"\
	".\FIFO_Send.h"\
	".\FIFO_Send.i"\
	".\FIFO_Send_Msg.h"\
	".\FIFO_Send_Msg.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FILE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FILE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FILE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FILE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FILE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_Connector.h"\
	".\FILE_Connector.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FILE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_Connector.h"\
	".\FILE_Connector.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FILE_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FILE_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\FILE.h"\
	".\FILE.i"\
	".\FILE_Addr.h"\
	".\FILE_Addr.i"\
	".\FILE_IO.h"\
	".\FILE_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Filecache.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FILEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Filecache.h"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FILEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Filecache.h"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Functor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FUNCT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FUNCT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Future.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_FUTUR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Future.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_FUTUR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Future.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stats.h"\
	".\Stats.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stats.h"\
	".\Stats.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\INET_Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_SAP.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOStream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_IOSTR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IOStream.h"\
	".\IOStream_T.cpp"\
	".\IOStream_T.h"\
	".\IOStream_T.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_IOSTR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IOStream.h"\
	".\IOStream_T.cpp"\
	".\IOStream_T.h"\
	".\IOStream_T.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LOCAL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LOCAL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LOCAL_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LOCAL_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LOG_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LOG_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Acceptor.h"\
	".\LSOCK_Acceptor.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Acceptor.h"\
	".\LSOCK_Acceptor.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_CODgram.h"\
	".\LSOCK_CODgram.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_CODgram.h"\
	".\LSOCK_CODgram.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Connector.h"\
	".\LSOCK_Connector.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Connector.h"\
	".\LSOCK_Connector.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Dgram.h"\
	".\LSOCK_Dgram.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Dgram.h"\
	".\LSOCK_Dgram.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_LSOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_LSOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\LSOCK.h"\
	".\LSOCK.i"\
	".\LSOCK_Stream.h"\
	".\LSOCK_Stream.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Malloc.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MALLO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MALLO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mem_Map.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MEMOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MEMOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message_Block.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MESSA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timeprobe.h"\
	".\Timeprobe.i"\
	".\Timeprobe_T.cpp"\
	".\Timeprobe_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MESSA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timeprobe.h"\
	".\Timeprobe.i"\
	".\Timeprobe_T.cpp"\
	".\Timeprobe_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message_Queue.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MESSAG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MESSAG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Method_Request.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Method_Request.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Method_Request.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MSG_W=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MSG_W=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_MULTI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Multiplexor.h"\
	".\Multiplexor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_MULTI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Multiplexor.h"\
	".\Multiplexor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Proxy.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_NAME_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_NAME_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_NAME_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Name_Request_Reply.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_NAME_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Name_Request_Reply.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Space.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_NAME_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_NAME_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming_Context.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_NAMIN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_NAMIN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Name_Space.h"\
	".\Local_Name_Space_T.cpp"\
	".\Local_Name_Space_T.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NT_Service.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_NT_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\NT_Service.h"\
	".\NT_Service.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_NT_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\NT_Service.h"\
	".\NT_Service.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_OBJEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_OBJEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Obstack.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OS.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_OS_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_OS_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pair.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PARSE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PARSE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PRIOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Priority_Reactor.h"\
	".\Priority_Reactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PRIOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Priority_Reactor.h"\
	".\Priority_Reactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Proactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PROAC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PROAC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_REACT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\TP_Reactor.h"\
	".\TP_Reactor.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_REACT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\TP_Reactor.h"\
	".\TP_Reactor.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_READ_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Read_Buffer.h"\
	".\Read_Buffer.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_READ_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Read_Buffer.h"\
	".\Read_Buffer.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Registry.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_REGIS=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Registry.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_REGIS=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Registry.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_REGIST=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_REGIST=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Registry.h"\
	".\Registry_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_REMOT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_REMOT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Name_Space.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_REMOTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Tokens.h"\
	".\Remote_Tokens.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_REMOTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Remote_Tokens.h"\
	".\Remote_Tokens.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched_Params.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SELEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SELEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SELECT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SELECT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Object.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SERVICE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SERVICE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Types.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SERVICE_T=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SERVICE_T=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SHARED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_MM.h"\
	".\Shared_Memory_MM.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SHARED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_MM.h"\
	".\Shared_Memory_MM.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SHARED_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_SV.h"\
	".\Shared_Memory_SV.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SHARED_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Memory.h"\
	".\Shared_Memory_SV.h"\
	".\Shared_Memory_SV.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Object.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SHARED_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SHARED_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Signal.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SIGNA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SIGNA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SPIPE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SPIPE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SPIPE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SPIPE_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SPIPE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SPIPE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SPIPE_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SPIPE_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SString.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SSTRI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SSTRI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stats.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_STATS=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stats.h"\
	".\Stats.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_STATS=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Stats.h"\
	".\Stats.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Strategies.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_STRAT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_STRAT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Message.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SV_ME=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SV_ME=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SV_MES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SV_MES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SV_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SV_SE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SV_SEM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SV_SEM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SV_SH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SV_SH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Shared_Memory.h"\
	".\SV_Shared_Memory.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SVC_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SVC_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SVC_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SVC_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\Obstack.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SVC_H=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Svc_Handler.h"\
	".\Svc_Handler.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SVC_H=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Svc_Handler.h"\
	".\Svc_Handler.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch_Options.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\System_Time.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_SYSTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_SYSTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TASK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TASK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIME_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Request_Reply.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIME_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Request_Reply.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timeprobe.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMEP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timeprobe.h"\
	".\Timeprobe.i"\
	".\Timeprobe_T.cpp"\
	".\Timeprobe_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMEP=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timeprobe.h"\
	".\Timeprobe.i"\
	".\Timeprobe_T.cpp"\
	".\Timeprobe_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Hash.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Heap.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_List.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER_L=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER_L=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER_Q=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER_Q=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Hash.h"\
	".\Timer_Hash_T.cpp"\
	".\Timer_Hash_T.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_Adapters.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER_QU=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_Adapters.h"\
	".\Timer_Queue_Adapters.i"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER_QU=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_Adapters.h"\
	".\Timer_Queue_Adapters.i"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TIMER_W=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TIMER_W=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TLI_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TLI_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TLI_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Acceptor.h"\
	".\TLI_Acceptor.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TLI_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Time_Value.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Acceptor.h"\
	".\TLI_Acceptor.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TLI_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Connector.h"\
	".\TLI_Connector.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TLI_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Connector.h"\
	".\TLI_Connector.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TLI_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TLI_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\TLI.h"\
	".\TLI.i"\
	".\TLI_Stream.h"\
	".\TLI_Stream.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TOKEN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TOKEN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Collection.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TOKEN_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Collection.h"\
	".\Token_Collection.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TOKEN_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Collection.h"\
	".\Token_Collection.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TOKEN_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TOKEN_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Invariants.h"\
	".\Token_Invariants.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Manager.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TOKEN_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TOKEN_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TOKEN_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TOKEN_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Request_Reply.h"\
	".\Token_Request_Reply.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TP_Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TP_RE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\TP_Reactor.h"\
	".\TP_Reactor.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TP_RE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\TP_Reactor.h"\
	".\TP_Reactor.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trace.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTY_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TTY_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\TTY_IO.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TTY_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\DEV.h"\
	".\DEV.i"\
	".\DEV_Addr.h"\
	".\DEV_Addr.i"\
	".\DEV_Connector.h"\
	".\DEV_Connector.i"\
	".\DEV_IO.h"\
	".\DEV_IO.i"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\TTY_IO.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TYPED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Typed_SV_Message.h"\
	".\Typed_SV_Message.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TYPED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\Typed_SV_Message.h"\
	".\Typed_SV_Message.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_TYPED_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Trace.h"\
	".\Typed_SV_Message.cpp"\
	".\Typed_SV_Message.h"\
	".\Typed_SV_Message.i"\
	".\Typed_SV_Message_Queue.h"\
	".\Typed_SV_Message_Queue.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_TYPED_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Message.h"\
	".\SV_Message.i"\
	".\SV_Message_Queue.h"\
	".\SV_Message_Queue.i"\
	".\Trace.h"\
	".\Typed_SV_Message.cpp"\
	".\Typed_SV_Message.h"\
	".\Typed_SV_Message.i"\
	".\Typed_SV_Message_Queue.h"\
	".\Typed_SV_Message_Queue.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_UNIX_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_UNIX_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc_Base.h"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Trace.h"\
	".\UNIX_Addr.h"\
	".\UNIX_Addr.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_UPIPE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Acceptor.h"\
	".\UPIPE_Acceptor.i"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_UPIPE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Acceptor.h"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Acceptor.h"\
	".\UPIPE_Acceptor.i"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_UPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Connector.h"\
	".\UPIPE_Connector.i"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_UPIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Connector.h"\
	".\UPIPE_Connector.i"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_UPIPE_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_UPIPE_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\UPIPE_Addr.h"\
	".\UPIPE_Stream.h"\
	".\UPIPE_Stream.i"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_WFMO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_WFMO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Functor.h"\
	".\Functor.i"\
	".\Functor_T.cpp"\
	".\Functor_T.h"\
	".\Functor_T.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.h"\
	".\Hash_Map_Manager.i"\
	".\Hash_Map_Manager_T.cpp"\
	".\Hash_Map_Manager_T.h"\
	".\Hash_Map_Manager_T.i"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Block_T.cpp"\
	".\Message_Block_T.h"\
	".\Message_Block_T.i"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Message_Queue_T.cpp"\
	".\Message_Queue_T.h"\
	".\Message_Queue_T.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies.i"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Strategies_T.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WIN32_Asynch_IO.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XtReactor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

DEP_CPP_XTREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	".\XtReactor.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

DEP_CPP_XTREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-borland.h"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.h"\
	".\Containers.i"\
	".\Containers_T.cpp"\
	".\Containers_T.h"\
	".\Containers_T.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_Base.h"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Select_Reactor_Base.h"\
	".\Select_Reactor_Base.i"\
	".\Select_Reactor_T.cpp"\
	".\Select_Reactor_T.h"\
	".\Select_Reactor_T.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_Options.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	".\XtReactor.h"\
	

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\ACE.h
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Addr.h
# End Source File
# Begin Source File

SOURCE=.\Arg_Shifter.h
# End Source File
# Begin Source File

SOURCE=.\ARGV.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.h
# End Source File
# Begin Source File

SOURCE=".\config-win32-common.h"
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\Connector.h
# End Source File
# Begin Source File

SOURCE=.\Containers.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.h
# End Source File
# Begin Source File

SOURCE=.\Date_Time.h
# End Source File
# Begin Source File

SOURCE=.\DEV.h
# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.h
# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.h
# End Source File
# Begin Source File

SOURCE=.\DEV_IO.h
# End Source File
# Begin Source File

SOURCE=.\Dirent.h
# End Source File
# Begin Source File

SOURCE=.\DLL.h
# End Source File
# Begin Source File

SOURCE=.\Dump.h
# End Source File
# Begin Source File

SOURCE=.\Dump_T.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service.h
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.h
# End Source File
# Begin Source File

SOURCE=.\FIFO.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.h
# End Source File
# Begin Source File

SOURCE=.\FILE.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\FILE_IO.h
# End Source File
# Begin Source File

SOURCE=.\Filecache.h
# End Source File
# Begin Source File

SOURCE=.\Free_List.h
# End Source File
# Begin Source File

SOURCE=.\Functor.h
# End Source File
# Begin Source File

SOURCE=.\Functor_T.h
# End Source File
# Begin Source File

SOURCE=.\Future.h
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.h
# End Source File
# Begin Source File

SOURCE=.\inc_user_config.h
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.h
# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.h
# End Source File
# Begin Source File

SOURCE=.\IO_SAP.h
# End Source File
# Begin Source File

SOURCE=.\IOStream.h
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.h
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.h
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.h
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Log_Priority.h
# End Source File
# Begin Source File

SOURCE=.\Log_Record.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Malloc.h
# End Source File
# Begin Source File

SOURCE=.\Malloc_Base.h
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Map_T.h
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.h
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.h
# End Source File
# Begin Source File

SOURCE=.\Message_Block.h
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.h
# End Source File
# Begin Source File

SOURCE=.\Method_Request.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.h
# End Source File
# Begin Source File

SOURCE=.\Name_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\Name_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\NT_Service.h
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Obstack.h
# End Source File
# Begin Source File

SOURCE=.\OS.h
# End Source File
# Begin Source File

SOURCE=.\Pair.h
# End Source File
# Begin Source File

SOURCE=.\Pair_T.h
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.h
# End Source File
# Begin Source File

SOURCE=.\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Proactor.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.h
# End Source File
# Begin Source File

SOURCE=.\RB_Tree.h
# End Source File
# Begin Source File

SOURCE=.\Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Reactor_Impl.h
# End Source File
# Begin Source File

SOURCE=.\ReactorEx.h
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Registry_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Sched_Params.h
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.h
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.h
# End Source File
# Begin Source File

SOURCE=.\Service_Config.h
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Service_Object.h
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.h
# End Source File
# Begin Source File

SOURCE=.\Service_Types.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Object.h
# End Source File
# Begin Source File

SOURCE=.\Signal.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\SOCK.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.h
# End Source File
# Begin Source File

SOURCE=.\SString.h
# End Source File
# Begin Source File

SOURCE=.\Stats.h
# End Source File
# Begin Source File

SOURCE=.\stdcpp.h
# End Source File
# Begin Source File

SOURCE=.\Strategies.h
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.h
# End Source File
# Begin Source File

SOURCE=.\streams.h
# End Source File
# Begin Source File

SOURCE=.\SV_Message.h
# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.h
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.h
# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Synch.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Options.h
# End Source File
# Begin Source File

SOURCE=.\Synch_T.h
# End Source File
# Begin Source File

SOURCE=.\System_Time.h
# End Source File
# Begin Source File

SOURCE=.\Task.h
# End Source File
# Begin Source File

SOURCE=.\Task_T.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Time_Value.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_List.h
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_Adapters.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.h
# End Source File
# Begin Source File

SOURCE=.\TLI.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Token.h
# End Source File
# Begin Source File

SOURCE=.\Token_Collection.h
# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.h
# End Source File
# Begin Source File

SOURCE=.\Token_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Trace.h
# End Source File
# Begin Source File

SOURCE=.\TTY_IO.h
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.h
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\ws2tcpip.h
# End Source File
# Begin Source File

SOURCE=.\XtReactor.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\ACE.i
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.i
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Addr.i
# End Source File
# Begin Source File

SOURCE=.\ARGV.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\Atomic_Op.i
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.i
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Connector.i
# End Source File
# Begin Source File

SOURCE=.\Containers.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.i
# End Source File
# Begin Source File

SOURCE=.\Date_Time.i
# End Source File
# Begin Source File

SOURCE=.\DEV.i
# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.i
# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.i
# End Source File
# Begin Source File

SOURCE=.\DEV_IO.i
# End Source File
# Begin Source File

SOURCE=.\Dirent.i
# End Source File
# Begin Source File

SOURCE=.\Dynamic.i
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.i
# End Source File
# Begin Source File

SOURCE=.\FIFO.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.i
# End Source File
# Begin Source File

SOURCE=.\FILE.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\FILE_IO.i
# End Source File
# Begin Source File

SOURCE=.\Free_List.i
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.i
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.i
# End Source File
# Begin Source File

SOURCE=.\IO_SAP.i
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.i
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.i
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.i
# End Source File
# Begin Source File

SOURCE=.\Log_Record.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Malloc.i
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.i
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.i
# End Source File
# Begin Source File

SOURCE=.\Map.i
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Map_T.i
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.i
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.i
# End Source File
# Begin Source File

SOURCE=.\Message_Block.i
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.i
# End Source File
# Begin Source File

SOURCE=.\Module.i
# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.i
# End Source File
# Begin Source File

SOURCE=.\NT_Service.i
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Obstack.i
# End Source File
# Begin Source File

SOURCE=.\OS.i
# End Source File
# Begin Source File

SOURCE=.\Pair.i
# End Source File
# Begin Source File

SOURCE=.\Pair_T.i
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.i
# End Source File
# Begin Source File

SOURCE=.\Pipe.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Proactor.i
# End Source File
# Begin Source File

SOURCE=.\Process.i
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.i
# End Source File
# Begin Source File

SOURCE=.\RB_Tree.i
# End Source File
# Begin Source File

SOURCE=.\Reactor.i
# End Source File
# Begin Source File

SOURCE=.\ReactorEx.i
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.i
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.i
# End Source File
# Begin Source File

SOURCE=.\Sched_Params.i
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.i
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.i
# End Source File
# Begin Source File

SOURCE=.\Service_Config.i
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Service_Object.i
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.i
# End Source File
# Begin Source File

SOURCE=.\Service_Types.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Object.i
# End Source File
# Begin Source File

SOURCE=.\Signal.i
# End Source File
# Begin Source File

SOURCE=.\Singleton.i
# End Source File
# Begin Source File

SOURCE=.\SOCK.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.i
# End Source File
# Begin Source File

SOURCE=.\SString.i
# End Source File
# Begin Source File

SOURCE=.\Stats.i
# End Source File
# Begin Source File

SOURCE=.\Strategies.i
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\Stream.i
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.i
# End Source File
# Begin Source File

SOURCE=.\SV_Message.i
# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.i
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.i
# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.i
# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.i
# End Source File
# Begin Source File

SOURCE=.\Synch.i
# End Source File
# Begin Source File

SOURCE=.\Synch_Options.i
# End Source File
# Begin Source File

SOURCE=.\Synch_T.i
# End Source File
# Begin Source File

SOURCE=.\Task.i
# End Source File
# Begin Source File

SOURCE=.\Task_T.i
# End Source File
# Begin Source File

SOURCE=.\Thread.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_Adapters.i
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.i
# End Source File
# Begin Source File

SOURCE=.\TLI.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Token.i
# End Source File
# Begin Source File

SOURCE=.\Token_Collection.i
# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.i
# End Source File
# Begin Source File

SOURCE=.\Token_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.i
# End Source File
# Begin Source File

SOURCE=.\Trace.i
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.i
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.i
# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Acceptor.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Connector.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dump_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Free_List.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Functor_T.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Map_Manager.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Map_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Pair_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\RB_Tree.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.cpp

!IF  "$(CFG)" == "ACE dynamic library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE dynamic library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Singleton.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Synch_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Task_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Readme
# End Source File
# Begin Source File

SOURCE=..\Version
# End Source File
# End Group
# End Target
# End Project
