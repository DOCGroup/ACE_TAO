# Microsoft Developer Studio Project File - Name="ImR_Activator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ImR_Activator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
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

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\ImR_Activator"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\ACEXML\common" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\ACEXML\common" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_Strategies.lib TAO_IORTable.lib TAO_Svc_Utils.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib ACEXML_Parser.lib ACEXML.lib TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:console /pdb:"Release\ImR_Activator.pdb"  /machine:I386 /out:"Release\ImR_Activator.exe"

!ELSEIF  "$(CFG)" == "ImR_Activator - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\ImR_Activator"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\ACEXML\common" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\ACEXML\common" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_Strategiesd.lib TAO_IORTabled.lib TAO_Svc_Utilsd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib ACEXML_Parserd.lib ACEXMLd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.1 /subsystem:console /pdb:".\ImR_Activator.pdb" /debug /machine:I386 /out:".\ImR_Activator.exe"

!ENDIF

# Begin Target

# Name "ImR_Activator - Win32 Release"
# Name "ImR_Activator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\ImR_Activator.cpp"
# End Source File
# Begin Source File

SOURCE=".\ImR_Activator_i.cpp"
# End Source File
# Begin Source File

SOURCE=".\Iterator.cpp"
# End Source File
# Begin Source File

SOURCE=".\Locator.cpp"
# End Source File
# Begin Source File

SOURCE=".\NT_Service.cpp"
# End Source File
# Begin Source File

SOURCE=".\Options.cpp"
# End Source File
# Begin Source File

SOURCE=".\Repository_Configuration.cpp"
# End Source File
# Begin Source File

SOURCE=".\Server_Info.cpp"
# End Source File
# Begin Source File

SOURCE=".\Server_Repository.cpp"
# End Source File
# Begin Source File

SOURCE=".\XML_ContentHandler.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\ImR_Activator_i.h"
# End Source File
# Begin Source File

SOURCE=".\Iterator.h"
# End Source File
# Begin Source File

SOURCE=".\Locator.h"
# End Source File
# Begin Source File

SOURCE=".\NT_Service.h"
# End Source File
# Begin Source File

SOURCE=".\Options.h"
# End Source File
# Begin Source File

SOURCE=".\Repository_Configuration.h"
# End Source File
# Begin Source File

SOURCE=".\Server_Info.h"
# End Source File
# Begin Source File

SOURCE=".\Server_Repository.h"
# End Source File
# Begin Source File

SOURCE=".\XML_ContentHandler.h"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README.txt"
# End Source File
# End Group
# End Target
# End Project
