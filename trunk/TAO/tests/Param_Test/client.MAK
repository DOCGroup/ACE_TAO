# Microsoft Developer Studio Generated NMAKE File, Based on client.dsp
!IF "$(CFG)" == ""
CFG=client - Win32 Debug
!MESSAGE No configuration specified. Defaulting to client - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "client - Win32 Release" && "$(CFG)" != "client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "client - Win32 Release"

OUTDIR=.
INTDIR=.\Release
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "param_testS.i" "param_testS.h" "param_testS.cpp" "param_testC.i"\
 "param_testC.h" "param_testC.cpp" "$(OUTDIR)\client.exe"

!ELSE 

ALL : "param_testS.i" "param_testS.h" "param_testS.cpp" "param_testC.i"\
 "param_testC.h" "param_testC.cpp" "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\driver.obj"
	-@erase "$(INTDIR)\helper.obj"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\param_testC.obj"
	-@erase "$(INTDIR)\results.obj"
	-@erase "$(INTDIR)\tests.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "param_testC.cpp"
	-@erase "param_testC.h"
	-@erase "param_testC.i"
	-@erase "param_testS.cpp"
	-@erase "param_testS.h"
	-@erase "param_testS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D\
 "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\client.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=TAO.lib ace.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\client.pdb" /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /libpath:"..\..\tao" /libpath:"..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\driver.obj" \
	"$(INTDIR)\helper.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\param_testC.obj" \
	"$(INTDIR)\results.obj" \
	"$(INTDIR)\tests.obj"

"$(OUTDIR)\client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "param_testS.i" "param_testS.h" "param_testS.cpp" "param_testC.i"\
 "param_testC.h" "param_testC.cpp" "$(OUTDIR)\client.exe"

!ELSE 

ALL : "param_testS.i" "param_testS.h" "param_testS.cpp" "param_testC.i"\
 "param_testC.h" "param_testC.cpp" "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\driver.obj"
	-@erase "$(INTDIR)\helper.obj"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\param_testC.obj"
	-@erase "$(INTDIR)\results.obj"
	-@erase "$(INTDIR)\tests.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "$(OUTDIR)\client.ilk"
	-@erase "$(OUTDIR)\client.pdb"
	-@erase "param_testC.cpp"
	-@erase "param_testC.h"
	-@erase "param_testC.i"
	-@erase "param_testS.cpp"
	-@erase "param_testS.h"
	-@erase "param_testS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /D\
 "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\client.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=TAO.lib aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\client.pdb" /debug /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\driver.obj" \
	"$(INTDIR)\helper.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\param_testC.obj" \
	"$(INTDIR)\results.obj" \
	"$(INTDIR)\tests.obj"

"$(OUTDIR)\client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(CFG)" == "client - Win32 Release" || "$(CFG)" == "client - Win32 Debug"
SOURCE=.\client.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_CLIEN=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Profile_Timer.h"\
	"..\..\..\ace\Profile_Timer.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\client.h"\
	".\options.h"\
	".\param_testC.h"\
	".\param_testC.i"\
	".\results.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\client.obj" : $(SOURCE) $(DEP_CPP_CLIEN) "$(INTDIR)"\
 ".\param_testC.h" ".\param_testC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"


"$(INTDIR)\client.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\driver.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_DRIVE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Profile_Timer.h"\
	"..\..\..\ace\Profile_Timer.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\client.cpp"\
	".\client.h"\
	".\driver.h"\
	".\options.h"\
	".\param_testC.h"\
	".\param_testC.i"\
	".\results.h"\
	".\tests.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\driver.obj" : $(SOURCE) $(DEP_CPP_DRIVE) "$(INTDIR)"\
 ".\param_testC.h" ".\client.cpp" ".\param_testC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"


"$(INTDIR)\driver.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\helper.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_HELPE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\helper.h"\
	".\param_testC.h"\
	".\param_testC.i"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\helper.obj" : $(SOURCE) $(DEP_CPP_HELPE) "$(INTDIR)"\
 ".\param_testC.h" ".\param_testC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"


"$(INTDIR)\helper.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\options.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_OPTIO=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\options.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"

DEP_CPP_OPTIO=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_server.h"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\options.h"\
	

"$(INTDIR)\options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ENDIF 

SOURCE=.\param_test.idl

!IF  "$(CFG)" == "client - Win32 Release"

InputPath=.\param_test.idl

"param_testS.h"	"param_testS.i"	"param_testS.cpp"	"param_testC.h"\
	"param_testC.i"	"param_testC.cpp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\tao_idl\tao_idl param_test.idl

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

InputPath=.\param_test.idl

"param_testS.h"	"param_testS.i"	"param_testS.cpp"	"param_testC.h"\
	"param_testC.i"	"param_testC.cpp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\tao_idl\tao_idl param_test.idl

!ENDIF 

SOURCE=.\param_testC.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_PARAM=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\param_testC.h"\
	".\param_testC.i"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\param_testC.obj" : $(SOURCE) $(DEP_CPP_PARAM) "$(INTDIR)"\
 ".\param_testC.h" ".\param_testC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"


"$(INTDIR)\param_testC.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\results.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_RESUL=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Profile_Timer.h"\
	"..\..\..\ace\Profile_Timer.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\results.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\results.obj" : $(SOURCE) $(DEP_CPP_RESUL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"

DEP_CPP_RESUL=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\Profile_Timer.h"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_server.h"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\results.h"\
	

"$(INTDIR)\results.obj" : $(SOURCE) $(DEP_CPP_RESUL) "$(INTDIR)"


!ENDIF 

SOURCE=.\tests.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_TESTS=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.cpp"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc.h"\
	"..\..\..\ace\Malloc.i"\
	"..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\ace\Malloc_T.h"\
	"..\..\..\ace\Malloc_T.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Module.cpp"\
	"..\..\..\ace\Module.h"\
	"..\..\..\ace\Module.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Service_Repository.h"\
	"..\..\..\ace\Service_Repository.i"\
	"..\..\..\ace\Service_Types.h"\
	"..\..\..\ace\Service_Types.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\Singleton.cpp"\
	"..\..\..\ace\Singleton.h"\
	"..\..\..\ace\Singleton.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\ace\Stream_Modules.h"\
	"..\..\..\ace\Stream_Modules.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\ace\Svc_Handler.h"\
	"..\..\..\ace\Svc_Handler.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_Options.h"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\sys_conf.h"\
	"..\..\..\ace\Task.h"\
	"..\..\..\ace\Task.i"\
	"..\..\..\ace\Task_T.cpp"\
	"..\..\..\ace\Task_T.h"\
	"..\..\..\ace\Task_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Thread_Manager.h"\
	"..\..\..\ace\Thread_Manager.i"\
	"..\..\..\ace\Time_Value.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\Version.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\client_factory.h"\
	"..\..\tao\compat\initguid.h"\
	"..\..\tao\compat\objbase.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbacom.h"\
	"..\..\tao\corbacom.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\except.h"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\iiopobj.h"\
	"..\..\tao\iiopobj.i"\
	"..\..\tao\iioporb.h"\
	"..\..\tao\iioporb.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\nvlist.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\objtable.h"\
	"..\..\tao\optable.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\orbobj.h"\
	"..\..\tao\orbobj.i"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\poa.i"\
	"..\..\tao\principa.h"\
	"..\..\tao\request.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\sequence_T.cpp"\
	"..\..\tao\sequence_T.h"\
	"..\..\tao\sequence_T.i"\
	"..\..\tao\server_factory.h"\
	"..\..\tao\server_factory.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\svrrqst.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\helper.h"\
	".\param_testC.h"\
	".\param_testC.i"\
	".\tests.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\tests.obj" : $(SOURCE) $(DEP_CPP_TESTS) "$(INTDIR)"\
 ".\param_testC.h" ".\param_testC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"


"$(INTDIR)\tests.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

