# Microsoft Developer Studio Project File - Name="RTEC Perf Federated_Roundtrip Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RTEC Perf Federated_Roundtrip Client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="RTEC Perf Federated_Roundtrip Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTEC Perf Federated_Roundtrip Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RTEC Perf Federated_Roundtrip Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTEC Perf Federated_Roundtrip Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release\client"
# PROP BASE Intermediate_Dir "Release\client"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\client"
# PROP Intermediate_Dir "Release\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../../../" /I "../../../../" /I "../../../" /I "../lib/" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ../../../../../ace/ACE.lib ../../../../tao/TAO.lib ../../../../tao/PortableServer/TAO_PortableServer.lib ../../../../tao/Strategies/TAO_Strategies.lib ../../../../tao/IORTable/TAO_IORTable.lib ../../../orbsvcs/TAO_CosNaming.lib ../../../orbsvcs/TAO_RTEvent.lib ../../../orbsvcs/TAO_Svc_Utils.lib ../../../orbsvcs/TAO_RTCORBAEvent.lib ../../../../tao/RTCORBA/TAO_RTCORBA.lib ../../../../tao/RTPortableServer/TAO_RTPortableServer.lib ../../../../tao/Messaging/TAO_Messaging.lib ../lib/RTECPerf.lib ../../../../tao/Valuetype/TAO_Valuetype.lib /nologo /subsystem:console /machine:I386 /out:"client.exe"

!ELSEIF  "$(CFG)" == "RTEC Perf Federated_Roundtrip Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\client"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "../../../../../" /I "../../../../" /I "../../../" /I "../lib/" /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../../tao/RTPortableServer/TAO_RTPortableServerd.lib ../../../orbsvcs/TAO_RTCORBAEventd.lib ../../../../tao/RTCORBA/TAO_RTCORBAd.lib ../../../../tao/Messaging/TAO_Messagingd.lib ../lib/RTECPerfd.lib ../../../../../ace/ACEd.lib ../../../../tao/TAOd.lib ../../../../tao/PortableServer/TAO_PortableServerd.lib ../../../../tao/Strategies/TAO_Strategiesd.lib ../../../../tao/IORTable/TAO_IORTabled.lib ../../../orbsvcs/TAO_CosNamingd.lib ../../../orbsvcs/TAO_RTEventd.lib ../../../orbsvcs/TAO_Svc_Utilsd.lib ../../../../tao/Valuetype/TAO_Valuetyped.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RTEC Perf Federated_Roundtrip Client - Win32 Release"
# Name "RTEC Perf Federated_Roundtrip Client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;fi;fd"
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl;pidl"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl;i"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
