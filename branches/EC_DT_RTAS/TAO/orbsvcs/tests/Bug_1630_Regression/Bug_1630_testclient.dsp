# Microsoft Developer Studio Project File - Name="Bug_1630_testclient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Bug_1630_testclient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1630_testclient.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1630_testclient.mak" CFG="Bug_1630_testclient - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Bug_1630_testclient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Bug_1630_testclient - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bug_1630_testclient - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Bug_1630_testclient"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Client.lib TAO.lib ACE.lib /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\..\lib" /nologo /version:1.3.5 /subsystem:console /pdb:"Release\testclient.pdb"  /machine:I386 /out:"Release\testclient.exe"

!ELSEIF  "$(CFG)" == "Bug_1630_testclient - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Bug_1630_testclient"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Clientd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\..\lib" /nologo /version:1.3.5 /subsystem:console /pdb:".\testclient.pdb" /debug /machine:I386 /out:".\testclient.exe"

!ENDIF

# Begin Target

# Name "Bug_1630_testclient - Win32 Release"
# Name "Bug_1630_testclient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\testclient.cpp"
# End Source File
# End Group
# End Target
# End Project
