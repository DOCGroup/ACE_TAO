# Microsoft Developer Studio Project File - Name="TAO_DIOP_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO_DIOP_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DIOP_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DIOP_Static.mak" CFG="TAO_DIOP_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_DIOP_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_DIOP_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_DIOP_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\DIOP"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../tao/" /I "../../../" /I "../../../../" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /D "TAO_STRATEGIES_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_DIOPs.lib"

!ELSEIF  "$(CFG)" == "TAO_DIOP_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_DIOP_Static___Win32_Debug"
# PROP BASE Intermediate_Dir "TAO_DIOP_Static___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\DIOP"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "../../../tao/" /I "../../../" /I "../../../../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_DIOPsd.lib"

!ENDIF 

# Begin Target

# Name "TAO_DIOP_Static - Win32 Static Release"
# Name "TAO_DIOP_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DIOP.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DIOP.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\DIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
