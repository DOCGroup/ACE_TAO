# Microsoft Developer Studio Project File - Name="DynamicAny_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DynamicAny_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DynamicAny_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DynamicAny_Static.mak" CFG="DynamicAny_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DynamicAny_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DynamicAny_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DynamicAny_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\DynamicAny"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
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
# ADD LIB32 /nologo /out:"TAO_DynamicAnys.lib"

!ELSEIF  "$(CFG)" == "DynamicAny_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\DynamicAny"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
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
# ADD LIB32 /nologo /out:"TAO_DynamicAnysd.lib"

!ENDIF 

# Begin Target

# Name "DynamicAny_Static - Win32 Static Release"
# Name "DynamicAny_Static - Win32 Static Debug"
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\DynamicAnyC.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DynamicAny.h
# End Source File
# Begin Source File

SOURCE=.\dynamicany_export.h
# End Source File
# Begin Source File

SOURCE=.\DynamicAnyC.h
# End Source File
# Begin Source File

SOURCE=.\DynAny_i.h
# End Source File
# Begin Source File

SOURCE=.\DynAnyFactory.h
# End Source File
# Begin Source File

SOURCE=.\DynArray_i.h
# End Source File
# Begin Source File

SOURCE=.\DynCommon.h
# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.h
# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.h
# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.h
# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DynamicAny.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicAnyC.cpp
# End Source File
# Begin Source File

SOURCE=.\DynAny_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynAnyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DynArray_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.cpp
# End Source File
# End Group
# End Target
# End Project
