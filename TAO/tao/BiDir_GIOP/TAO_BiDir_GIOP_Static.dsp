# Microsoft Developer Studio Project File - Name="BiDirGIOP_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BiDirGIOP_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_BiDir_GIOP_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_BiDir_GIOP_Static.mak" CFG="BiDirGIOP_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BiDirGIOP_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BiDirGIOP_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BiDirGIOP_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\BiDirGIOP"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "NDEBUG" /D "WIN32" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_BiDirGIOPs.lib"

!ELSEIF  "$(CFG)" == "BiDirGIOP_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\BiDirGIOP"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_BiDirGIOPsd.lib"

!ENDIF 

# Begin Target

# Name "BiDirGIOP_Static - Win32 Static Release"
# Name "BiDirGIOP_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\BiDir_Policy_i.inl
# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\BiDir_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\BiDirGIOP.h
# End Source File
# Begin Source File

SOURCE=.\bidirgiop_export.h
# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\BiDir_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\BiDir_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\BiDir_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\BiDirGIOP.cpp
# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.cpp
# End Source File
# End Group
# End Target
# End Project
