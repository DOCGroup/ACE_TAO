# Microsoft Developer Studio Project File - Name="ACE_SSL_LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACE_SSL_LIB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ACE_SSL_LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ACE_SSL_LIB.mak" CFG="ACE_SSL_LIB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE_SSL_LIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE_SSL_LIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/StentorSoft/ACE_Wrappers/ace/SSL", WYQDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACE_SSL_LIB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"ACE_SSLs.lib"

!ELSEIF  "$(CFG)" == "ACE_SSL_LIB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"ACE_SSLsd.lib"

!ENDIF 

# Begin Target

# Name "ACE_SSL_LIB - Win32 Release"
# Name "ACE_SSL_LIB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SSL_Asynch_BIO.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Stream.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\SSL_Asynch_Stream.h
# End Source File
# Begin Source File

SOURCE=.\SSL_Context.h
# End Source File
# Begin Source File

SOURCE=.\SSL_Export.h
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK.h
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Stream.h
# End Source File
# Begin Source File

SOURCE=.\sslconf.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\SSL_Context.inl
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK.i
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Connector.i
# End Source File
# Begin Source File

SOURCE=.\SSL_SOCK_Stream.i
# End Source File
# End Group
# End Target
# End Project
