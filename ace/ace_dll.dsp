# Microsoft Developer Studio Project File - Name="ACE DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ACE DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll.mak" CFG="ACE DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE DLL - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE DLL - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ACE DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACE DLL - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE_DLL___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "ACE_DLL___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "../" /D ACE_HAS_DLL=1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /Ob2 /I "../" /I "../PACE" /D "NDEBUG" /D ACE_HAS_MFC=1 /D ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER=1 /D "WIN32" /D "_WINDOWS" /D "ACE_BUILD_DLL" /D "ACE_OS_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\ace-r.pdb" /machine:I386 /out:"..\bin\ace.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\acemfc.pdb" /map /debug /machine:I386 /out:"..\bin\acemfc.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE DLL - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_DLL___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "ACE_DLL___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /D ACE_HAS_DLL=1 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /I "../PACE" /D "_DEBUG" /D ACE_HAS_MFC=1 /D ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER=1 /D "WIN32" /D "_WINDOWS" /D "ACE_BUILD_DLL" /D "ACE_OS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\ace.pdb" /debug /machine:I386 /out:"..\bin\aced.dll" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\acemfcd.pdb" /debug /machine:I386 /out:"..\bin\acemfcd.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE DLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /Ob2 /I "../" /I "../PACE" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ACE_BUILD_DLL" /D "ACE_OS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\ace.pdb" /map /debug /machine:I386 /out:"..\bin\ace.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ACE DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ".\DLL\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /I "../PACE" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ACE_BUILD_DLL" /D "ACE_OS_BUILD_DLL" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\ace.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 advapi32.lib user32.lib /nologo /subsystem:windows /dll /pdb:".\aced.pdb" /debug /machine:I386 /out:"..\bin\aced.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ACE DLL - Win32 MFC Release"
# Name "ACE DLL - Win32 MFC Debug"
# Name "ACE DLL - Win32 Release"
# Name "ACE DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ACE.cpp
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\Arg_Shifter.cpp
# End Source File
# Begin Source File

SOURCE=.\ARGV.cpp
# End Source File
# Begin Source File

SOURCE=.\Argv_Type_Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Pseudo_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_Params.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_QoS.cpp
# End Source File
# Begin Source File

SOURCE=.\ATM_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Atomic_Op.cpp
# End Source File
# Begin Source File

SOURCE=.\Base_Thread_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_Repository.cpp
# End Source File
# Begin Source File

SOURCE=.\Basic_Stats.cpp
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Capabilities.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Codecs.cpp
# End Source File
# Begin Source File

SOURCE=.\Codeset_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Codeset_Registry_db.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuration_Import_Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection_Recycling_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Containers.cpp
# End Source File
# Begin Source File

SOURCE=.\Copy_Disabled.cpp
# End Source File
# Begin Source File

SOURCE=.\Date_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\DEV.cpp
# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\DEV_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Dirent.cpp
# End Source File
# Begin Source File

SOURCE=.\Dirent_Selector.cpp
# End Source File
# Begin Source File

SOURCE=.\DLL.cpp
# End Source File
# Begin Source File

SOURCE=.\DLL_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Dump.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\File_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Filecache.cpp
# End Source File
# Begin Source File

SOURCE=.\Flag_Manip.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework_Component.cpp
# End Source File
# Begin Source File

SOURCE=.\Functor.cpp
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Ops.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Hashable.cpp
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\Init_ACE.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_SAP.cpp
# End Source File
# Begin Source File

SOURCE=.\IOStream.cpp
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib_Find.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_Backend.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_Callback.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_IPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_NT_Event_Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_UNIX_Syslog.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp
# End Source File
# Begin Source File

SOURCE=.\Logging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Malloc.cpp
# End Source File
# Begin Source File

SOURCE=.\Malloc_Allocator.cpp
# End Source File
# Begin Source File

SOURCE=.\Malloc_Instantiations.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_SAP.cpp
# End Source File
# Begin Source File

SOURCE=.\MEM_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.cpp
# End Source File
# Begin Source File

SOURCE=.\Message_Block.cpp
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Method_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Metrics_Cache.cpp
# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Multihomed_INET_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.cpp
# End Source File
# Begin Source File

SOURCE=.\Name_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp
# End Source File
# Begin Source File

SOURCE=.\Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\Notification_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\NT_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Obchunk.cpp
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Obstack.cpp
# End Source File
# Begin Source File

SOURCE=.\OS.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_Dirent.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_Errno.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_Log_Msg_Attributes.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_Memory.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_QoS.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_String.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_Thread_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\OS_TLI.cpp
# End Source File
# Begin Source File

SOURCE=.\Pair.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp
# End Source File
# Begin Source File

SOURCE=.\PI_Malloc.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp
# End Source File
# Begin Source File

SOURCE=.\POSIX_Asynch_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\POSIX_Proactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Proactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Process.cpp
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Process_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Process_Semaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor_Notification_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Recyclable.cpp
# End Source File
# Begin Source File

SOURCE=.\Refcountable.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.cpp
# End Source File
# Begin Source File

SOURCE=.\RW_Process_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample_History.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched_Params.cpp
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Templates.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory.cpp
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp
# End Source File
# Begin Source File

SOURCE=.\Shared_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Signal.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp
# End Source File
# Begin Source File

SOURCE=.\Sock_Connect.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Association.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPE.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\SString.cpp
# End Source File
# Begin Source File

SOURCE=.\Stats.cpp
# End Source File
# Begin Source File

SOURCE=.\String_Base_Const.cpp
# End Source File
# Begin Source File

SOURCE=.\SV_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp
# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_Lexer_Guard.cpp
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp
# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\System_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Template_Instantiations.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Control.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Exit.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Hook.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Value.cpp
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_Adapters.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel.cpp
# End Source File
# Begin Source File

SOURCE=.\TLI.cpp
# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Token.cpp
# End Source File
# Begin Source File

SOURCE=.\Token_Collection.cpp
# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.cpp
# End Source File
# Begin Source File

SOURCE=.\Token_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp
# End Source File
# Begin Source File

SOURCE=.\TP_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\TTY_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\WIN32_Asynch_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_Svc_Conf.cpp
# End Source File
# Begin Source File

SOURCE=.\XtReactor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\ACE.h
# End Source File
# Begin Source File

SOURCE=.\ACE_export.h
# End Source File
# Begin Source File

SOURCE=.\ace_wchar.h
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

SOURCE=.\Argv_Type_Converter.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Connector.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Pseudo_Task.h
# End Source File
# Begin Source File

SOURCE=.\ATM_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\ATM_Addr.h
# End Source File
# Begin Source File

SOURCE=.\ATM_Connector.h
# End Source File
# Begin Source File

SOURCE=.\ATM_Params.h
# End Source File
# Begin Source File

SOURCE=.\ATM_QoS.h
# End Source File
# Begin Source File

SOURCE=.\ATM_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Atomic_Op.h
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Base_Thread_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_Repository.h
# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_T.h
# End Source File
# Begin Source File

SOURCE=.\Basic_Stats.h
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.h
# End Source File
# Begin Source File

SOURCE=.\Bound_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Cache_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Cached_Connect_Strategy_T.h
# End Source File
# Begin Source File

SOURCE=.\Caching_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\Caching_Utility_T.h
# End Source File
# Begin Source File

SOURCE=.\Capabilities.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Base.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\Codecs.h
# End Source File
# Begin Source File

SOURCE=.\Codeset_Registry.h
# End Source File
# Begin Source File

SOURCE=".\config-all.h"
# End Source File
# Begin Source File

SOURCE=".\config-all.h"
# End Source File
# Begin Source File

SOURCE=".\config-win32-common.h"
# End Source File
# Begin Source File

SOURCE=".\config-win32-msvc.h"
# End Source File
# Begin Source File

SOURCE=".\config-win32.h"
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\Configuration.h
# End Source File
# Begin Source File

SOURCE=.\Configuration_Import_Export.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Recycling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Connector.h
# End Source File
# Begin Source File

SOURCE=.\Containers.h
# End Source File
# Begin Source File

SOURCE=.\Containers_T.h
# End Source File
# Begin Source File

SOURCE=.\Copy_Disabled.h
# End Source File
# Begin Source File

SOURCE=.\Date_Time.h
# End Source File
# Begin Source File

SOURCE=.\Default_Constants.h
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

SOURCE=.\Dirent_Selector.h
# End Source File
# Begin Source File

SOURCE=.\DLL.h
# End Source File
# Begin Source File

SOURCE=.\DLL_Manager.h
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

SOURCE=.\Dynamic_Service_Base.h
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

SOURCE=.\File_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Filecache.h
# End Source File
# Begin Source File

SOURCE=.\Flag_Manip.h
# End Source File
# Begin Source File

SOURCE=.\Framework_Component.h
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

SOURCE=.\Global_Macros.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Ops.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Cache_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_rt.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_With_Allocator_T.h
# End Source File
# Begin Source File

SOURCE=.\Hashable.h
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.h
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.h
# End Source File
# Begin Source File

SOURCE=.\Init_ACE.h
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

SOURCE=.\Lib_Find.h
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

SOURCE=.\Log_Msg_Backend.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_Callback.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_IPC.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_NT_Event_Log.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg_UNIX_Syslog.h
# End Source File
# Begin Source File

SOURCE=.\Log_Priority.h
# End Source File
# Begin Source File

SOURCE=.\Log_Record.h
# End Source File
# Begin Source File

SOURCE=.\Logging_Strategy.h
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

SOURCE=.\Malloc_Allocator.h
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

SOURCE=.\MEM_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\MEM_Addr.h
# End Source File
# Begin Source File

SOURCE=.\MEM_Connector.h
# End Source File
# Begin Source File

SOURCE=.\MEM_IO.h
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.h
# End Source File
# Begin Source File

SOURCE=.\MEM_SAP.h
# End Source File
# Begin Source File

SOURCE=.\MEM_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.h
# End Source File
# Begin Source File

SOURCE=.\Message_Block.h
# End Source File
# Begin Source File

SOURCE=.\Message_Block_T.h
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

SOURCE=.\Metrics_Cache.h
# End Source File
# Begin Source File

SOURCE=.\Metrics_Cache_T.h
# End Source File
# Begin Source File

SOURCE=.\Min_Max.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Multihomed_INET_Addr.h
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

SOURCE=.\Notification_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\NT_Service.h
# End Source File
# Begin Source File

SOURCE=.\Obchunk.h
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Obstack.h
# End Source File
# Begin Source File

SOURCE=.\Obstack_T.h
# End Source File
# Begin Source File

SOURCE=.\OS.h
# End Source File
# Begin Source File

SOURCE=.\OS_Dirent.h
# End Source File
# Begin Source File

SOURCE=.\OS_Errno.h
# End Source File
# Begin Source File

SOURCE=.\OS_Log_Msg_Attributes.h
# End Source File
# Begin Source File

SOURCE=.\OS_Memory.h
# End Source File
# Begin Source File

SOURCE=.\OS_QoS.h
# End Source File
# Begin Source File

SOURCE=.\OS_String.h
# End Source File
# Begin Source File

SOURCE=.\OS_Thread_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\OS_TLI.h
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

SOURCE=.\PI_Malloc.h
# End Source File
# Begin Source File

SOURCE=.\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\POSIX_Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\POSIX_Proactor.h
# End Source File
# Begin Source File

SOURCE=.\post.h
# End Source File
# Begin Source File

SOURCE=.\pre.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Proactor.h
# End Source File
# Begin Source File

SOURCE=.\Proactor_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Process_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\Process_Semaphore.h
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Decorator.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Manager.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Session.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Factory.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Impl.h
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

SOURCE=.\Reactor_Notification_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Recyclable.h
# End Source File
# Begin Source File

SOURCE=.\Refcountable.h
# End Source File
# Begin Source File

SOURCE=.\Refcounted_Auto_Ptr.h
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

SOURCE=.\RW_Process_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\Sample_History.h
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

SOURCE=.\Service_Templates.h
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

SOURCE=.\Sock_Connect.h
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

SOURCE=.\SOCK_Dgram_Mcast_QoS.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Association.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Connector.h
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

SOURCE=.\String_Base.h
# End Source File
# Begin Source File

SOURCE=.\String_Base_Const.h
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

SOURCE=.\Svc_Conf_Lexer_Guard.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\svc_export.h
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

SOURCE=.\Test_and_Set.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Control.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Exit.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Hook.h
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

SOURCE=.\Timeprobe.h
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

SOURCE=.\TP_Reactor.h
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

SOURCE=.\Unbounded_Queue.h
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

SOURCE=.\WIN32_Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.h
# End Source File
# Begin Source File

SOURCE=.\XML_Svc_Conf.h
# End Source File
# Begin Source File

SOURCE=.\XtReactor.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
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

SOURCE=.\Argv_Type_Converter.inl
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.i
# End Source File
# Begin Source File

SOURCE=.\ATM_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\ATM_Addr.i
# End Source File
# Begin Source File

SOURCE=.\ATM_Connector.i
# End Source File
# Begin Source File

SOURCE=.\ATM_Params.i
# End Source File
# Begin Source File

SOURCE=.\ATM_QoS.i
# End Source File
# Begin Source File

SOURCE=.\ATM_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Atomic_Op.i
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Base_Thread_Adapter.inl
# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_T.i
# End Source File
# Begin Source File

SOURCE=.\Basic_Stats.inl
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.i
# End Source File
# Begin Source File

SOURCE=.\Bound_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Cache_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Cached_Connect_Strategy_T.i
# End Source File
# Begin Source File

SOURCE=.\Caching_Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\Caching_Strategy_Utility_T.i
# End Source File
# Begin Source File

SOURCE=.\Capabilities.i
# End Source File
# Begin Source File

SOURCE=.\CDR_Base.inl
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\Codeset_Registry.inl
# End Source File
# Begin Source File

SOURCE=.\Connector.i
# End Source File
# Begin Source File

SOURCE=.\Containers.i
# End Source File
# Begin Source File

SOURCE=.\Containers_T.i
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

SOURCE=.\Dirent_Selector.inl
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

SOURCE=.\File_Lock.inl
# End Source File
# Begin Source File

SOURCE=.\Flag_Manip.i
# End Source File
# Begin Source File

SOURCE=.\Framework_Component.inl
# End Source File
# Begin Source File

SOURCE=.\Free_List.i
# End Source File
# Begin Source File

SOURCE=.\Functor.i
# End Source File
# Begin Source File

SOURCE=.\Functor_T.i
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Ops.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Cache_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_rt.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_With_Allocator_T.i
# End Source File
# Begin Source File

SOURCE=.\Hashable.inl
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.i
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.i
# End Source File
# Begin Source File

SOURCE=.\Init_ACE.i
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

SOURCE=.\Lib_Find.i
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

SOURCE=.\Malloc_Allocator.i
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.i
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.i
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Map_T.i
# End Source File
# Begin Source File

SOURCE=.\MEM_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\MEM_Addr.i
# End Source File
# Begin Source File

SOURCE=.\MEM_Connector.i
# End Source File
# Begin Source File

SOURCE=.\MEM_IO.i
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.i
# End Source File
# Begin Source File

SOURCE=.\MEM_SAP.i
# End Source File
# Begin Source File

SOURCE=.\MEM_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.i
# End Source File
# Begin Source File

SOURCE=.\Message_Block.i
# End Source File
# Begin Source File

SOURCE=.\Message_Block_T.i
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_Cache.i
# End Source File
# Begin Source File

SOURCE=.\Metrics_Cache_T.i
# End Source File
# Begin Source File

SOURCE=.\Module.i
# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Multihomed_INET_Addr.i
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.i
# End Source File
# Begin Source File

SOURCE=.\Notification_Strategy.inl
# End Source File
# Begin Source File

SOURCE=.\NT_Service.i
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Obstack_T.i
# End Source File
# Begin Source File

SOURCE=.\OS.i
# End Source File
# Begin Source File

SOURCE=.\OS_Dirent.inl
# End Source File
# Begin Source File

SOURCE=.\OS_Errno.inl
# End Source File
# Begin Source File

SOURCE=.\OS_Log_Msg_Attributes.inl
# End Source File
# Begin Source File

SOURCE=.\OS_Memory.inl
# End Source File
# Begin Source File

SOURCE=.\OS_String.inl
# End Source File
# Begin Source File

SOURCE=.\OS_Thread_Adapter.inl
# End Source File
# Begin Source File

SOURCE=.\OS_TLI.inl
# End Source File
# Begin Source File

SOURCE=.\Pair_T.i
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.i
# End Source File
# Begin Source File

SOURCE=.\PI_Malloc.i
# End Source File
# Begin Source File

SOURCE=.\Pipe.i
# End Source File
# Begin Source File

SOURCE=.\POSIX_Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\POSIX_Proactor.i
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

SOURCE=.\Process_Mutex.inl
# End Source File
# Begin Source File

SOURCE=.\Process_Semaphore.inl
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.i
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Impl.i
# End Source File
# Begin Source File

SOURCE=.\RB_Tree.i
# End Source File
# Begin Source File

SOURCE=.\Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Reactor_Notification_Strategy.inl
# End Source File
# Begin Source File

SOURCE=.\ReactorEx.i
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.i
# End Source File
# Begin Source File

SOURCE=.\Recyclable.inl
# End Source File
# Begin Source File

SOURCE=.\Refcountable.inl
# End Source File
# Begin Source File

SOURCE=.\Refcounted_Auto_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.i
# End Source File
# Begin Source File

SOURCE=.\RW_Process_Mutex.inl
# End Source File
# Begin Source File

SOURCE=.\Sample_History.inl
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

SOURCE=.\Sock_Connect.i
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

SOURCE=.\SOCK_Dgram_Mcast_QoS.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Association.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_SEQPACK_Connector.i
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

SOURCE=.\Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\Stream.i
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.i
# End Source File
# Begin Source File

SOURCE=.\String_Base.i
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

SOURCE=.\Test_and_Set.i
# End Source File
# Begin Source File

SOURCE=.\Thread.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Adapter.inl
# End Source File
# Begin Source File

SOURCE=.\Thread_Control.inl
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Time_Value.inl
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

SOURCE=.\TP_Reactor.i
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
# PROP Exclude_From_Build 1
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
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Cache_Map_Manager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Cached_Connect_Strategy_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Caching_Strategies_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Caching_Utility_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Strategies_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Connector.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Containers_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Dump_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Framework_Component_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Free_List.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Functor_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Future.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Hash_Cache_Map_Manager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_With_Allocator_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Map_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Message_Block_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Obstack_T.cpp
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
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Singleton.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\String_Base.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Synch_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Task_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test_and_Set.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.cpp
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ace.rc
# End Source File
# End Group
# End Target
# End Project
