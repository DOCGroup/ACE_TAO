# Microsoft Developer Studio Project File - Name="RMCast" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RMCast - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RMCast.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RMCast.mak" CFG="RMCast - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RMCast - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RMCast - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RMCast - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMCAST_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACE_RMCAST_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib /nologo /dll /machine:I386 /out:"..\..\bin\ACE_RMCast.dll" /libpath:".."

!ELSEIF  "$(CFG)" == "RMCast - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMCAST_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACE_RMCAST_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\bin\ACE_RMCastd.dll" /pdbtype:sept /libpath:".."

!ENDIF 

# Begin Target

# Name "RMCast - Win32 Release"
# Name "RMCast - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RMCast.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Ack_Worker.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fork.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fragment.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_IO_UDP.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Membership.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Partial_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reassembly.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Receiver_Module.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reliable_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reordering.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Worker.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Retransmission.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Sequencer.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_Singleton_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Event_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Sender.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RMCast.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Ack_Worker.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Export.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fork.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fragment.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_IO_UDP.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Membership.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Partial_Message.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reassembly.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Receiver_Module.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reliable_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reordering.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Handler.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Worker.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Retransmission.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Sequencer.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_Singleton_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Event_Handler.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Receiver.h
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Sender.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\RMCast.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Ack_Worker.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fork.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Fragment.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_IO_UDP.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Membership.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Module_Factory.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Partial_Message.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Proxy.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reassembly.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Receiver_Module.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reliable_Factory.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Reordering.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Handler.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Resend_Worker.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Retransmission.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Sequencer.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_Singleton_Factory.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Event_Handler.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Proxy.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Receiver.i
# End Source File
# Begin Source File

SOURCE=.\RMCast_UDP_Reliable_Sender.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
