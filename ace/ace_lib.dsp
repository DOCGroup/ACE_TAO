# Microsoft Developer Studio Project File - Name="ACE LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACE LIB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_lib.mak" CFG="ACE LIB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE LIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE LIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE LIB - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE LIB - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE LIB - Win32 Alpha Debug" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE LIB - Win32 Alpha Release" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE LIB - Win32 Alpha Unicode Debug" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE LIB - Win32 Alpha Unicode Release" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "ACE LIB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /Gy /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace.lib"
# ADD LIB32 /nologo /out:".\acesd.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /O1 /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace-r.lib"
# ADD LIB32 /nologo /out:".\aces.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Unicode_Debug"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /Gy /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace.lib"
# ADD LIB32 /nologo /out:".\acesud.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Unicode_Release"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /O1 /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace-r.lib"
# ADD LIB32 /nologo /out:".\acesu.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_stat"
# PROP BASE Intermediate_Dir "ACE_stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Lib\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /nologo /W3 /GX /Zi /Od /Gy /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /Gy /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesd.lib"
# ADD LIB32 /nologo /out:".\acesd.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE_stat"
# PROP BASE Intermediate_Dir "ACE_stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Lib\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /nologo /Gt0 /W3 /GX /O1 /D "NDEBUG" /D "_WINDOWS" /FD /I "../" /D "WIN32" /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O1 /I "../" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\aces.lib"
# ADD LIB32 /nologo /out:".\aces.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Alpha Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_sta0"
# PROP BASE Intermediate_Dir "ACE_sta0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX Zi Od Gy D "_DEBUG" D "_WINDOWS" D "UNICODE" FD I "..\\ /D "WIN32" /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /Gy /I "../" /D" /D "_WINDOWS" /D "UNICODE" /D "WIN32" /D ACE_HAS_DLL=0 /FD /MTd /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesud.lib"
# ADD LIB32 /nologo /out:".\acesud.lib"

!ELSEIF  "$(CFG)" == "ACE LIB - Win32 Alpha Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE_sta1"
# PROP BASE Intermediate_Dir "ACE_sta1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /nologo /Gt0 /W3 /GX /O1 /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /FD /I "../" /D "WIN32" /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O1 /I "../" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "WIN32" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesu.lib"
# ADD LIB32 /nologo /out:".\acesu.lib"

!ENDIF 

# Begin Target

# Name "ACE LIB - Win32 Debug"
# Name "ACE LIB - Win32 Release"
# Name "ACE LIB - Win32 Unicode Debug"
# Name "ACE LIB - Win32 Unicode Release"
# Name "ACE LIB - Win32 Alpha Debug"
# Name "ACE LIB - Win32 Alpha Release"
# Name "ACE LIB - Win32 Alpha Unicode Debug"
# Name "ACE LIB - Win32 Alpha Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
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

SOURCE=.\Asynch_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Containers.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.cpp
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

SOURCE=.\DLL.cpp
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

SOURCE=.\Event_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.cpp
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

SOURCE=.\Filecache.cpp
# End Source File
# Begin Source File

SOURCE=.\Functor.cpp
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.cpp
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

SOURCE=.\Local_Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp
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

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.cpp
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

SOURCE=.\Msg_WFMO_Reactor.cpp
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

SOURCE=.\NT_Service.cpp
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

SOURCE=.\Pair.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp
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

SOURCE=.\Profile_Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Reactor.cpp
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.cpp
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

SOURCE=.\Strategies.cpp
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

SOURCE=.\Thread_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp
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

SOURCE=.\XtReactor.cpp
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

SOURCE=.\Asynch_IO_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.h
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

SOURCE=.\Caching_Strategy_Utility_T.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=".\config-win32-borland.h"
# End Source File
# Begin Source File

SOURCE=".\config-win32-common.h"
# End Source File
# Begin Source File

SOURCE=".\config-win32.h"
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

SOURCE=.\Containers_T.h
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

SOURCE=.\Hash_Cache_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_With_Allocator_T.h
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

SOURCE=.\Proactor_Impl.h
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

SOURCE=.\Asynch_IO_Impl.i
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

SOURCE=.\CDR_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Strategies_T.i
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

SOURCE=.\Functor.i
# End Source File
# Begin Source File

SOURCE=.\Functor_T.i
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.i
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

SOURCE=.\Hash_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_With_Allocator_T.i
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

SOURCE=.\Message_Block_T.i
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
# Begin Source File

SOURCE=.\WIN32_Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.i
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

SOURCE=.\Synch_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Task_T.cpp
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
# End Target
# End Project
