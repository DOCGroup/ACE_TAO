# Microsoft Developer Studio Project File - Name="Message_Queue_Notifications_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=Message_Queue_Notifications_Test - Win32 Alpha Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Message_Queue_Notifications_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Message_Queue_Notifications_Test.mak"\
 CFG="Message_Queue_Notifications_Test - Win32 Alpha Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Message_Queue_Notifications_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Unicode Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Unicode Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Alpha Unicode Debug" (based\
 on "Win32 (ALPHA) Console Application")
!MESSAGE "Message_Queue_Notifications_Test - Win32 Alpha Unicode Release"\
 (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_"
# PROP BASE Intermediate_Dir "Message_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 odbc32.lib odbccp32.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_"
# PROP BASE Intermediate_Dir "Message_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
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

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_"
# PROP BASE Intermediate_Dir "Message_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Release"
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 odbc32.lib odbccp32.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 odbc32.lib odbccp32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /incremental:no /machine:I386 /libpath:"..\ace"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message0"
# PROP BASE Intermediate_Dir "Message0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Unicode_Debug"
# PROP Intermediate_Dir ".\DLL\Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 odbc32.lib odbccp32.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 odbc32.lib odbccp32.lib aceud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_"
# PROP BASE Intermediate_Dir "Message_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DLL\Unicode_Release"
# PROP Intermediate_Dir ".\DLL\Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /FD /c
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

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message_"
# PROP BASE Intermediate_Dir "Message_"
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

!ELSEIF  "$(CFG)" ==\
 "Message_Queue_Notifications_Test - Win32 Alpha Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message0"
# PROP BASE Intermediate_Dir "Message0"
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

!ELSEIF  "$(CFG)" ==\
 "Message_Queue_Notifications_Test - Win32 Alpha Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Message1"
# PROP BASE Intermediate_Dir "Message1"
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

# Name "Message_Queue_Notifications_Test - Win32 Debug"
# Name "Message_Queue_Notifications_Test - Win32 Alpha Debug"
# Name "Message_Queue_Notifications_Test - Win32 Release"
# Name "Message_Queue_Notifications_Test - Win32 Unicode Debug"
# Name "Message_Queue_Notifications_Test - Win32 Unicode Release"
# Name "Message_Queue_Notifications_Test - Win32 Alpha Release"
# Name "Message_Queue_Notifications_Test - Win32 Alpha Unicode Debug"
# Name "Message_Queue_Notifications_Test - Win32 Alpha Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Message_Queue_Notifications_Test.cpp

!IF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Debug"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Alpha Debug"

DEP_CPP_MESSA=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\IO_Cntl_Msg.h"\
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
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
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
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Release"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "Message_Queue_Notifications_Test - Win32 Alpha Release"

!ELSEIF  "$(CFG)" ==\
 "Message_Queue_Notifications_Test - Win32 Alpha Unicode Debug"

DEP_CPP_MESSA=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\IO_Cntl_Msg.h"\
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
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
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
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	

!ELSEIF  "$(CFG)" ==\
 "Message_Queue_Notifications_Test - Win32 Alpha Unicode Release"

DEP_CPP_MESSA=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Handle_Set.h"\
	"..\ace\Handle_Set.i"\
	"..\ace\Hash_Map_Manager.cpp"\
	"..\ace\Hash_Map_Manager.h"\
	"..\ace\IO_Cntl_Msg.h"\
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
	"..\ace\Reactor.h"\
	"..\ace\Reactor.i"\
	"..\ace\Reactor_Impl.h"\
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
	"..\ace\Timer_Queue.h"\
	"..\ace\Timer_Queue_T.cpp"\
	"..\ace\Timer_Queue_T.h"\
	"..\ace\Timer_Queue_T.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\WFMO_Reactor.h"\
	"..\ace\WFMO_Reactor.i"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\test_config.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
