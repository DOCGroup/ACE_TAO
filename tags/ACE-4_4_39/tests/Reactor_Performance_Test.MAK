# Microsoft Developer Studio Generated NMAKE File, Based on Reactor_Performance_Test.dsp
!IF "$(CFG)" == ""
CFG=Reactor_Performance_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Reactor_Performance_Test -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Reactor_Performance_Test - Win32 Debug" && "$(CFG)" !=\
 "Reactor_Performance_Test - Win32 Alpha Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Reactor_Performance_Test.mak"\
 CFG="Reactor_Performance_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Reactor_Performance_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Reactor_Performance_Test - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

RSC=rc.exe

!IF  "$(CFG)" == "Reactor_Performance_Test - Win32 Debug"

OUTDIR=.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Reactor_Performance_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Reactor_Performance_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Reactor_Performance_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.ilk"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\DLL\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Reactor_Performance_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Reactor_Performance_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Reactor_Performance_Test.exe" /pdbtype:sept /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Performance_Test.obj"

"$(OUTDIR)\Reactor_Performance_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Reactor_Performance_Test - Win32 Alpha Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Reactor_Performance_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Reactor_Performance_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Reactor_Performance_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.exe"
	-@erase "$(OUTDIR)\Reactor_Performance_Test.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /I ".." /I "..\\" /D "WIN32" /D "_DEBUG"\
 /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Reactor_Performance_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console\
 /pdb:"$(OUTDIR)\Reactor_Performance_Test.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\Reactor_Performance_Test.exe" /pdbtype:sept /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Reactor_Performance_Test.obj"

"$(OUTDIR)\Reactor_Performance_Test.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Reactor_Performance_Test - Win32 Debug" || "$(CFG)" ==\
 "Reactor_Performance_Test - Win32 Alpha Debug"
SOURCE=.\Reactor_Performance_Test.cpp

!IF  "$(CFG)" == "Reactor_Performance_Test - Win32 Debug"

DEP_CPP_REACT=\
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
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
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
	"..\ace\High_Res_Timer.h"\
	"..\ace\High_Res_Timer.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\IO_Cntl_Msg.h"\
	"..\ace\iosfwd.h"\
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
	"..\ace\Profile_Timer.h"\
	"..\ace\Profile_Timer.i"\
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
	"..\ace\Strategies.h"\
	"..\ace\Strategies_T.cpp"\
	"..\ace\Strategies_T.h"\
	"..\ace\Stream_Modules.cpp"\
	"..\ace\Stream_Modules.h"\
	"..\ace\Stream_Modules.i"\
	"..\ace\streams.h"\
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
	"..\ace\sys_conf.h"\
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
	".\Reactor_Performance_Test.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Reactor_Performance_Test.obj" : $(SOURCE) $(DEP_CPP_REACT)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Reactor_Performance_Test - Win32 Alpha Debug"

DEP_CPP_REACT=\
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
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
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
	"..\ace\High_Res_Timer.h"\
	"..\ace\High_Res_Timer.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\IO_Cntl_Msg.h"\
	"..\ace\iosfwd.h"\
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
	"..\ace\Profile_Timer.h"\
	"..\ace\Profile_Timer.i"\
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
	"..\ace\Strategies.h"\
	"..\ace\Strategies_T.cpp"\
	"..\ace\Strategies_T.h"\
	"..\ace\Stream_Modules.cpp"\
	"..\ace\Stream_Modules.h"\
	"..\ace\Stream_Modules.i"\
	"..\ace\streams.h"\
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
	"..\ace\sys_conf.h"\
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
	".\Reactor_Performance_Test.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Reactor_Performance_Test.obj" : $(SOURCE) $(DEP_CPP_REACT)\
 "$(INTDIR)"


!ENDIF 


!ENDIF 

