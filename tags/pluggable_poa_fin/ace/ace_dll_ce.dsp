# Microsoft Developer Studio Project File - Name="ace_dll_ce" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE x86em) Dynamic-Link Library" 0x7f02
# TARGTYPE "Win32 (WCE SH3) Dynamic-Link Library" 0x8102
# TARGTYPE "Win32 (WCE MIPS) Dynamic-Link Library" 0x8202

CFG=ace_dll_ce - Win32 (WCE SH3) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll_ce.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_dll_ce.mak" CFG="ace_dll_ce - Win32 (WCE SH3) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ace_dll_ce - Win32 (WCE SH3) Release" (based on "Win32 (WCE SH3) Dynamic-Link Library")
!MESSAGE "ace_dll_ce - Win32 (WCE SH3) Debug" (based on "Win32 (WCE SH3) Dynamic-Link Library")
!MESSAGE "ace_dll_ce - Win32 (WCE MIPS) Release" (based on "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "ace_dll_ce - Win32 (WCE MIPS) Debug" (based on "Win32 (WCE MIPS) Dynamic-Link Library")
!MESSAGE "ace_dll_ce - Win32 (WCE x86em) Release" (based on "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE "ace_dll_ce - Win32 (WCE x86em) Debug" (based on "Win32 (WCE x86em) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "H/PC Ver. 2.00"
# PROP WCE_FormatVersion "6.0"

!IF  "$(CFG)" == "ace_dll_ce - Win32 (WCE SH3) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESH3Rel"
# PROP BASE Intermediate_Dir "WCESH3Rel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DLL\Release\SH3"
# PROP Intermediate_Dir "DLL\Release\SH3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
PFILE=pfile.exe
CPP=shcl.exe
# ADD BASE CPP /nologo /M$(CECrt) /W3 /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "ACE_DLL_CE_EXPORTS" /YX /c
# ADD CPP /nologo /M$(CECrtMT) /W3 /O2 /I "../" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "NDEBUG"
# ADD RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "NDEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /nodefaultlib:"$(CENoDefaultLib)" /subsystem:$(CESubsystem) /STACK:65536,4096
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:SH3 /nodefaultlib:"$(CENoDefaultLib)" /out:"DLL\Release\SH3\ace.dll" /subsystem:$(CESubsystem) /STACK:65536,4096
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "ace_dll_ce - Win32 (WCE SH3) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESH3Dbg"
# PROP BASE Intermediate_Dir "WCESH3Dbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL\Debug\SH3"
# PROP Intermediate_Dir "DLL\Debug\SH3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
PFILE=pfile.exe
CPP=shcl.exe
# ADD BASE CPP /nologo /M$(CECrtDebug) /W3 /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "ACE_DLL_CE_EXPORTS" /YX /c
# ADD CPP /nologo /M$(CECrtMTDebug) /W3 /Zi /Od /I "../" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x409 /r /d "SHx" /d "SH3" /d "_SH3_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 commctrl.lib coredll.lib /nologo /dll /debug /machine:SH3 /nodefaultlib:"$(CENoDefaultLib)" /subsystem:$(CESubsystem) /STACK:65536,4096
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib winsock.lib corelibc.lib /nologo /dll /debug /machine:SH3 /nodefaultlib:"$(CENoDefaultLib)" /out:"DLL\Debug\SH3\aced.dll" /subsystem:$(CESubsystem) /STACK:65536,4096
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "ace_dll_ce - Win32 (WCE MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ace_dll_ce___Win32__WCE_MIPS__Release"
# PROP BASE Intermediate_Dir "ace_dll_ce___Win32__WCE_MIPS__Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DLL\Release\MIPS"
# PROP Intermediate_Dir "DLL\Release\MIPS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /M$(CECrt) /W3 /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /YX /QMRWCE /c
# ADD CPP /nologo /M$(CECrt) /W3 /O2 /I "../" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /QMRWCE /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "NDEBUG"
# ADD RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "NDEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(CEx86Corelibc) commctrl.lib coredll.lib /nologo /dll /machine:MIPS /nodefaultlib:"$(CENoDefaultLib)" /windowsce:emulation
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 commctrl.lib coredll.lib /nologo /dll /machine:MIPS /nodefaultlib:"$(CENoDefaultLib)" /out:"DLL\Release\MIPS\ace.dll" /subsystem:$(CESubsystem)
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ace_dll_ce - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ace_dll_ce___Win32__WCE_MIPS__Debug"
# PROP BASE Intermediate_Dir "ace_dll_ce___Win32__WCE_MIPS__Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL\Debug\MIPS"
# PROP Intermediate_Dir "DLL\Debug\MIPS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /M$(CECrtDebug) /W3 /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /YX /QMRWCE /c
# ADD CPP /nologo /M$(CECrtMTDebug) /W3 /Zi /Od /I "../" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "ACE_BUILD_DLL" /QMRWCE /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
# ADD RSC /l 0x409 /r /d "MIPS" /d "_MIPS_" /d UNDER_CE=$(CEVersion) /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "UNICODE" /d "DEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(CEx86Corelibc) winsock.lib commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /nodefaultlib:"$(CENoDefaultLib)" /windowsce:emulation
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 winsock.lib commctrl.lib coredll.lib /nologo /dll /debug /machine:MIPS /nodefaultlib:"$(CENoDefaultLib)" /out:"DLL\Debug\MIPS\aced.dll" /subsystem:$(CESubsystem)
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ace_dll_ce - Win32 (WCE x86em) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x86emRel"
# PROP BASE Intermediate_Dir "x86emRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DLL\Release\x86\"
# PROP Intermediate_Dir "DLL\Release\x86\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
EMPFILE=empfile.exe
CPP=cl.exe
# ADD BASE CPP /nologo /ML /W3 /O2 /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D "_MBCS" /D "_USRDLL" /D "ACE_DLL_CE_EXPORTS" /YX /c
# ADD CPP /nologo /W3 /O2 /I "../" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D "ACE_BUILD_DLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "NDEBUG"
# ADD RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "NDEBUG"
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(CEx86Corelibc) commctrl.lib coredll.lib /nologo /stack:0x10000,0x1000 /dll /machine:I386 /nodefaultlib:"$(CENoDefaultLib)" /windowsce:emulation
# ADD LINK32 $(CEx86Corelibc) commctrl.lib coredll.lib winsock.lib /nologo /stack:0x10000,0x1000 /dll /machine:I386 /nodefaultlib:"$(CENoDefaultLib)" /nodefaultlib /out:"DLL\Release\x86\ace.dll" /windowsce:emulation

!ELSEIF  "$(CFG)" == "ace_dll_ce - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DLL\Debug\x86"
# PROP Intermediate_Dir "DLL\Debug\x86"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
EMPFILE=empfile.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MLd /W3 /Gm /Zi /Od /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D "_MBCS" /D "_USRDLL" /D "ACE_DLL_CE_EXPORTS" /YX /c
# ADD CPP /nologo /W3 /Gm /Zi /Od /I "../" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D "ACE_BUILD_DLL" /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_"
# ADD RSC /l 0x409 /d UNDER_CE=$(CEVersion) /d "UNICODE" /d "_UNICODE" /d "WIN32" /d "STRICT" /d _WIN32_WCE=$(CEVersion) /d "$(CEConfigName)" /d "_WIN32_WCE_EMULATION" /d "INTERNATIONAL" /d "USA" /d "INTLMSG_CODEPAGE" /d "_DEBUG" /d "x86" /d "i486" /d "_x86_"
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $(CEx86Corelibc) commctrl.lib coredll.lib /nologo /stack:0x10000,0x1000 /dll /debug /machine:I386 /nodefaultlib:"$(CENoDefaultLib)" /windowsce:emulation
# ADD LINK32 $(CEx86Corelibc) commctrl.lib coredll.lib winsock.lib /nologo /stack:0x10000,0x1000 /dll /debug /machine:I386 /nodefaultlib:"$(CENoDefaultLib)" /nodefaultlib /out:"DLL\Debug\x86\aced.dll" /windowsce:emulation

!ENDIF 

# Begin Target

# Name "ace_dll_ce - Win32 (WCE SH3) Release"
# Name "ace_dll_ce - Win32 (WCE SH3) Debug"
# Name "ace_dll_ce - Win32 (WCE MIPS) Release"
# Name "ace_dll_ce - Win32 (WCE MIPS) Debug"
# Name "ace_dll_ce - Win32 (WCE x86em) Release"
# Name "ace_dll_ce - Win32 (WCE x86em) Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\ARGV.cpp


# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp


# End Source File
# Begin Source File

SOURCE=.\Based_Pointer_Repository.cpp


# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp


# End Source File
# Begin Source File

SOURCE=.\Containers.cpp


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

SOURCE=.\Event_Handler.cpp


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

SOURCE=.\IPC_SAP.cpp


# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp


# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp


# End Source File
# Begin Source File

SOURCE=.\Malloc.cpp


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

SOURCE=.\Object_Manager.cpp


# End Source File
# Begin Source File

SOURCE=.\Obstack.cpp


# End Source File
# Begin Source File

SOURCE=.\OS.cpp


# End Source File
# Begin Source File

SOURCE=.\OS_Dirent.cpp


# End Source File
# Begin Source File

SOURCE=.\OS_Memory.cpp


# End Source File
# Begin Source File

SOURCE=.\OS_String.cpp


# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp


# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp


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

SOURCE=.\SString.cpp


# End Source File
# Begin Source File

SOURCE=.\Stats.cpp


# End Source File
# Begin Source File

SOURCE=.\Strategies.cpp


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

SOURCE=.\System_Time.cpp


# End Source File
# Begin Source File

SOURCE=.\Task.cpp


# End Source File
# Begin Source File

SOURCE=.\Thread.cpp


# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.cpp


# End Source File
# Begin Source File

SOURCE=.\Token.cpp


# End Source File
# Begin Source File

SOURCE=.\TP_Reactor.cpp


# End Source File
# Begin Source File

SOURCE=.\Trace.cpp


# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp


# End Source File
# Begin Source File

SOURCE=.\XtReactor.cpp


# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
