# Microsoft Developer Studio Project File - Name="Lanes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Lanes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Lanes.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Lanes.mak" CFG="Lanes - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Lanes - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Lanes - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Lanes - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\Lanes"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /GR /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO /nologo /version:1.0 /subsystem:console /pdb:"Release\Consumer_Client.pdb"  /machine:I386 /out:"Release\Consumer_Client.exe"

!ELSEIF  "$(CFG)" == "Lanes - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\Lanes"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO /nologo /version:1.0 /subsystem:console /pdb:".\Consumer_Client.pdb" /debug /machine:I386 /out:".\Consumer_Client.exe"

!ENDIF

# Begin Target

# Name "Lanes - Win32 Release"
# Name "Lanes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Consumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Consumer_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Supplier_Client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Consumer.h
# End Source File
# Begin Source File

SOURCE=.\Consumer_Client.h
# End Source File
# Begin Source File

SOURCE=.\Supplier.h
# End Source File
# Begin Source File

SOURCE=.\Supplier_Client.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Consumer.inl
# End Source File
# Begin Source File

SOURCE=.\Consumer_Client.inl
# End Source File
# Begin Source File

SOURCE=.\Supplier.inl
# End Source File
# Begin Source File

SOURCE=.\Supplier_Client.inl
# End Source File
# End Group
# End Target
# End Project
