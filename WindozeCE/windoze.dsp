# Microsoft Developer Studio Project File - Name="windoez" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE SH) Static Library" 0x0904
# TARGTYPE "Win32 (WCE MIPS) Static Library" 0x0a04
# TARGTYPE "Win32 (WCE x86em) Static Library" 0x0b04

CFG=windoez - Win32 (WCE x86em) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "windoez.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "windoez.mak" CFG="windoez - Win32 (WCE x86em) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "windoez - Win32 (WCE x86em) Release" (based on\
 "Win32 (WCE x86em) Static Library")
!MESSAGE "windoez - Win32 (WCE x86em) Debug" (based on\
 "Win32 (WCE x86em) Static Library")
!MESSAGE "windoez - Win32 (WCE MIPS) Release" (based on\
 "Win32 (WCE MIPS) Static Library")
!MESSAGE "windoez - Win32 (WCE MIPS) Debug" (based on\
 "Win32 (WCE MIPS) Static Library")
!MESSAGE "windoez - Win32 (WCE SH) Release" (based on\
 "Win32 (WCE SH) Static Library")
!MESSAGE "windoez - Win32 (WCE SH) Debug" (based on\
 "Win32 (WCE SH) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "windoez - Win32 (WCE x86em) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x86emRel"
# PROP BASE Intermediate_Dir "x86emRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x86emRel"
# PROP Intermediate_Dir "x86emRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /O2 /D "UNDER_CE" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=200 /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /YX /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\\" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x86emDbg"
# PROP Intermediate_Dir "x86emDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Z7 /Od /D "UNDER_CE" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=200 /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /YX /c
# ADD CPP /nologo /MTd /W3 /Z7 /Od /I "..\\" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WMIPSRel"
# PROP BASE Intermediate_Dir "WMIPSRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WMIPSRel"
# PROP Intermediate_Dir "WMIPSRel"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /QMRWCE /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\\" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /YX /QMRWCE /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSDbg"
# PROP BASE Intermediate_Dir "WMIPSDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WMIPSDbg"
# PROP Intermediate_Dir "WMIPSDbg"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /QMRWCE /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\\" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /YX /QMRWCE /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESHRel"
# PROP BASE Intermediate_Dir "WCESHRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCESHRel"
# PROP Intermediate_Dir "WCESHRel"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\\" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESHDbg"
# PROP BASE Intermediate_Dir "WCESHDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCESHDbg"
# PROP Intermediate_Dir "WCESHDbg"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\\" /D "DEBUG" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_MFC=1 /D ACE_HAS_WINCE=1 /D "_AFXDLL" /c
# SUBTRACT CPP /Gy /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "windoez - Win32 (WCE x86em) Release"
# Name "windoez - Win32 (WCE x86em) Debug"
# Name "windoez - Win32 (WCE MIPS) Release"
# Name "windoez - Win32 (WCE MIPS) Debug"
# Name "windoez - Win32 (WCE SH) Release"
# Name "windoez - Win32 (WCE SH) Debug"
# Begin Source File

SOURCE=.\ACEApp.cpp

!IF  "$(CFG)" == "windoez - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Debug"

DEP_CPP_ACEAP=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config-WinCE.h"\
	"..\ace\config.h"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\sys_conf.h"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\ACEApp.h"\
	".\stdafx.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "windoez - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WinAdapter.cpp

!IF  "$(CFG)" == "windoez - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Debug"

DEP_CPP_WINAD=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config-WinCE.h"\
	"..\ace\config.h"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\sys_conf.h"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\ACEApp.h"\
	".\stdafx.h"\
	".\WinAdapter.h"\
	".\WinAdapterDlg.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WinAdapter.rc
# End Source File
# Begin Source File

SOURCE=.\WinAdapterDlg.cpp

!IF  "$(CFG)" == "windoez - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "windoez - Win32 (WCE SH) Debug"

DEP_CPP_WINADA=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\ARGV.h"\
	"..\ace\ARGV.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config-WinCE.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
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
	"..\ace\streams.h"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\sys_conf.h"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\ACEApp.h"\
	".\stdafx.h"\
	".\WinAdapter.h"\
	".\WinAdapterDlg.h"\
	

!ENDIF 

# End Source File
# End Target
# End Project
