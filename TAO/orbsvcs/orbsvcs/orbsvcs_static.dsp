# Microsoft Developer Studio Project File - Name="ORB Services Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ORB Services Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "orbsvcs_static.mak" CFG="ORB Services Static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ORB Services Static - Win32 Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "ORB Services Static - Win32 Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D TAO_HAS_DLL=0 /D __ACE_INLINE__=0 /D TAO_ORBSVCS_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"orbsvcss.lib"

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir ""
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\\" /I "..\..\\" /I "..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D TAO_HAS_DLL=0 /D ACE_HAS_DLL=0 /D __ACE_INLINE__=0 /D TAO_ORBSVCS_HAS_DLL=0 /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"orbsvcssd.lib"

!ENDIF 

# Begin Target

# Name "ORB Services Static - Win32 Release"
# Name "ORB Services Static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AV\AVStreams_i.cpp
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.cpp
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.cpp
# End Source File
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

SOURCE=.\CosEvent\ConsumerAdmin_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosConcurrencyControlS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventCommC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosLifeCycleS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNamingC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.cpp
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.cpp
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

SOURCE=.\Event\EC_Basic_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_UDP.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierFiltering.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Manip.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\EventChannel_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Hash_Naming_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplRepoC.cpp
# End Source File
# Begin Source File

SOURCE=.\ImplRepoS.cpp
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

SOURCE=.\Naming\Naming_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.cpp
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Bindings_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.cpp
# End Source File
# Begin Source File

SOURCE=.\PingC.cpp
# End Source File
# Begin Source File

SOURCE=.\PingS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushConsumer_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushSupplier_i.cpp
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

SOURCE=.\RtecUDPAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.cpp
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

SOURCE=.\AV\sfp.cpp
# End Source File
# Begin Source File

SOURCE=.\sfpC.cpp
# End Source File
# Begin Source File

SOURCE=.\sfpS.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Strategy_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\SupplierAdmin_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Clerk.cpp
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_Time_Service_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_TIO.cpp
# End Source File
# Begin Source File

SOURCE=.\Time\TAO_UTO.cpp
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\Time\Timer_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Timer_Module.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeServiceC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeServiceS.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
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

SOURCE=.\CosNamingC.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingS.h
# End Source File
# Begin Source File

SOURCE=.\Property\CosPropertyService_i.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceC.h
# End Source File
# Begin Source File

SOURCE=.\CosPropertyServiceS.h
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

SOURCE=.\Event\Debug_Macros.h
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

SOURCE=.\Event\EC_Command.h
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

SOURCE=.\Event\EC_Null_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set_T.h
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

SOURCE=.\Event\EC_Type_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Entries.h
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Manip.h
# End Source File
# Begin Source File

SOURCE=.\Event_Service_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Event\Fast_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Event\GPlot_File.h
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

SOURCE=.\Naming\Naming_Context.h
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

SOURCE=.\Naming\Persistent_Bindings_Map.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Context_Index_T.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Entries.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Persistent_Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\PingC.h
# End Source File
# Begin Source File

SOURCE=.\PingS.h
# End Source File
# Begin Source File

SOURCE=.\PingS_T.h
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

SOURCE=.\Naming\Shared_Hash_Map_T.h
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

SOURCE=.\Event\Task_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Time_Utilities.h
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
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
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

SOURCE=.\Event\EC_Default_Factory.i
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

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\AVStreams.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosConcurrencyControl.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosConcurrencyControl.idl
InputName=CosConcurrencyControl

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosEventChannelAdmin.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosEventComm.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosLifeCycle.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosNaming.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosPropertyService.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\CosTrading.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\ImplRepo.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\Logger.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Logger.idl
InputName=Logger

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\Null_MediaCtrl.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\Ping.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\Ping.idl
InputName=Ping

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\RtecEventComm.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\RtecScheduler.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\RtecUDPAdmin.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\sfp.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\TimeBase.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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

SOURCE=.\TimeService.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -I../../\
 -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h\
 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "ORB Services Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl_static -I../../ -Wb,export_macro=TAO_ORBSVCS_Export\
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
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosPropertyServiceS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosTradingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyPushSupplier_Set_T.cpp
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

SOURCE=.\Naming\Persistent_Context_Index_T.cpp
# End Source File
# Begin Source File

SOURCE=.\PingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Naming\Shared_Hash_Map_T.cpp
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
