# Microsoft Developer Studio Project File - Name="UDP Performance Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=UDP Performance Server - Win32 Static Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "server.mak" CFG="UDP Performance Server - Win32 Static Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UDP Performance Server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Performance Server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Performance Server - Win32 Alpha Debug" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UDP Performance Server - Win32 Alpha Release" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UDP Performance Server - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Performance Server - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\server\Release"
# PROP BASE Intermediate_Dir ".\server\Release"
# PROP BASE Target_Dir ".\server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\server"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_Strategies.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\tao\Strategies" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\server\Debug"
# PROP BASE Intermediate_Dir ".\server\Debug"
# PROP BASE Target_Dir ".\server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\server"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_Strategiesd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\tao\Strategies" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "server\Alpha Debug"
# PROP BASE Intermediate_Dir "server\Alpha Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "server"
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
# ADD LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "server\Alpha Release"
# PROP BASE Intermediate_Dir "server\Alpha Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "server"
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
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "server\Static Debug"
# PROP BASE Intermediate_Dir "server\Static Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "server"
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"
# ADD LINK32 TAO_ObjRefTemplatesd.lib TAO_Valuetypesd.lib TAO_IORInterceptorsd.lib TAOsd.lib acesd.lib TAO_PortableServersd.lib TAO_Strategiessd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\tao\Strategies" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "server\Static Release"
# PROP BASE Intermediate_Dir "server\Static Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "server\Static Release"
# PROP Intermediate_Dir "server\Static Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "server"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:IX86 /pdbtype:sept /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\PortableServer"
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 TAO_ObjRefTemplates.lib TAO_Valuetypes.lib TAO_IORInterceptors.lib TAOs.lib aces.lib TAO_PortableServers.lib TAO_Strategiess.lib advapi32.lib user32.lib /nologo /subsystem:console /incremental:no /machine:IX86 /libpath:"..\..\..\..\tao\Strategies" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "UDP Performance Server - Win32 Release"
# Name "UDP Performance Server - Win32 Debug"
# Name "UDP Performance Server - Win32 Alpha Debug"
# Name "UDP Performance Server - Win32 Alpha Release"
# Name "UDP Performance Server - Win32 Static Debug"
# Name "UDP Performance Server - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\server.cpp

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

NODEP_CPP_SERVE=\
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
	"..\..\..\..\ace\sys_conf.h"\
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
	"..\..\Align.h"\
	"..\..\any.h"\
	"..\..\any.i"\
	"..\..\cdr.h"\
	"..\..\cdr.i"\
	"..\..\Client_Strategy_Factory.h"\
	"..\..\Connect.h"\
	"..\..\Connect.i"\
	"..\..\corba.h"\
	"..\..\CurrentC.h"\
	"..\..\CurrentC.i"\
	"..\..\debug.h"\
	"..\..\default_client.h"\
	"..\..\default_client.i"\
	"..\..\default_server.h"\
	"..\..\default_server.i"\
	"..\..\Environment.h"\
	"..\..\Environment.i"\
	"..\..\Exception.h"\
	"..\..\Exception.i"\
	"..\..\giop.h"\
	"..\..\giop.i"\
	"..\..\IIOP_Interpreter.h"\
	"..\..\IIOP_Object.h"\
	"..\..\IIOP_Object.i"\
	"..\..\IIOP_ORB.h"\
	"..\..\IIOP_ORB.i"\
	"..\..\Invocation.h"\
	"..\..\Invocation.i"\
	"..\..\marshal.h"\
	"..\..\marshal.i"\
	"..\..\nvlist.h"\
	"..\..\NVList.i"\
	"..\..\object.h"\
	"..\..\object.i"\
	"..\..\Object_KeyC.h"\
	"..\..\Object_KeyC.i"\
	"..\..\Object_Table.h"\
	"..\..\Operation_Table.h"\
	"..\..\orb.h"\
	"..\..\ORB.i"\
	"..\..\orb_core.h"\
	"..\..\orb_core.i"\
	"..\..\ORB_Strategies_T.cpp"\
	"..\..\ORB_Strategies_T.h"\
	"..\..\ORB_Strategies_T.i"\
	"..\..\orbconf.h"\
	"..\..\params.h"\
	"..\..\params.i"\
	"..\..\poa.h"\
	"..\..\POA.i"\
	"..\..\POA_CORBA.h"\
	"..\..\poa_macros.h"\
	"..\..\poaC.h"\
	"..\..\poaC.i"\
	"..\..\poaS.h"\
	"..\..\poaS.i"\
	"..\..\PolicyC.h"\
	"..\..\PolicyC.i"\
	"..\..\Principal.h"\
	"..\..\Principal.i"\
	"..\..\request.h"\
	"..\..\Request.i"\
	"..\..\sequence.h"\
	"..\..\sequence.i"\
	"..\..\Sequence_T.cpp"\
	"..\..\Sequence_T.h"\
	"..\..\Sequence_T.i"\
	"..\..\servant_base.h"\
	"..\..\Server_Request.h"\
	"..\..\Server_Request.i"\
	"..\..\Server_Strategy_Factory.h"\
	"..\..\singletons.h"\
	"..\..\stub.h"\
	"..\..\stub.i"\
	"..\..\TAO.h"\
	"..\..\typecode.h"\
	"..\..\typecode.i"\
	"..\..\Union.h"\
	"..\..\varout.cpp"\
	"..\..\varout.h"\
	"..\..\varout.i"\
	".\Server_impl.h"\
	".\Time_impl.h"\
	".\TimeC.h"\
	".\TimeC.i"\
	".\TimeS.h"\
	".\TimeS.i"\
	".\TimeS_T.cpp"\
	".\TimeS_T.h"\
	".\TimeS_T.i"\
	

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDP_i.cpp

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDPC.cpp

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

# ADD CPP /GR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDPS.cpp

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\UDP_i.h
# End Source File
# Begin Source File

SOURCE=.\UDPC.h
# End Source File
# Begin Source File

SOURCE=.\UDPS.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\UDP.idl

!IF  "$(CFG)" == "UDP Performance Server - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\tao_idl_static -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "UDP Performance Server - Win32 Static Release"

USERDEP__UDP_I="..\..\..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl_static -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
