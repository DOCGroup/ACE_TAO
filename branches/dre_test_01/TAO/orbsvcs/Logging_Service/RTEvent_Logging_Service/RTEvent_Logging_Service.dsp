# Microsoft Developer Studio Project File - Name="RTEvent_Logging_Service" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RTEvent_Logging_Service - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTEvent_Logging_Service.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTEvent_Logging_Service.mak" CFG="RTEvent_Logging_Service - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTEvent_Logging_Service - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RTEvent_Logging_Service - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTEvent_Logging_Service - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../" /I "../../../" /I "../../" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_CosNaming.lib TAO_RTEvent.lib TAO_RTOLDEvent.lib TAO_RTSched.lib TAO_PortableServer.lib TAO.lib ace.lib TAO_DsLogAdmin.lib TAO_RTEventLog.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\orbsvcs"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTEvent_Logging_Service - Win32 Debug"

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
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../../" /I "../../../" /I "../../" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D TAO_ORBSVCS_HAS_DLL=1 /FD /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_CosNamingd.lib TAO_RTEventd.lib TAO_RTOLDEventd.lib TAO_RTSchedd.lib TAO_PortableServerd.lib TAOd.lib aced.lib TAO_DsLogAdmind.lib TAO_RTEventLogd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\orbsvcs"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RTEvent_Logging_Service - Win32 Release"
# Name "RTEvent_Logging_Service - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RTEvent_Logging_Service.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RTEvent_Logging_Service.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
