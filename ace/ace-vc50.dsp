# Microsoft Developer Studio Project File - Name="ace" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ace - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace-vc50.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace-vc50.mak" CFG="ace - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ace - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ace - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ace - Win32 Unicode Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ace - Win32 Unicode Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ace - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\ace-r.dll"

!ELSEIF  "$(CFG)" == "ace - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"ace.dll"

!ELSEIF  "$(CFG)" == "ace - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\ace___Wi"
# PROP BASE Intermediate_Dir ".\ace___Wi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\STL" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "UNICODE" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\aceu.dll"

!ELSEIF  "$(CFG)" == "ace - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ace___W0"
# PROP BASE Intermediate_Dir ".\ace___W0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\STL" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "UNICODE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ace-r.dll"
# ADD LINK32 wsock32.lib /nologo /subsystem:windows /dll /machine:I386 /out:".\aceu-r.dll"

!ENDIF 

# Begin Target

# Name "ace - Win32 Release"
# Name "ace - Win32 Debug"
# Name "ace - Win32 Unicode Debug"
# Name "ace - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ACE.cpp
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Addr.cpp
# End Source File
# Begin Source File

SOURCE=.\ARGV.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Containers.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.cpp
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

SOURCE=.\Dump.cpp
# End Source File
# Begin Source File

SOURCE=.\Dump_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Dynamic.cpp
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

SOURCE=.\Free_List.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\IOStream_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.cpp
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Malloc_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Method_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Obstack.cpp
# End Source File
# Begin Source File

SOURCE=.\OS.cpp
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp
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

SOURCE=.\ReactorEx.cpp
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

SOURCE=.\Service_Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Record.cpp
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.cpp
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

SOURCE=.\Singleton.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Strategies.cpp
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Synch.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Synch_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\System_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Task_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Timer_Hash_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\TTY_IO.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ACE.h
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Addr.h
# End Source File
# Begin Source File

SOURCE=.\ARGV.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=".\config-win32-common.h"
# End Source File
# Begin Source File

SOURCE=.\Containers.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.h
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

SOURCE=.\Dump.h
# End Source File
# Begin Source File

SOURCE=.\Dump_T.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic.h
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.h
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

SOURCE=.\Free_List.h
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.h
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

SOURCE=.\Malloc_T.h
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.h
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

SOURCE=.\Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Method_Object.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
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

SOURCE=.\Obstack.h
# End Source File
# Begin Source File

SOURCE=.\OS.h
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.h
# End Source File
# Begin Source File

SOURCE=.\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\Proactor.h
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.h
# End Source File
# Begin Source File

SOURCE=.\Reactor.h
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

SOURCE=.\Service_Config.h
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Service_Object.h
# End Source File
# Begin Source File

SOURCE=.\Service_Record.h
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.h
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

SOURCE=.\Trace.h
# End Source File
# Begin Source File

SOURCE=.\TTY_IO.h
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
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ACE.i
# End Source File
# Begin Source File

SOURCE=.\Addr.i
# End Source File
# Begin Source File

SOURCE=.\ARGV.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Containers.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.i
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

SOURCE=.\Get_Opt.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.i
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

SOURCE=.\Map_Manager.i
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

SOURCE=.\Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\Module.i
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.i
# End Source File
# Begin Source File

SOURCE=.\OS.i
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.i
# End Source File
# Begin Source File

SOURCE=.\Pipe.i
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

SOURCE=.\Service_Config.i
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Service_Object.i
# End Source File
# Begin Source File

SOURCE=.\Service_Record.i
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.i
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

SOURCE=.\Strategies_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\Synch.i
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

SOURCE=.\Trace.i
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
# End Group
# End Target
# End Project
