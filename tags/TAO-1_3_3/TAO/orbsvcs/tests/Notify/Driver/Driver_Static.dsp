# Microsoft Developer Studio Project File - Name="Driver Static EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Driver Static EXE - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Driver_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Driver_Static.mak" CFG="Driver Static EXE - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Driver Static EXE - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Driver Static EXE - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Driver Static EXE - Win32 Static Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_Cosnamings.lib TAO_CosNotifications.lib TAO_NotifyTestss.lib TAOs.lib ACEs.lib TAO_PortableServers.lib  TAO_CosEvents.lib TAO_Svc_Utilss.lib  TAO_ObjRefTemplates.lib TAO_Valuetypes.lib TAO_IORInterceptors.lib /nologo /subsystem:console /machine:I386 /out:"Notify_Tests_Driver.exe" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /version:1.3.1

!ELSEIF  "$(CFG)" == "Driver Static EXE - Win32 Static Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_Cosnamingsd.lib TAO_CosNotificationsd.lib TAO_NotifyTestssd.lib TAOsd.lib ACEsd.lib TAO_PortableServersd.lib TAO_CosEventsd.lib TAO_Svc_Utilssd.lib TAO_ObjRefTemplatesd.lib TAO_Valuetypesd.lib TAO_IORInterceptorsd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"Notify_Tests_Driver.exe" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\tests\Notify\lib" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\ObjRefTemplate" /libpath:"..\..\..\..\tao\Valuetype" /libpath:"..\..\..\..\tao\IORInterceptor" /version:1.3.1

!ENDIF 

# Begin Target

# Name "Driver Static EXE - Win32 Static Release"
# Name "Driver Static EXE - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# End Target
# End Project
