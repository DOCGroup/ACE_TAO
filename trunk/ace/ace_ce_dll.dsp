# Microsoft Developer Studio Project File - Name="ace_ce_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE SH) Dynamic-Link Library" 0x0902
# TARGTYPE "Win32 (WCE x86em) Dynamic-Link Library" 0x0b02
# TARGTYPE "Win32 (WCE MIPS) Dynamic-Link Library" 0x0a02

CFG=ace_ce_dll - Win32 (WCE x86em) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_ce_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_ce_dll.mak" CFG="ace_ce_dll - Win32 (WCE x86em) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ace_ce_dll - Win32 (WCE x86em) Release" (based on\
 "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE "ace_ce_dll - Win32 (WCE x86em) Debug" (based on\
 "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE "ace_ce_dll - Win32 (WCE MIPS) Release" (based on\
 "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "ace_ce_dll - Win32 (WCE MIPS) Debug" (based on\
 "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "ace_ce_dll - Win32 (WCE SH) Release" (based on\
 "Win32 (WCE SH) Dynamic-Link Library")
!MESSAGE "ace_ce_dll - Win32 (WCE SH) Debug" (based on\
 "Win32 (WCE SH) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP WCE_Configuration "H/PC Ver. 2.00"

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x86emRel"
# PROP BASE Intermediate_Dir "x86emRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCE/x86em"
# PROP Intermediate_Dir "WCE/x86em/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /O2 /D "UNDER_CE" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=200 /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /YX /c
# ADD CPP /nologo /W3 /O2 /I "..\\" /I "..\STL\\" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "UNDER_CE" /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=200 /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "NDEBUG"
# ADD RSC /l 0x409 /d "UNDER_CE" /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=200 /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "NDEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /mktyplib203 /o NUL /win32 /NDEBUG /NDEBUG
# ADD MTL /nologo /mktyplib203 /o NUL /win32 /NDEBUG /NDEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib winmm.lib ole32m.lib oleautm.lib msvcrt.lib uuid.lib asfcguid.lib asfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"msvcrt.lib" /windowsce:emulation
# ADD LINK32 commctrl.lib coredll.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"ace.dll" /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -COPY
# ADD EMPFILE -COPY

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCE/x86em"
# PROP Intermediate_Dir "WCE/x86em/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /Zi /Od /D "UNDER_CE" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=200 /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /YX /c
# ADD CPP /nologo /W3 /Gm /Zi /Od /I "..\\" /I "..\STL\\" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "UNDER_CE" /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=200 /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_"
# ADD RSC /l 0x409 /d "UNDER_CE" /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=200 /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib winmm.lib ole32m.lib oleautm.lib msvcrt.lib uuid.lib asfcguid.lib asfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrtd.lib" /windowsce:emulation
# ADD LINK32 commctrl.lib coredll.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrtd.lib" /out:"aced.dll" /windowsce:emulation
EMPFILE=empfile.exe
# ADD BASE EMPFILE -COPY
# ADD EMPFILE -COPY

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WMIPSRel"
# PROP BASE Intermediate_Dir "WMIPSRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCE/MIPS"
# PROP Intermediate_Dir "WCE/MIPS/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /QMRWCE /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\\" /I "..\STL\\" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /YX /QMRWCE /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d "UNDER_CE" /d "UNICODE" /d "NDEBUG" /d _WIN32_WCE=200
# ADD RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d "UNDER_CE" /d "UNICODE" /d "NDEBUG" /d _WIN32_WCE=200
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /machine:MIPS /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:MIPS /nodefaultlib:"wsock32.lib" /out:"ace.dll" /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSDbg"
# PROP BASE Intermediate_Dir "WMIPSDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCE/MIPS"
# PROP Intermediate_Dir "WCE/MIPS/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /QMRWCE /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /I "..\STL\\" /I "..\\" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /YX /QMRWCE /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d "UNDER_CE" /d "UNICODE" /d "DEBUG" /d _WIN32_WCE=200
# ADD RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d "UNDER_CE" /d "UNICODE" /d "DEBUG" /d _WIN32_WCE=200
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /nodefaultlib:"wsock32.lib" /out:"aced.dll" /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT LINK32 /pdb:none /nodefaultlib
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESHRel"
# PROP BASE Intermediate_Dir "WCESHRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCE/SH"
# PROP Intermediate_Dir "WCE/SH/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\\" /I "..\STL\\" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d "UNDER_CE" /d "UNICODE" /d "NDEBUG" /d _WIN32_WCE=200
# ADD RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d "UNDER_CE" /d "UNICODE" /d "NDEBUG" /d _WIN32_WCE=200
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /nodefaultlib:"wsock32.lib" /out:"WCE\sh\ace.dll" /subsystem:windowsce,2.0
# SUBTRACT LINK32 /pdb:none
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESHDbg"
# PROP BASE Intermediate_Dir "WCESHDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCE/SH"
# PROP Intermediate_Dir "WCE/SH/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /MLd /W3 /Zi /Od /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /YX /c
# ADD CPP /nologo /MTd /W3 /Zi /Od /Gy /I "..\\" /I "..\STL\\" /I "..\..\WinCE\mfcdll" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D "UNDER_CE" /D "UNICODE" /D _WIN32_WCE=200 /D ACE_HAS_WINCE=1 /D ACE_HAS_DLL=1 /D "_AFXDLL" /YX /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d "UNDER_CE" /d "UNICODE" /d "DEBUG" /d _WIN32_WCE=200
# ADD RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d "UNDER_CE" /d "UNICODE" /d "DEBUG" /d _WIN32_WCE=200 /d "_AFXDLL"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:SH3 /subsystem:windowsce,2.0 /windowsce:noconvert
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 /nologo /dll /incremental:no /debug /machine:SH3 /out:"WCE\SH\aced.dll" /subsystem:windowsce,2.0
# SUBTRACT LINK32 /pdb:none
PFILE=pfile.exe
# ADD BASE PFILE COPY
# ADD PFILE COPY

!ENDIF 

# Begin Target

# Name "ace_ce_dll - Win32 (WCE x86em) Release"
# Name "ace_ce_dll - Win32 (WCE x86em) Debug"
# Name "ace_ce_dll - Win32 (WCE MIPS) Release"
# Name "ace_ce_dll - Win32 (WCE MIPS) Debug"
# Name "ace_ce_dll - Win32 (WCE SH) Release"
# Name "ace_ce_dll - Win32 (WCE SH) Debug"
# Begin Source File

SOURCE=.\ACE.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ACE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ACE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ACE_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ACTIV=\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ACTIV=\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ACTIV=\
	".\ACE.h"\
	".\ACE.i"\
	".\Activation_Queue.h"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Addr.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ADDR_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ARGV.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ARGV_=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ace/Array.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ARRAY=\
	".\ACE.h"\
	".\ACE.i"\
	".\Array.h"\
	".\Array.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ARRAY=\
	".\ACE.h"\
	".\ACE.i"\
	".\Array.h"\
	".\Array.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ARRAY=\
	".\ACE.h"\
	".\ACE.i"\
	".\Array.h"\
	".\Array.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_ASYNC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_ASYNC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_ASYNC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_BASIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_BASIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_BASIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dump.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_DUMP_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dump.h"\
	".\Dump_T.cpp"\
	".\Dump_T.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_DYNAM=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Handler.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_EVENT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_EVENT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_EVENT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_GET_O=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_HANDL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_HIGH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\INET_Addr.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_INET_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_SAP.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_IO_SA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\IO_SAP.h"\
	".\IO_SAP.i"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_IPC_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_LOCAL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_LOCAL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_LOCAL=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_LOG_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_LOG_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_LOG_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SPIPE.h"\
	".\SPIPE.i"\
	".\SPIPE_Addr.h"\
	".\SPIPE_Addr.i"\
	".\SPIPE_Connector.h"\
	".\SPIPE_Connector.i"\
	".\SPIPE_Stream.h"\
	".\SPIPE_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_LOG_R=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Malloc.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_MALLO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_MALLO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_MALLO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mem_Map.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_MEM_M=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_MEMOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_MEMOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_MEMOR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message_Block.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_MESSA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_MESSA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_MESSA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Method_Object.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_METHO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Method_Object.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Manager.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_OBJEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Array.cpp"\
	".\Array.h"\
	".\Array.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_OBJEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Array.cpp"\
	".\Array.h"\
	".\Array.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_OBJEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Array.cpp"\
	".\Array.h"\
	".\Array.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Name_Proxy.h"\
	".\Name_Request_Reply.h"\
	".\Name_Space.h"\
	".\Naming_Context.h"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Obstack.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_OBSTA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OS.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_OS_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Array.cpp"\
	".\Array.h"\
	".\Array.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_OS_CP=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Array.cpp"\
	".\Array.h"\
	".\Array.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_PARSE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_PARSE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_PARSE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_PIPE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_PROCE=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process_Manager.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_PROCES=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Process.h"\
	".\Process.i"\
	".\Process_Manager.h"\
	".\Process_Manager.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_PROFI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Profile_Timer.h"\
	".\Profile_Timer.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reactor.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_REACT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_REACT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_REACT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Msg_WFMO_Reactor.h"\
	".\Msg_WFMO_Reactor.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched_Params.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SCHED=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Sched_Params.h"\
	".\Sched_Params.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SELEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SELEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SELEC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SERVI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Asynch_IO.h"\
	".\Asynch_IO.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\High_Res_Timer.h"\
	".\High_Res_Timer.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Proactor.h"\
	".\Proactor.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_List.h"\
	".\Timer_List_T.cpp"\
	".\Timer_List_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Timer_Wheel.h"\
	".\Timer_Wheel_T.cpp"\
	".\Timer_Wheel_T.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SERVIC=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Get_Opt.h"\
	".\Get_Opt.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Manager.h"\
	".\Service_Manager.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Object.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SERVICE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SERVICE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SERVICE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SERVICE_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Types.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SERVICE_T=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SERVICE_T=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SERVICE_T=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Object.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SHARE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Signal.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SIGNA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SIGNA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SIGNA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_A=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_CODgram.h"\
	".\SOCK_CODgram.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Time_Value.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_D=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_DG=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Bcast.h"\
	".\SOCK_Dgram_Bcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_DGR=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Dgram.h"\
	".\SOCK_Dgram.i"\
	".\SOCK_Dgram_Mcast.h"\
	".\SOCK_Dgram_Mcast.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_I=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SOCK_S=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SString.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SSTRI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SSTRI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SSTRI=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Strategies.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_STRAT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_STRAT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_STRAT=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SVC_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SVC_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SVC_C=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SVC_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SVC_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SVC_CO=\
	".\ACE.h"\
	".\ACE.i"\
	".\ARGV.h"\
	".\ARGV.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\Obstack.h"\
	".\OS.h"\
	".\OS.i"\
	".\Parse_Node.h"\
	".\Parse_Node.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream.cpp"\
	".\Stream.h"\
	".\Stream.i"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf.h"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SYNCH=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch_Options.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Synch_Options.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Synch_Options.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SYNCH_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\Synch_Options.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\System_Time.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_SYSTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_SYSTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_SYSTE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\System_Time.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_TASK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_TASK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_TASK_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Module.cpp"\
	".\Module.h"\
	".\Module.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\Stream_Modules.cpp"\
	".\Stream_Modules.h"\
	".\Stream_Modules.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Task.h"\
	".\Task.i"\
	".\Task_T.cpp"\
	".\Task_T.h"\
	".\Task_T.i"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_THREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_THREAD=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Dynamic.h"\
	".\Dynamic.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\Singleton.cpp"\
	".\Singleton.h"\
	".\Singleton.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_TOKEN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_TOKEN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_TOKEN=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Manager.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_TOKEN_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_TOKEN_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_TOKEN_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token_Manager.h"\
	".\Token_Manager.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trace.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\Version.h"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_TRACE=\
	".\ACE.h"\
	".\ACE.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\inc_user_config.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\sys_conf.h"\
	".\Trace.h"\
	".\Trace.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_WFMO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_WFMO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\Version.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_WFMO_=\
	".\ACE.h"\
	".\ACE.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\Hash_Map_Manager.cpp"\
	".\Hash_Map_Manager.h"\
	".\inc_user_config.h"\
	".\IO_Cntl_Msg.h"\
	".\iosfwd.h"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Message_Block.h"\
	".\Message_Block.i"\
	".\Message_Queue.cpp"\
	".\Message_Queue.h"\
	".\Message_Queue.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Service_Repository.h"\
	".\Service_Repository.i"\
	".\Service_Types.h"\
	".\Service_Types.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SString.h"\
	".\SString.i"\
	".\Strategies.h"\
	".\Strategies_T.cpp"\
	".\Strategies_T.h"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Thread_Manager.h"\
	".\Thread_Manager.i"\
	".\Timer_Heap.h"\
	".\Timer_Heap_T.cpp"\
	".\Timer_Heap_T.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Trace.h"\
	".\WFMO_Reactor.h"\
	".\WFMO_Reactor.i"\
	".\ws2tcpip.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XtReactor.cpp

!IF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE x86em) Debug"

DEP_CPP_XTREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	".\XtReactor.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE MIPS) Debug"

DEP_CPP_XTREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\Version.h"\
	".\ws2tcpip.h"\
	".\XtReactor.h"\
	

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Release"

!ELSEIF  "$(CFG)" == "ace_ce_dll - Win32 (WCE SH) Debug"

DEP_CPP_XTREA=\
	".\ACE.h"\
	".\ACE.i"\
	".\Addr.h"\
	".\Addr.i"\
	".\Atomic_Op.i"\
	".\Auto_Ptr.cpp"\
	".\Auto_Ptr.h"\
	".\Auto_Ptr.i"\
	".\Basic_Types.h"\
	".\Basic_Types.i"\
	".\config-win32-common.h"\
	".\config-win32.h"\
	".\config-WinCE.h"\
	".\config.h"\
	".\Containers.cpp"\
	".\Containers.h"\
	".\Containers.i"\
	".\Event_Handler.h"\
	".\Event_Handler.i"\
	".\Free_List.cpp"\
	".\Free_List.h"\
	".\Free_List.i"\
	".\Handle_Set.h"\
	".\Handle_Set.i"\
	".\inc_user_config.h"\
	".\INET_Addr.h"\
	".\INET_Addr.i"\
	".\iosfwd.h"\
	".\IPC_SAP.h"\
	".\IPC_SAP.i"\
	".\Local_Tokens.h"\
	".\Local_Tokens.i"\
	".\Log_Msg.h"\
	".\Log_Priority.h"\
	".\Log_Record.h"\
	".\Log_Record.i"\
	".\Malloc.h"\
	".\Malloc.i"\
	".\Malloc_T.cpp"\
	".\Malloc_T.h"\
	".\Malloc_T.i"\
	".\Managed_Object.cpp"\
	".\Managed_Object.h"\
	".\Managed_Object.i"\
	".\Map_Manager.cpp"\
	".\Map_Manager.h"\
	".\Map_Manager.i"\
	".\Mem_Map.h"\
	".\Mem_Map.i"\
	".\Memory_Pool.h"\
	".\Memory_Pool.i"\
	".\Object_Manager.h"\
	".\Object_Manager.i"\
	".\OS.h"\
	".\OS.i"\
	".\Pipe.h"\
	".\Pipe.i"\
	".\Reactor.h"\
	".\Reactor.i"\
	".\Reactor_Impl.h"\
	".\Select_Reactor.h"\
	".\Select_Reactor.i"\
	".\Service_Config.h"\
	".\Service_Config.i"\
	".\Service_Object.h"\
	".\Service_Object.i"\
	".\Shared_Object.h"\
	".\Shared_Object.i"\
	".\Signal.h"\
	".\Signal.i"\
	".\SOCK.h"\
	".\SOCK.i"\
	".\SOCK_Acceptor.h"\
	".\SOCK_Acceptor.i"\
	".\SOCK_Connector.h"\
	".\SOCK_Connector.i"\
	".\SOCK_IO.h"\
	".\SOCK_IO.i"\
	".\SOCK_Stream.h"\
	".\SOCK_Stream.i"\
	".\SString.h"\
	".\SString.i"\
	".\streams.h"\
	".\SV_Semaphore_Complex.h"\
	".\SV_Semaphore_Complex.i"\
	".\SV_Semaphore_Simple.h"\
	".\SV_Semaphore_Simple.i"\
	".\Svc_Conf_Tokens.h"\
	".\Synch.h"\
	".\Synch.i"\
	".\Synch_Options.h"\
	".\Synch_T.cpp"\
	".\Synch_T.h"\
	".\Synch_T.i"\
	".\sys_conf.h"\
	".\Thread.h"\
	".\Thread.i"\
	".\Time_Value.h"\
	".\Timer_Queue.h"\
	".\Timer_Queue_T.cpp"\
	".\Timer_Queue_T.h"\
	".\Timer_Queue_T.i"\
	".\Token.h"\
	".\Token.i"\
	".\Trace.h"\
	".\ws2tcpip.h"\
	".\XtReactor.h"\
	

!ENDIF 

# End Source File
# End Target
# End Project
