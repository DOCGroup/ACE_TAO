# Microsoft Developer Studio Project File - Name="RTKokyuEvent_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RTKokyuEvent_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTKokyuEvent_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTKokyuEvent_Static.mak" CFG="RTKokyuEvent_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTKokyuEvent_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RTKokyuEvent_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTKokyuEvent_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\RTKokyuEvent"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../" /I "../../" /I "../../../" /I "../../../Kokyu" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_RTKokyuEvents.lib"

!ELSEIF  "$(CFG)" == "RTKokyuEvent_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\RTKokyuEvent"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../" /I "../../" /I "../../../" /I "../../../Kokyu" /D "_LIB" /D "_MBCS" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_RTKokyuEventsd.lib"

!ENDIF 

# Begin Target

# Name "RTKokyuEvent_Static - Win32 Static Release"
# Name "RTKokyuEvent_Static - Win32 Static Debug"
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Scheduling.i
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Scheduling.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Kokyu_Scheduling.h
# End Source File
# End Group
# End Target
# End Project
