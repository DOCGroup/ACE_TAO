# Microsoft Developer Studio Project File - Name="CC_client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CC_client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "CC_client.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "CC_client.mak" CFG="CC_client - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "CC_client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "CC_client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CC_client - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\CC_client"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_CosConcurrency.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:"Release\CC_client.pdb"  /machine:I386 /out:"Release\CC_client.exe"

!ELSEIF  "$(CFG)" == "CC_client - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\CC_client"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_CosConcurrencyd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:".\CC_client.pdb" /debug /machine:I386 /out:".\CC_client.exe"

!ENDIF

# Begin Target

# Name "CC_client - Win32 Release"
# Name "CC_client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\CC_client.cpp"
# End Source File
# Begin Source File

SOURCE=".\CC_command.cpp"
# End Source File
# Begin Source File

SOURCE=".\CC_command.tab.cpp"
# End Source File
# Begin Source File

SOURCE=".\CC_naming_service.cpp"
# End Source File
# Begin Source File

SOURCE=".\CC_test_utils.cpp"
# End Source File
# Begin Source File

SOURCE=".\CC_tests.cpp"
# End Source File
# Begin Source File

SOURCE=".\lex.CC_command.cpp"
# End Source File
# Begin Source File

SOURCE=".\tmplinst.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\CC_client.h"
# End Source File
# Begin Source File

SOURCE=".\CC_command.h"
# End Source File
# Begin Source File

SOURCE=".\CC_command.tab.h"
# End Source File
# Begin Source File

SOURCE=".\CC_naming_service.h"
# End Source File
# Begin Source File

SOURCE=".\CC_test_utils.h"
# End Source File
# Begin Source File

SOURCE=".\CC_tests.h"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# End Target
# End Project
