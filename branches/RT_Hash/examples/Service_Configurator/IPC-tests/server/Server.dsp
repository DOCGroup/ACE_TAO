# Microsoft Developer Studio Project File - Name="Examples Service Configurator Server Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Examples Service Configurator Server Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Examples Service Configurator Server Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Examples Service Configurator Server Library - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Examples Service Configurator Server Library - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Examples Service Configurator Server Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Examples Service Configurator Server Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Examples Service Configurator Server Library - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release\MFC\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release\MFC\Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "ACE_SVC_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386
# ADD LINK32 ../../../../ace/ACEmfc.lib /nologo /dll /machine:I386 /out:"../../../../bin/Servermfc.dll"

!ELSEIF  "$(CFG)" == "Examples Service Configurator Server Library - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\MFC\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\MFC\Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../../" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /D "ACE_SVC_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../../ace/ACEmfcd.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/Servermfcd.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Examples Service Configurator Server Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release\Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "ACE_SVC_BUILD_DLL" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386
# ADD LINK32 ../../../../ace/ACE.lib advapi32.lib /nologo /dll /machine:I386 /out:"../../../../bin/Server.dll"

!ELSEIF  "$(CFG)" == "Examples Service Configurator Server Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../../" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /D "ACE_SVC_BUILD_DLL" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../../ace/ACEd.lib /nologo /dll /debug /machine:I386 /out:"../../../../bin/Serverd.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Examples Service Configurator Server Library - Win32 MFC Release"
# Name "Examples Service Configurator Server Library - Win32 MFC Debug"
# Name "Examples Service Configurator Server Library - Win32 Release"
# Name "Examples Service Configurator Server Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Handle_Broadcast.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_CODgram.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Dgram.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_FIFO.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Pipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_SPIPE.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Dgram.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Thr_Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle_Timeout.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;fi;fd"
# Begin Source File

SOURCE=.\Handle_Broadcast.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_CODgram.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_FIFO.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Pipe.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_SPIPE.h
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Thr_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Timeout.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl;pidl"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl;i"
# Begin Source File

SOURCE=.\Handle_Broadcast.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_CODgram.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_FIFO.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Pipe.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_SPIPE.i
# End Source File
# Begin Source File

SOURCE=.\Handle_L_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\Handle_R_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Thr_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Timeout.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
