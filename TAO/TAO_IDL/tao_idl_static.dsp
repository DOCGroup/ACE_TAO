# Microsoft Developer Studio Project File - Name="TAO_IDL Compiler Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TAO_IDL Compiler Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl_static.mak" CFG="TAO_IDL Compiler Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL Compiler Static - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL Compiler Static - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL Compiler Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "LIB\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "be_include" /I "../../" /D "_CONSOLE" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_IDL_BE_LIBs.lib TAO_IDL_FE_LIBs.lib aces.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\Release\tao_idl_static.exe" /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "../../" /D "ACE_NO_INLINE" /D "_CONSOLE" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_IDL_FE_LIBsd.lib TAO_IDL_BE_LIBsd.lib acesd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\tao_idl_static.exe" /pdbtype:sept /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "TAO_IDL Compiler Static - Win32 Static Release"
# Name "TAO_IDL Compiler Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\driver\drv_args.cpp
# End Source File
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
# End Source File
# Begin Source File

SOURCE=.\tao_idl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\include\drv_extern.h
# End Source File
# End Group
# End Target
# End Project
