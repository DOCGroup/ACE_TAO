# Microsoft Developer Studio Generated NMAKE File, Based on client.dsp
!IF "$(CFG)" == ""
CFG=client - Win32 Release
!MESSAGE No configuration specified. Defaulting to client - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "client - Win32 Release" && "$(CFG)" != "client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="client - Win32 Release"
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

ALL : "$(OUTDIR)\client.exe"

!ELSE 

ALL : "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\clnt.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\client.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D\
 "NDEBUG" /D "WIN32" /D "_CONSOLE" /Fp"$(INTDIR)\client.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib tao.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\client.pdb" /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /libpath:"..\..\..\..\tao\\" /libpath:"..\..\..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\clnt.obj" \
	"$(INTDIR)\cubitC.obj"

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

ALL : "$(OUTDIR)\client.exe"

!ELSE 

ALL : "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\clnt.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "$(OUTDIR)\client.ilk"
	-@erase "$(OUTDIR)\client.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I\
 "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE"\
 /Fp"$(INTDIR)\client.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib TAO.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\client.pdb" /debug /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\clnt.obj" \
	"$(INTDIR)\cubitC.obj"

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
SOURCE=.\clnt.cpp
DEP_CPP_CLNT_=\
	"..\..\..\..\..\ace\Acceptor.cpp"\
	"..\..\..\..\..\ace\Acceptor.h"\
	"..\..\..\..\..\ace\Acceptor.i"\
	"..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\ace\Addr.h"\
	"..\..\..\..\..\ace\Addr.i"\
	"..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\ace\Connector.cpp"\
	"..\..\..\..\..\ace\Connector.h"\
	"..\..\..\..\..\ace\Connector.i"\
	"..\..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\ace\Containers.i"\
	"..\..\..\..\..\ace\Dynamic.h"\
	"..\..\..\..\..\ace\Dynamic.i"\
	"..\..\..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\..\..\ace\Env_Value_T.cpp"\
	"..\..\..\..\..\ace\Env_Value_T.h"\
	"..\..\..\..\..\ace\Env_Value_T.i"\
	"..\..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\..\..\ace\inc_user_config.h"\
	"..\..\..\..\..\ace\INET_Addr.h"\
	"..\..\..\..\..\ace\INET_Addr.i"\
	"..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\..\ace\IPC_SAP.h"\
	"..\..\..\..\..\ace\IPC_SAP.i"\
	"..\..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\..\ace\Module.h"\
	"..\..\..\..\..\ace\Module.i"\
	"..\..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\..\ace\OS.h"\
	"..\..\..\..\..\ace\OS.i"\
	"..\..\..\..\..\ace\Profile_Timer.h"\
	"..\..\..\..\..\ace\Profile_Timer.i"\
	"..\..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\ace\Signal.i"\
	"..\..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\..\ace\SOCK.h"\
	"..\..\..\..\..\ace\SOCK.i"\
	"..\..\..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\..\..\ace\SOCK_IO.h"\
	"..\..\..\..\..\ace\SOCK_IO.i"\
	"..\..\..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\..\..\ace\SString.h"\
	"..\..\..\..\..\ace\SString.i"\
	"..\..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\..\ace\Stream_Modules.i"\
	"..\..\..\..\..\ace\streams.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\..\..\ace\Svc_Handler.h"\
	"..\..\..\..\..\ace\Svc_Handler.i"\
	"..\..\..\..\..\ace\Synch.h"\
	"..\..\..\..\..\ace\Synch.i"\
	"..\..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\..\ace\sys_conf.h"\
	"..\..\..\..\..\ace\Task.h"\
	"..\..\..\..\..\ace\Task.i"\
	"..\..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\..\ace\Thread.h"\
	"..\..\..\..\..\ace\Thread.i"\
	"..\..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\..\ace\Time_Value.h"\
	"..\..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\..\ace\Trace.h"\
	"..\..\..\..\..\ace\Version.h"\
	"..\..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\..\ace\ws2tcpip.h"\
	"..\..\..\..\tao\align.h"\
	"..\..\..\..\tao\any.h"\
	"..\..\..\..\tao\any.i"\
	"..\..\..\..\tao\cdr.h"\
	"..\..\..\..\tao\cdr.i"\
	"..\..\..\..\tao\client_factory.h"\
	"..\..\..\..\tao\compat\initguid.h"\
	"..\..\..\..\tao\compat\objbase.h"\
	"..\..\..\..\tao\connect.h"\
	"..\..\..\..\tao\connect.i"\
	"..\..\..\..\tao\corba.h"\
	"..\..\..\..\tao\corbacom.h"\
	"..\..\..\..\tao\corbacom.i"\
	"..\..\..\..\tao\debug.h"\
	"..\..\..\..\tao\default_client.h"\
	"..\..\..\..\tao\default_client.i"\
	"..\..\..\..\tao\default_server.h"\
	"..\..\..\..\tao\default_server.i"\
	"..\..\..\..\tao\except.h"\
	"..\..\..\..\tao\giop.h"\
	"..\..\..\..\tao\giop.i"\
	"..\..\..\..\tao\iiopobj.h"\
	"..\..\..\..\tao\iiopobj.i"\
	"..\..\..\..\tao\iioporb.h"\
	"..\..\..\..\tao\iioporb.i"\
	"..\..\..\..\tao\marshal.h"\
	"..\..\..\..\tao\marshal.i"\
	"..\..\..\..\tao\nvlist.h"\
	"..\..\..\..\tao\nvlist.i"\
	"..\..\..\..\tao\object.h"\
	"..\..\..\..\tao\object.i"\
	"..\..\..\..\tao\objtable.h"\
	"..\..\..\..\tao\optable.h"\
	"..\..\..\..\tao\orb.h"\
	"..\..\..\..\tao\orb_core.h"\
	"..\..\..\..\tao\orb_core.i"\
	"..\..\..\..\tao\orbconf.h"\
	"..\..\..\..\tao\orbobj.h"\
	"..\..\..\..\tao\orbobj.i"\
	"..\..\..\..\tao\params.h"\
	"..\..\..\..\tao\params.i"\
	"..\..\..\..\tao\poa.h"\
	"..\..\..\..\tao\poa.i"\
	"..\..\..\..\tao\principa.h"\
	"..\..\..\..\tao\request.h"\
	"..\..\..\..\tao\sequence.h"\
	"..\..\..\..\tao\sequence.i"\
	"..\..\..\..\tao\sequence_T.cpp"\
	"..\..\..\..\tao\sequence_T.h"\
	"..\..\..\..\tao\sequence_T.i"\
	"..\..\..\..\tao\server_factory.h"\
	"..\..\..\..\tao\server_factory.i"\
	"..\..\..\..\tao\singletons.h"\
	"..\..\..\..\tao\stub.h"\
	"..\..\..\..\tao\stub.i"\
	"..\..\..\..\tao\svrrqst.h"\
	"..\..\..\..\tao\typecode.h"\
	"..\..\..\..\tao\typecode.i"\
	".\clnt.h"\
	".\cubitC.h"\
	".\cubitC.i"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\clnt.obj" : $(SOURCE) $(DEP_CPP_CLNT_) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


SOURCE=.\cubit.idl

!IF  "$(CFG)" == "client - Win32 Release"

InputPath=.\cubit.idl
InputName=cubit

"$(InputName)C.cpp"	"$(InputName)C.i"	"$(InputName)C.h"	 : $(SOURCE)\
 "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\tao_idl\tao_idl $(InputName).idl

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

InputPath=.\cubit.idl
InputName=cubit

"$(InputName)C.cpp"	"$(InputName)C.i"	"$(InputName)C.h"	 : $(SOURCE)\
 "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\tao_idl\tao_idl $(InputName).idl

!ENDIF 

SOURCE=.\cubitC.cpp
DEP_CPP_CUBIT=\
	"..\..\..\..\..\ace\Acceptor.cpp"\
	"..\..\..\..\..\ace\Acceptor.h"\
	"..\..\..\..\..\ace\Acceptor.i"\
	"..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\ace\Addr.h"\
	"..\..\..\..\..\ace\Addr.i"\
	"..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\ace\Connector.cpp"\
	"..\..\..\..\..\ace\Connector.h"\
	"..\..\..\..\..\ace\Connector.i"\
	"..\..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\ace\Containers.i"\
	"..\..\..\..\..\ace\Dynamic.h"\
	"..\..\..\..\..\ace\Dynamic.i"\
	"..\..\..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\..\ace\inc_user_config.h"\
	"..\..\..\..\..\ace\INET_Addr.h"\
	"..\..\..\..\..\ace\INET_Addr.i"\
	"..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\..\ace\IPC_SAP.h"\
	"..\..\..\..\..\ace\IPC_SAP.i"\
	"..\..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\..\ace\Message_Queue.cpp"\
	"..\..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\..\ace\Module.h"\
	"..\..\..\..\..\ace\Module.i"\
	"..\..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\..\ace\OS.h"\
	"..\..\..\..\..\ace\OS.i"\
	"..\..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\ace\Signal.i"\
	"..\..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\..\ace\SOCK.h"\
	"..\..\..\..\..\ace\SOCK.i"\
	"..\..\..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\..\..\ace\SOCK_IO.h"\
	"..\..\..\..\..\ace\SOCK_IO.i"\
	"..\..\..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\..\..\ace\SString.h"\
	"..\..\..\..\..\ace\SString.i"\
	"..\..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\..\ace\Stream_Modules.i"\
	"..\..\..\..\..\ace\streams.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\..\..\ace\Svc_Handler.h"\
	"..\..\..\..\..\ace\Svc_Handler.i"\
	"..\..\..\..\..\ace\Synch.h"\
	"..\..\..\..\..\ace\Synch.i"\
	"..\..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\..\ace\sys_conf.h"\
	"..\..\..\..\..\ace\Task.h"\
	"..\..\..\..\..\ace\Task.i"\
	"..\..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\..\ace\Thread.h"\
	"..\..\..\..\..\ace\Thread.i"\
	"..\..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\..\ace\Time_Value.h"\
	"..\..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\..\ace\Trace.h"\
	"..\..\..\..\..\ace\Version.h"\
	"..\..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\..\ace\ws2tcpip.h"\
	"..\..\..\..\tao\align.h"\
	"..\..\..\..\tao\any.h"\
	"..\..\..\..\tao\any.i"\
	"..\..\..\..\tao\cdr.h"\
	"..\..\..\..\tao\cdr.i"\
	"..\..\..\..\tao\client_factory.h"\
	"..\..\..\..\tao\compat\initguid.h"\
	"..\..\..\..\tao\compat\objbase.h"\
	"..\..\..\..\tao\connect.h"\
	"..\..\..\..\tao\connect.i"\
	"..\..\..\..\tao\corba.h"\
	"..\..\..\..\tao\corbacom.h"\
	"..\..\..\..\tao\corbacom.i"\
	"..\..\..\..\tao\debug.h"\
	"..\..\..\..\tao\default_client.h"\
	"..\..\..\..\tao\default_client.i"\
	"..\..\..\..\tao\default_server.h"\
	"..\..\..\..\tao\default_server.i"\
	"..\..\..\..\tao\except.h"\
	"..\..\..\..\tao\giop.h"\
	"..\..\..\..\tao\giop.i"\
	"..\..\..\..\tao\iiopobj.h"\
	"..\..\..\..\tao\iiopobj.i"\
	"..\..\..\..\tao\iioporb.h"\
	"..\..\..\..\tao\iioporb.i"\
	"..\..\..\..\tao\marshal.h"\
	"..\..\..\..\tao\marshal.i"\
	"..\..\..\..\tao\nvlist.h"\
	"..\..\..\..\tao\nvlist.i"\
	"..\..\..\..\tao\object.h"\
	"..\..\..\..\tao\object.i"\
	"..\..\..\..\tao\objtable.h"\
	"..\..\..\..\tao\optable.h"\
	"..\..\..\..\tao\orb.h"\
	"..\..\..\..\tao\orb_core.h"\
	"..\..\..\..\tao\orb_core.i"\
	"..\..\..\..\tao\orbconf.h"\
	"..\..\..\..\tao\orbobj.h"\
	"..\..\..\..\tao\orbobj.i"\
	"..\..\..\..\tao\params.h"\
	"..\..\..\..\tao\params.i"\
	"..\..\..\..\tao\poa.h"\
	"..\..\..\..\tao\poa.i"\
	"..\..\..\..\tao\principa.h"\
	"..\..\..\..\tao\request.h"\
	"..\..\..\..\tao\sequence.h"\
	"..\..\..\..\tao\sequence.i"\
	"..\..\..\..\tao\sequence_T.cpp"\
	"..\..\..\..\tao\sequence_T.h"\
	"..\..\..\..\tao\sequence_T.i"\
	"..\..\..\..\tao\server_factory.h"\
	"..\..\..\..\tao\server_factory.i"\
	"..\..\..\..\tao\singletons.h"\
	"..\..\..\..\tao\stub.h"\
	"..\..\..\..\tao\stub.i"\
	"..\..\..\..\tao\svrrqst.h"\
	"..\..\..\..\tao\typecode.h"\
	"..\..\..\..\tao\typecode.i"\
	".\cubitC.h"\
	".\cubitC.i"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\cubitC.obj" : $(SOURCE) $(DEP_CPP_CUBIT) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"



!ENDIF 

