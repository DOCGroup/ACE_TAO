# Microsoft Developer Studio Project File - Name="Structured_Consumer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Structured_Consumer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Structured_Consumer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Structured_Consumer.mak" CFG="Structured_Consumer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Structured_Consumer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Structured_Consumer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Structured_Consumer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\lib" /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_NotifyTests.lib TAO_ETCL.lib TAO_CosNotification.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /subsystem:console /machine:I386 /out:"Release/Structured_Consumer.exe" /libpath:"..\lib" /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace"

!ELSEIF  "$(CFG)" == "Structured_Consumer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\lib" /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_NotifyTestsd.lib TAO_ETCLd.lib TAO_CosNotificationd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_PortableServerd.lib TAOd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"Structured_Consumer.exe" /pdbtype:sept /libpath:"..\lib" /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace"

!ENDIF 

# Begin Target

# Name "Structured_Consumer - Win32 Release"
# Name "Structured_Consumer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Structured_Consumer.cpp
# End Source File
# Begin Source File

SOURCE=.\goC.cpp
# End Source File
# Begin Source File

SOURCE=.\goS.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_Structured_Push_Consumer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\goC.h
# End Source File
# Begin Source File

SOURCE=.\goS.h
# End Source File
# Begin Source File

SOURCE=.\Notify_Structured_Push_Consumer.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\goC.i
# End Source File
# Begin Source File

SOURCE=.\goS.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\go.idl

!IF  "$(CFG)" == "Structured_Consumer - Win32 Release"

# Begin Custom Build
InputPath=.\go.idl
InputName=go

"$(InputName)C.h": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\..\bin\Release\tao_idl -Ge 1 $(InputName).idl

# End Custom Build

!ELSEIF  "$(CFG)" == "Structured_Consumer - Win32 Debug"

# Begin Custom Build
InputPath=.\go.idl
InputName=go

"$(InputName)C.h": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\..\bin\tao_idl -Ge 1 $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
