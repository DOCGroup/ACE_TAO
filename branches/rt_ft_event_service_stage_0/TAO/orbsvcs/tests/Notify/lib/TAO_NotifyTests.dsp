# Microsoft Developer Studio Project File - Name="TAO_NotifyTests DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_NotifyTests DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_NotifyTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_NotifyTests.mak" CFG="TAO_NotifyTests DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_NotifyTests DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_NotifyTests DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "../../../" /I "../../../../" /I "../../../../../" /D "WIN32" /D "_WINDOWS" /D "TAO_NOTIFY_TEST_BUILD_DLL" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_ETCL.lib TAO_ETCL.lib TAO_CosNotification.lib TAO_CosNaming.lib TAO_CosEvent.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\..\..\bin\TAO_NotifyTests.dll" /pdbtype:sept /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ELSEIF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../" /I "../../../../" /I "../../../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_NOTIFY_TEST_BUILD_DLL" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_ETCLd.lib TAO_ETCLd.lib TAO_CosNotificationd.lib TAO_CosNamingd.lib TAO_CosEventd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\..\bin\TAO_NotifyTestsd.dll" /pdbtype:sept /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TAO_NotifyTests DLL - Win32 Release"
# Name "TAO_NotifyTests DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Notify_PushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_PushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_Test_Client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Notify_PushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_PushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_Test_Client.h
# End Source File
# Begin Source File

SOURCE=.\notify_test_export.h
# End Source File
# End Group
# End Target
# End Project
