# Microsoft Developer Studio Project File - Name="Throughput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Throughput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Throughput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Throughput.mak" CFG="Throughput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Throughput - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Throughput - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Throughput - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\..\..\\" /I "..\..\..\..\..\..\\" /I "..\..\lib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_CosNotification.lib TAO_CosNaming.lib TAO_Svc_Utils.lib /nologo /subsystem:console /incremental:yes /pdb:"Throughput.pdb" /map /debug /machine:I386 /out:"Throughput.exe" /pdbtype:sept /libpath:"..\..\..\..\orbsvcs" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Throughput - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\lib" /I "..\..\..\..\..\..\\" /I "..\..\lib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"Release/Throughput.pch" /YX /Fo"Release/"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_CosNotificationd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_NotifyTestsd.lib /nologo /subsystem:console /pdb:"Throughput.pdb" /map:"Release/Throughput.map" /debug /machine:I386 /out:"Throughput.exe" /pdbtype:sept /libpath:"..\..\..\..\orbsvcs" /libpath:"..\..\..\..\..\tao" /libpath:"..\..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\..\ace" /libpath:"..\..\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Throughput - Win32 Release"
# Name "Throughput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Throughput.cpp
DEP_CPP_THROU=\
	"..\..\..\..\..\..\ace\ACE.h"\
	"..\..\..\..\..\..\ace\ACE.i"\
	"..\..\..\..\..\..\ace\ACE_export.h"\
	"..\..\..\..\..\..\ace\ace_wchar.h"\
	"..\..\..\..\..\..\ace\ace_wchar.inl"\
	"..\..\..\..\..\..\ace\Arg_Shifter.h"\
	"..\..\..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\..\..\ace\Base_Thread_Adapter.h"\
	"..\..\..\..\..\..\ace\Base_Thread_Adapter.inl"\
	"..\..\..\..\..\..\ace\Basic_Stats.h"\
	"..\..\..\..\..\..\ace\Basic_Stats.inl"\
	"..\..\..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\..\..\ace\CDR_Base.h"\
	"..\..\..\..\..\..\ace\CDR_Base.inl"\
	"..\..\..\..\..\..\ace\CDR_Stream.h"\
	"..\..\..\..\..\..\ace\CDR_Stream.i"\
	"..\..\..\..\..\..\ace\config-all.h"\
	"..\..\..\..\..\..\ace\config-win32-borland.h"\
	"..\..\..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\..\..\ace\config-win32-msvc.h"\
	"..\..\..\..\..\..\ace\config-win32-visualage.h"\
	"..\..\..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\..\..\ace\config-WinCE.h"\
	"..\..\..\..\..\..\ace\config.h"\
	"..\..\..\..\..\..\ace\Containers.h"\
	"..\..\..\..\..\..\ace\Containers.i"\
	"..\..\..\..\..\..\ace\Containers_T.cpp"\
	"..\..\..\..\..\..\ace\Containers_T.h"\
	"..\..\..\..\..\..\ace\Containers_T.i"\
	"..\..\..\..\..\..\ace\CORBA_macros.h"\
	"..\..\..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\..\..\ace\File_Lock.h"\
	"..\..\..\..\..\..\ace\File_Lock.inl"\
	"..\..\..\..\..\..\ace\Flag_Manip.h"\
	"..\..\..\..\..\..\ace\Flag_Manip.i"\
	"..\..\..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\..\..\ace\Functor.h"\
	"..\..\..\..\..\..\ace\Functor.i"\
	"..\..\..\..\..\..\ace\Functor_T.cpp"\
	"..\..\..\..\..\..\ace\Functor_T.h"\
	"..\..\..\..\..\..\ace\Functor_T.i"\
	"..\..\..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\..\..\ace\Handle_Ops.h"\
	"..\..\..\..\..\..\ace\Handle_Ops.i"\
	"..\..\..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\..\..\..\ace\Init_ACE.h"\
	"..\..\..\..\..\..\ace\Init_ACE.i"\
	"..\..\..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\..\..\ace\Lib_Find.h"\
	"..\..\..\..\..\..\ace\Lib_Find.i"\
	"..\..\..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\..\..\ace\Log_Msg_Callback.h"\
	"..\..\..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\..\..\ace\Malloc_Allocator.h"\
	"..\..\..\..\..\..\ace\Malloc_Allocator.i"\
	"..\..\..\..\..\..\ace\Malloc_Base.h"\
	"..\..\..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\..\..\..\ace\Message_Block_T.h"\
	"..\..\..\..\..\..\ace\Message_Block_T.i"\
	"..\..\..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\..\..\ace\Message_Queue_T.cpp"\
	"..\..\..\..\..\..\ace\Message_Queue_T.h"\
	"..\..\..\..\..\..\ace\Message_Queue_T.i"\
	"..\..\..\..\..\..\ace\Min_Max.h"\
	"..\..\..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\..\..\ace\Module.h"\
	"..\..\..\..\..\..\ace\Module.i"\
	"..\..\..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\..\..\ace\OS.h"\
	"..\..\..\..\..\..\ace\OS.i"\
	"..\..\..\..\..\..\ace\OS_Dirent.h"\
	"..\..\..\..\..\..\ace\OS_Dirent.inl"\
	"..\..\..\..\..\..\ace\OS_Errno.h"\
	"..\..\..\..\..\..\ace\OS_Errno.inl"\
	"..\..\..\..\..\..\ace\OS_Export.h"\
	"..\..\..\..\..\..\ace\OS_Log_Msg_Attributes.h"\
	"..\..\..\..\..\..\ace\OS_Log_Msg_Attributes.inl"\
	"..\..\..\..\..\..\ace\OS_Memory.h"\
	"..\..\..\..\..\..\ace\OS_Memory.inl"\
	"..\..\..\..\..\..\ace\OS_String.h"\
	"..\..\..\..\..\..\ace\OS_String.inl"\
	"..\..\..\..\..\..\ace\OS_TLI.h"\
	"..\..\..\..\..\..\ace\OS_TLI.inl"\
	"..\..\..\..\..\..\ace\post.h"\
	"..\..\..\..\..\..\ace\pre.h"\
	"..\..\..\..\..\..\ace\Process_Mutex.h"\
	"..\..\..\..\..\..\ace\Process_Mutex.inl"\
	"..\..\..\..\..\..\ace\Process_Semaphore.h"\
	"..\..\..\..\..\..\ace\Process_Semaphore.inl"\
	"..\..\..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\..\..\ace\RW_Process_Mutex.h"\
	"..\..\..\..\..\..\ace\RW_Process_Mutex.inl"\
	"..\..\..\..\..\..\ace\Sched_Params.h"\
	"..\..\..\..\..\..\ace\Sched_Params.i"\
	"..\..\..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\..\..\ace\Signal.h"\
	"..\..\..\..\..\..\ace\Signal.i"\
	"..\..\..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\..\..\ace\Sock_Connect.h"\
	"..\..\..\..\..\..\ace\Sock_Connect.i"\
	"..\..\..\..\..\..\ace\SString.h"\
	"..\..\..\..\..\..\ace\SString.i"\
	"..\..\..\..\..\..\ace\Stats.h"\
	"..\..\..\..\..\..\ace\Stats.i"\
	"..\..\..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\..\..\ace\Strategies.i"\
	"..\..\..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\..\..\ace\Strategies_T.i"\
	"..\..\..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\..\..\ace\streams.h"\
	"..\..\..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\..\..\ace\svc_export.h"\
	"..\..\..\..\..\..\ace\Synch.h"\
	"..\..\..\..\..\..\ace\Synch.i"\
	"..\..\..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\..\..\ace\Synch_Options.i"\
	"..\..\..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\..\..\ace\Task.h"\
	"..\..\..\..\..\..\ace\Task.i"\
	"..\..\..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\..\..\ace\Test_and_Set.cpp"\
	"..\..\..\..\..\..\ace\Test_and_Set.h"\
	"..\..\..\..\..\..\ace\Test_and_Set.i"\
	"..\..\..\..\..\..\ace\Thread.h"\
	"..\..\..\..\..\..\ace\Thread.i"\
	"..\..\..\..\..\..\ace\Thread_Adapter.h"\
	"..\..\..\..\..\..\ace\Thread_Adapter.inl"\
	"..\..\..\..\..\..\ace\Thread_Control.h"\
	"..\..\..\..\..\..\ace\Thread_Control.inl"\
	"..\..\..\..\..\..\ace\Thread_Exit.h"\
	"..\..\..\..\..\..\ace\Thread_Hook.h"\
	"..\..\..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\..\..\ace\Trace.h"\
	"..\..\..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\..\tao\Abstract_Servant_Base.h"\
	"..\..\..\..\..\tao\Adapter.h"\
	"..\..\..\..\..\tao\Adapter.i"\
	"..\..\..\..\..\tao\Any.h"\
	"..\..\..\..\..\tao\Any.i"\
	"..\..\..\..\..\tao\BiDirPolicyC.h"\
	"..\..\..\..\..\tao\BiDirPolicyC.i"\
	"..\..\..\..\..\tao\BoundsC.h"\
	"..\..\..\..\..\tao\BoundsC.i"\
	"..\..\..\..\..\tao\Cache_Entries.h"\
	"..\..\..\..\..\tao\Cache_Entries.inl"\
	"..\..\..\..\..\tao\CDR.h"\
	"..\..\..\..\..\tao\CDR.i"\
	"..\..\..\..\..\tao\Cleanup_Func_Registry.h"\
	"..\..\..\..\..\tao\Cleanup_Func_Registry.inl"\
	"..\..\..\..\..\tao\Client_Strategy_Factory.h"\
	"..\..\..\..\..\tao\ClientRequestInfo.h"\
	"..\..\..\..\..\tao\ClientRequestInfo.inl"\
	"..\..\..\..\..\tao\Connection_Cache_Manager.h"\
	"..\..\..\..\..\tao\Connection_Cache_Manager.inl"\
	"..\..\..\..\..\tao\Connection_Descriptor_Interface.h"\
	"..\..\..\..\..\tao\Connection_Descriptor_Interface.inl"\
	"..\..\..\..\..\tao\CONV_FRAMEC.h"\
	"..\..\..\..\..\tao\CONV_FRAMEC.i"\
	"..\..\..\..\..\tao\corba.h"\
	"..\..\..\..\..\tao\CORBA_String.h"\
	"..\..\..\..\..\tao\CORBA_String.inl"\
	"..\..\..\..\..\tao\corbafwd.h"\
	"..\..\..\..\..\tao\corbafwd.i"\
	"..\..\..\..\..\tao\CurrentC.h"\
	"..\..\..\..\..\tao\CurrentC.i"\
	"..\..\..\..\..\tao\debug.h"\
	"..\..\..\..\..\tao\DomainC.h"\
	"..\..\..\..\..\tao\DomainC.i"\
	"..\..\..\..\..\tao\DynamicC.h"\
	"..\..\..\..\..\tao\DynamicC.i"\
	"..\..\..\..\..\tao\Encodable.h"\
	"..\..\..\..\..\tao\Endpoint.h"\
	"..\..\..\..\..\tao\Endpoint.i"\
	"..\..\..\..\..\tao\Environment.h"\
	"..\..\..\..\..\tao\Environment.i"\
	"..\..\..\..\..\tao\Exception.h"\
	"..\..\..\..\..\tao\Exception.i"\
	"..\..\..\..\..\tao\Fault_Tolerance_Service.h"\
	"..\..\..\..\..\tao\Fault_Tolerance_Service.i"\
	"..\..\..\..\..\tao\GIOP_Message_State.h"\
	"..\..\..\..\..\tao\GIOP_Message_State.i"\
	"..\..\..\..\..\tao\GIOPC.h"\
	"..\..\..\..\..\tao\GIOPC.i"\
	"..\..\..\..\..\tao\Interceptor_List.h"\
	"..\..\..\..\..\tao\Interceptor_List.inl"\
	"..\..\..\..\..\tao\Invocation.h"\
	"..\..\..\..\..\tao\Invocation.i"\
	"..\..\..\..\..\tao\Invocation_Endpoint_Selectors.h"\
	"..\..\..\..\..\tao\Invocation_Endpoint_Selectors.i"\
	"..\..\..\..\..\tao\IOPC.h"\
	"..\..\..\..\..\tao\IOPC.i"\
	"..\..\..\..\..\tao\IOPS.h"\
	"..\..\..\..\..\tao\IOPS.i"\
	"..\..\..\..\..\tao\IOPS_T.cpp"\
	"..\..\..\..\..\tao\IOPS_T.h"\
	"..\..\..\..\..\tao\IOPS_T.i"\
	"..\..\..\..\..\tao\LocalObject.h"\
	"..\..\..\..\..\tao\LocalObject.i"\
	"..\..\..\..\..\tao\Managed_Types.h"\
	"..\..\..\..\..\tao\Managed_Types.i"\
	"..\..\..\..\..\tao\MessagingC.h"\
	"..\..\..\..\..\tao\MessagingC.i"\
	"..\..\..\..\..\tao\MProfile.h"\
	"..\..\..\..\..\tao\MProfile.i"\
	"..\..\..\..\..\tao\NVList.h"\
	"..\..\..\..\..\tao\NVList.i"\
	"..\..\..\..\..\tao\Object.h"\
	"..\..\..\..\..\tao\Object.i"\
	"..\..\..\..\..\tao\Object_KeyC.h"\
	"..\..\..\..\..\tao\Object_KeyC.i"\
	"..\..\..\..\..\tao\Object_Proxy_Broker.h"\
	"..\..\..\..\..\tao\Object_Proxy_Impl.h"\
	"..\..\..\..\..\tao\Object_Ref_Table.h"\
	"..\..\..\..\..\tao\ObjectIDList.h"\
	"..\..\..\..\..\tao\ObjectIDList.i"\
	"..\..\..\..\..\tao\OctetSeqC.h"\
	"..\..\..\..\..\tao\OctetSeqC.i"\
	"..\..\..\..\..\tao\operation_details.h"\
	"..\..\..\..\..\tao\operation_details.i"\
	"..\..\..\..\..\tao\ORB.h"\
	"..\..\..\..\..\tao\ORB.i"\
	"..\..\..\..\..\tao\ORB_Core.h"\
	"..\..\..\..\..\tao\ORB_Core.i"\
	"..\..\..\..\..\tao\orbconf.h"\
	"..\..\..\..\..\tao\params.h"\
	"..\..\..\..\..\tao\params.i"\
	"..\..\..\..\..\tao\Parser_Registry.h"\
	"..\..\..\..\..\tao\Parser_Registry.i"\
	"..\..\..\..\..\tao\Pluggable.h"\
	"..\..\..\..\..\tao\Pluggable.i"\
	"..\..\..\..\..\tao\Policy_Manager.h"\
	"..\..\..\..\..\tao\Policy_Manager.i"\
	"..\..\..\..\..\tao\PolicyC.h"\
	"..\..\..\..\..\tao\PolicyC.i"\
	"..\..\..\..\..\tao\PolicyFactory_Registry.h"\
	"..\..\..\..\..\tao\PollableC.h"\
	"..\..\..\..\..\tao\PollableC.i"\
	"..\..\..\..\..\tao\PortableInterceptor.h"\
	"..\..\..\..\..\tao\PortableInterceptor.i"\
	"..\..\..\..\..\tao\PortableInterceptorC.h"\
	"..\..\..\..\..\tao\PortableInterceptorC.i"\
	"..\..\..\..\..\tao\PortableServer\Collocated_Object.h"\
	"..\..\..\..\..\tao\PortableServer\Collocated_Object.i"\
	"..\..\..\..\..\tao\PortableServer\Direct_Object_Proxy_Impl.h"\
	"..\..\..\..\..\tao\PortableServer\PortableServer.h"\
	"..\..\..\..\..\tao\PortableServer\portableserver_export.h"\
	"..\..\..\..\..\tao\PortableServer\PortableServerC.h"\
	"..\..\..\..\..\tao\PortableServer\PortableServerC.i"\
	"..\..\..\..\..\tao\PortableServer\RTPortableServerC.h"\
	"..\..\..\..\..\tao\PortableServer\RTPortableServerC.i"\
	"..\..\..\..\..\tao\PortableServer\Servant_Base.h"\
	"..\..\..\..\..\tao\PortableServer\Servant_Base.i"\
	"..\..\..\..\..\tao\PortableServer\ServerRequestInfo.h"\
	"..\..\..\..\..\tao\PortableServer\ServerRequestInfo.inl"\
	"..\..\..\..\..\tao\PortableServer\ThruPOA_Object_Proxy_Impl.h"\
	"..\..\..\..\..\tao\Principal.h"\
	"..\..\..\..\..\tao\Principal.i"\
	"..\..\..\..\..\tao\Priority_Mapping.h"\
	"..\..\..\..\..\tao\Priority_Mapping.i"\
	"..\..\..\..\..\tao\Priority_Mapping_Manager.h"\
	"..\..\..\..\..\tao\Priority_Mapping_Manager.i"\
	"..\..\..\..\..\tao\Profile.h"\
	"..\..\..\..\..\tao\Profile.i"\
	"..\..\..\..\..\tao\Protocol_Factory.h"\
	"..\..\..\..\..\tao\Protocols_Hooks.h"\
	"..\..\..\..\..\tao\Remote_Object_Proxy_Impl.h"\
	"..\..\..\..\..\tao\Reply_Dispatcher.h"\
	"..\..\..\..\..\tao\Reply_Dispatcher.i"\
	"..\..\..\..\..\tao\Resource_Factory.h"\
	"..\..\..\..\..\tao\RT_Policy_i.h"\
	"..\..\..\..\..\tao\RT_Policy_i.i"\
	"..\..\..\..\..\tao\RTCORBAC.h"\
	"..\..\..\..\..\tao\RTCORBAC.i"\
	"..\..\..\..\..\tao\RTCORBAS.h"\
	"..\..\..\..\..\tao\RTCORBAS.i"\
	"..\..\..\..\..\tao\RTCORBAS_T.cpp"\
	"..\..\..\..\..\tao\RTCORBAS_T.h"\
	"..\..\..\..\..\tao\RTCORBAS_T.i"\
	"..\..\..\..\..\tao\Sequence.h"\
	"..\..\..\..\..\tao\Sequence.i"\
	"..\..\..\..\..\tao\Sequence_T.cpp"\
	"..\..\..\..\..\tao\Sequence_T.h"\
	"..\..\..\..\..\tao\Sequence_T.i"\
	"..\..\..\..\..\tao\Server_Strategy_Factory.h"\
	"..\..\..\..\..\tao\Service_Callbacks.h"\
	"..\..\..\..\..\tao\Service_Callbacks.i"\
	"..\..\..\..\..\tao\Service_Context.h"\
	"..\..\..\..\..\tao\Service_Context.inl"\
	"..\..\..\..\..\tao\Services.h"\
	"..\..\..\..\..\tao\Services.i"\
	"..\..\..\..\..\tao\StringSeqC.h"\
	"..\..\..\..\..\tao\StringSeqC.i"\
	"..\..\..\..\..\tao\Stub.h"\
	"..\..\..\..\..\tao\Stub.i"\
	"..\..\..\..\..\tao\Synch_Reply_Dispatcher.h"\
	"..\..\..\..\..\tao\Tagged_Components.h"\
	"..\..\..\..\..\tao\Tagged_Components.i"\
	"..\..\..\..\..\tao\Tagged_Profile.h"\
	"..\..\..\..\..\tao\Tagged_Profile.i"\
	"..\..\..\..\..\tao\TAO_Export.h"\
	"..\..\..\..\..\tao\TAO_Server_Request.h"\
	"..\..\..\..\..\tao\TAO_Server_Request.i"\
	"..\..\..\..\..\tao\TAO_Singleton.cpp"\
	"..\..\..\..\..\tao\TAO_Singleton.h"\
	"..\..\..\..\..\tao\TAO_Singleton.inl"\
	"..\..\..\..\..\tao\TAO_Singleton_Manager.h"\
	"..\..\..\..\..\tao\TAO_Singleton_Manager.inl"\
	"..\..\..\..\..\tao\TAOC.h"\
	"..\..\..\..\..\tao\TAOC.i"\
	"..\..\..\..\..\tao\target_specification.h"\
	"..\..\..\..\..\tao\target_specification.i"\
	"..\..\..\..\..\tao\TimeBaseC.h"\
	"..\..\..\..\..\tao\TimeBaseC.i"\
	"..\..\..\..\..\tao\TimeBaseS.h"\
	"..\..\..\..\..\tao\TimeBaseS.i"\
	"..\..\..\..\..\tao\TimeBaseS_T.cpp"\
	"..\..\..\..\..\tao\TimeBaseS_T.h"\
	"..\..\..\..\..\tao\TimeBaseS_T.i"\
	"..\..\..\..\..\tao\try_macros.h"\
	"..\..\..\..\..\tao\Typecode.h"\
	"..\..\..\..\..\tao\Typecode.i"\
	"..\..\..\..\..\tao\ValueBase.h"\
	"..\..\..\..\..\tao\ValueBase.i"\
	"..\..\..\..\..\tao\ValueFactory.h"\
	"..\..\..\..\..\tao\ValueFactory.i"\
	"..\..\..\..\..\tao\varbase.h"\
	"..\..\..\..\..\tao\WrongTransactionC.h"\
	"..\..\..\..\orbsvcs\CosEvent\event_export.h"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminC.h"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminC.i"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminS.h"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminS.i"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminS_T.cpp"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminS_T.h"\
	"..\..\..\..\orbsvcs\CosEventChannelAdminS_T.i"\
	"..\..\..\..\orbsvcs\CosEventCommC.h"\
	"..\..\..\..\orbsvcs\CosEventCommC.i"\
	"..\..\..\..\orbsvcs\CosEventCommS.h"\
	"..\..\..\..\orbsvcs\CosEventCommS.i"\
	"..\..\..\..\orbsvcs\CosEventCommS_T.cpp"\
	"..\..\..\..\orbsvcs\CosEventCommS_T.h"\
	"..\..\..\..\orbsvcs\CosEventCommS_T.i"\
	"..\..\..\..\orbsvcs\CosNamingC.h"\
	"..\..\..\..\orbsvcs\CosNamingC.i"\
	"..\..\..\..\orbsvcs\CosNotificationC.h"\
	"..\..\..\..\orbsvcs\CosNotificationC.i"\
	"..\..\..\..\orbsvcs\CosNotificationS.h"\
	"..\..\..\..\orbsvcs\CosNotificationS.i"\
	"..\..\..\..\orbsvcs\CosNotificationS_T.cpp"\
	"..\..\..\..\orbsvcs\CosNotificationS_T.h"\
	"..\..\..\..\orbsvcs\CosNotificationS_T.i"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminC.h"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminC.i"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminS.h"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminS.i"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminS_T.cpp"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminS_T.h"\
	"..\..\..\..\orbsvcs\CosNotifyChannelAdminS_T.i"\
	"..\..\..\..\orbsvcs\CosNotifyCommC.h"\
	"..\..\..\..\orbsvcs\CosNotifyCommC.i"\
	"..\..\..\..\orbsvcs\CosNotifyCommS.h"\
	"..\..\..\..\orbsvcs\CosNotifyCommS.i"\
	"..\..\..\..\orbsvcs\CosNotifyCommS_T.cpp"\
	"..\..\..\..\orbsvcs\CosNotifyCommS_T.h"\
	"..\..\..\..\orbsvcs\CosNotifyCommS_T.i"\
	"..\..\..\..\orbsvcs\CosNotifyFilterC.h"\
	"..\..\..\..\orbsvcs\CosNotifyFilterC.i"\
	"..\..\..\..\orbsvcs\CosNotifyFilterS.h"\
	"..\..\..\..\orbsvcs\CosNotifyFilterS.i"\
	"..\..\..\..\orbsvcs\CosNotifyFilterS_T.cpp"\
	"..\..\..\..\orbsvcs\CosNotifyFilterS_T.h"\
	"..\..\..\..\orbsvcs\CosNotifyFilterS_T.i"\
	"..\..\..\..\orbsvcs\ESF\ESF_Worker.cpp"\
	"..\..\..\..\orbsvcs\ESF\ESF_Worker.h"\
	"..\..\..\..\orbsvcs\ESF\ESF_Worker.i"\
	"..\..\..\..\orbsvcs\Naming\naming_export.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Channel_Objects_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Collection.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Collection_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Default_CO_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Default_Collection_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Default_EMO_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Default_POA_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Event.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_Event.i"\
	"..\..\..\..\orbsvcs\Notify\Notify_Event_Manager_Objects_Factory.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_EventChannelFactory_i.h"\
	"..\..\..\..\orbsvcs\Notify\notify_export.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_ID_Pool_T.cpp"\
	"..\..\..\..\orbsvcs\Notify\Notify_ID_Pool_T.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_ID_Pool_T.i"\
	"..\..\..\..\orbsvcs\Notify\Notify_Listeners.h"\
	"..\..\..\..\orbsvcs\Notify\Notify_POA_Factory.h"\
	"..\..\..\..\orbsvcs\svc_utils_export.h"\
	"..\..\..\..\orbsvcs\Time_Utilities.h"\
	"..\..\..\..\orbsvcs\Time_Utilities.i"\
	"..\..\..\..\orbsvcs\TimeBaseC.h"\
	"..\..\..\..\orbsvcs\TimeBaseC.i"\
	"..\..\lib\notify_structuredpushconsumer.h"\
	"..\..\lib\notify_structuredpushsupplier.h"\
	"..\..\lib\notify_test_client.h"\
	"..\..\lib\notify_test_export.h"\
	".\Throughput.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
