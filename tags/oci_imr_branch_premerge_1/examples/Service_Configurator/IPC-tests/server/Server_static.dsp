# Microsoft Developer Studio Project File - Name="Examples Service Configurator Server Library Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Examples Service Configurator Server Library Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Examples Service Configurator Server Library Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Examples Service Configurator Server Library Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Examples Service Configurator Server Library Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Examples Service Configurator Server Library Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release\Server"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /D "NDEBUG" /O1 /MD /I "../../../../" /I "" /D "ACE_SVC_HAS_DLL=0"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo /out:"Servers.lib" 

!ELSEIF  "$(CFG)" == "Examples Service Configurator Server Library Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\Server"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\Server"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi /D "_DEBUG" /Od /Gy /MDd /I "../../../../" /I "" /D "ACE_SVC_HAS_DLL=0"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo 
# ADD LIB32 /nologo /out:"Serversd.lib" 

!ENDIF

# Begin Target

# Name "Examples Service Configurator Server Library Static - Win32 Static Release"
# Name "Examples Service Configurator Server Library Static - Win32 Static Debug"
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
