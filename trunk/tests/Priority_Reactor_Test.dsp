# Microsoft Developer Studio Project File - Name="Priority_Reactor_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=Priority_Reactor_Test - Win32 Alpha Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Priority_Reactor_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Priority_Reactor_Test.mak"\
 CFG="Priority_Reactor_Test - Win32 Alpha Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Priority_Reactor_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Unicode Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Unicode Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Alpha Unicode Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Priority_Reactor_Test - Win32 Alpha Unicode Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Priority_Reactor_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority"
# PROP BASE Intermediate_Dir "Priority"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority"
# PROP BASE Intermediate_Dir "Priority"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority"
# PROP BASE Intermediate_Dir "Priority"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Release"
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D "_MBCS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 odbc32.lib odbccp32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /machine:I386 /libpath:"..\ace"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priorit0"
# PROP BASE Intermediate_Dir "Priorit0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Unicode_Debug"
# PROP Intermediate_Dir ".\DLL\Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "_MBCS" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 odbc32.lib odbccp32.lib aceud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority"
# PROP BASE Intermediate_Dir "Priority"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Unicode_Release"
# PROP Intermediate_Dir ".\DLL\Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "_MBCS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D "_MBCS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 odbc32.lib odbccp32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /machine:I386 /libpath:"..\ace"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 odbc32.lib odbccp32.lib aceu.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /machine:I386 /libpath:"..\ace"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priority"
# PROP BASE Intermediate_Dir "Priority"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL/Release"
# PROP Intermediate_Dir "DLL/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /O2 /Ob2 /I "..\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Zi /O2 /Ob2 /I "..\\" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\ace"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\ace"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priorit0"
# PROP BASE Intermediate_Dir "Priorit0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL/Unicode_Debug"
# PROP Intermediate_Dir "DLL/Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aceud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 aceud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Priorit1"
# PROP BASE Intermediate_Dir "Priorit1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL/Unicode_Release"
# PROP Intermediate_Dir "DLL/Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /O2 /Ob2 /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /Zi /O2 /Ob2 /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aceu.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\ace"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 aceu.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\ace"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Priority_Reactor_Test - Win32 Debug"
# Name "Priority_Reactor_Test - Win32 Alpha Debug"
# Name "Priority_Reactor_Test - Win32 Release"
# Name "Priority_Reactor_Test - Win32 Unicode Debug"
# Name "Priority_Reactor_Test - Win32 Unicode Release"
# Name "Priority_Reactor_Test - Win32 Alpha Release"
# Name "Priority_Reactor_Test - Win32 Alpha Unicode Debug"
# Name "Priority_Reactor_Test - Win32 Alpha Unicode Release"
# Begin Source File

SOURCE=.\Priority_Reactor_Test.cpp

!IF  "$(CFG)" == "Priority_Reactor_Test - Win32 Debug"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Debug"

DEP_CPP_PRIOR=\
	"..\ace\Acceptor.cpp"\
	"..\ace\Acceptor.h"\
	"..\ace\Acceptor.i"\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Connector.cpp"\
	"..\ace\Connector.h"\
	"..\ace\Connector.i"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Dynamic.h"\
	"..\ace\Dynamic.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Get_Opt.h"\
	"..\ace\Get_Opt.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\IO_Cntl_Msg.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
	"..\ace\Local_Tokens.h"\
	"..\ace\Local_Tokens.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Map_Manager.cpp"\
	"..\ace\Map_Manager.h"\
	"..\ace\Map_Manager.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Message_Block.h"\
	"..\ace\Message_Block.i"\
	"..\ace\Message_Queue.cpp"\
	"..\ace\Message_Queue.h"\
	"..\ace\Message_Queue.i"\
	"..\ace\Module.cpp"\
	"..\ace\Module.h"\
	"..\ace\Module.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Pipe.h"\
	"..\ace\Pipe.i"\
	"..\ace\Priority_Reactor.h"\
	"..\ace\Priority_Reactor.i"\
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
	"..\ace\Select_Reactor.h"\
	"..\ace\Select_Reactor.i"\
	"..\ace\Service_Config.h"\
	"..\ace\Service_Config.i"\
	"..\ace\Service_Object.h"\
	"..\ace\Service_Object.i"\
	"..\ace\Service_Types.h"\
	"..\ace\Service_Types.i"\
	"..\ace\Shared_Object.h"\
	"..\ace\Shared_Object.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\SOCK.h"\
	"..\ace\SOCK.i"\
	"..\ace\SOCK_Acceptor.h"\
	"..\ace\SOCK_Acceptor.i"\
	"..\ace\SOCK_Connector.h"\
	"..\ace\SOCK_Connector.i"\
	"..\ace\SOCK_IO.h"\
	"..\ace\SOCK_IO.i"\
	"..\ace\SOCK_Stream.h"\
	"..\ace\SOCK_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\stdcpp.h"\
	"..\ace\Strategies.h"\
	"..\ace\Strategies_T.cpp"\
	"..\ace\Strategies_T.h"\
	"..\ace\Stream_Modules.cpp"\
	"..\ace\Stream_Modules.h"\
	"..\ace\Stream_Modules.i"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Svc_Conf_Tokens.h"\
	"..\ace\Svc_Handler.cpp"\
	"..\ace\Svc_Handler.h"\
	"..\ace\Svc_Handler.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_Options.h"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\Task.h"\
	"..\ace\Task.i"\
	"..\ace\Task_T.cpp"\
	"..\ace\Task_T.h"\
	"..\ace\Task_T.i"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Time_Value.h"\
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Token.h"\
	"..\ace\Token.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\Priority_Reactor_Test.h"\
	".\test_config.h"\
	

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Release"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Unicode Debug"

DEP_CPP_PRIOR=\
	"..\ace\Acceptor.cpp"\
	"..\ace\Acceptor.h"\
	"..\ace\Acceptor.i"\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Connector.cpp"\
	"..\ace\Connector.h"\
	"..\ace\Connector.i"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Dynamic.h"\
	"..\ace\Dynamic.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Get_Opt.h"\
	"..\ace\Get_Opt.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\IO_Cntl_Msg.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
	"..\ace\Local_Tokens.h"\
	"..\ace\Local_Tokens.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Map_Manager.cpp"\
	"..\ace\Map_Manager.h"\
	"..\ace\Map_Manager.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Message_Block.h"\
	"..\ace\Message_Block.i"\
	"..\ace\Message_Queue.cpp"\
	"..\ace\Message_Queue.h"\
	"..\ace\Message_Queue.i"\
	"..\ace\Module.cpp"\
	"..\ace\Module.h"\
	"..\ace\Module.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Pipe.h"\
	"..\ace\Pipe.i"\
	"..\ace\Priority_Reactor.h"\
	"..\ace\Priority_Reactor.i"\
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
	"..\ace\Select_Reactor.h"\
	"..\ace\Select_Reactor.i"\
	"..\ace\Service_Config.h"\
	"..\ace\Service_Config.i"\
	"..\ace\Service_Object.h"\
	"..\ace\Service_Object.i"\
	"..\ace\Service_Types.h"\
	"..\ace\Service_Types.i"\
	"..\ace\Shared_Object.h"\
	"..\ace\Shared_Object.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\Singleton.cpp"\
	"..\ace\Singleton.h"\
	"..\ace\Singleton.i"\
	"..\ace\SOCK.h"\
	"..\ace\SOCK.i"\
	"..\ace\SOCK_Acceptor.h"\
	"..\ace\SOCK_Acceptor.i"\
	"..\ace\SOCK_Connector.h"\
	"..\ace\SOCK_Connector.i"\
	"..\ace\SOCK_IO.h"\
	"..\ace\SOCK_IO.i"\
	"..\ace\SOCK_Stream.h"\
	"..\ace\SOCK_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\stdcpp.h"\
	"..\ace\Strategies.h"\
	"..\ace\Strategies_T.cpp"\
	"..\ace\Strategies_T.h"\
	"..\ace\Stream_Modules.cpp"\
	"..\ace\Stream_Modules.h"\
	"..\ace\Stream_Modules.i"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Svc_Conf_Tokens.h"\
	"..\ace\Svc_Handler.cpp"\
	"..\ace\Svc_Handler.h"\
	"..\ace\Svc_Handler.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_Options.h"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\Task.h"\
	"..\ace\Task.i"\
	"..\ace\Task_T.cpp"\
	"..\ace\Task_T.h"\
	"..\ace\Task_T.i"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Time_Value.h"\
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Token.h"\
	"..\ace\Token.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\Priority_Reactor_Test.h"\
	".\test_config.h"\
	

!ELSEIF  "$(CFG)" == "Priority_Reactor_Test - Win32 Alpha Unicode Release"

DEP_CPP_PRIOR=\
	"..\ace\Acceptor.cpp"\
	"..\ace\Acceptor.h"\
	"..\ace\Acceptor.i"\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Connector.cpp"\
	"..\ace\Connector.h"\
	"..\ace\Connector.i"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Dynamic.h"\
	"..\ace\Dynamic.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Get_Opt.h"\
	"..\ace\Get_Opt.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\IO_Cntl_Msg.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
	"..\ace\Local_Tokens.h"\
	"..\ace\Local_Tokens.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Map_Manager.cpp"\
	"..\ace\Map_Manager.h"\
	"..\ace\Map_Manager.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Message_Block.h"\
	"..\ace\Message_Block.i"\
	"..\ace\Message_Queue.cpp"\
	"..\ace\Message_Queue.h"\
	"..\ace\Message_Queue.i"\
	"..\ace\Module.cpp"\
	"..\ace\Module.h"\
	"..\ace\Module.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Pipe.h"\
	"..\ace\Pipe.i"\
	"..\ace\Priority_Reactor.h"\
	"..\ace\Priority_Reactor.i"\
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
	"..\ace\Select_Reactor.h"\
	"..\ace\Select_Reactor.i"\
	"..\ace\Service_Config.h"\
	"..\ace\Service_Config.i"\
	"..\ace\Service_Object.h"\
	"..\ace\Service_Object.i"\
	"..\ace\Service_Types.h"\
	"..\ace\Service_Types.i"\
	"..\ace\Shared_Object.h"\
	"..\ace\Shared_Object.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\Singleton.cpp"\
	"..\ace\Singleton.h"\
	"..\ace\Singleton.i"\
	"..\ace\SOCK.h"\
	"..\ace\SOCK.i"\
	"..\ace\SOCK_Acceptor.h"\
	"..\ace\SOCK_Acceptor.i"\
	"..\ace\SOCK_Connector.h"\
	"..\ace\SOCK_Connector.i"\
	"..\ace\SOCK_IO.h"\
	"..\ace\SOCK_IO.i"\
	"..\ace\SOCK_Stream.h"\
	"..\ace\SOCK_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\stdcpp.h"\
	"..\ace\Strategies.h"\
	"..\ace\Strategies_T.cpp"\
	"..\ace\Strategies_T.h"\
	"..\ace\Stream_Modules.cpp"\
	"..\ace\Stream_Modules.h"\
	"..\ace\Stream_Modules.i"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Svc_Conf_Tokens.h"\
	"..\ace\Svc_Handler.cpp"\
	"..\ace\Svc_Handler.h"\
	"..\ace\Svc_Handler.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_Options.h"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\Task.h"\
	"..\ace\Task.i"\
	"..\ace\Task_T.cpp"\
	"..\ace\Task_T.h"\
	"..\ace\Task_T.i"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Time_Value.h"\
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Token.h"\
	"..\ace\Token.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\Priority_Reactor_Test.h"\
	".\test_config.h"\
	

!ENDIF 

# End Source File
# End Target
# End Project
