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

ALL : "cubitS.i" "cubitS.h" "cubitS.cpp" "cubitC.i" "cubitC.h" "cubitC.cpp"\
 "$(OUTDIR)\client.exe"

!ELSE 

ALL : "cubitS.i" "cubitS.h" "cubitS.cpp" "cubitC.i" "cubitC.h" "cubitC.cpp"\
 "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\Task_Client.obj"
	-@erase "$(INTDIR)\Util_Thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "cubitC.cpp"
	-@erase "cubitC.h"
	-@erase "cubitC.i"
	-@erase "cubitS.cpp"
	-@erase "cubitS.h"
	-@erase "cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D\
 "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "PRIORITY_INV_TEST"\
 /Fp"$(INTDIR)\client.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib TAO.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\client.pdb" /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\cubitC.obj" \
	"$(INTDIR)\Task_Client.obj" \
	"$(INTDIR)\Util_Thread.obj"

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

ALL : "cubitS.i" "cubitS.h" "cubitS.cpp" "cubitC.i" "cubitC.h" "cubitC.cpp"\
 "$(OUTDIR)\client.exe"

!ELSE 

ALL : "cubitS.i" "cubitS.h" "cubitS.cpp" "cubitC.i" "cubitC.h" "cubitC.cpp"\
 "$(OUTDIR)\client.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\Task_Client.obj"
	-@erase "$(INTDIR)\Util_Thread.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "$(OUTDIR)\client.ilk"
	-@erase "$(OUTDIR)\client.pdb"
	-@erase "cubitC.cpp"
	-@erase "cubitC.h"
	-@erase "cubitC.i"
	-@erase "cubitS.cpp"
	-@erase "cubitS.h"
	-@erase "cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I\
 "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "PRIORITY_INV_TEST"\
 /Fp"$(INTDIR)\client.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib TAO.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\client.pdb" /debug /machine:I386 /out:"$(OUTDIR)\client.exe"\
 /pdbtype:sept /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\cubitC.obj" \
	"$(INTDIR)\Task_Client.obj" \
	"$(INTDIR)\Util_Thread.obj"

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
DEP_CPP_CLIEN=\
	"..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\ace\Containers.i"\
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
	"..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\ace\iosfwd.h"\
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
	"..\..\..\..\..\ace\Sched_Params.h"\
	"..\..\..\..\..\ace\Sched_Params.i"\
	"..\..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\ace\Signal.i"\
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
	".\client.h"\
	".\Task_Client.h"\
	".\Util_Thread.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_CLIEN=\
	".\cubitC.h"\
	

"$(INTDIR)\client.obj" : $(SOURCE) $(DEP_CPP_CLIEN) "$(INTDIR)" ".\cubitC.h"


SOURCE=.\cubit.idl

!IF  "$(CFG)" == "client - Win32 Release"

InputPath=.\cubit.idl

"cubitC.h"	"cubitC.i"	"cubitC.cpp"	"cubitS.h"	"cubitS.i"	"cubitS.cpp"	 : \
$(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\tao_idl\tao_idl cubit.idl

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

InputPath=.\cubit.idl

"cubitC.h"	"cubitC.i"	"cubitC.cpp"	"cubitS.h"	"cubitS.i"	"cubitS.cpp"	 : \
$(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\tao_idl\tao_idl cubit.idl

!ENDIF 

SOURCE=.\cubitC.cpp

"$(INTDIR)\cubitC.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Task_Client.cpp
DEP_CPP_TASK_=\
	"..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\ace\Containers.i"\
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
	"..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\ace\iosfwd.h"\
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
	"..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\ace\Signal.i"\
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
	".\Task_Client.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_TASK_=\
	".\cubitC.h"\
	".\my_time.h"\
	

"$(INTDIR)\Task_Client.obj" : $(SOURCE) $(DEP_CPP_TASK_) "$(INTDIR)"\
 ".\cubitC.h"


SOURCE=.\Util_Thread.cpp
DEP_CPP_UTIL_=\
	"..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\ace\Containers.i"\
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
	"..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\ace\iosfwd.h"\
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
	"..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\ace\Signal.i"\
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
	".\Task_Client.h"\
	".\Util_Thread.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_UTIL_=\
	".\cubitC.h"\
	

"$(INTDIR)\Util_Thread.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 ".\cubitC.h"



!ENDIF 

