# Microsoft Developer Studio Project File - Name="IFR_Client_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=IFR_Client_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IFR_Client_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_IFR_Client_Static.mak" CFG="IFR_Client_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IFR_Client_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "IFR_Client_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IFR_Client_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "LIB/Release/IFR_Client"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\IFR_Client"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_IFR_Clients.lib"

!ELSEIF  "$(CFG)" == "IFR_Client_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "LIB/Debug/IFR_Client"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\IFR_Client"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_IFR_Clientsd.lib"

!ENDIF 

# Begin Target

# Name "IFR_Client_Static - Win32 Static Release"
# Name "IFR_Client_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IFR_BaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IFR_BaseC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_Client_Adapter_Impl.h
# End Source File
# Begin Source File

SOURCE=.\ifr_client_export.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\IFR_BaseC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsC.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedC.i
# End Source File
# End Group
# End Target
# End Project
