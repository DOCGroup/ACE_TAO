# Microsoft Developer Studio Project File - Name="DLList_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=DLList_Test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DLList_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DLList_Test.mak" CFG="DLList_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DLList_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "DLList_Test - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "DLList_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\DLList_Test\Debug"
# PROP BASE Intermediate_Dir ".\DLList_Test\Debug"
# PROP BASE Target_Dir ".\DLList_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\DLList_Test"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "DLList_Test - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DLList_Test\Alpha Debug"
# PROP BASE Intermediate_Dir "DLList_Test\Alpha Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "DLList_Test"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "DLList_Test"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /MTd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /MDd /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\ace"
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\ace"

!ENDIF 

# Begin Target

# Name "DLList_Test - Win32 Debug"
# Name "DLList_Test - Win32 Alpha Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\DLList_Test.cpp

!IF  "$(CFG)" == "DLList_Test - Win32 Debug"

!ELSEIF  "$(CFG)" == "DLList_Test - Win32 Alpha Debug"

DEP_CPP_BARRI=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\stdcpp.h"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\test_config.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
