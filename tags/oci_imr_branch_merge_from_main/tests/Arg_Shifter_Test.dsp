# Microsoft Developer Studio Project File - Name="Arg_Shifter_Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Arg_Shifter_Test - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Arg_Shifter_Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Arg_Shifter_Test.mak" CFG="Arg_Shifter_Test - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Arg_Shifter_Test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Arg_Shifter_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Arg_Shifter_Test - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Arg_Shifter_Test - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Arg_Shifter_Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Arg_Shifter_Test___Win32_Release"
# PROP BASE Intermediate_Dir "Arg_Shifter_Test___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib Test_Output.lib /nologo /subsystem:console /machine:I386 /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Arg_Shifter_Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Arg_Shifter_Test___Win32_Debug"
# PROP BASE Intermediate_Dir "Arg_Shifter_Test___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib Test_Outputd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Arg_Shifter_Test - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Arg_Shifter_Test___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "Arg_Shifter_Test___Win32_Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"
# ADD LINK32 user32.lib acesd.lib advapi32.lib Test_Outputsd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\ace"

!ELSEIF  "$(CFG)" == "Arg_Shifter_Test - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Arg_Shifter_Test___Win32_Static_Release"
# PROP BASE Intermediate_Dir "Arg_Shifter_Test___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\ace"
# ADD LINK32 user32.lib aces.lib advapi32.lib Test_Outputs.lib /nologo /subsystem:console /machine:I386 /libpath:"..\ace"

!ENDIF 

# Begin Target

# Name "Arg_Shifter_Test - Win32 Release"
# Name "Arg_Shifter_Test - Win32 Debug"
# Name "Arg_Shifter_Test - Win32 Static Debug"
# Name "Arg_Shifter_Test - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Arg_Shifter_Test.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
