# Microsoft Developer Studio Project File - Name="context_switch_time" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=context_switch_time - Win32 static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "context_switch_time.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "context_switch_time.mak" CFG="context_switch_time - Win32 static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "context_switch_time - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "context_switch_time - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "context_switch_time - Win32 static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "context_switch_time - Win32 static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "context_switch_time - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\context_switch_time\Release"
# PROP BASE Intermediate_Dir ".\context_switch_time\Release"
# PROP BASE Target_Dir ".\context_switch_time"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\context_switch_time"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "context_switch_time - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\context_switch_time\Debug"
# PROP BASE Intermediate_Dir ".\context_switch_time\Debug"
# PROP BASE Target_Dir ".\context_switch_time"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\context_switch_time"
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "context_switch_time - Win32 static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "context_switch_time\static Debug"
# PROP BASE Intermediate_Dir "context_switch_time\static Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "context_switch_time"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "static_Debug"
# PROP Intermediate_Dir "static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "context_switch_time"
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\ace"
# ADD LINK32 acesd.lib advapi32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "context_switch_time - Win32 static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "context_switch_time\static Release"
# PROP BASE Intermediate_Dir "context_switch_time\static Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "context_switch_time"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "static_Release"
# PROP Intermediate_Dir "static_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "context_switch_time"
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D ACE_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"
# ADD LINK32 aces.lib advapi32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "context_switch_time - Win32 Release"
# Name "context_switch_time - Win32 Debug"
# Name "context_switch_time - Win32 static Debug"
# Name "context_switch_time - Win32 static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\context_switch_time.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
