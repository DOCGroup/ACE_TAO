# Microsoft Developer Studio Project File - Name="ORB Services DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=ORB Services DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak" CFG="ORB Services DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ORB Services DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ORB Services DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ORB Services DLL - Win32 Alpha Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "ORB Services DLL - Win32 Alpha Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\bin\orbsvcs.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "orbsvcs_"
# PROP BASE Intermediate_Dir "orbsvcs_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
# SUBTRACT CPP /WX /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\bin\orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "orbsvcs_"
# PROP BASE Intermediate_Dir "orbsvcs_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /nologo /MT /Gt0 /W3 /GX /O2 /I "../" /I" I "../../../" /D"NDEBUG" /D "WIN32" /FD /c
# ADD CPP /Gt0 /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "orbsvcs0"
# PROP BASE Intermediate_Dir "orbsvcs0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /nologo /W3 /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /D "WIN32" /D "_DEBUG" /FD /MTd /c
# ADD CPP /Gt0 /nologo /W3 /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /D "WIN32" /D "_DEBUG" /FD /MDd /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOd.lib aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "ORB Services DLL - Win32 Release"
# Name "ORB Services DLL - Win32 Debug"
# Name "ORB Services DLL - Win32 Alpha Release"
# Name "ORB Services DLL - Win32 Alpha Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\AV\AVStreams_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\BCU.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_l.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_y.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ConsumerAdmin_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNamingC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNamingS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Event_Manip.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\EventChannel_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Hash_Naming_Context.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IR_Helper.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Local_ESTypes.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushConsumer_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushSupplier_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\sfp.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfpC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfpS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEvent\SupplierAdmin_i.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Clerk.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Server.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_TIO.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\TAO_UTO.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time\Timer_Helper.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming\Transient_Naming_Context.cpp

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\AV\AVStreams_i.h
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i_T.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event\BCU.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Bindings_Iterator_T.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.h
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.h
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i_T.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.h
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.h
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event_Service_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Hash_Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.h
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.h
# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.h
# End Source File
# Begin Source File

SOURCE=.\IR_Helper.h
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.h
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.h
# End Source File
# Begin Source File

SOURCE=.\Event\Local_ESTypes.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Locking.h
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.h
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.h
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.h
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.h
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.h
# End Source File
# Begin Source File

SOURCE=.\sfpC.h
# End Source File
# Begin Source File

SOURCE=.\sfpS.h
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Clerk.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Server.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_TIO.h
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_UTO.h
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.h
# End Source File
# Begin Source File

SOURCE=.\Time\Timer_Helper.h
# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.h
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.h
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Transient_Naming_Context.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\CosLifeCycleC.i
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.i
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.i
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.i
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.i
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.i
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.i
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.i
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.i
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.i
# End Source File
# Begin Source File

SOURCE=.\PingC.i
# End Source File
# Begin Source File

SOURCE=.\PingS.i
# End Source File
# Begin Source File

SOURCE=.\PingS_T.i
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.i
# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.i
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\AVStreams.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdmin.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosEventComm.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSEVE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycle.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSLI="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLI="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSLI="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSLI="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNaming.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSNA="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSNA="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSNA="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSNA="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosPropertyService.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSPR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSPR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSPR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSPR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTrading.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamic.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTradingRepos.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImplRepo.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__IMPLR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LifeCycleService.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__LIFEC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LIFEC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__LIFEC="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__LIFEC="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrl.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventComm.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecScheduler.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECS="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdmin.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfp.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBase.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEB="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEB="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEB="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMEB="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeService.idl

!IF  "$(CFG)" == "ORB Services DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../   -Wb,export_macro=TAO_ORBSVCS_Export            -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -i../../   -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "ORB Services DLL - Win32 Alpha Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIMES="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -Wb,export_macro=TAO_ORBSVCS_Export                                                   -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AV\AVStreams_i_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
