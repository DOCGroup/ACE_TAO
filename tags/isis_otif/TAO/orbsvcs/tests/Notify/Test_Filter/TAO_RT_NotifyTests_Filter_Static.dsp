# Microsoft Developer Studio Project File - Name="TAO_RT_NotifyTests_Filter LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TAO_RT_NotifyTests_Filter LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "TAO_RT_NotifyTests_Filter_Static.mak" CFG="TAO_RT_NotifyTests_Filter LIB - Win32 Static Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "TAO_RT_NotifyTests_Filter LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_RT_NotifyTests_Filter LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_RT_NotifyTests_Filter LIB - Win32 Static Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /GX /O2 /MD /Zi /GR /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_NotifyTests_Filter.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\TAO_RT_NotifyTests_Filters.lib"

!ELSEIF  "$(CFG)" == "TAO_RT_NotifyTests_Filter LIB - Win32 Static Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /Gm /GX /Zi /Od /GR /Gy /MDd /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_NotifyTests_Filter.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\TAO_RT_NotifyTests_Filtersd.lib"

!ENDIF

# Begin Target

# Name "TAO_RT_NotifyTests_Filter LIB - Win32 Static Release"
# Name "TAO_RT_NotifyTests_Filter LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\RT_Test_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Test_FilterFactory.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Command_Factory_T.h
# End Source File
# Begin Source File

SOURCE=.\RT_Test_Filter.h
# End Source File
# Begin Source File

SOURCE=.\RT_Test_FilterFactory.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Activation_ManagerC.i
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerS.i
# End Source File
# Begin Source File

SOURCE=.\Task_Stats.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Command_Factory_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# End Target
# End Project
