# Microsoft Developer Studio Project File - Name="gperf_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=gperf_lib - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gperf_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gperf_lib.mak" CFG="gperf_lib - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gperf_lib - Win32 Alpha Release" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "gperf_lib - Win32 Alpha Debug" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "gperf_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gperf_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "gperf_lib - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "gperf_lib - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf_l1"
# PROP BASE Intermediate_Dir "gperf_l1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\LIB"
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\\ /D " WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\\ /D " WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\lib\gperf.lib"
# ADD LIB32 /nologo /out:".\gperf.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf_l0"
# PROP BASE Intermediate_Dir "gperf_l0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\LIB"
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "..\..\..\\ /D " WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Z7 /Od /I "..\..\..\\ /D " WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\lib\gperfd.lib"
# ADD LIB32 /nologo /out:".\gperfd.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf_li"
# PROP BASE Intermediate_Dir "gperf_li"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\gperf.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf_l0"
# PROP BASE Intermediate_Dir "gperf_l0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\..\\" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\gperfd.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf_lib___Win32_Static_Release"
# PROP BASE Intermediate_Dir "gperf_lib___Win32_Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "_WINDOWS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\lib\gperf.lib"
# ADD LIB32 /nologo /out:".\gperfs.lib"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf_lib___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "gperf_lib___Win32_Static_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\..\\" /D "_WINDOWS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\lib\gperfd.lib"
# ADD LIB32 /nologo /out:".\gperfsd.lib"

!ENDIF 

# Begin Target

# Name "gperf_lib - Win32 Alpha Release"
# Name "gperf_lib - Win32 Alpha Debug"
# Name "gperf_lib - Win32 Release"
# Name "gperf_lib - Win32 Debug"
# Name "gperf_lib - Win32 Static Release"
# Name "gperf_lib - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Bool_Array.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_BOOL_=\
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
	".\Bool_Array.h"\
	".\Options.h"\
	
NODEP_CPP_BOOL_=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Gen_Perf.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_GEN_P=\
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
	".\Bool_Array.h"\
	".\Gen_Perf.h"\
	".\Key_List.h"\
	".\List_Node.h"\
	".\Options.h"\
	".\Vectors.h"\
	
NODEP_CPP_GEN_P=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hash_Table.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_HASH_=\
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
	".\Hash_Table.h"\
	".\List_Node.h"\
	".\Options.h"\
	
NODEP_CPP_HASH_=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Iterator.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_ITERA=\
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
	".\Iterator.h"\
	".\Options.h"\
	
NODEP_CPP_ITERA=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Key_List.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_KEY_L=\
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
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\Read_Buffer.h"\
	"..\..\..\ace\Read_Buffer.i"\
	"..\..\..\ace\Signal.h"\
	"..\..\..\ace\Signal.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
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
	".\Hash_Table.h"\
	".\Key_List.h"\
	".\List_Node.h"\
	".\Options.h"\
	".\Vectors.h"\
	
NODEP_CPP_KEY_L=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\List_Node.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_LIST_=\
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
	".\List_Node.h"\
	".\Options.h"\
	".\Vectors.h"\
	
NODEP_CPP_LIST_=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Options.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_OPTIO=\
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
	"..\..\..\ace\Get_Opt.h"\
	"..\..\..\ace\Get_Opt.i"\
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
	".\Iterator.h"\
	".\Options.h"\
	
NODEP_CPP_OPTIO=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Vectors.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_VECTO=\
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
	".\Vectors.h"\
	
NODEP_CPP_VECTO=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Version.cpp

!IF  "$(CFG)" == "gperf_lib - Win32 Alpha Release"

DEP_CPP_VERSI=\
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
	
NODEP_CPP_VERSI=\
	"..\..\..\ace\inc_user_config.h"\
	

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Release"

!ELSEIF  "$(CFG)" == "gperf_lib - Win32 Static Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Bool_Array.h
# End Source File
# Begin Source File

SOURCE=.\Gen_Perf.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Table.h
# End Source File
# Begin Source File

SOURCE=.\Iterator.h
# End Source File
# Begin Source File

SOURCE=.\Key_List.h
# End Source File
# Begin Source File

SOURCE=.\List_Node.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\Vectors.h
# End Source File
# End Group
# End Target
# End Project
