# Microsoft Developer Studio Project File - Name="Kokyu LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Kokyu LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Kokyu_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Kokyu_Static.mak" CFG="Kokyu LIB - Win32 Static Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Kokyu LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Kokyu LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Kokyu LIB - Win32 Static Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /GX /O2 /MD /Zi /GR /I ".." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Kokyu.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\Kokyus.lib"

!ELSEIF  "$(CFG)" == "Kokyu LIB - Win32 Static Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /Gm /GX /Zi /Od /GR /Gy /MDd /I ".." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i ".."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\Kokyu.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\Kokyusd.lib"

!ENDIF

# Begin Target

# Name "Kokyu LIB - Win32 Static Release"
# Name "Kokyu LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Default_Dispatcher_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Kokyu.cpp
# End Source File
# Begin Source File

SOURCE=.\Kokyu_defs.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Default_Dispatcher_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Task.h
# End Source File
# Begin Source File

SOURCE=.\DSRT_Direct_Dispatcher_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatch_Item_T.h
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatcher_Impl_T.h
# End Source File
# Begin Source File

SOURCE=.\DSRT_Sched_Queue_T.h
# End Source File
# Begin Source File

SOURCE=.\Kokyu.h
# End Source File
# Begin Source File

SOURCE=.\Kokyu_defs.h
# End Source File
# Begin Source File

SOURCE=.\Kokyu_dsrt.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Default_Dispatcher_Impl.i
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Impl.i
# End Source File
# Begin Source File

SOURCE=.\Dispatcher_Task.i
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatch_Item_T.i
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatcher_Impl_T.i
# End Source File
# Begin Source File

SOURCE=.\Kokyu.i
# End Source File
# Begin Source File

SOURCE=.\Kokyu_defs.i
# End Source File
# Begin Source File

SOURCE=.\Kokyu_dsrt.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DSRT_Direct_Dispatcher_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatch_Item_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DSRT_Dispatcher_Impl_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DSRT_Sched_Queue_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Kokyu_dsrt.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
