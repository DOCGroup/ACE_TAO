# Microsoft Developer Studio Project File - Name="Notify_Performance_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE x86em) Application" 0x0b01
# TARGTYPE "Win32 (WCE MIPS) Application" 0x0a01
# TARGTYPE "Win32 (WCE SH) Application" 0x0901

CFG=Notify_Performance_Test - Win32 (WCE x86em) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Notify_Performance_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Notify_Performance_Test.mak"\
 CFG="Notify_Performance_Test - Win32 (WCE x86em) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Notify_Performance_Test - Win32 (WCE x86em) Debug" (based on\
 "Win32 (WCE x86em) Application")
!MESSAGE "Notify_Performance_Test - Win32 (WCE MIPS) Debug" (based on\
 "Win32 (WCE MIPS) Application")
!MESSAGE "Notify_Performance_Test - Win32 (WCE SH) Debug" (based on\
 "Win32 (WCE SH) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP WCE_Configuration "H/PC Ver. 2.00"

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x86emDbg"
# PROP Intermediate_Dir "x86emDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MLd /W3 /Gm /Zi /Od /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /Zi /Od /I "..\\" /I "..\..\\" /I "..\..\WindozeCE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_HAS_WINCE" /D ACE_HAS_DLL=1 /D "_AFXDLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_"
# ADD RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_" /d "_AFXDLL"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /subsystem:windows /debug /machine:I386 /windowsce:emulation
# ADD LINK32 aced.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /libpath:"..\..\ace\WCE\x86em" /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -COPY
# ADD EMPFILE -COPY

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSDbg"
# PROP BASE Intermediate_Dir "WMIPSDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WMIPSDbg"
# PROP Intermediate_Dir "WMIPSDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /YX /QMRWCE /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\\" /I "..\..\\" /I "..\..\WindozeCE" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_HAS_WINCE" /D ACE_HAS_DLL=1 /D "_AFXDLL" /QMRWCE /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG" /d "_AFXDLL"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /debug /machine:MIPS /subsystem:$(CESubsystem)
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 aced.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:MIPS /libpath:"..\..\ace\WCE\MIPS" /subsystem:$(CESubsystem)
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESHDbg"
# PROP BASE Intermediate_Dir "WCESHDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCESHDbg"
# PROP Intermediate_Dir "WCESHDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /YX /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\\" /I "..\..\\" /I "..\..\WindozeCE" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_HAS_WINCE" /D ACE_HAS_DLL=1 /D "_AFXDLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG" /d "_AFXDLL"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /debug /machine:SH3 /subsystem:$(CESubsystem)
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 aced.lib /nologo /entry:"wWinMainCRTStartup" /debug /machine:SH3 /libpath:"..\..\ace\WCE\SH" /subsystem:$(CESubsystem)
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ENDIF 

# Begin Target

# Name "Notify_Performance_Test - Win32 (WCE x86em) Debug"
# Name "Notify_Performance_Test - Win32 (WCE MIPS) Debug"
# Name "Notify_Performance_Test - Win32 (WCE SH) Debug"
# Begin Source File

SOURCE=..\..\WindozeCE\ACEApp.cpp

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

DEP_CPP_ACEAP=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

DEP_CPP_ACEAP=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

DEP_CPP_ACEAP=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\stdafx.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Notify_Performance_Test.cpp

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

DEP_CPP_NOTIF=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\Get_Opt.h"\
	"..\..\ace\Get_Opt.i"\
	"..\..\ace\Handle_Set.h"\
	"..\..\ace\Handle_Set.i"\
	"..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\ace\Hash_Map_Manager.h"\
	"..\..\ace\High_Res_Timer.h"\
	"..\..\ace\High_Res_Timer.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\IO_Cntl_Msg.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Local_Tokens.h"\
	"..\..\ace\Local_Tokens.i"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Map_Manager.cpp"\
	"..\..\ace\Map_Manager.h"\
	"..\..\ace\Map_Manager.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Message_Block.h"\
	"..\..\ace\Message_Block.i"\
	"..\..\ace\Message_Queue.cpp"\
	"..\..\ace\Message_Queue.h"\
	"..\..\ace\Message_Queue.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Pipe.h"\
	"..\..\ace\Pipe.i"\
	"..\..\ace\Profile_Timer.h"\
	"..\..\ace\Profile_Timer.i"\
	"..\..\ace\Reactor.h"\
	"..\..\ace\Reactor.i"\
	"..\..\ace\Reactor_Impl.h"\
	"..\..\ace\Select_Reactor.h"\
	"..\..\ace\Select_Reactor.i"\
	"..\..\ace\Service_Config.h"\
	"..\..\ace\Service_Config.i"\
	"..\..\ace\Service_Object.h"\
	"..\..\ace\Service_Object.i"\
	"..\..\ace\Service_Repository.h"\
	"..\..\ace\Service_Repository.i"\
	"..\..\ace\Service_Types.h"\
	"..\..\ace\Service_Types.i"\
	"..\..\ace\Shared_Object.h"\
	"..\..\ace\Shared_Object.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\Strategies.h"\
	"..\..\ace\Strategies.i"\
	"..\..\ace\Strategies_T.cpp"\
	"..\..\ace\Strategies_T.h"\
	"..\..\ace\Strategies_T.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_Options.h"\
	"..\..\ace\Synch_Options.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Thread_Manager.h"\
	"..\..\ace\Thread_Manager.i"\
	"..\..\ace\Time_Value.h"\
	"..\..\ace\Timer_Queue.h"\
	"..\..\ace\Timer_Queue_T.cpp"\
	"..\..\ace\Timer_Queue_T.h"\
	"..\..\ace\Timer_Queue_T.i"\
	"..\..\ace\Token.h"\
	"..\..\ace\Token.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\WFMO_Reactor.h"\
	"..\..\ace\WFMO_Reactor.i"\
	"..\..\ace\ws2tcpip.h"\
	"..\test_config.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

DEP_CPP_NOTIF=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\Get_Opt.h"\
	"..\..\ace\Get_Opt.i"\
	"..\..\ace\Handle_Set.h"\
	"..\..\ace\Handle_Set.i"\
	"..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\ace\Hash_Map_Manager.h"\
	"..\..\ace\High_Res_Timer.h"\
	"..\..\ace\High_Res_Timer.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\IO_Cntl_Msg.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Local_Tokens.h"\
	"..\..\ace\Local_Tokens.i"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Map_Manager.cpp"\
	"..\..\ace\Map_Manager.h"\
	"..\..\ace\Map_Manager.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Message_Block.h"\
	"..\..\ace\Message_Block.i"\
	"..\..\ace\Message_Queue.cpp"\
	"..\..\ace\Message_Queue.h"\
	"..\..\ace\Message_Queue.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Pipe.h"\
	"..\..\ace\Pipe.i"\
	"..\..\ace\Profile_Timer.h"\
	"..\..\ace\Profile_Timer.i"\
	"..\..\ace\Reactor.h"\
	"..\..\ace\Reactor.i"\
	"..\..\ace\Reactor_Impl.h"\
	"..\..\ace\Select_Reactor.h"\
	"..\..\ace\Select_Reactor.i"\
	"..\..\ace\Service_Config.h"\
	"..\..\ace\Service_Config.i"\
	"..\..\ace\Service_Object.h"\
	"..\..\ace\Service_Object.i"\
	"..\..\ace\Service_Repository.h"\
	"..\..\ace\Service_Repository.i"\
	"..\..\ace\Service_Types.h"\
	"..\..\ace\Service_Types.i"\
	"..\..\ace\Shared_Object.h"\
	"..\..\ace\Shared_Object.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\Strategies.h"\
	"..\..\ace\Strategies.i"\
	"..\..\ace\Strategies_T.cpp"\
	"..\..\ace\Strategies_T.h"\
	"..\..\ace\Strategies_T.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_Options.h"\
	"..\..\ace\Synch_Options.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Thread_Manager.h"\
	"..\..\ace\Thread_Manager.i"\
	"..\..\ace\Time_Value.h"\
	"..\..\ace\Timer_Queue.h"\
	"..\..\ace\Timer_Queue_T.cpp"\
	"..\..\ace\Timer_Queue_T.h"\
	"..\..\ace\Timer_Queue_T.i"\
	"..\..\ace\Token.h"\
	"..\..\ace\Token.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\WFMO_Reactor.h"\
	"..\..\ace\WFMO_Reactor.i"\
	"..\..\ace\ws2tcpip.h"\
	"..\test_config.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

DEP_CPP_NOTIF=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\Get_Opt.h"\
	"..\..\ace\Get_Opt.i"\
	"..\..\ace\Handle_Set.h"\
	"..\..\ace\Handle_Set.i"\
	"..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\ace\Hash_Map_Manager.h"\
	"..\..\ace\High_Res_Timer.h"\
	"..\..\ace\High_Res_Timer.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\IO_Cntl_Msg.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Local_Tokens.h"\
	"..\..\ace\Local_Tokens.i"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Map_Manager.cpp"\
	"..\..\ace\Map_Manager.h"\
	"..\..\ace\Map_Manager.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Message_Block.h"\
	"..\..\ace\Message_Block.i"\
	"..\..\ace\Message_Queue.cpp"\
	"..\..\ace\Message_Queue.h"\
	"..\..\ace\Message_Queue.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Pipe.h"\
	"..\..\ace\Pipe.i"\
	"..\..\ace\Profile_Timer.h"\
	"..\..\ace\Profile_Timer.i"\
	"..\..\ace\Reactor.h"\
	"..\..\ace\Reactor.i"\
	"..\..\ace\Reactor_Impl.h"\
	"..\..\ace\Select_Reactor.h"\
	"..\..\ace\Select_Reactor.i"\
	"..\..\ace\Service_Config.h"\
	"..\..\ace\Service_Config.i"\
	"..\..\ace\Service_Object.h"\
	"..\..\ace\Service_Object.i"\
	"..\..\ace\Service_Repository.h"\
	"..\..\ace\Service_Repository.i"\
	"..\..\ace\Service_Types.h"\
	"..\..\ace\Service_Types.i"\
	"..\..\ace\Shared_Object.h"\
	"..\..\ace\Shared_Object.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\Strategies.h"\
	"..\..\ace\Strategies.i"\
	"..\..\ace\Strategies_T.cpp"\
	"..\..\ace\Strategies_T.h"\
	"..\..\ace\Strategies_T.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_Options.h"\
	"..\..\ace\Synch_Options.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Thread_Manager.h"\
	"..\..\ace\Thread_Manager.i"\
	"..\..\ace\Time_Value.h"\
	"..\..\ace\Timer_Queue.h"\
	"..\..\ace\Timer_Queue_T.cpp"\
	"..\..\ace\Timer_Queue_T.h"\
	"..\..\ace\Timer_Queue_T.i"\
	"..\..\ace\Token.h"\
	"..\..\ace\Token.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\WFMO_Reactor.h"\
	"..\..\ace\WFMO_Reactor.i"\
	"..\..\ace\ws2tcpip.h"\
	"..\test_config.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WindozeCE\StdAfx.cpp

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

DEP_CPP_STDAF=\
	"..\..\WindozeCE\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

DEP_CPP_STDAF=\
	"..\..\WindozeCE\stdafx.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

DEP_CPP_STDAF=\
	"..\..\WindozeCE\stdafx.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WindozeCE\WindozeCE.cpp

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

DEP_CPP_WINDO=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

DEP_CPP_WINDO=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

DEP_CPP_WINDO=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WindozeCE\WindozeCE.rc

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WindozeCE\WindozeCEDlg.cpp

!IF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE x86em) Debug"

DEP_CPP_WINDOZ=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\ARGV.h"\
	"..\..\ace\ARGV.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE MIPS) Debug"

DEP_CPP_WINDOZ=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\ARGV.h"\
	"..\..\ace\ARGV.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ELSEIF  "$(CFG)" == "Notify_Performance_Test - Win32 (WCE SH) Debug"

DEP_CPP_WINDOZ=\
	"..\..\ace\ACE.h"\
	"..\..\ace\ACE.i"\
	"..\..\ace\ARGV.h"\
	"..\..\ace\ARGV.i"\
	"..\..\ace\Atomic_Op.i"\
	"..\..\ace\Auto_Ptr.cpp"\
	"..\..\ace\Auto_Ptr.h"\
	"..\..\ace\Auto_Ptr.i"\
	"..\..\ace\Basic_Types.h"\
	"..\..\ace\Basic_Types.i"\
	"..\..\ace\config-win32-common.h"\
	"..\..\ace\config-win32.h"\
	"..\..\ace\config-WinCE.h"\
	"..\..\ace\config.h"\
	"..\..\ace\Containers.cpp"\
	"..\..\ace\Containers.h"\
	"..\..\ace\Containers.i"\
	"..\..\ace\Event_Handler.h"\
	"..\..\ace\Event_Handler.i"\
	"..\..\ace\Free_List.cpp"\
	"..\..\ace\Free_List.h"\
	"..\..\ace\Free_List.i"\
	"..\..\ace\inc_user_config.h"\
	"..\..\ace\iosfwd.h"\
	"..\..\ace\Log_Msg.h"\
	"..\..\ace\Log_Priority.h"\
	"..\..\ace\Log_Record.h"\
	"..\..\ace\Log_Record.i"\
	"..\..\ace\Malloc.h"\
	"..\..\ace\Malloc.i"\
	"..\..\ace\Malloc_T.cpp"\
	"..\..\ace\Malloc_T.h"\
	"..\..\ace\Malloc_T.i"\
	"..\..\ace\Managed_Object.cpp"\
	"..\..\ace\Managed_Object.h"\
	"..\..\ace\Managed_Object.i"\
	"..\..\ace\Mem_Map.h"\
	"..\..\ace\Mem_Map.i"\
	"..\..\ace\Memory_Pool.h"\
	"..\..\ace\Memory_Pool.i"\
	"..\..\ace\Object_Manager.h"\
	"..\..\ace\Object_Manager.i"\
	"..\..\ace\OS.h"\
	"..\..\ace\OS.i"\
	"..\..\ace\Signal.h"\
	"..\..\ace\Signal.i"\
	"..\..\ace\SString.h"\
	"..\..\ace\SString.i"\
	"..\..\ace\streams.h"\
	"..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\ace\Synch.h"\
	"..\..\ace\Synch.i"\
	"..\..\ace\Synch_T.cpp"\
	"..\..\ace\Synch_T.h"\
	"..\..\ace\Synch_T.i"\
	"..\..\ace\sys_conf.h"\
	"..\..\ace\Thread.h"\
	"..\..\ace\Thread.i"\
	"..\..\ace\Trace.h"\
	"..\..\ace\ws2tcpip.h"\
	"..\..\WindozeCE\ACEApp.h"\
	"..\..\WindozeCE\resource.h"\
	"..\..\WindozeCE\stdafx.h"\
	"..\..\WindozeCE\WindozeCE.h"\
	"..\..\WindozeCE\WindozeCEDlg.h"\
	

!ENDIF 

# End Source File
# End Target
# End Project
