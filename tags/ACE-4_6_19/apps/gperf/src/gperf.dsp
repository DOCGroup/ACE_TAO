# Microsoft Developer Studio Project File - Name="gperf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=gperf - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gperf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gperf.mak" CFG="gperf - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gperf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "gperf - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE "gperf - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "gperf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 gperf.lib ace.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gperfd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gperf__0"
# PROP BASE Intermediate_Dir "gperf__0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 gperfd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT BASE LINK32 /incremental:no /nodefaultlib
# ADD LINK32 gperfd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /out:"..\..\..\bin\gperf.exe" /pdbtype:sept /libpath:"..\..\..\ace" /libpath:".\lib\\"
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ELSEIF  "$(CFG)" == "gperf - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gperf__1"
# PROP BASE Intermediate_Dir "gperf__1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 gperf.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 gperf.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /out:"..\..\..\bin\Release\gperf.exe" /libpath:"..\..\..\ace" /libpath:".\lib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "gperf - Win32 Release"
# Name "gperf - Win32 Debug"
# Name "gperf - Win32 Alpha Debug"
# Name "gperf - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\gperf.cpp

!IF  "$(CFG)" == "gperf - Win32 Release"

!ELSEIF  "$(CFG)" == "gperf - Win32 Debug"

!ELSEIF  "$(CFG)" == "gperf - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "gperf - Win32 Alpha Release"

DEP_CPP_GPERF=\
	"..\..\..\ace\ACE.h"\
	"..\..\..\ace\ACE.i"\
	"..\..\..\ace\Auto_Ptr.cpp"\
	"..\..\..\ace\Auto_Ptr.h"\
	"..\..\..\ace\Auto_Ptr.i"\
	"..\..\..\ace\Basic_Types.h"\
	"..\..\..\ace\Basic_Types.i"\
	"..\..\..\ace\config-win32-borland.h"\
	"..\..\..\ace\config-win32-common.h"\
	"..\..\..\ace\config-win32.h"\
	"..\..\..\ace\config-WinCE.h"\
	"..\..\..\ace\config.h"\
	"..\..\..\ace\inc_user_config.h"\
	"..\..\..\ace\iosfwd.h"\
	"..\..\..\ace\Log_Msg.h"\
	"..\..\..\ace\Log_Priority.h"\
	"..\..\..\ace\Log_Record.h"\
	"..\..\..\ace\Log_Record.i"\
	"..\..\..\ace\Malloc_Base.h"\
	"..\..\..\ace\Managed_Object.cpp"\
	"..\..\..\ace\Managed_Object.h"\
	"..\..\..\ace\Managed_Object.i"\
	"..\..\..\ace\Object_Manager.h"\
	"..\..\..\ace\Object_Manager.i"\
	"..\..\..\ace\OS.h"\
	"..\..\..\ace\OS.i"\
	"..\..\..\ace\SString.h"\
	"..\..\..\ace\SString.i"\
	"..\..\..\ace\streams.h"\
	"..\..\..\ace\Trace.h"\
	"..\..\..\ace\ws2tcpip.h"\
	".\Bool_Array.h"\
	".\Gen_Perf.h"\
	".\Key_List.h"\
	".\List_Node.h"\
	".\Options.h"\
	".\Vectors.h"\
	

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
