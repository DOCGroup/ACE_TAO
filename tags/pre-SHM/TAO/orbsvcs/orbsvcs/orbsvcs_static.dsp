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
!MESSAGE "ORB Services Static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ORB Services Static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
RSC=rc.exe

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D TAO_HAS_DLL=0 /D "ACE_NO_INLINE" /D TAO_ORBSVCS_HAS_DLL=0 /D "TAO_ORBSVCS_HAS_Trader" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D TAO_HAS_DLL=0 /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D TAO_ORBSVCS_HAS_DLL=0 /D "TAO_ORBSVCS_HAS_Trader" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Log\BasicLog_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\BasicLogFactory_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\BCU.cpp
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

SOURCE=.\CosEvent_Utilities.cpp
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

SOURCE=.\CosTradingDynamicC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingDynamicS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposC.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingReposS.cpp
# End Source File
# Begin Source File

SOURCE=.\CosTradingS.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Dispatching_Modules.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminC.cpp
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.cpp
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

SOURCE=.\Event\EC_Dispatching_Task.cpp
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

SOURCE=.\Event\EC_MT_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Negation_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Priority_Scheduling.cpp
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

SOURCE=.\Event\EC_Sched_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Sched_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.cpp
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

SOURCE=.\Event\EC_Trivial_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.cpp
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

SOURCE=.\Trader\Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Interpreter_Utils_T.cpp
# End Source File
# Begin Source File

SOURCE=.\IOR_Multicast.cpp
# End Source File
# Begin Source File

SOURCE=.\IR_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Iterator_i.cpp
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

SOURCE=.\Log\Log_Constraint_Interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\Log_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\LogMgr_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Log\LogRecordStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Offer_Iterators.cpp
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

SOURCE=.\CosEvent\ProxyPushConsumer_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent\ProxyPushSupplier_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ReactorTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Sched_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Sched\Reconfig_Scheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\RT_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataC.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.cpp
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

SOURCE=.\Trader\Trader_Constraint_Visitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Trader\Trader_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Naming\Transient_Naming_Context.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Log\BasicLog_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\BasicLogFactory_i.h
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

SOURCE=.\DsLogAdminC.h
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.h
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS_T.h
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

SOURCE=.\Event\EC_Negation_Filter.h
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

SOURCE=.\Log\Iterator_i.h
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

SOURCE=.\Log\Log_Constraint_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Log\Log_Constraint_Visitors.h
# End Source File
# Begin Source File

SOURCE=.\Log\Log_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\LogMgr_i.h
# End Source File
# Begin Source File

SOURCE=.\Log\LogRecordStore.h
# End Source File
# Begin Source File

SOURCE=.\Event\Memory_Pools.h
# End Source File
# Begin Source File

SOURCE=.\Event\Module_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Naming\Naming_Context_Interface.h
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

SOURCE=.\RtecDefaultEventDataC.h
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.h
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

# PROP Default_Filter ""
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

SOURCE=.\DsLogAdminC.i
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS.i
# End Source File
# Begin Source File

SOURCE=.\DsLogAdminS_T.i
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

SOURCE=.\Event\EC_Negation_Filter.i
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

SOURCE=.\RtecDefaultEventDataC.i
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.i
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

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CosEventChannelAdmin.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSEV="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSEV="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventChannelAdmin.idl
InputName=CosEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSEVE="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSEVE="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosEventComm.idl
InputName=CosEventComm

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSLI="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSLI="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosLifeCycle.idl
InputName=CosLifeCycle

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSNA="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSNA="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosNaming.idl
InputName=CosNaming

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSPR="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSPR="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosPropertyService.idl
InputName=CosPropertyService

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSTR="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSTR="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTrading.idl
InputName=CosTrading

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRA="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSTRA="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingDynamic.idl
InputName=CosTradingDynamic

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__COSTRAD="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__COSTRAD="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\CosTradingRepos.idl
InputName=CosTradingRepos

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

SOURCE=.\DsLogAdmin.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__DSLOG="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\DsLogAdmin.idl
InputName=DsLogAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__DSLOG="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\DsLogAdmin.idl
InputName=DsLogAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__IMPLR="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__IMPLR="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\ImplRepo.idl
InputName=ImplRepo

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__LIFEC="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__LIFEC="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\LifeCycleService.idl
InputName=LifeCycleService

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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

SOURCE=.\RtecDefaultEventData.idl

!IF  "$(CFG)" == "ORB Services Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECD="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECD="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECE="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECE="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECEV="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECEV="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECS="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECS="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecScheduler.idl
InputName=RtecScheduler

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECU="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__RTECU="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__TIMEB="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__TIMEB="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeBase.idl
InputName=TimeBase

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__TIMES="..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../../ -I../../tao      -Wb,export_macro=TAO_ORBSVCS_Export               -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
USERDEP__TIMES="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\TimeService.idl
InputName=TimeService

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../../ -I../../tao    -Wb,export_macro=TAO_ORBSVCS_Export                                                      -Wb,export_include=orbsvcs_export.h $(InputName).idl

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
# Begin Group "Resource files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\orbsvcs.rc
# End Source File
# End Group
# End Target
# End Project
