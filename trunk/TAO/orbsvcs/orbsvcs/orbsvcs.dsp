# Microsoft Developer Studio Project File - Name="orbsvcs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=orbsvcs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs.mak" CFG="orbsvcs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "orbsvcs - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "orbsvcs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "../.." /I "../../.." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /D "TAO_HAS_OBJECT_IN_STRUCT_MARSHAL_BUG" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Desc=Copying DLL to $TAO_ROOT/tao
PostBuild_Cmds=copy orbsvcs.dll ..\..\tao
# End Special Build Tool

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".." /I "../.." /I "../../.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D TAO_ORBSVCS_HAS_DLL=1 /D "TAO_ORBSVCS_BUILD_DLL" /FD /c
# SUBTRACT CPP /WX /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAOd.lib aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"orbsvcsd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "orbsvcs - Win32 Release"
# Name "orbsvcs - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Event\BCU.cpp
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSetFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Concurrency\Concurrency_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Config_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_l.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Nodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Constraint_y.cpp
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\CosNaming_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTimeBaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\DynSched.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.cpp
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceC.cpp
# End Source File
# Begin Source File

SOURCE=.\LifeCycleServiceS.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Local_ESTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Logger_i.cpp
# End Source File
# Begin Source File

SOURCE=.\LoggerC.cpp
# End Source File
# Begin Source File

SOURCE=.\LoggerS.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecSchedulerS.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\SchedEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Scheduler_Generic.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Service_Type_Repository.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Task_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Event\BCU.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSet.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\CC_LockSetFactory.h
# End Source File
# Begin Source File

SOURCE=.\Concurrency\Concurrency_Utils.h
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

SOURCE=.\CosConcurrencyControlC.h
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.h
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.h
# End Source File
# Begin Source File

SOURCE=.\Naming\CosNaming_i.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.h
# End Source File
# Begin Source File

SOURCE=.\CosTimeBaseC.h
# End Source File
# Begin Source File

SOURCE=.\CosTimeBaseS.h
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.h
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

SOURCE=.\Event\EC_Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.h
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

SOURCE=.\IOR_Multicast.h
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

SOURCE=.\LoggerC.h
# End Source File
# Begin Source File

SOURCE=.\LoggerS.h
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.h
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.h
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

SOURCE=.\Event\ReactorTask.h
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

SOURCE=.\Sched\Strategy_Scheduler.h
# End Source File
# Begin Source File

SOURCE=.\Event\Task_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.h
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
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\CosConcurrencyControlC.i
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.i
# End Source File
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

SOURCE=.\CosTimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\CosTimeBaseS.i
# End Source File
# Begin Source File

SOURCE=.\CosTradingC.i
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

SOURCE=.\Event\Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.i
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

SOURCE=.\Sched\SchedEntry.i
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\CosConcurrencyControl.idl
USERDEP__COSCO="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosLifeCycle.idl
USERDEP__COSLI="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosNaming.idl
USERDEP__COSNA="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTimeBase.idl
USERDEP__COSTI="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosTimeBase.idl
InputName=CosTimeBase

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosTimeBase.idl
InputName=CosTimeBase

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CosTrading.idl
USERDEP__COSTR="..\..\tao_idl\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__LIFEC="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Logger.idl
USERDEP__LOGGE="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

USERDEP__RTECE="..\..\TAO_IDL\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

USERDEP__RTECE="..\..\TAO_IDL\tao_idl.exe"	"RtecEventComm.idl"\
	"RtecScheduler.idl"	
# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventComm.idl
USERDEP__RTECEV="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecScheduler.idl
USERDEP__RTECS="..\..\TAO_IDL\tao_idl.exe"	

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL compiler
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\tao_idl\tao_idl -Wb,export_macro=TAO_ORBSVCS_Export\
                          -Wb,export_include=orbsvcs_export.h $(InputName).idl

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosTradingS_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Database.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Interfaces.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_T.cpp

!IF  "$(CFG)" == "orbsvcs - Win32 Release"

!ELSEIF  "$(CFG)" == "orbsvcs - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
