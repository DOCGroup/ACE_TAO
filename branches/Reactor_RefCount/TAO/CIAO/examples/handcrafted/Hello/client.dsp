# Microsoft Developer Studio Project File - Name="client EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=client EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="client EXE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "client EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "client EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "client EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\.." /I "..\..\..\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\.." /i "..\..\..\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO.lib TAO_IFR_Client.lib CIAO_Client.lib ACE.lib  hello_stubd.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "client EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\.." /I "..\..\..\ciao" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\.." /i "..\..\..\ciao" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAOd.lib TAO_IFR_Clientd.lib CIAO_Clientd.lib ACEd.lib hello_stubd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /libpath:"..\..\..\..\tao\IFR_Client" /libpath:"..\..\..\ciao" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /version:1.3.1
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "client EXE - Win32 Release"
# Name "client EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\hello_executors.h
# End Source File
# Begin Source File

SOURCE=.\hello_executors_export.h
# End Source File
# Begin Source File

SOURCE=.\hello_servants.h
# End Source File
# Begin Source File

SOURCE=.\hello_servants_export.h
# End Source File
# Begin Source File

SOURCE=.\hello_stub_export.h
# End Source File
# Begin Source File

SOURCE=.\helloC.h
# End Source File
# Begin Source File

SOURCE=.\helloEC.h
# End Source File
# Begin Source File

SOURCE=.\helloES.h
# End Source File
# Begin Source File

SOURCE=.\helloS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\helloC.i
# End Source File
# Begin Source File

SOURCE=.\helloEC.i
# End Source File
# Begin Source File

SOURCE=.\helloES.i
# End Source File
# Begin Source File

SOURCE=.\helloS.i
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
