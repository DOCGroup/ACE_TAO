# Microsoft Developer Studio Project File - Name="ImR_Activator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ImR_Activator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImR_Activator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImR_Activator.mak" CFG="ImR_Activator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImR_Activator - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ImR_Activator - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImR_Activator - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_Svc_Utils.lib TAO_IORTable.lib TAO_PortableServer.lib TAO_Strategies.lib TAO.lib ACEXML_Parser.lib ACEXML.lib ace.lib user32.lib advapi32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ACEXML\parser\parser" /libpath:"..\..\..\ACEXML\common" /libpath:"..\..\..\ace" /libpath:"..\..\..\TAO\Strategies"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ImR_Activator - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_PortableServerd.lib TAO_Strategiesd.lib TAOd.lib ACEXML_Parserd.lib ACEXMLd.lib aced.lib user32.lib advapi32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ACEXML\parser\parser" /libpath:"..\..\..\ACEXML\common" /libpath:"..\..\..\ace" /libpath:"..\..\tao\Strategies"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ImR_Activator - Win32 Release"
# Name "ImR_Activator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ImR_Activator.cpp
# End Source File
# Begin Source File

SOURCE=.\ImR_Activator_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Iterator.cpp
# End Source File
# Begin Source File

SOURCE=.\Locator.cpp
# End Source File
# Begin Source File

SOURCE=.\NT_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Repository_Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Repository.cpp
# End Source File
# Begin Source File

SOURCE=.\XML_ContentHandler.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ImR_Activator_i.h
# End Source File
# Begin Source File

SOURCE=.\Iterator.h
# End Source File
# Begin Source File

SOURCE=.\Locator.h
# End Source File
# Begin Source File

SOURCE=.\NT_Service.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\Repository_Configuration.h
# End Source File
# Begin Source File

SOURCE=.\Server_Info.h
# End Source File
# Begin Source File

SOURCE=.\Server_Repository.h
# End Source File
# Begin Source File

SOURCE=.\XML_ContentHandler.h
# End Source File
# End Group
# End Target
# End Project
