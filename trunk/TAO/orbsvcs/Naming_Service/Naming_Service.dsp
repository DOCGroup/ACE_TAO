# Microsoft Developer Studio Project File - Name="Naming_Service" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=Naming_Service - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Naming_Service.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Naming_Service.mak" CFG="Naming_Service - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Naming_Service - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Naming_Service - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Naming_Service - Win32 Alpha Debug" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "Naming_Service - Win32 Alpha Release" (based on "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Naming_Service - Win32 Release"

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
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 orbsvcs.lib TAO.lib ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib orbsvcsd.lib aced.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Naming_S"
# PROP BASE Intermediate_Dir "Naming_S"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I ".." /I "..\.." /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /MDd /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib orbsvcsd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOd.lib orbsvcsd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Naming_0"
# PROP BASE Intermediate_Dir "Naming_0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I ".." /I "..\.." /I "..\..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 orbsvcs.lib TAO.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 orbsvcs.lib TAO.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\orbsvcs" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "Naming_Service - Win32 Release"
# Name "Naming_Service - Win32 Debug"
# Name "Naming_Service - Win32 Alpha Debug"
# Name "Naming_Service - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Naming_Service.cpp

!IF  "$(CFG)" == "Naming_Service - Win32 Release"

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Debug"

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Alpha Debug"

DEP_CPP_NAMIN=\
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
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
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
	"..\..\..\ace\Malloc_Base.h"\
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
	"..\..\..\ace\Message_Queue.h"\
	"..\..\..\ace\Message_Queue.i"\
	"..\..\..\ace\Message_Queue_T.cpp"\
	"..\..\..\ace\Message_Queue_T.h"\
	"..\..\..\ace\Message_Queue_T.i"\
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
	"..\..\..\ace\SOCK_Dgram.h"\
	"..\..\..\ace\SOCK_Dgram.i"\
	"..\..\..\ace\SOCK_Dgram_Mcast.h"\
	"..\..\..\ace\SOCK_Dgram_Mcast.i"\
	"..\..\..\ace\SOCK_IO.h"\
	"..\..\..\ace\SOCK_IO.i"\
	"..\..\..\ace\SOCK_Stream.h"\
	"..\..\..\ace\SOCK_Stream.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\Strategies.h"\
	"..\..\..\ace\Strategies.i"\
	"..\..\..\ace\Strategies_T.cpp"\
	"..\..\..\ace\Strategies_T.h"\
	"..\..\..\ace\Strategies_T.i"\
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
	"..\..\..\ace\Synch_Options.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
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
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\align.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\default_client.h"\
	"..\..\tao\default_client.i"\
	"..\..\tao\default_server.h"\
	"..\..\tao\default_server.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Interpreter.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\ORB_Strategies_T.cpp"\
	"..\..\tao\ORB_Strategies_T.h"\
	"..\..\tao\ORB_Strategies_T.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\orbsvcs\CosNamingC.h"\
	"..\orbsvcs\CosNamingC.i"\
	"..\orbsvcs\CosNamingS.h"\
	"..\orbsvcs\CosNamingS.i"\
	"..\orbsvcs\CosNamingS_T.cpp"\
	"..\orbsvcs\CosNamingS_T.h"\
	"..\orbsvcs\CosNamingS_T.i"\
	"..\orbsvcs\IOR_Multicast.h"\
	"..\orbsvcs\Naming\CosNaming_i.h"\
	"..\orbsvcs\Naming\Entries.h"\
	"..\orbsvcs\Naming\Naming_Utils.h"\
	"..\orbsvcs\orbsvcs_export.h"\
	".\Naming_Service.h"\
	
NODEP_CPP_NAMIN=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\Object_Table.h"\
	

!ELSEIF  "$(CFG)" == "Naming_Service - Win32 Alpha Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosNaming_i.h
# End Source File
# Begin Source File

SOURCE=.\Naming_Service.h
# End Source File
# Begin Source File

SOURCE=.\NS_CosNaming.h
# End Source File
# End Group
# End Target
# End Project
