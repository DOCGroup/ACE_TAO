# Microsoft Developer Studio Project File - Name="orbsvcs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=orbsvcs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak" CFG="orbsvcs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "orbsvcs - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "orbsvcs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "orbsvcs - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "orbsvcs - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\bin\orbsvcs.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "orbsvcs_"
# PROP BASE Intermediate_Dir "orbsvcs_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
# SUBTRACT CPP /WX /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\bin\orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "orbsvcs_"
# PROP BASE Intermediate_Dir "orbsvcs_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
CPP=cl.exe
# ADD BASE CPP /Gt0 Gt0 nologo MT Gt0 W3 GX O2 I "..\\ /I" I "..\..\..\\ /D" D "WIN32" D "..\..\\ NDEBUG"
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\\" /I "..\..\..\\" /I "..\..\\" /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "orbsvcs0"
# PROP BASE Intermediate_Dir "orbsvcs0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
CPP=cl.exe
# ADD BASE CPP /Gt0 Gt0 nologo Gt0 W3 GX Zi Od I "..\\ /I" I "..\..\..\\ /D" D "_WINDOWS" D TAO_ORBSVCS_HAS_DLL=1 D "TAO_ORBSVCS_BUILD_DLL" D "WIN32" FD MTd ..\..\\ _DEBUG" c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /D "WIN32" /D "_DEBUG" /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "orbsvcs - Win32 Release"
# Name "orbsvcs - Win32 Debug"
# Name "orbsvcs - Win32 Alpha Release"
# Name "orbsvcs - Win32 Alpha Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\AV\AVStreams_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_AVSTR=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Process.h"\
	"..\..\..\ace\Process.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\AV\AVStreams_i.h"\
	".\AV\Endpoint_Strategy.h"\
	".\AV\Endpoint_Strategy_T.cpp"\
	".\AV\Endpoint_Strategy_T.h"\
	".\orbsvcs_export.h"\
	".\Trader\Trader.h"\
	
NODEP_CPP_AVSTR=\
	"..\..\..\..\..\Property\CosPropertyService_i.h"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.cpp"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.h"\
	".\AV\AVStreams_i_T.cpp"\
	".\AV\AVStreams_i_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\BCU.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_BCU_C=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc_Base.h"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\BCU.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_BCU_C=\
	"..\..\..\ace\sys_conf.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSet.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CC_LO=\
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
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Concurrency\CC_LockSet.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_CC_LO=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosConcurrencyControlC.h"\
	".\CosConcurrencyControlC.i"\
	".\CosConcurrencyControlS.h"\
	".\CosConcurrencyControlS.i"\
	".\CosConcurrencyControlS_T.cpp"\
	".\CosConcurrencyControlS_T.h"\
	".\CosConcurrencyControlS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSetFactory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CC_LOC=\
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
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Concurrency\CC_LockSet.h"\
	".\Concurrency\CC_LockSetFactory.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_CC_LOC=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosConcurrencyControlC.h"\
	".\CosConcurrencyControlC.i"\
	".\CosConcurrencyControlS.h"\
	".\CosConcurrencyControlS.i"\
	".\CosConcurrencyControlS_T.cpp"\
	".\CosConcurrencyControlS_T.h"\
	".\CosConcurrencyControlS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Concurrency\Concurrency_Utils.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONCU=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Concurrency\CC_LockSetFactory.h"\
	".\Concurrency\Concurrency_Utils.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_CONCU=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosConcurrencyControlC.h"\
	".\CosConcurrencyControlC.i"\
	".\CosConcurrencyControlS.h"\
	".\CosConcurrencyControlS.i"\
	".\CosConcurrencyControlS_T.cpp"\
	".\CosConcurrencyControlS_T.h"\
	".\CosConcurrencyControlS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONFI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\Config_Scheduler.h"\
	".\Sched\Config_Scheduler.i"\
	".\Sched\DynSched.h"\
	".\Sched\DynSched.i"\
	".\Sched\SchedEntry.h"\
	".\Sched\SchedEntry.i"\
	".\Sched\Scheduler.h"\
	".\Sched\Scheduler.i"\
	".\Sched\Scheduler_Generic.h"\
	".\Sched\Scheduler_Generic.i"\
	".\Sched\Strategy_Scheduler.h"\
	".\Sched\Strategy_Scheduler.i"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_CONFI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONST=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Interpreter.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_CONST=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_l.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONSTR=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Interpreter.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_CONSTR=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONSTRA=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_CONSTRA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONSTRAI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Interpreter.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Offer_Database.cpp"\
	".\Trader\Offer_Database.h"\
	".\Trader\Offer_Iterators.h"\
	".\Trader\Offer_Iterators_T.cpp"\
	".\Trader\Offer_Iterators_T.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Interfaces.cpp"\
	".\Trader\Trader_Interfaces.h"\
	".\Trader\Trader_T.cpp"\
	".\Trader\Trader_T.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_CONSTRAI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_y.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONSTRAIN=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Interpreter.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_CONSTRAIN=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ConsumerAdmin_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_CONSU=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\CosEvent\ConsumerAdmin_i.h"\
	".\CosEvent\ProxyPushSupplier_i.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSCO=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSCO=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosConcurrencyControlC.h"\
	".\CosConcurrencyControlC.i"\
	".\CosConcurrencyControlS.h"\
	".\CosConcurrencyControlS.i"\
	".\CosConcurrencyControlS_T.cpp"\
	".\CosConcurrencyControlS_T.h"\
	".\CosConcurrencyControlS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSCON=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSCON=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosConcurrencyControlC.h"\
	".\CosConcurrencyControlC.i"\
	".\CosConcurrencyControlS.h"\
	".\CosConcurrencyControlS.i"\
	".\CosConcurrencyControlS_T.cpp"\
	".\CosConcurrencyControlS_T.h"\
	".\CosConcurrencyControlS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSLI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSLI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosLifeCycleC.h"\
	".\CosLifeCycleC.i"\
	".\CosLifeCycleS.h"\
	".\CosLifeCycleS.i"\
	".\CosLifeCycleS_T.cpp"\
	".\CosLifeCycleS_T.h"\
	".\CosLifeCycleS_T.i"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSLIF=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSLIF=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosLifeCycleC.h"\
	".\CosLifeCycleC.i"\
	".\CosLifeCycleS.h"\
	".\CosLifeCycleS.i"\
	".\CosLifeCycleS_T.cpp"\
	".\CosLifeCycleS_T.h"\
	".\CosLifeCycleS_T.i"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNamingC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSNA=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSNA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNamingS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSNAM=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSNAM=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSPR=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSPR=\
	"..\..\..\..\..\Property\CosPropertyService_i.h"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.cpp"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSTR=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSTR=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_COSTRA=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_COSTRA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_DISPA=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Dispatching_Modules.h"\
	".\Event\Dispatching_Modules.i"\
	".\Event\Event_Channel.h"\
	".\Event\Event_Channel.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\Memory_Pools.h"\
	".\Event\Memory_Pools.i"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_DISPA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\Event\CORBA_Utils_T.cpp"\
	".\Event\CORBA_Utils_T.h"\
	".\Event\CORBA_Utils_T.i"\
	".\Event\Task_Manager.h"\
	".\Event\Task_Manager.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_DYNSC=\
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
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\DynSched.h"\
	".\Sched\DynSched.i"\
	".\Sched\SchedEntry.h"\
	".\Sched\SchedEntry.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_DYNSC=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_BA=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\RB_Tree.cpp"\
	"..\..\..\ace\RB_Tree.h"\
	"..\..\..\ace\RB_Tree.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Basic_Factory.h"\
	".\Event\EC_Basic_Factory.i"\
	".\Event\EC_Basic_Filter_Builder.h"\
	".\Event\EC_Basic_Filter_Builder.i"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_Null_Scheduling.h"\
	".\Event\EC_Null_Scheduling.i"\
	".\Event\EC_ObserverStrategy.h"\
	".\Event\EC_ObserverStrategy.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxyPushSupplier_Set_T.cpp"\
	".\Event\EC_ProxyPushSupplier_Set_T.h"\
	".\Event\EC_ProxyPushSupplier_Set_T.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Reactive_Timeout_Generator.h"\
	".\Event\EC_Reactive_Timeout_Generator.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\Event\EC_Trivial_Supplier_Filter.h"\
	".\Event\EC_Trivial_Supplier_Filter.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_BAS=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\EC_Basic_Filter_Builder.h"\
	".\Event\EC_Basic_Filter_Builder.i"\
	".\Event\EC_Conjunction_Filter.h"\
	".\Event\EC_Conjunction_Filter.i"\
	".\Event\EC_Disjunction_Filter.h"\
	".\Event\EC_Disjunction_Filter.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Type_Filter.h"\
	".\Event\EC_Type_Filter.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_BU=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_CO=\
	".\Event\EC_Conjunction_Filter.h"\
	".\Event\EC_Conjunction_Filter.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_CON=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_DE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Arg_Shifter.h"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\RB_Tree.cpp"\
	"..\..\..\ace\RB_Tree.h"\
	"..\..\..\ace\RB_Tree.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Basic_Filter_Builder.h"\
	".\Event\EC_Basic_Filter_Builder.i"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Default_Factory.h"\
	".\Event\EC_Default_Factory.i"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_Null_Scheduling.h"\
	".\Event\EC_Null_Scheduling.i"\
	".\Event\EC_ObserverStrategy.h"\
	".\Event\EC_ObserverStrategy.i"\
	".\Event\EC_Per_Supplier_Filter.h"\
	".\Event\EC_Per_Supplier_Filter.i"\
	".\Event\EC_Priority_Dispatching.h"\
	".\Event\EC_Priority_Dispatching.i"\
	".\Event\EC_Priority_Scheduling.h"\
	".\Event\EC_Priority_Scheduling.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxyPushSupplier_Set_T.cpp"\
	".\Event\EC_ProxyPushSupplier_Set_T.h"\
	".\Event\EC_ProxyPushSupplier_Set_T.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Reactive_Timeout_Generator.h"\
	".\Event\EC_Reactive_Timeout_Generator.i"\
	".\Event\EC_Sched_Filter_Builder.h"\
	".\Event\EC_Sched_Filter_Builder.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\Event\EC_Trivial_Supplier_Filter.h"\
	".\Event\EC_Trivial_Supplier_Filter.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_DI=\
	".\Event\EC_Disjunction_Filter.h"\
	".\Event\EC_Disjunction_Filter.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_DIS=\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_DISP=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\EC_Dispatching_Task.h"\
	".\Event\EC_Dispatching_Task.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_EV=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\RB_Tree.cpp"\
	"..\..\..\ace\RB_Tree.h"\
	"..\..\..\ace\RB_Tree.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Default_Factory.h"\
	".\Event\EC_Default_Factory.i"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ObserverStrategy.h"\
	".\Event\EC_ObserverStrategy.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_FA=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_FI=\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_FIL=\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_GA=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Channel_Clients_T.cpp"\
	".\Channel_Clients_T.h"\
	".\Channel_Clients_T.i"\
	".\Event\EC_Gateway.h"\
	".\Event_Service_Constants.h"\
	".\Event_Utilities.h"\
	".\Event_Utilities.i"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_EC_GA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_GAT=\
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
	"..\..\..\ace\SOCK_CODgram.h"\
	"..\..\..\ace\SOCK_CODgram.i"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\EC_Gateway_UDP.h"\
	".\Event_Service_Constants.h"\
	".\Event_Utilities.h"\
	".\Event_Utilities.i"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_EC_GAT=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_NU=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\RB_Tree.cpp"\
	"..\..\..\ace\RB_Tree.h"\
	"..\..\..\ace\RB_Tree.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_Null_Factory.h"\
	".\Event\EC_Null_Factory.i"\
	".\Event\EC_Null_Scheduling.h"\
	".\Event\EC_Null_Scheduling.i"\
	".\Event\EC_ObserverStrategy.h"\
	".\Event\EC_ObserverStrategy.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxyPushSupplier_Set_T.cpp"\
	".\Event\EC_ProxyPushSupplier_Set_T.h"\
	".\Event\EC_ProxyPushSupplier_Set_T.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Reactive_Timeout_Generator.h"\
	".\Event\EC_Reactive_Timeout_Generator.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\Event\EC_Trivial_Supplier_Filter.h"\
	".\Event\EC_Trivial_Supplier_Filter.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_NUL=\
	".\Event\EC_Null_Scheduling.h"\
	".\Event\EC_Null_Scheduling.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_OB=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\RB_Tree.cpp"\
	"..\..\..\ace\RB_Tree.h"\
	"..\..\..\ace\RB_Tree.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ObserverStrategy.h"\
	".\Event\EC_ObserverStrategy.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Per_Supplier_Filter.h"\
	".\Event\EC_Per_Supplier_Filter.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PR=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Dispatching_Task.h"\
	".\Event\EC_Dispatching_Task.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Priority_Dispatching.h"\
	".\Event\EC_Priority_Dispatching.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PRI=\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Priority_Scheduling.h"\
	".\Event\EC_Priority_Scheduling.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PRO=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PROX=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_PROXY=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Dispatching.h"\
	".\Event\EC_Dispatching.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_QO=\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_RE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Reactive_Timeout_Generator.h"\
	".\Event\EC_Reactive_Timeout_Generator.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SC=\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Sched_Filter.h"\
	".\Event\EC_Sched_Filter.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SCH=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Conjunction_Filter.h"\
	".\Event\EC_Conjunction_Filter.i"\
	".\Event\EC_Disjunction_Filter.h"\
	".\Event\EC_Disjunction_Filter.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Filter_Builder.h"\
	".\Event\EC_Filter_Builder.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Sched_Filter.h"\
	".\Event\EC_Sched_Filter.i"\
	".\Event\EC_Sched_Filter_Builder.h"\
	".\Event\EC_Sched_Filter_Builder.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Type_Filter.h"\
	".\Event\EC_Type_Filter.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SCHE=\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Scheduling_Strategy.h"\
	".\Event\EC_Scheduling_Strategy.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SU=\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SUP=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxyConsumer.h"\
	".\Event\EC_ProxyConsumer.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_SupplierAdmin.h"\
	".\Event\EC_SupplierAdmin.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_SUPP=\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_TI=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_TIM=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Timeout_Filter.h"\
	".\Event\EC_Timeout_Filter.i"\
	".\Event\EC_Timeout_Generator.h"\
	".\Event\EC_Timeout_Generator.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_TR=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\EC_Busy_Lock.cpp"\
	".\Event\EC_Busy_Lock.h"\
	".\Event\EC_Busy_Lock.i"\
	".\Event\EC_Command.cpp"\
	".\Event\EC_Command.h"\
	".\Event\EC_Command.i"\
	".\Event\EC_ConsumerAdmin.h"\
	".\Event\EC_ConsumerAdmin.i"\
	".\Event\EC_Event_Channel.h"\
	".\Event\EC_Event_Channel.i"\
	".\Event\EC_Factory.h"\
	".\Event\EC_Factory.i"\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_ProxyPushSupplier_Set.h"\
	".\Event\EC_ProxyPushSupplier_Set.i"\
	".\Event\EC_ProxySupplier.h"\
	".\Event\EC_ProxySupplier.i"\
	".\Event\EC_QOS_Info.h"\
	".\Event\EC_QOS_Info.i"\
	".\Event\EC_Supplier_Filter_Builder.h"\
	".\Event\EC_Supplier_Filter_Builder.i"\
	".\Event\EC_SupplierFiltering.h"\
	".\Event\EC_SupplierFiltering.i"\
	".\Event\EC_Trivial_Supplier_Filter.h"\
	".\Event\EC_Trivial_Supplier_Filter.i"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_TY=\
	".\Event\EC_Filter.h"\
	".\Event\EC_Filter.i"\
	".\Event\EC_Type_Filter.h"\
	".\Event\EC_Type_Filter.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EC_UD=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Event\EC_UDP_Admin.h"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_ENDPO=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Process.h"\
	"..\..\..\ace\Process.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Service_Config.h"\
	"..\..\..\ace\Service_Config.i"\
	"..\..\..\ace\Service_Object.h"\
	"..\..\..\ace\Service_Object.i"\
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\AV\AVStreams_i.h"\
	".\AV\Endpoint_Strategy.h"\
	".\AV\Endpoint_Strategy_T.cpp"\
	".\AV\Endpoint_Strategy_T.h"\
	".\orbsvcs_export.h"\
	".\Trader\Trader.h"\
	
NODEP_CPP_ENDPO=\
	"..\..\..\..\..\Property\CosPropertyService_i.h"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.cpp"\
	"..\..\..\..\..\Property\CosPropertyService_i_T.h"\
	".\AV\AVStreams_i_T.cpp"\
	".\AV\AVStreams_i_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_ENTRI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Naming\Entries.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_ENTRI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EVENT=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Channel_Clients_T.cpp"\
	".\Channel_Clients_T.h"\
	".\Channel_Clients_T.i"\
	".\Event\Dispatching_Modules.h"\
	".\Event\Dispatching_Modules.i"\
	".\Event\EC_Gateway.h"\
	".\Event\Event_Channel.h"\
	".\Event\Event_Channel.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\Memory_Pools.h"\
	".\Event\Memory_Pools.i"\
	".\Event\Module_Factory.h"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\Event_Utilities.h"\
	".\Event_Utilities.i"\
	".\orbsvcs_export.h"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_EVENT=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\Event\CORBA_Utils_T.cpp"\
	".\Event\CORBA_Utils_T.h"\
	".\Event\CORBA_Utils_T.i"\
	".\Event\Task_Manager.h"\
	".\Event\Task_Manager.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Event_Manip.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EVENT_=\
	".\Event\Event_Manip.h"\
	".\Event\Event_Manip.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EVENT_U=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\Event_Utilities.h"\
	".\Event_Utilities.i"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_EVENT_U=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\EventChannel_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_EVENTC=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\CosEvent\ConsumerAdmin_i.h"\
	".\CosEvent\EventChannel_i.h"\
	".\CosEvent\ProxyPushConsumer_i.h"\
	".\CosEvent\ProxyPushSupplier_i.h"\
	".\CosEvent\SupplierAdmin_i.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Hash_Naming_Context.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_HASH_=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Naming\Entries.h"\
	".\Naming\Hash_Naming_Context.h"\
	
NODEP_CPP_HASH_=\
	"..\..\..\..\..\Naming\Naming_Context.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_IOR_M=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\IOR_Multicast.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_IOR_M=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LIFEC=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_LIFEC=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosLifeCycleC.h"\
	".\CosLifeCycleC.i"\
	".\CosLifeCycleS.h"\
	".\CosLifeCycleS.i"\
	".\CosLifeCycleS_T.cpp"\
	".\CosLifeCycleS_T.h"\
	".\CosLifeCycleS_T.i"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	".\LifeCycleServiceC.h"\
	".\LifeCycleServiceC.i"\
	".\LifeCycleServiceS.h"\
	".\LifeCycleServiceS.i"\
	".\LifeCycleServiceS_T.cpp"\
	".\LifeCycleServiceS_T.h"\
	".\LifeCycleServiceS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LIFECY=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_LIFECY=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosLifeCycleC.h"\
	".\CosLifeCycleC.i"\
	".\CosLifeCycleS.h"\
	".\CosLifeCycleS.i"\
	".\CosLifeCycleS_T.cpp"\
	".\CosLifeCycleS_T.h"\
	".\CosLifeCycleS_T.i"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	".\LifeCycleServiceC.h"\
	".\LifeCycleServiceC.i"\
	".\LifeCycleServiceS.h"\
	".\LifeCycleServiceS.i"\
	".\LifeCycleServiceS_T.cpp"\
	".\LifeCycleServiceS_T.h"\
	".\LifeCycleServiceS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Local_ESTypes.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LOCAL=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_LOCAL=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log\Logger_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LOGGE=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Log\Logger_i.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_LOGGE=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\LoggerC.h"\
	".\LoggerC.i"\
	".\LoggerS.h"\
	".\LoggerS.i"\
	".\LoggerS_T.cpp"\
	".\LoggerS_T.h"\
	".\LoggerS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LoggerC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LOGGER=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_LOGGER=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\LoggerC.h"\
	".\LoggerC.i"\
	".\LoggerS.h"\
	".\LoggerS.i"\
	".\LoggerS_T.cpp"\
	".\LoggerS_T.h"\
	".\LoggerS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LoggerS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_LOGGERS=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_LOGGERS=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\LoggerC.h"\
	".\LoggerC.i"\
	".\LoggerS.h"\
	".\LoggerS.i"\
	".\LoggerS_T.cpp"\
	".\LoggerS_T.h"\
	".\LoggerS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_MEMOR=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Dispatching_Modules.h"\
	".\Event\Dispatching_Modules.i"\
	".\Event\Event_Channel.h"\
	".\Event\Event_Channel.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\Memory_Pools.h"\
	".\Event\Memory_Pools.i"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_MEMOR=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\Event\CORBA_Utils_T.cpp"\
	".\Event\CORBA_Utils_T.h"\
	".\Event\CORBA_Utils_T.i"\
	".\Event\Task_Manager.h"\
	".\Event\Task_Manager.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_MODUL=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Dispatching_Modules.h"\
	".\Event\Dispatching_Modules.i"\
	".\Event\Event_Channel.h"\
	".\Event\Event_Channel.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\Module_Factory.h"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_MODUL=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\Event\CORBA_Utils_T.cpp"\
	".\Event\CORBA_Utils_T.h"\
	".\Event\CORBA_Utils_T.i"\
	".\Event\Task_Manager.h"\
	".\Event\Task_Manager.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_NAMIN=\
	".\Naming\Naming_Context.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_NAMING=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Arg_Shifter.h"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\IOR_Multicast.h"\
	".\Naming\Entries.h"\
	".\Naming\Naming_Utils.h"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_NAMING=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\CosNamingS.h"\
	".\CosNamingS.i"\
	".\CosNamingS_T.cpp"\
	".\CosNamingS_T.h"\
	".\CosNamingS_T.i"\
	".\Naming\CosNaming_i.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_OFFER=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Offer_Iterators.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_OFFER=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Bindings_Map.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PERSI=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Naming\Persistent_Bindings_Map.h"\
	".\Naming\Persistent_Entries.h"\
	".\Naming\Shared_Hash_Map_T.cpp"\
	".\Naming\Shared_Hash_Map_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PERSIS=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Naming\Persistent_Context_Index.h"\
	".\Naming\Persistent_Entries.h"\
	".\Naming\Shared_Hash_Map_T.cpp"\
	".\Naming\Shared_Hash_Map_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PERSIST=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Naming\Persistent_Entries.h"\
	".\Naming\Shared_Hash_Map_T.cpp"\
	".\Naming\Shared_Hash_Map_T.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PERSISTE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Naming\Persistent_Bindings_Map.h"\
	".\Naming\Persistent_Context_Index.h"\
	".\Naming\Persistent_Entries.h"\
	".\Naming\Persistent_Naming_Context.h"\
	".\Naming\Shared_Hash_Map_T.cpp"\
	".\Naming\Shared_Hash_Map_T.h"\
	
NODEP_CPP_PERSISTE=\
	"..\..\..\..\..\Naming\Naming_Context.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PingC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PingS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushConsumer_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PROXY=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\CosEvent\ProxyPushConsumer_i.h"\
	".\Event_Service_Constants.h"\
	".\Event_Utilities.h"\
	".\Event_Utilities.i"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushSupplier_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_PROXYP=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\CosEvent\ProxyPushSupplier_i.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_REACT=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_REACT=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RT_TA=\
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
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event\Debug_Macros.h"\
	".\Event\Dispatching_Modules.h"\
	".\Event\Dispatching_Modules.i"\
	".\Event\Event_Channel.h"\
	".\Event\Event_Channel.i"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\Memory_Pools.h"\
	".\Event\Memory_Pools.i"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_RT_TA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\Event\ace\ReactorEx.h"\
	".\Event\CORBA_Utils_T.cpp"\
	".\Event\CORBA_Utils_T.h"\
	".\Event\CORBA_Utils_T.i"\
	".\Event\Task_Manager.h"\
	".\Event\Task_Manager.i"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECE=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECE=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECEV=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECEV=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventChannelAdminC.h"\
	".\RtecEventChannelAdminC.i"\
	".\RtecEventChannelAdminS.h"\
	".\RtecEventChannelAdminS.i"\
	".\RtecEventChannelAdminS_T.cpp"\
	".\RtecEventChannelAdminS_T.h"\
	".\RtecEventChannelAdminS_T.i"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECEVE=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECEVE=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECEVEN=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECEVEN=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecEventCommC.h"\
	".\RtecEventCommC.i"\
	".\RtecEventCommS.h"\
	".\RtecEventCommS.i"\
	".\RtecEventCommS_T.cpp"\
	".\RtecEventCommS_T.h"\
	".\RtecEventCommS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECS=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECS=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RTECSC=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_RTECSC=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_RUNTI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Runtime_Scheduler.h"\
	".\Runtime_Scheduler.i"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_RUNTI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SCHED=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\SchedEntry.h"\
	".\Sched\SchedEntry.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_SCHED=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SCHEDU=\
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
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\Scheduler.h"\
	".\Sched\Scheduler.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_SCHEDU=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SCHEDUL=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Runtime_Scheduler.h"\
	".\Runtime_Scheduler.i"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	
NODEP_CPP_SCHEDUL=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosNamingC.h"\
	".\CosNamingC.i"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\RtecSchedulerS.h"\
	".\RtecSchedulerS.i"\
	".\RtecSchedulerS_T.cpp"\
	".\RtecSchedulerS_T.h"\
	".\RtecSchedulerS_T.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SCHEDULE=\
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
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\Scheduler.h"\
	".\Sched\Scheduler.i"\
	".\Sched\Scheduler_Generic.h"\
	".\Sched\Scheduler_Generic.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_SCHEDULE=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SCHEDULER=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Scheduler_Utilities.h"\
	".\Scheduler_Utilities.i"\
	
NODEP_CPP_SCHEDULER=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SERVI=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Service_Type_Repository.h"\
	".\Trader\Trader.h"\
	
NODEP_CPP_SERVI=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\sfp.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SFP_C=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\ARGV.h"\
	"..\..\..\ace\ARGV.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\inc_user_config.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SOCK.h"\
	"..\..\..\ace\SOCK.i"\
	"..\..\..\ace\SOCK_Dgram.h"\
	"..\..\..\ace\SOCK_Dgram.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\AV\sfp.h"\
	".\orbsvcs_export.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfpC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfpS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_STRAT=\
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
	"..\..\..\ace\Sched_Params.h"\
	"..\..\..\ace\Sched_Params.i"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Sched\DynSched.h"\
	".\Sched\DynSched.i"\
	".\Sched\SchedEntry.h"\
	".\Sched\SchedEntry.i"\
	".\Sched\Strategy_Scheduler.h"\
	".\Sched\Strategy_Scheduler.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_STRAT=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\RtecSchedulerC.h"\
	".\RtecSchedulerC.i"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\SupplierAdmin_i.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_SUPPL=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
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
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\CosEvent\ProxyPushConsumer_i.h"\
	".\CosEvent\SupplierAdmin_i.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Clerk.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TAO_T=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\IOR_Multicast.h"\
	".\Naming\Naming_Utils.h"\
	".\orbsvcs_export.h"\
	".\Time\TAO_Time_Service_Clerk.h"\
	".\Time\TAO_TIO.h"\
	".\Time\TAO_UTO.h"\
	".\Time\Timer_Helper.h"\
	
NODEP_CPP_TAO_T=\
	"..\..\..\..\..\Naming\Naming_Context.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Server.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TAO_TI=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\Time\TAO_Time_Service_Server.h"\
	".\Time\TAO_TIO.h"\
	".\Time\TAO_UTO.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_TIO.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TAO_TIO=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\Time\TAO_TIO.h"\
	".\Time\TAO_UTO.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_UTO.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TAO_U=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
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
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Shared_Object.h"\
	"..\..\..\ace\Shared_Object.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\SV_Semaphore_Complex.h"\
	"..\..\..\ace\SV_Semaphore_Complex.i"\
	"..\..\..\ace\SV_Semaphore_Simple.h"\
	"..\..\..\ace\SV_Semaphore_Simple.i"\
	"..\..\..\ace\Svc_Conf_Tokens.h"\
	"..\..\..\ace\Synch.h"\
	"..\..\..\ace\Synch.i"\
	"..\..\..\ace\Synch_T.cpp"\
	"..\..\..\ace\Synch_T.h"\
	"..\..\..\ace\Synch_T.i"\
	"..\..\..\ace\Thread.h"\
	"..\..\..\ace\Thread.i"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\TAO.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	".\Time\TAO_TIO.h"\
	".\Time\TAO_UTO.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TIME_=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_TIME_=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TIMEB=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_TIMEB=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TIMEBA=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	
NODEP_CPP_TIMEBA=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\TimeBaseC.h"\
	".\TimeBaseC.i"\
	".\TimeBaseS.h"\
	".\TimeBaseS.i"\
	".\TimeBaseS_T.cpp"\
	".\TimeBaseS_T.h"\
	".\TimeBaseS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\Timer_Helper.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TIMER=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
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
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\IOR_Multicast.h"\
	".\Naming\Naming_Utils.h"\
	".\orbsvcs_export.h"\
	".\Time\TAO_Time_Service_Clerk.h"\
	".\Time\Timer_Helper.h"\
	
NODEP_CPP_TIMER=\
	"..\..\..\..\..\Naming\Naming_Context.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TIMER_=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Active_Map_Manager.h"\
	"..\..\..\ace\Active_Map_Manager.i"\
	"..\..\..\ace\Active_Map_Manager_T.cpp"\
	"..\..\..\ace\Active_Map_Manager_T.h"\
	"..\..\..\ace\Active_Map_Manager_T.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Atomic_Op.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\CDR_Stream.h"\
	"..\..\..\ace\CDR_Stream.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\Connector.cpp"\
	"..\..\..\ace\Connector.h"\
	"..\..\..\ace\Connector.i"\
	"..\..\..\ace\Containers.h"\
	"..\..\..\ace\Containers.i"\
	"..\..\..\ace\Containers_T.cpp"\
	"..\..\..\ace\Containers_T.h"\
	"..\..\..\ace\Containers_T.i"\
	"..\..\..\ace\CORBA_macros.h"\
	"..\..\..\ace\Dynamic.h"\
	"..\..\..\ace\Dynamic.i"\
	"..\..\..\ace\Dynamic_Service.cpp"\
	"..\..\..\ace\Dynamic_Service.h"\
	"..\..\..\ace\Event_Handler.h"\
	"..\..\..\ace\Event_Handler.i"\
	"..\..\..\ace\Free_List.cpp"\
	"..\..\..\ace\Free_List.h"\
	"..\..\..\ace\Free_List.i"\
	"..\..\..\ace\Functor.h"\
	"..\..\..\ace\Functor.i"\
	"..\..\..\ace\Functor_T.cpp"\
	"..\..\..\ace\Functor_T.h"\
	"..\..\..\ace\Functor_T.i"\
	"..\..\..\ace\Handle_Set.h"\
	"..\..\..\ace\Handle_Set.i"\
	"..\..\..\ace\Hash_Map_Manager.h"\
	"..\..\..\ace\Hash_Map_Manager.i"\
	"..\..\..\ace\Hash_Map_Manager_T.cpp"\
	"..\..\..\ace\Hash_Map_Manager_T.h"\
	"..\..\..\ace\Hash_Map_Manager_T.i"\
	"..\..\..\ace\High_Res_Timer.h"\
	"..\..\..\ace\High_Res_Timer.i"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\INET_Addr.h"\
	"..\..\..\ace\INET_Addr.i"\
	"..\..\..\ace\IO_Cntl_Msg.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\IPC_SAP.h"\
	"..\..\..\ace\IPC_SAP.i"\
	"..\..\..\ace\Local_Tokens.h"\
	"..\..\..\ace\Local_Tokens.i"\
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
	"..\..\..\ace\Map.h"\
	"..\..\..\ace\Map.i"\
	"..\..\..\ace\Map_Manager.cpp"\
	"..\..\..\ace\Map_Manager.h"\
	"..\..\..\ace\Map_Manager.i"\
	"..\..\..\ace\Map_T.cpp"\
	"..\..\..\ace\Map_T.h"\
	"..\..\..\ace\Map_T.i"\
	"..\..\..\ace\Mem_Map.h"\
	"..\..\..\ace\Mem_Map.i"\
	"..\..\..\ace\Memory_Pool.h"\
	"..\..\..\ace\Memory_Pool.i"\
	"..\..\..\ace\Message_Block.h"\
	"..\..\..\ace\Message_Block.i"\
	"..\..\..\ace\Message_Block_T.cpp"\
	"..\..\..\ace\Message_Block_T.h"\
	"..\..\..\ace\Message_Block_T.i"\
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
	"..\..\..\ace\Pair.h"\
	"..\..\..\ace\Pair.i"\
	"..\..\..\ace\Pair_T.cpp"\
	"..\..\..\ace\Pair_T.h"\
	"..\..\..\ace\Pair_T.i"\
	"..\..\..\ace\Pipe.h"\
	"..\..\..\ace\Pipe.i"\
	"..\..\..\ace\Reactor.h"\
	"..\..\..\ace\Reactor.i"\
	"..\..\..\ace\Reactor_Impl.h"\
	"..\..\..\ace\Select_Reactor.h"\
	"..\..\..\ace\Select_Reactor.i"\
	"..\..\..\ace\Select_Reactor_Base.h"\
	"..\..\..\ace\Select_Reactor_Base.i"\
	"..\..\..\ace\Select_Reactor_T.cpp"\
	"..\..\..\ace\Select_Reactor_T.h"\
	"..\..\..\ace\Select_Reactor_T.i"\
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
	"..\..\..\ace\Timeprobe.h"\
	"..\..\..\ace\Timeprobe.i"\
	"..\..\..\ace\Timeprobe_T.cpp"\
	"..\..\..\ace\Timeprobe_T.h"\
	"..\..\..\ace\Timer_Heap.h"\
	"..\..\..\ace\Timer_Heap_T.cpp"\
	"..\..\..\ace\Timer_Heap_T.h"\
	"..\..\..\ace\Timer_List.h"\
	"..\..\..\ace\Timer_List_T.cpp"\
	"..\..\..\ace\Timer_List_T.h"\
	"..\..\..\ace\Timer_Queue.h"\
	"..\..\..\ace\Timer_Queue_T.cpp"\
	"..\..\..\ace\Timer_Queue_T.h"\
	"..\..\..\ace\Timer_Queue_T.i"\
	"..\..\..\ace\Token.h"\
	"..\..\..\ace\Token.i"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\WFMO_Reactor.h"\
	"..\..\..\ace\WFMO_Reactor.i"\
	"..\..\..\ace\ws2tcpip.h"\
	"..\..\tao\Active_Object_Map.h"\
	"..\..\tao\Active_Object_Map.i"\
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\Client_Strategy_Factory.h"\
	"..\..\tao\connect.h"\
	"..\..\tao\connect.i"\
	"..\..\tao\Context.h"\
	"..\..\tao\Context.i"\
	"..\..\tao\corba.h"\
	"..\..\tao\corbafwd.h"\
	"..\..\tao\corbafwd.i"\
	"..\..\tao\CurrentC.h"\
	"..\..\tao\CurrentC.i"\
	"..\..\tao\debug.h"\
	"..\..\tao\DomainC.h"\
	"..\..\tao\DomainC.i"\
	"..\..\tao\DynAny_i.h"\
	"..\..\tao\DynAnyC.h"\
	"..\..\tao\DynAnyC.i"\
	"..\..\tao\Environment.h"\
	"..\..\tao\Environment.i"\
	"..\..\tao\Exception.h"\
	"..\..\tao\Exception.i"\
	"..\..\tao\giop.h"\
	"..\..\tao\giop.i"\
	"..\..\tao\IIOP_Connector.h"\
	"..\..\tao\InconsistentTypeCodeC.h"\
	"..\..\tao\Invocation.h"\
	"..\..\tao\Invocation.i"\
	"..\..\tao\IOR_LookupTable.h"\
	"..\..\tao\Key_Adapters.h"\
	"..\..\tao\Key_Adapters.i"\
	"..\..\tao\Managed_Types.h"\
	"..\..\tao\Managed_Types.i"\
	"..\..\tao\marshal.h"\
	"..\..\tao\marshal.i"\
	"..\..\tao\MProfile.h"\
	"..\..\tao\MProfile.i"\
	"..\..\tao\nvlist.h"\
	"..\..\tao\NVList.i"\
	"..\..\tao\object.h"\
	"..\..\tao\object.i"\
	"..\..\tao\Object_Adapter.h"\
	"..\..\tao\Object_Adapter.i"\
	"..\..\tao\Object_KeyC.h"\
	"..\..\tao\Object_KeyC.i"\
	"..\..\tao\ObjectIDList.h"\
	"..\..\tao\ObjectIDList.i"\
	"..\..\tao\Operation_Table.h"\
	"..\..\tao\orb.h"\
	"..\..\tao\ORB.i"\
	"..\..\tao\orb_core.h"\
	"..\..\tao\orb_core.i"\
	"..\..\tao\orbconf.h"\
	"..\..\tao\params.h"\
	"..\..\tao\params.i"\
	"..\..\tao\Pluggable.h"\
	"..\..\tao\Pluggable.i"\
	"..\..\tao\poa.h"\
	"..\..\tao\POA.i"\
	"..\..\tao\POA_CORBA.h"\
	"..\..\tao\poa_macros.h"\
	"..\..\tao\poaC.h"\
	"..\..\tao\poaC.i"\
	"..\..\tao\POAManager.h"\
	"..\..\tao\POAManager.i"\
	"..\..\tao\poaS.h"\
	"..\..\tao\poaS.i"\
	"..\..\tao\Policy_Manager.h"\
	"..\..\tao\Policy_Manager.i"\
	"..\..\tao\PolicyC.h"\
	"..\..\tao\PolicyC.i"\
	"..\..\tao\PollableC.h"\
	"..\..\tao\PollableC.i"\
	"..\..\tao\Principal.h"\
	"..\..\tao\Principal.i"\
	"..\..\tao\request.h"\
	"..\..\tao\Request.i"\
	"..\..\tao\Resource_Factory.h"\
	"..\..\tao\sequence.h"\
	"..\..\tao\sequence.i"\
	"..\..\tao\Sequence_T.cpp"\
	"..\..\tao\Sequence_T.h"\
	"..\..\tao\Sequence_T.i"\
	"..\..\tao\servant_base.h"\
	"..\..\tao\Servant_Base.i"\
	"..\..\tao\Server_Request.h"\
	"..\..\tao\Server_Request.i"\
	"..\..\tao\Server_Strategy_Factory.h"\
	"..\..\tao\Services.h"\
	"..\..\tao\Services.i"\
	"..\..\tao\singletons.h"\
	"..\..\tao\stub.h"\
	"..\..\tao\stub.i"\
	"..\..\tao\Timeprobe.h"\
	"..\..\tao\try_macros.h"\
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\ValueBase.h"\
	"..\..\tao\ValueBase.i"\
	"..\..\tao\ValueFactory.h"\
	"..\..\tao\ValueFactory.i"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	"..\..\tao\WrongTransactionC.h"\
	".\Event\Fast_Reactor.h"\
	".\Event\Local_ESTypes.h"\
	".\Event\Local_ESTypes.i"\
	".\Event\ReactorTask.h"\
	".\Event\RT_Task.h"\
	".\Event\RT_Task.i"\
	".\Event\Timer_Module.h"\
	".\Event\Timer_Module.i"\
	".\Event_Service_Constants.h"\
	".\orbsvcs_export.h"\
	".\Scheduler_Factory.h"\
	".\Scheduler_Factory.i"\
	".\Time_Utilities.h"\
	".\Time_Utilities.i"\
	
NODEP_CPP_TIMER_=\
	".\Event\ace\ReactorEx.h"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TRADE=\
	"..\..\..\ace\Acceptor.cpp"\
	"..\..\..\ace\Acceptor.h"\
	"..\..\..\ace\Acceptor.i"\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Addr.h"\
	"..\..\..\ace\Addr.i"\
	"..\..\..\ace\Arg_Shifter.h"\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Constraint_Interpreter.h"\
	".\Trader\Constraint_Nodes.h"\
	".\Trader\Constraint_Tokens.h"\
	".\Trader\Constraint_Visitors.h"\
	".\Trader\Offer_Database.cpp"\
	".\Trader\Offer_Database.h"\
	".\Trader\Offer_Iterators.h"\
	".\Trader\Offer_Iterators_T.cpp"\
	".\Trader\Offer_Iterators_T.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Interfaces.cpp"\
	".\Trader\Trader_Interfaces.h"\
	".\Trader\Trader_T.cpp"\
	".\Trader\Trader_T.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_TRADE=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

DEP_CPP_TRADER=\
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
	"..\..\tao\any.h"\
	"..\..\tao\any.i"\
	"..\..\tao\cdr.h"\
	"..\..\tao\cdr.i"\
	"..\..\tao\CDR_Interpreter.h"\
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
	"..\..\tao\typecode.h"\
	"..\..\tao\typecode.i"\
	"..\..\tao\Union.h"\
	"..\..\tao\varout.cpp"\
	"..\..\tao\varout.h"\
	"..\..\tao\varout.i"\
	".\orbsvcs_export.h"\
	".\Trader\Trader.h"\
	".\Trader\Trader_Utils.h"\
	
NODEP_CPP_TRADER=\
	"..\..\..\ace\sys_conf.h"\
	"..\..\tao\align.h"\
	"..\..\tao\IIOP_Object.h"\
	"..\..\tao\IIOP_Object.i"\
	"..\..\tao\IIOP_ORB.h"\
	"..\..\tao\IIOP_ORB.i"\
	"..\..\tao\Object_Table.h"\
	".\CosTradingC.h"\
	".\CosTradingC.i"\
	".\CosTradingS.h"\
	".\CosTradingS.i"\
	".\CosTradingS_T.cpp"\
	".\CosTradingS_T.h"\
	".\CosTradingS_T.i"\
	

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\AV\AVStreams_i.h
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i_T.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event\BCU.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSet.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSetFactory.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\Concurrency_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlC.h
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.h
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.h
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i_T.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.h
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.h
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event_Service_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Hash_Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.h
# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.h
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.h
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.h
# End Source File
# Begin Source File

SOURCE=.\Event\Local_ESTypes.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Locking.h
# End Source File
# Begin Source File

SOURCE=.\LoggerC.h
# End Source File
# Begin Source File

SOURCE=.\LoggerS.h
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.h
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Bindings_Map.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\PingC.h
# End Source File
# Begin Source File

SOURCE=.\PingS.h
# End Source File
# Begin Source File

SOURCE=.\PingS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.h
# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.h
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.h
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.h
# End Source File
# Begin Source File

SOURCE=.\sfpC.h
# End Source File
# Begin Source File

SOURCE=.\sfpS.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Shared_Hash_Map_T.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Clerk.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Server.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_TIO.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_UTO.h
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.h
# End Source File
# Begin Source File

SOURCE=.\Time\Timer_Helper.h
# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\CosConcurrencyControlC.i
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.i
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.i
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.i
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.i
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.i
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.i
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.i
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.i
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.i
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.i
# End Source File
# Begin Source File

SOURCE=.\PingC.i
# End Source File
# Begin Source File

SOURCE=.\PingS.i
# End Source File
# Begin Source File

SOURCE=.\PingS_T.i
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.i
# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\AVStreams.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTRE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTRE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTRE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTRE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosConcurrencyControl.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSCONC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSCONC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSCONC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSCONC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosEventChannelAdmin.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosEventComm.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosLifeCycle.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSLIFE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLIFE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSLIFE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLIFE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosNaming.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSNAMI="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSNAMI="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSNAMI="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSNAMI="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosPropertyService.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSPRO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSPRO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSPRO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSPRO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosTrading.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosTradingDynamic.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRADI="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRADI="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\CosTradingRepos.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRADIN="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRADIN="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\ImplRepo.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\LifeCycleService.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__LIFECYC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LIFECYC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__LIFECYC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LIFECYC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\Logger.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__LOGGER_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LOGGER_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__LOGGER_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LOGGER_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\Null_MediaCtrl.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\Ping.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__PING_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PING_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__PING_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PING_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENT="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENT="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENT="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENT="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\RtecEventComm.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENTC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENTC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENTC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEVENTC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\RtecScheduler.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECSCH="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECSCH="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECSCH="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECSCH="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\RtecUDPAdmin.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\sfp.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\TimeBase.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEBAS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEBAS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on TimeBase.idl
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl              -I../../ \
               -Wb,export_macro=TAO_ORBSVCS_Export \
                                                          -Wb,export_include=orbsvcs_export.h              $(InputName).idl \
             -I..\..\ 

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEBAS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEBAS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\TimeService.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
       -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -i../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
                                              -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AV\AVStreams_i_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Naming\Shared_Hash_Map_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
