# Microsoft Developer Studio Project File - Name="RMCast" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RMCast - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
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
RSC=rc.exe

!IF  "$(CFG)" == "RMCast - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Release\RMCast"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_RMCAST_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\ACE_RMCast.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib /libpath:"..\..\lib" /nologo /version:5.3.3 /subsystem:windows /pdb:"..\..\lib\ACE_RMCast.pdb" /dll  /machine:I386 /out:"..\..\lib\ACE_RMCast.dll"

!ELSEIF  "$(CFG)" == "RMCast - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib"
# PROP Intermediate_Dir "Debug\RMCast"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_RMCAST_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\lib\ACE_RMCast.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib /libpath:"..\..\lib" /nologo /version:5.3.3 /subsystem:windows /pdb:"..\..\lib\ACE_RMCastd.pdb" /dll /debug /machine:I386 /out:"..\..\lib\ACE_RMCastd.dll"

!ENDIF

# Begin Target

# Name "RMCast - Win32 Release"
# Name "RMCast - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\RMCast.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Ack_Worker.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Copy_On_Write.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fork.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fragment.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_IO_UDP.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Membership.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module_Factory.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Partial_Message.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Proxy.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reassembly.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Receiver_Module.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reliable_Factory.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reordering.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Handler.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Worker.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Retransmission.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Sequencer.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Singleton_Factory.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Event_Handler.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Proxy.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Receiver.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Sender.cpp"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Worker.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\RMCast.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Ack_Worker.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Copy_On_Write.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Export.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fork.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fragment.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_IO_UDP.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Membership.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module_Factory.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Partial_Message.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Proxy.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reassembly.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Receiver_Module.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reliable_Factory.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reordering.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Handler.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Worker.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Retransmission.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Sequencer.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Singleton_Factory.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Event_Handler.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Proxy.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Receiver.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Sender.h"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Worker.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\RMCast.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Ack_Worker.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Copy_On_Write.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fork.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Fragment.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_IO_UDP.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Membership.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Module_Factory.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Partial_Message.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Proxy.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reassembly.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Receiver_Module.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reliable_Factory.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Reordering.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Handler.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Resend_Worker.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Retransmission.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Sequencer.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Singleton_Factory.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Event_Handler.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Proxy.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Receiver.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_UDP_Reliable_Sender.i"
# End Source File
# Begin Source File

SOURCE=".\RMCast_Worker.i"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# End Target
# End Project
