# Microsoft Developer Studio Project File - Name="ACE_QoS LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACE_QoS LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ACE_QoS_Lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ACE_QoS_Lib.mak" CFG="ACE_QoS LIB - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE_QoS LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE_QoS LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ACE_QoS LIB - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Static_Release"
# PROP BASE Intermediate_Dir "Static_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ACE_QoS_BUILD_DLL" /D "ACE_AS_STATIC_LIBS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"ACE_QoSs.lib"

!ELSEIF  "$(CFG)" == "ACE_QoS LIB - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_QoS LIB___Win32_Debug"
# PROP BASE Intermediate_Dir "ACE_QoS LIB___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "ACE_QoS_BUILD_DLL" /D "ACE_AS_STATIC_LIBS" /Fo"DLL\Debug/" /Fd"DLL\Debug/" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"ACE_QoSsd.lib"

!ENDIF 

# Begin Target

# Name "ACE_QoS LIB - Win32 Static Release"
# Name "ACE_QoS LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\QoS_Decorator.cpp
# End Source File
# Begin Source File

SOURCE=.\QoS_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast_QoS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\QoS_Decorator.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Manager.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Factory.h
# End Source File
# Begin Source File

SOURCE=.\QoS_Session_Impl.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast_QoS.h
# End Source File
# End Group
# End Target
# End Project
