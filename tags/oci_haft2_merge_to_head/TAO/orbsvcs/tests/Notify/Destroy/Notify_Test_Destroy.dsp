# Microsoft Developer Studio Project File - Name="Notify_Test_Destroy EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Notify_Test_Destroy EXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Notify_Test_Destroy.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Notify_Test_Destroy.mak" CFG="Notify_Test_Destroy EXE - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Notify_Test_Destroy EXE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Notify_Test_Destroy EXE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Notify_Test_Destroy EXE - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /GR /I "..\..\..\..\orbsvcs\tests\Notify\lib" /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\..\..\orbsvcs\tests\Notify\lib" /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_NotifyTests.lib TAO_CosNotification.lib TAO_ETCL.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_DynamicAny.lib TAO_PortableServer.lib TAO.lib ACE.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:console  /machine:I386 /out:"Notify_Tests_Destroy.exe"

!ELSEIF  "$(CFG)" == "Notify_Test_Destroy EXE - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\..\..\orbsvcs\tests\Notify\lib" /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\..\..\orbsvcs\tests\Notify\lib" /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_NotifyTestsd.lib TAO_CosNotificationd.lib TAO_ETCLd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_DynamicAnyd.lib TAO_PortableServerd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /nologo /version:1.3 /subsystem:console /debug /machine:I386 /out:"Notify_Tests_Destroy.exe"

!ENDIF

# Begin Target

# Name "Notify_Test_Destroy EXE - Win32 Release"
# Name "Notify_Test_Destroy EXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# End Target
# End Project
