# Microsoft Developer Studio Project File - Name="Simple Time Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=Simple Time Client - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="Simple Time Client - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simple Time Client - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Simple Time Client - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Simple Time Client - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "Simple Time Client - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\client\Release"
# PROP BASE Intermediate_Dir ".\client\Release"
# PROP BASE Target_Dir ".\client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\client"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /I "..\..\..\..\\" /I "..\..\..\orbsvcs" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib orbsvcs.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\client\Debug"
# PROP BASE Intermediate_Dir ".\client\Debug"
# PROP BASE Target_Dir ".\client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\client"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /I "..\..\..\..\\" /I "..\..\..\orbsvcs" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib TAOd.lib orbsvcsd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "client\Alpha Debug"
# PROP BASE Intermediate_Dir "client\Alpha Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\\ /I" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD ..\..\..\..\\ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\\ /I" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /MDd ..\..\..\..\\ /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# ADD LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "client\Alpha Release"
# PROP BASE Intermediate_Dir "client\Alpha Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\\ /I" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD ..\..\..\..\\ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\\ /I" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD ..\..\..\..\\ /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"

!ENDIF 

# Begin Target

# Name "Simple Time Client - Win32 Release"
# Name "Simple Time Client - Win32 Debug"
# Name "Simple Time Client - Win32 Alpha Debug"
# Name "Simple Time Client - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\client.cpp

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

DEP_CPP_CLIEN=\
	"..\..\..\..\ace\Acceptor.cpp"\
	"..\..\..\..\ace\Acceptor.h"\
	"..\..\..\..\ace\Acceptor.i"\
	"..\..\..\..\ace\ACE.h"\
	"..\..\..\..\ace\ACE.i"\
	"..\..\..\..\ace\Addr.h"\
	"..\..\..\..\ace\Addr.i"\
	"..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\ace\config-win32-borland.h"\
	"..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\ace\config-WinCE.h"\
	"..\..\..\..\ace\config.h"\
	"..\..\..\..\ace\Connector.cpp"\
	"..\..\..\..\ace\Connector.h"\
	"..\..\..\..\ace\Connector.i"\
	"..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\ace\Containers.h"\
	"..\..\..\..\ace\Containers.i"\
	"..\..\..\..\ace\Dynamic.h"\
	"..\..\..\..\ace\Dynamic.i"\
	"..\..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\ace\inc_user_config.h"\
	"..\..\..\..\ace\INET_Addr.h"\
	"..\..\..\..\ace\INET_Addr.i"\
	"..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\ace\IPC_SAP.h"\
	"..\..\..\..\ace\IPC_SAP.i"\
	"..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\ace\Malloc_Base.h"\
	"..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\ace\Message_Queue_T.cpp"\
	"..\..\..\..\ace\Message_Queue_T.h"\
	"..\..\..\..\ace\Message_Queue_T.i"\
	"..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\ace\Module.h"\
	"..\..\..\..\ace\Module.i"\
	"..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\ace\OS.h"\
	"..\..\..\..\ace\OS.i"\
	"..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\ace\Signal.h"\
	"..\..\..\..\ace\Signal.i"\
	"..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\ace\SOCK.h"\
	"..\..\..\..\ace\SOCK.i"\
	"..\..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\..\ace\SOCK_IO.h"\
	"..\..\..\..\ace\SOCK_IO.i"\
	"..\..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\..\ace\SString.h"\
	"..\..\..\..\ace\SString.i"\
	"..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\ace\Strategies.i"\
	"..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\ace\Strategies_T.i"\
	"..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\ace\Stream_Modules.i"\
	"..\..\..\..\ace\streams.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\..\ace\Svc_Handler.h"\
	"..\..\..\..\ace\Svc_Handler.i"\
	"..\..\..\..\ace\Synch.h"\
	"..\..\..\..\ace\Synch.i"\
	"..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\ace\Synch_Options.i"\
	"..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\ace\Task.h"\
	"..\..\..\..\ace\Task.i"\
	"..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\ace\Thread.h"\
	"..\..\..\..\ace\Thread.i"\
	"..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\ace\Time_Value.h"\
	"..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\ace\Trace.h"\
	"..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\ace\ws2tcpip.h"\
	"..\..\..\tao\Any.h"\
	"..\..\..\tao\Any.i"\
	"..\..\..\tao\CDR.h"\
	"..\..\..\tao\CDR.i"\
	"..\..\..\tao\CDR_Interpreter.h"\
	"..\..\..\tao\Client_Strategy_Factory.h"\
	"..\..\..\tao\Connect.h"\
	"..\..\..\tao\Connect.i"\
	"..\..\..\tao\corba.h"\
	"..\..\..\tao\CurrentC.h"\
	"..\..\..\tao\CurrentC.i"\
	"..\..\..\tao\debug.h"\
	"..\..\..\tao\default_client.h"\
	"..\..\..\tao\default_client.i"\
	"..\..\..\tao\default_server.h"\
	"..\..\..\tao\default_server.i"\
	"..\..\..\tao\Environment.h"\
	"..\..\..\tao\Environment.i"\
	"..\..\..\tao\Exception.h"\
	"..\..\..\tao\Exception.i"\
	"..\..\..\tao\GIOP.h"\
	"..\..\..\tao\GIOP.i"\
	"..\..\..\tao\IIOP_ORB.h"\
	"..\..\..\tao\IIOP_ORB.i"\
	"..\..\..\tao\Invocation.h"\
	"..\..\..\tao\Invocation.i"\
	"..\..\..\tao\Marshal.h"\
	"..\..\..\tao\Marshal.i"\
	"..\..\..\tao\NVList.h"\
	"..\..\..\tao\NVList.i"\
	"..\..\..\tao\Object.h"\
	"..\..\..\tao\Object.i"\
	"..\..\..\tao\Object_KeyC.h"\
	"..\..\..\tao\Object_KeyC.i"\
	"..\..\..\tao\Operation_Table.h"\
	"..\..\..\tao\ORB.h"\
	"..\..\..\tao\ORB.i"\
	"..\..\..\tao\ORB_Core.h"\
	"..\..\..\tao\ORB_Core.i"\
	"..\..\..\tao\ORB_Strategies_T.cpp"\
	"..\..\..\tao\ORB_Strategies_T.h"\
	"..\..\..\tao\ORB_Strategies_T.i"\
	"..\..\..\tao\orbconf.h"\
	"..\..\..\tao\params.h"\
	"..\..\..\tao\params.i"\
	"..\..\..\tao\POA.h"\
	"..\..\..\tao\POA.i"\
	"..\..\..\tao\POA_CORBA.h"\
	"..\..\..\tao\poa_macros.h"\
	"..\..\..\tao\POAC.h"\
	"..\..\..\tao\POAC.i"\
	"..\..\..\tao\POAS.h"\
	"..\..\..\tao\POAS.i"\
	"..\..\..\tao\PolicyC.h"\
	"..\..\..\tao\PolicyC.i"\
	"..\..\..\tao\Principal.h"\
	"..\..\..\tao\Principal.i"\
	"..\..\..\tao\Request.h"\
	"..\..\..\tao\Request.i"\
	"..\..\..\tao\Sequence.h"\
	"..\..\..\tao\Sequence.i"\
	"..\..\..\tao\Sequence_T.cpp"\
	"..\..\..\tao\Sequence_T.h"\
	"..\..\..\tao\Sequence_T.i"\
	"..\..\..\tao\Servant_Base.h"\
	"..\..\..\tao\Server_Request.h"\
	"..\..\..\tao\Server_Request.i"\
	"..\..\..\tao\Server_Strategy_Factory.h"\
	"..\..\..\tao\singletons.h"\
	"..\..\..\tao\Stub.h"\
	"..\..\..\tao\Stub.i"\
	"..\..\..\tao\Typecode.h"\
	"..\..\..\tao\Typecode.i"\
	"..\..\..\tao\Union.h"\
	"..\..\..\tao\varout.cpp"\
	"..\..\..\tao\varout.h"\
	"..\..\..\tao\varout.i"\
	
NODEP_CPP_CLIEN=\
	"..\..\..\..\ace\sys_conf.h"\
	"..\..\..\tao\Align.h"\
	"..\..\..\tao\IIOP_Object.h"\
	"..\..\..\tao\IIOP_Object.i"\
	"..\..\..\tao\Object_Table.h"\
	".\Client_impl.h"\
	".\TimeC.h"\
	

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time_Client_i.cpp

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeC.cpp

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

DEP_CPP_TIMEC=\
	"..\..\..\..\ace\Acceptor.cpp"\
	"..\..\..\..\ace\Acceptor.h"\
	"..\..\..\..\ace\Acceptor.i"\
	"..\..\..\..\ace\ACE.h"\
	"..\..\..\..\ace\ACE.i"\
	"..\..\..\..\ace\Addr.h"\
	"..\..\..\..\ace\Addr.i"\
	"..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\ace\config-win32-borland.h"\
	"..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\ace\config-WinCE.h"\
	"..\..\..\..\ace\config.h"\
	"..\..\..\..\ace\Connector.cpp"\
	"..\..\..\..\ace\Connector.h"\
	"..\..\..\..\ace\Connector.i"\
	"..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\ace\Containers.h"\
	"..\..\..\..\ace\Containers.i"\
	"..\..\..\..\ace\Dynamic.h"\
	"..\..\..\..\ace\Dynamic.i"\
	"..\..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\ace\inc_user_config.h"\
	"..\..\..\..\ace\INET_Addr.h"\
	"..\..\..\..\ace\INET_Addr.i"\
	"..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\ace\IPC_SAP.h"\
	"..\..\..\..\ace\IPC_SAP.i"\
	"..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\ace\Malloc_Base.h"\
	"..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\ace\Message_Queue_T.cpp"\
	"..\..\..\..\ace\Message_Queue_T.h"\
	"..\..\..\..\ace\Message_Queue_T.i"\
	"..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\ace\Module.h"\
	"..\..\..\..\ace\Module.i"\
	"..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\ace\OS.h"\
	"..\..\..\..\ace\OS.i"\
	"..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\ace\Signal.h"\
	"..\..\..\..\ace\Signal.i"\
	"..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\ace\SOCK.h"\
	"..\..\..\..\ace\SOCK.i"\
	"..\..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\..\ace\SOCK_IO.h"\
	"..\..\..\..\ace\SOCK_IO.i"\
	"..\..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\..\ace\SString.h"\
	"..\..\..\..\ace\SString.i"\
	"..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\ace\Strategies.i"\
	"..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\ace\Strategies_T.i"\
	"..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\ace\Stream_Modules.i"\
	"..\..\..\..\ace\streams.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\..\ace\Svc_Handler.h"\
	"..\..\..\..\ace\Svc_Handler.i"\
	"..\..\..\..\ace\Synch.h"\
	"..\..\..\..\ace\Synch.i"\
	"..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\ace\Synch_Options.i"\
	"..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\ace\Task.h"\
	"..\..\..\..\ace\Task.i"\
	"..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\ace\Thread.h"\
	"..\..\..\..\ace\Thread.i"\
	"..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\ace\Time_Value.h"\
	"..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\ace\Trace.h"\
	"..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\ace\ws2tcpip.h"\
	"..\..\..\tao\Any.h"\
	"..\..\..\tao\Any.i"\
	"..\..\..\tao\CDR.h"\
	"..\..\..\tao\CDR.i"\
	"..\..\..\tao\CDR_Interpreter.h"\
	"..\..\..\tao\Client_Strategy_Factory.h"\
	"..\..\..\tao\Connect.h"\
	"..\..\..\tao\Connect.i"\
	"..\..\..\tao\corba.h"\
	"..\..\..\tao\CurrentC.h"\
	"..\..\..\tao\CurrentC.i"\
	"..\..\..\tao\debug.h"\
	"..\..\..\tao\default_client.h"\
	"..\..\..\tao\default_client.i"\
	"..\..\..\tao\default_server.h"\
	"..\..\..\tao\default_server.i"\
	"..\..\..\tao\Environment.h"\
	"..\..\..\tao\Environment.i"\
	"..\..\..\tao\Exception.h"\
	"..\..\..\tao\Exception.i"\
	"..\..\..\tao\GIOP.h"\
	"..\..\..\tao\GIOP.i"\
	"..\..\..\tao\IIOP_ORB.h"\
	"..\..\..\tao\IIOP_ORB.i"\
	"..\..\..\tao\Invocation.h"\
	"..\..\..\tao\Invocation.i"\
	"..\..\..\tao\Marshal.h"\
	"..\..\..\tao\Marshal.i"\
	"..\..\..\tao\NVList.h"\
	"..\..\..\tao\NVList.i"\
	"..\..\..\tao\Object.h"\
	"..\..\..\tao\Object.i"\
	"..\..\..\tao\Object_KeyC.h"\
	"..\..\..\tao\Object_KeyC.i"\
	"..\..\..\tao\Operation_Table.h"\
	"..\..\..\tao\ORB.h"\
	"..\..\..\tao\ORB.i"\
	"..\..\..\tao\ORB_Core.h"\
	"..\..\..\tao\ORB_Core.i"\
	"..\..\..\tao\ORB_Strategies_T.cpp"\
	"..\..\..\tao\ORB_Strategies_T.h"\
	"..\..\..\tao\ORB_Strategies_T.i"\
	"..\..\..\tao\orbconf.h"\
	"..\..\..\tao\params.h"\
	"..\..\..\tao\params.i"\
	"..\..\..\tao\POA.h"\
	"..\..\..\tao\POA.i"\
	"..\..\..\tao\POA_CORBA.h"\
	"..\..\..\tao\poa_macros.h"\
	"..\..\..\tao\POAC.h"\
	"..\..\..\tao\POAC.i"\
	"..\..\..\tao\POAS.h"\
	"..\..\..\tao\POAS.i"\
	"..\..\..\tao\PolicyC.h"\
	"..\..\..\tao\PolicyC.i"\
	"..\..\..\tao\Principal.h"\
	"..\..\..\tao\Principal.i"\
	"..\..\..\tao\Request.h"\
	"..\..\..\tao\Request.i"\
	"..\..\..\tao\Sequence.h"\
	"..\..\..\tao\Sequence.i"\
	"..\..\..\tao\Sequence_T.cpp"\
	"..\..\..\tao\Sequence_T.h"\
	"..\..\..\tao\Sequence_T.i"\
	"..\..\..\tao\Servant_Base.h"\
	"..\..\..\tao\Server_Request.h"\
	"..\..\..\tao\Server_Request.i"\
	"..\..\..\tao\Server_Strategy_Factory.h"\
	"..\..\..\tao\singletons.h"\
	"..\..\..\tao\Stub.h"\
	"..\..\..\tao\Stub.i"\
	"..\..\..\tao\Typecode.h"\
	"..\..\..\tao\Typecode.i"\
	"..\..\..\tao\Union.h"\
	"..\..\..\tao\varout.cpp"\
	"..\..\..\tao\varout.h"\
	"..\..\..\tao\varout.i"\
	
NODEP_CPP_TIMEC=\
	"..\..\..\..\ace\sys_conf.h"\
	"..\..\..\tao\Align.h"\
	"..\..\..\tao\IIOP_Object.h"\
	"..\..\..\tao\IIOP_Object.i"\
	"..\..\..\tao\Object_Table.h"\
	".\TimeC.h"\
	".\TimeC.i"\
	".\TimeS.h"\
	".\TimeS.i"\
	".\TimeS_T.cpp"\
	".\TimeS_T.h"\
	".\TimeS_T.i"\
	

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeS.cpp

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

DEP_CPP_TIMES=\
	"..\..\..\..\ace\Acceptor.cpp"\
	"..\..\..\..\ace\Acceptor.h"\
	"..\..\..\..\ace\Acceptor.i"\
	"..\..\..\..\ace\ACE.h"\
	"..\..\..\..\ace\ACE.i"\
	"..\..\..\..\ace\Addr.h"\
	"..\..\..\..\ace\Addr.i"\
	"..\..\..\..\ace\Atomic_Op.i"\
	"..\..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\..\ace\Basic_Types.h"\
	"..\..\..\..\ace\Basic_Types.i"\
	"..\..\..\..\ace\config-win32-borland.h"\
	"..\..\..\..\ace\config-win32-common.h"\
	"..\..\..\..\ace\config-win32.h"\
	"..\..\..\..\ace\config-WinCE.h"\
	"..\..\..\..\ace\config.h"\
	"..\..\..\..\ace\Connector.cpp"\
	"..\..\..\..\ace\Connector.h"\
	"..\..\..\..\ace\Connector.i"\
	"..\..\..\..\ace\Containers.cpp"\
	"..\..\..\..\ace\Containers.h"\
	"..\..\..\..\ace\Containers.i"\
	"..\..\..\..\ace\Dynamic.h"\
	"..\..\..\..\ace\Dynamic.i"\
	"..\..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\..\ace\Event_Handler.h"\
	"..\..\..\..\ace\Event_Handler.i"\
	"..\..\..\..\ace\Free_List.cpp"\
	"..\..\..\..\ace\Free_List.h"\
	"..\..\..\..\ace\Free_List.i"\
	"..\..\..\..\ace\Get_Opt.h"\
	"..\..\..\..\ace\Get_Opt.i"\
	"..\..\..\..\ace\Handle_Set.h"\
	"..\..\..\..\ace\Handle_Set.i"\
	"..\..\..\..\ace\Hash_Map_Manager.cpp"\
	"..\..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\..\ace\inc_user_config.h"\
	"..\..\..\..\ace\INET_Addr.h"\
	"..\..\..\..\ace\INET_Addr.i"\
	"..\..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\..\ace\iosfwd.h"\
	"..\..\..\..\ace\IPC_SAP.h"\
	"..\..\..\..\ace\IPC_SAP.i"\
	"..\..\..\..\ace\Log_Msg.h"\
	"..\..\..\..\ace\Log_Priority.h"\
	"..\..\..\..\ace\Log_Record.h"\
	"..\..\..\..\ace\Log_Record.i"\
	"..\..\..\..\ace\Malloc.h"\
	"..\..\..\..\ace\Malloc.i"\
	"..\..\..\..\ace\Malloc_Base.h"\
	"..\..\..\..\ace\Malloc_T.cpp"\
	"..\..\..\..\ace\Malloc_T.h"\
	"..\..\..\..\ace\Malloc_T.i"\
	"..\..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\..\ace\Managed_Object.h"\
	"..\..\..\..\ace\Managed_Object.i"\
	"..\..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\..\ace\Map_Manager.h"\
	"..\..\..\..\ace\Map_Manager.i"\
	"..\..\..\..\ace\Mem_Map.h"\
	"..\..\..\..\ace\Mem_Map.i"\
	"..\..\..\..\ace\Memory_Pool.h"\
	"..\..\..\..\ace\Memory_Pool.i"\
	"..\..\..\..\ace\Message_Block.h"\
	"..\..\..\..\ace\Message_Block.i"\
	"..\..\..\..\ace\Message_Queue.h"\
	"..\..\..\..\ace\Message_Queue.i"\
	"..\..\..\..\ace\Message_Queue_T.cpp"\
	"..\..\..\..\ace\Message_Queue_T.h"\
	"..\..\..\..\ace\Message_Queue_T.i"\
	"..\..\..\..\ace\Module.cpp"\
	"..\..\..\..\ace\Module.h"\
	"..\..\..\..\ace\Module.i"\
	"..\..\..\..\ace\Object_Manager.h"\
	"..\..\..\..\ace\Object_Manager.i"\
	"..\..\..\..\ace\OS.h"\
	"..\..\..\..\ace\OS.i"\
	"..\..\..\..\ace\Reactor.h"\
	"..\..\..\..\ace\Reactor.i"\
	"..\..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\..\ace\Service_Config.h"\
	"..\..\..\..\ace\Service_Config.i"\
	"..\..\..\..\ace\Service_Object.h"\
	"..\..\..\..\ace\Service_Object.i"\
	"..\..\..\..\ace\Service_Repository.h"\
	"..\..\..\..\ace\Service_Repository.i"\
	"..\..\..\..\ace\Service_Types.h"\
	"..\..\..\..\ace\Service_Types.i"\
	"..\..\..\..\ace\Shared_Object.h"\
	"..\..\..\..\ace\Shared_Object.i"\
	"..\..\..\..\ace\Signal.h"\
	"..\..\..\..\ace\Signal.i"\
	"..\..\..\..\ace\Singleton.cpp"\
	"..\..\..\..\ace\Singleton.h"\
	"..\..\..\..\ace\Singleton.i"\
	"..\..\..\..\ace\SOCK.h"\
	"..\..\..\..\ace\SOCK.i"\
	"..\..\..\..\ace\SOCK_Acceptor.h"\
	"..\..\..\..\ace\SOCK_Acceptor.i"\
	"..\..\..\..\ace\SOCK_Connector.h"\
	"..\..\..\..\ace\SOCK_Connector.i"\
	"..\..\..\..\ace\SOCK_IO.h"\
	"..\..\..\..\ace\SOCK_IO.i"\
	"..\..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\..\ace\SString.h"\
	"..\..\..\..\ace\SString.i"\
	"..\..\..\..\ace\Strategies.h"\
	"..\..\..\..\ace\Strategies.i"\
	"..\..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\..\ace\Strategies_T.h"\
	"..\..\..\..\ace\Strategies_T.i"\
	"..\..\..\..\ace\Stream_Modules.cpp"\
	"..\..\..\..\ace\Stream_Modules.h"\
	"..\..\..\..\ace\Stream_Modules.i"\
	"..\..\..\..\ace\streams.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\..\ace\Svc_Handler.cpp"\
	"..\..\..\..\ace\Svc_Handler.h"\
	"..\..\..\..\ace\Svc_Handler.i"\
	"..\..\..\..\ace\Synch.h"\
	"..\..\..\..\ace\Synch.i"\
	"..\..\..\..\ace\Synch_Options.h"\
	"..\..\..\..\ace\Synch_Options.i"\
	"..\..\..\..\ace\Synch_T.cpp"\
	"..\..\..\..\ace\Synch_T.h"\
	"..\..\..\..\ace\Synch_T.i"\
	"..\..\..\..\ace\Task.h"\
	"..\..\..\..\ace\Task.i"\
	"..\..\..\..\ace\Task_T.cpp"\
	"..\..\..\..\ace\Task_T.h"\
	"..\..\..\..\ace\Task_T.i"\
	"..\..\..\..\ace\Thread.h"\
	"..\..\..\..\ace\Thread.i"\
	"..\..\..\..\ace\Thread_Manager.h"\
	"..\..\..\..\ace\Thread_Manager.i"\
	"..\..\..\..\ace\Time_Value.h"\
	"..\..\..\..\ace\Timer_Queue.h"\
	"..\..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\..\ace\Trace.h"\
	"..\..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\..\ace\ws2tcpip.h"\
	"..\..\..\tao\Any.h"\
	"..\..\..\tao\Any.i"\
	"..\..\..\tao\CDR.h"\
	"..\..\..\tao\CDR.i"\
	"..\..\..\tao\CDR_Interpreter.h"\
	"..\..\..\tao\Client_Strategy_Factory.h"\
	"..\..\..\tao\Connect.h"\
	"..\..\..\tao\Connect.i"\
	"..\..\..\tao\corba.h"\
	"..\..\..\tao\CurrentC.h"\
	"..\..\..\tao\CurrentC.i"\
	"..\..\..\tao\debug.h"\
	"..\..\..\tao\default_client.h"\
	"..\..\..\tao\default_client.i"\
	"..\..\..\tao\default_server.h"\
	"..\..\..\tao\default_server.i"\
	"..\..\..\tao\Environment.h"\
	"..\..\..\tao\Environment.i"\
	"..\..\..\tao\Exception.h"\
	"..\..\..\tao\Exception.i"\
	"..\..\..\tao\GIOP.h"\
	"..\..\..\tao\GIOP.i"\
	"..\..\..\tao\IIOP_ORB.h"\
	"..\..\..\tao\IIOP_ORB.i"\
	"..\..\..\tao\Invocation.h"\
	"..\..\..\tao\Invocation.i"\
	"..\..\..\tao\Marshal.h"\
	"..\..\..\tao\Marshal.i"\
	"..\..\..\tao\NVList.h"\
	"..\..\..\tao\NVList.i"\
	"..\..\..\tao\Object.h"\
	"..\..\..\tao\Object.i"\
	"..\..\..\tao\Object_KeyC.h"\
	"..\..\..\tao\Object_KeyC.i"\
	"..\..\..\tao\Operation_Table.h"\
	"..\..\..\tao\ORB.h"\
	"..\..\..\tao\ORB.i"\
	"..\..\..\tao\ORB_Core.h"\
	"..\..\..\tao\ORB_Core.i"\
	"..\..\..\tao\ORB_Strategies_T.cpp"\
	"..\..\..\tao\ORB_Strategies_T.h"\
	"..\..\..\tao\ORB_Strategies_T.i"\
	"..\..\..\tao\orbconf.h"\
	"..\..\..\tao\params.h"\
	"..\..\..\tao\params.i"\
	"..\..\..\tao\POA.h"\
	"..\..\..\tao\POA.i"\
	"..\..\..\tao\POA_CORBA.h"\
	"..\..\..\tao\poa_macros.h"\
	"..\..\..\tao\POAC.h"\
	"..\..\..\tao\POAC.i"\
	"..\..\..\tao\POAS.h"\
	"..\..\..\tao\POAS.i"\
	"..\..\..\tao\PolicyC.h"\
	"..\..\..\tao\PolicyC.i"\
	"..\..\..\tao\Principal.h"\
	"..\..\..\tao\Principal.i"\
	"..\..\..\tao\Request.h"\
	"..\..\..\tao\Request.i"\
	"..\..\..\tao\Sequence.h"\
	"..\..\..\tao\Sequence.i"\
	"..\..\..\tao\Sequence_T.cpp"\
	"..\..\..\tao\Sequence_T.h"\
	"..\..\..\tao\Sequence_T.i"\
	"..\..\..\tao\Servant_Base.h"\
	"..\..\..\tao\Server_Request.h"\
	"..\..\..\tao\Server_Request.i"\
	"..\..\..\tao\Server_Strategy_Factory.h"\
	"..\..\..\tao\singletons.h"\
	"..\..\..\tao\Stub.h"\
	"..\..\..\tao\Stub.i"\
	"..\..\..\tao\Typecode.h"\
	"..\..\..\tao\Typecode.i"\
	"..\..\..\tao\Union.h"\
	"..\..\..\tao\varout.cpp"\
	"..\..\..\tao\varout.h"\
	"..\..\..\tao\varout.i"\
	
NODEP_CPP_TIMES=\
	"..\..\..\..\ace\sys_conf.h"\
	"..\..\..\tao\Align.h"\
	"..\..\..\tao\IIOP_Object.h"\
	"..\..\..\tao\IIOP_Object.i"\
	"..\..\..\tao\Object_Table.h"\
	".\TimeC.h"\
	".\TimeC.i"\
	".\TimeS.h"\
	".\TimeS.i"\
	".\TimeS_T.cpp"\
	".\TimeS_T.h"\
	".\TimeS_T.i"\
	

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Time_Client_i.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\Time.idl

!IF  "$(CFG)" == "Simple Time Client - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	..\..\..\..\bin\tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\tao_idl\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	..\..\..\tao_idl\tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Time Client - Win32 Alpha Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\tao_idl\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	..\..\..\tao_idl\Release\tao_idl $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
