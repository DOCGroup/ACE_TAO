
# $Id$
#   Win32 autobuild helper.

# use lib "$ENV{ACE_ROOT}/bin";

# **** We no longer use this script in our daily build.  So we
#      will need to depend on your help to maintain the file
#      up-to-date if you want to continue using it.

## TO-DO
##   * Added command line options to selective compile library, et.al.
##   * Added cvs update.  Also controllable from command line.
##   * Added building an running version_tests/.  It uses a different
##     naming scheme.
##   * Selectively build some specific configs from command line.

## Available command line switches
##   * -k : Ignore error.  Compile all projects in a collection regardless
##          of errors.
##   * -a : Build Alpha collections.
##   * -D : Build DLLs only.
##   * -L : Build LIBs only.
##   * -d : Debug the script.  No compilation will occur.
##   * -v : Verbose mode.
##   * -r : Rebuild all projects.  (Default is Build.)
##   * -r : Clean all projects.

@Win32_Lists= ("Win32 Debug",
               "Win32 Release",
               "Win32 Unicode Debug",
               "Win32 Unicode Release");

@Win32_DLL_Version_Tests_Configs= ("Win32 DLL Debug",
                                   "Win32 DLL Release",
                                   "Win32 DLL Unicode Debug",
                                   "Win32 DLL Unicode Release");

@Win32_LIB_Version_Tests_Configs = ("Win32 static Debug",
                                    "Win32 static Release",
                                    "Win32 static Unicode Debug",
                                    "Win32 static Unicode Release");

@Alpha_Lists= ("Win32 Alpha Debug",
               "Win32 Alpha Release",
               "Win32 Alpha Unicode Debug",
               "Win32 Alpha Unicode Release");

%Win32_DLL_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE DLL:
apps/gperf/src/gperf.dsw, gperf:
TAO/tao/TAO.dsw, TAO DLL:
TAO/tao/TAO.dsw, PortableServer:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, Svc Utils:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosConcurrency:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosNaming:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosTime:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosLifeCycle:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTSched:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTSchedEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTOLDEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosNotification:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosProperty:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, DsLogAdmin:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosTrading:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, AV:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
TAO/orbsvcs/Scheduling_Service/Scheduling_Service.dsw, Scheduling_Service:
TAO/orbsvcs/Event_Service/Event_Service.dsw, Event_Service:
TAO/orbsvcs/Dump_Schedule/Dump_Schedule.dsw, Dump_Schedule:
TAO/orbsvcs/Notify_Service/Notify_Service.dsw, Notify_Service:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Server:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Clerk:
TAO/orbsvcs/CosEvent_Service/CosEvent_Service.dsw, CosEvent_Service:
TAO/orbsvcs/CosEvent_Service/CosEvent_Service.dsw, CosEvent_Service_Native:
TAO/orbsvcs/Concurrency_Service/Concurrency_Service.dsw, Concurrency_Service:
TAO/orbsvcs/ImplRepo_Service/ImplRepo.dsw, ImplRepo_Service:
TAO/orbsvcs/ImplRepo_Service/ImplRepo.dsw, tao_imr:
TAO/orbsvcs/LifeCycle_Service/LifeCycle_Service.dsw, LifeCycle_Service:
TAO/orbsvcs/Trading_Service/Trading_Service.dsw, Trading_Service:
TAO/orbsvcs/tests/Simple_Naming/Simple_Naming.dsw, Simple Naming Client:
TAO/orbsvcs/tests/Event_Latency/Event_Latency.dsw, Event_Latency:
TAO/orbsvcs/tests/EC_Multiple/EC_Multiple.dsw, EC_Multiple:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Throughput:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Supplier:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Consumer:
TAO/orbsvcs/tests/EC_Mcast/EC_Mcast.dsw, EC_Mcast:
TAO/orbsvcs/tests/EC_Custom_Marshal/EC_Custom_Marshal.dsw, ECCM_Supplier:
TAO/orbsvcs/tests/EC_Custom_Marshal/EC_Custom_Marshal.dsw, ECCM_Consumer:
TAO/orbsvcs/tests/EC_Basic/EC_Basic.dsw, EC_Basic:
TAO/orbsvcs/tests/Property/PropertyTest.dsw, server:
TAO/orbsvcs/tests/Property/PropertyTest.dsw, client:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf_Anomalies:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf_Runtime:
TAO/orbsvcs/tests/Time/Time_Service_Test.dsw, Time_Service_Test:
TAO/orbsvcs/tests/Event/lib/ECTest.dsw, ECTest:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Atomic_Reconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, BCast:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Bitmask:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Complex:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Control:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Disconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Gateway:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, MT_Disconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Negation:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Observer:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Reconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Schedule:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Shutdown:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Timeout:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Wildcard:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Connect:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Inversion:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Throughput:
TAO/orbsvcs/tests/CosEvent/lib/CECTest.dsw, CECTest:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Disconnect:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, MT_Disconnect:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Pull_Push_Event:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Push_Event:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Shutdown:
TAO/orbsvcs/tests/Concurrency/Concurrency.dsw, CC_client:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Airplane Server:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Airplane Client:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Nestea Server:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Nestea Client:
TAO/orbsvcs/tests/Trading/Trading.dsw, TradingLib:
TAO/orbsvcs/tests/Trading/Trading.dsw, Import_Test:
TAO/orbsvcs/tests/Trading/Trading.dsw, Export_Test:
TAO/orbsvcs/tests/Trading/Trading.dsw, Colocated_Test:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_server:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_client:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_child:
TAO/orbsvcs/examples/CosEC/Factory/Factory.dsw, FactoryServer:
TAO/orbsvcs/examples/CosEC/Factory/Factory.dsw, FactoryClient:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Service:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Supplier:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Consumer:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Notifier:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Supplier:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Consumer:
TAO/examples/POA/Generic_Servant/Generic_Servant.dsw, POA Generic Servant DLL:
TAO/examples/POA/Generic_Servant/Generic_Servant.dsw, POA Generic Servant Client:
TAO/examples/POA/RootPOA/RootPOA.dsw, POA RootPOA:
TAO/examples/POA/Reference_Counted_Servant/Reference_Counted_Servant.dsw, POA Reference Counted Servant Server:
TAO/examples/POA/Adapter_Activator/Adaptor_Activator.dsw, POA Adapter Activator Server:
TAO/examples/POA/DSI/DSI.DSW, POA DSI Server:
TAO/examples/POA/DSI/DSI.DSW, POA DSI Client:
TAO/examples/POA/Default_Servant/Default_Servant.dsw, POA Default Servant Server:
TAO/examples/POA/Default_Servant/Default_Servant.dsw, POA Default Servant Client:
TAO/examples/POA/Explicit_Activation/Explicit_Activation.dsw, POA Explicit Activation Server:
TAO/examples/POA/FindPOA/FindPOA.dsw, POA FindPOA:
TAO/examples/POA/Forwarding/Forwarding.dsw, POA Forwarding Server:
TAO/examples/POA/Forwarding/Forwarding.dsw, POA Forwarding Client:
TAO/examples/POA/Loader/Loader.dsw, POA Loader Server:
TAO/examples/POA/On_Demand_Activation/On_Demand_Activation.dsw, POA On Demand Activation Server:
TAO/examples/POA/On_Demand_Loading/On_Demand_Loading.dsw, POA On Demand Loading Server:
TAO/examples/POA/NewPOA/NewPOA.dsw, POA NewPOA:
TAO/examples/POA/TIE/TIE.dsw, POA TIE Server:
TAO/examples/POA/TIE/TIE.dsw, POA TIE Client:
TAO/examples/Simple/bank/bank.dsw, Simple Bank Server:
TAO/examples/Simple/bank/bank.dsw, Simple Bank Client:
TAO/examples/Simple/chat/chat.dsw, Simple Chat Server:
TAO/examples/Simple/chat/chat.dsw, Simple Chat Client:
TAO/examples/Simple/echo/Echo.dsw, Simple Echo Server:
TAO/examples/Simple/echo/Echo.dsw, Simple Echo Client:
TAO/examples/Simple/grid/Grid.dsw, Simple Grid Server:
TAO/examples/Simple/grid/Grid.dsw, Simple Grid Client:
TAO/examples/Simple/time/Time.dsw, Simple Time Server:
TAO/examples/Simple/time/Time.dsw, Simple Time Client:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Library:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Server:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Client:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Notifier:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Supplier:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Consumer:
TAO/examples/OBV/Typed_Events/Typed_Events.dsw, OBV Typed Events Server:
TAO/examples/OBV/Typed_Events/Typed_Events.dsw, OBV Typed Events Client:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Server:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Client:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Second Client:
TAO/examples/Quoter/Quoter.dsw, Quoter Generic Factory:
TAO/examples/Quoter/Quoter.dsw, Quoter Factory Finder:
TAO/examples/Quoter/Quoter.dsw, Quoter Server:
TAO/examples/Quoter/Quoter.dsw, Quoter Client:
TAO/examples/Buffered_Oneways/Buffered_Oneways.dsw, Buffered_Oneways Server:
TAO/examples/Buffered_Oneways/Buffered_Oneways.dsw, Buffered_Oneways Client:
TAO/examples/Buffered_AMI/Buffered_AMI.dsw, Buffered_AMI Server:
TAO/examples/Buffered_AMI/Buffered_AMI.dsw, Buffered_AMI Client:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Server:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Client:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Collocation Test:
TAO/performance-tests/Cubit/TAO/MT_Cubit/MT_Cubit.dsw, MT_Cubit Server:
TAO/performance-tests/Cubit/TAO/MT_Cubit/MT_Cubit.dsw, MT_Cubit Client:
TAO/performance-tests/POA/Object_Creation_And_Registration/registration.dsw, POA Registration:
TAO/performance-tests/Pluggable/Pluggable.dsw, Pluggable Server:
TAO/performance-tests/Pluggable/Pluggable.dsw, Pluggable Client:
TAO/performance-tests/RTCorba/Oneways/Reliable/Reliable.dsw, Reliable Oneways Server:
TAO/performance-tests/RTCorba/Oneways/Reliable/Reliable.dsw, Reliable Oneways Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Common/RTCORBA_Common.dsw, RTCORBA Test Common DLL:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Single_Endpoint/Single_Endpoint.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Single_Endpoint/Single_Endpoint.dsw, Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Orb_Per_Priority/ORB_Per_Priority.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Orb_Per_Priority/ORB_Per_Priority.dsw, Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Endpoint_Per_Priority/Project.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Endpoint_Per_Priority/Project.dsw, Client:
TAO/tests/CDR/CDR.dsw, CDR Basic Types:
TAO/tests/CDR/CDR.dsw, CDR Growth:
TAO/tests/CDR/CDR.dsw, CDR Octet_Sequence:
TAO/tests/CDR/CDR.dsw, CDR TC:
TAO/tests/IDL_Test/IDL_Test.dsw, idl_test:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq Server:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq Client:
TAO/tests/InterOp-Naming/InterOp-Naming.dsw, InterOp Naming Server:
TAO/tests/InterOp-Naming/InterOp-Naming.dsw, InterOp Naming Client:
TAO/tests/NestedUpcall/Simple/Simple.dsw, Nested Upcall Simple Server:
TAO/tests/NestedUpcall/Simple/Simple.dsw, Nested Upcall Simple Client:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Server_A:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Server_B:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Initiator:
TAO/tests/NestedUpcall/MT_Client_Test/MT_Client_Test.dsw, Nested Upcall MT_Client Server:
TAO/tests/NestedUpcall/MT_Client_Test/MT_Client_Test.dsw, Nested Upcall MT_Client Client:
TAO/tests/Multiple_Inheritance/Multiple_Inheritance.dsw, Multiple Inheritance Server:
TAO/tests/Multiple_Inheritance/Multiple_Inheritance.dsw, Multiple Inheritance Client:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client ORB Creation:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Server:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Simple Client:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Client:
TAO/tests/MT_Server/MT_Server.dsw, MT_Server Server:
TAO/tests/MT_Server/MT_Server.dsw, MT_Server Client:
TAO/tests/POA/Destruction/Destruction.dsw, POA Destruction:
TAO/tests/POA/Single_Threaded_POA/Single_Threaded_POA.dsw, POA Single_Threaded_POA:
TAO/tests/POA/Non_Servant_Upcalls/Non_Servant_Upcalls.dsw, POA Non_Servant_Upcalls:
TAO/tests/POA/Deactivation/Deactivation.dsw, POA Deactivation:
TAO/tests/POA/Identity/Identity.dsw, POA Identity:
TAO/tests/IORManipulation/IORManipulation.dsw, IORTest:
TAO/tests/Connection_Purging/Connection_Purging.dsw, POA Connection Purging:
TAO/tests/ORB_init/ORB_init.dsw, ORB_init:
TAO/tests/ORB_destroy/ORB_destroy.dsw, ORB_destroy:
TAO/tests/Nested_Event_Loop/Nested_Event_Loop.dsw, Nested_Event_Loop Server:
TAO/tests/Nested_Event_Loop/Nested_Event_Loop.dsw, Nested_Event_Loop Client:
TAO/tests/Leader_Followers/Leader_Followers.dsw, Leader_Followers Server:
TAO/tests/Leader_Followers/Leader_Followers.dsw, Leader_Followers Client:
TAO/tests/Object_Loader/Object_Loader.dsw, Test_DLL:
TAO/tests/Object_Loader/Object_Loader.dsw, Object_Loader Driver:
TAO/tests/DynAny_Test/DynAny_Test.dsw, DynAny Basics:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Server:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Client:
TAO/tests/Faults/Faults.dsw, Faults Server:
TAO/tests/Faults/Faults.dsw, Faults Client:
TAO/tests/Faults/Faults.dsw, Faults Ping:
TAO/tests/Faults/Faults.dsw, Faults Pong:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Server:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Client:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Simple_Client:
TAO/tests/Timeout/Timeout.dsw, Timeout Server:
TAO/tests/Timeout/Timeout.dsw, Timeout Client:
TAO/tests/Policies/Policies.dsw, Manipulation:
TAO/tests/RTCORBA/RTMutex/RTMutex.dsw, RTMutex Server:
TAO/tests/Timed_Buffered_Oneways/Timed_Buffered_Oneways.dsw, Timed_Buffered_Oneways Server:
TAO/tests/Timed_Buffered_Oneways/Timed_Buffered_Oneways.dsw, Timed_Buffered_Oneways Client:
TAO/tests/Smart_Proxies/Smart_Proxies.dsw, server:
TAO/tests/Smart_Proxies/Smart_Proxies.dsw, client:
TAO/utils/catior/catior.dsw, catior:
TAO/utils/IOR-Parser/IOR_Parser.dsw, IOR Parser:
TAO/utils/nslist/nslist.dsw, nslist:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE DLL:
apps/gperf/src/gperf.dsw, gperf:
TAO/tao/TAO.dsw, TAO DLL:
TAO/tao/TAO.dsw, PortableServer:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, Svc Utils:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosConcurrency:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosNaming:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosTime:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosLifeCycle:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTSched:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTSchedEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, RTOLDEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosEvent:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosNotification:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosProperty:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, DsLogAdmin:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, CosTrading:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, AV:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
TAO/orbsvcs/Scheduling_Service/Scheduling_Service.dsw, Scheduling_Service:
TAO/orbsvcs/Event_Service/Event_Service.dsw, Event_Service:
TAO/orbsvcs/Dump_Schedule/Dump_Schedule.dsw, Dump_Schedule:
TAO/orbsvcs/Notify_Service/Notify_Service.dsw, Notify_Service:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Server:
TAO/orbsvcs/Time_Service/Time_Service.dsw, Time_Service_Clerk:
TAO/orbsvcs/CosEvent_Service/CosEvent_Service.dsw, CosEvent_Service:
TAO/orbsvcs/CosEvent_Service/CosEvent_Service.dsw, CosEvent_Service_Native:
TAO/orbsvcs/Concurrency_Service/Concurrency_Service.dsw, Concurrency_Service:
TAO/orbsvcs/ImplRepo_Service/ImplRepo.dsw, ImplRepo_Service:
TAO/orbsvcs/ImplRepo_Service/ImplRepo.dsw, tao_imr:
TAO/orbsvcs/LifeCycle_Service/LifeCycle_Service.dsw, LifeCycle_Service:
TAO/orbsvcs/Trading_Service/Trading_Service.dsw, Trading_Service:
TAO/orbsvcs/tests/Simple_Naming/Simple_Naming.dsw, Simple Naming Client:
TAO/orbsvcs/tests/Event_Latency/Event_Latency.dsw, Event_Latency:
TAO/orbsvcs/tests/EC_Multiple/EC_Multiple.dsw, EC_Multiple:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Throughput:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Supplier:
TAO/orbsvcs/tests/EC_Throughput/EC_Throughput.dsw, ECT_Consumer:
TAO/orbsvcs/tests/EC_Mcast/EC_Mcast.dsw, EC_Mcast:
TAO/orbsvcs/tests/EC_Custom_Marshal/EC_Custom_Marshal.dsw, ECCM_Supplier:
TAO/orbsvcs/tests/EC_Custom_Marshal/EC_Custom_Marshal.dsw, ECCM_Consumer:
TAO/orbsvcs/tests/EC_Basic/EC_Basic.dsw, EC_Basic:
TAO/orbsvcs/tests/Property/PropertyTest.dsw, server:
TAO/orbsvcs/tests/Property/PropertyTest.dsw, client:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf_Anomalies:
TAO/orbsvcs/tests/Sched_Conf/Sched_Conf.dsw, Sched_Conf_Runtime:
TAO/orbsvcs/tests/Time/Time_Service_Test.dsw, Time_Service_Test:
TAO/orbsvcs/tests/Event/lib/ECTest.dsw, ECTest:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Atomic_Reconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, BCast:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Bitmask:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Complex:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Control:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Disconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Gateway:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, MT_Disconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Negation:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Observer:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Reconnect:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Schedule:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Shutdown:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Timeout:
TAO/orbsvcs/tests/Event/Basic/Basic.dsw, Wildcard:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Connect:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Inversion:
TAO/orbsvcs/tests/Event/Performance/ECPerformance.dsw, Throughput:
TAO/orbsvcs/tests/CosEvent/lib/CECTest.dsw, CECTest:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Disconnect:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, MT_Disconnect:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Pull_Push_Event:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Push_Event:
TAO/orbsvcs/tests/CosEvent/Basic/Basic.dsw, Shutdown:
TAO/orbsvcs/tests/Concurrency/Concurrency.dsw, CC_client:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Airplane Server:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Airplane Client:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Nestea Server:
TAO/orbsvcs/tests/ImplRepo/ImplRepo.dsw, Nestea Client:
TAO/orbsvcs/tests/Trading/Trading.dsw, TradingLib:
TAO/orbsvcs/tests/Trading/Trading.dsw, Import_Test:
TAO/orbsvcs/tests/Trading/Trading.dsw, Export_Test:
TAO/orbsvcs/tests/Trading/Trading.dsw, Colocated_Test:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_server:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_client:
TAO/orbsvcs/tests/AVStreams/benchmark/AV_benchmark.dsw, av_benchmark_child:
TAO/orbsvcs/examples/CosEC/Factory/Factory.dsw, FactoryServer:
TAO/orbsvcs/examples/CosEC/Factory/Factory.dsw, FactoryClient:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Service:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Supplier:
TAO/orbsvcs/examples/RtEC/Simple/Simple.dsw, EC_Simple_Consumer:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Notifier:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Supplier:
TAO/examples/Callback_Quoter/Callback_Quoter.dsw, Callback Quoter Consumer:
TAO/examples/POA/Generic_Servant/Generic_Servant.dsw, POA Generic Servant DLL:
TAO/examples/POA/Generic_Servant/Generic_Servant.dsw, POA Generic Servant Client:
TAO/examples/POA/RootPOA/RootPOA.dsw, POA RootPOA:
TAO/examples/POA/Reference_Counted_Servant/Reference_Counted_Servant.dsw, POA Reference Counted Servant Server:
TAO/examples/POA/Adapter_Activator/Adaptor_Activator.dsw, POA Adapter Activator Server:
TAO/examples/POA/DSI/DSI.DSW, POA DSI Server:
TAO/examples/POA/DSI/DSI.DSW, POA DSI Client:
TAO/examples/POA/Default_Servant/Default_Servant.dsw, POA Default Servant Server:
TAO/examples/POA/Default_Servant/Default_Servant.dsw, POA Default Servant Client:
TAO/examples/POA/Explicit_Activation/Explicit_Activation.dsw, POA Explicit Activation Server:
TAO/examples/POA/FindPOA/FindPOA.dsw, POA FindPOA:
TAO/examples/POA/Forwarding/Forwarding.dsw, POA Forwarding Server:
TAO/examples/POA/Forwarding/Forwarding.dsw, POA Forwarding Client:
TAO/examples/POA/Loader/Loader.dsw, POA Loader Server:
TAO/examples/POA/On_Demand_Activation/On_Demand_Activation.dsw, POA On Demand Activation Server:
TAO/examples/POA/On_Demand_Loading/On_Demand_Loading.dsw, POA On Demand Loading Server:
TAO/examples/POA/NewPOA/NewPOA.dsw, POA NewPOA:
TAO/examples/POA/TIE/TIE.dsw, POA TIE Server:
TAO/examples/POA/TIE/TIE.dsw, POA TIE Client:
TAO/examples/Simple/bank/bank.dsw, Simple Bank Server:
TAO/examples/Simple/bank/bank.dsw, Simple Bank Client:
TAO/examples/Simple/chat/chat.dsw, Simple Chat Server:
TAO/examples/Simple/chat/chat.dsw, Simple Chat Client:
TAO/examples/Simple/echo/Echo.dsw, Simple Echo Server:
TAO/examples/Simple/echo/Echo.dsw, Simple Echo Client:
TAO/examples/Simple/grid/Grid.dsw, Simple Grid Server:
TAO/examples/Simple/grid/Grid.dsw, Simple Grid Client:
TAO/examples/Simple/time/Time.dsw, Simple Time Server:
TAO/examples/Simple/time/Time.dsw, Simple Time Client:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Library:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Server:
TAO/examples/Simple/time-date/Time_Date.dsw, Simple Time Date Client:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Notifier:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Supplier:
TAO/examples/Event_Comm/Event_Comm.dsw, Event_Comm Consumer:
TAO/examples/OBV/Typed_Events/Typed_Events.dsw, OBV Typed Events Server:
TAO/examples/OBV/Typed_Events/Typed_Events.dsw, OBV Typed Events Client:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Server:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Client:
TAO/examples/Persistent_Grid/Persistent_Grid.dsw, Persistent Grid Second Client:
TAO/examples/Quoter/Quoter.dsw, Quoter Generic Factory:
TAO/examples/Quoter/Quoter.dsw, Quoter Factory Finder:
TAO/examples/Quoter/Quoter.dsw, Quoter Server:
TAO/examples/Quoter/Quoter.dsw, Quoter Client:
TAO/examples/Buffered_Oneways/Buffered_Oneways.dsw, Buffered_Oneways Server:
TAO/examples/Buffered_Oneways/Buffered_Oneways.dsw, Buffered_Oneways Client:
TAO/examples/Buffered_AMI/Buffered_AMI.dsw, Buffered_AMI Server:
TAO/examples/Buffered_AMI/Buffered_AMI.dsw, Buffered_AMI Client:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Server:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Client:
TAO/performance-tests/Cubit/TAO/IDL_Cubit/IDL_Cubit.dsw, IDL_Cubit Collocation Test:
TAO/performance-tests/Cubit/TAO/MT_Cubit/MT_Cubit.dsw, MT_Cubit Server:
TAO/performance-tests/Cubit/TAO/MT_Cubit/MT_Cubit.dsw, MT_Cubit Client:
TAO/performance-tests/POA/Object_Creation_And_Registration/registration.dsw, POA Registration:
TAO/performance-tests/Pluggable/Pluggable.dsw, Pluggable Server:
TAO/performance-tests/Pluggable/Pluggable.dsw, Pluggable Client:
TAO/performance-tests/RTCorba/Oneways/Reliable/Reliable.dsw, Reliable Oneways Server:
TAO/performance-tests/RTCorba/Oneways/Reliable/Reliable.dsw, Reliable Oneways Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Common/RTCORBA_Common.dsw, RTCORBA Test Common DLL:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Single_Endpoint/Single_Endpoint.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Single_Endpoint/Single_Endpoint.dsw, Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Orb_Per_Priority/ORB_Per_Priority.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Orb_Per_Priority/ORB_Per_Priority.dsw, Client:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Endpoint_Per_Priority/Project.dsw, Server:
TAO/performance-tests/RTCorba/Multiple_Endpoints/Endpoint_Per_Priority/Project.dsw, Client:
TAO/tests/CDR/CDR.dsw, CDR Basic Types:
TAO/tests/CDR/CDR.dsw, CDR Growth:
TAO/tests/CDR/CDR.dsw, CDR Octet_Sequence:
TAO/tests/CDR/CDR.dsw, CDR TC:
TAO/tests/IDL_Test/IDL_Test.dsw, idl_test:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq Server:
TAO/tests/OctetSeq/OctetSeq.dsw, OctetSeq Client:
TAO/tests/InterOp-Naming/InterOp-Naming.dsw, InterOp Naming Server:
TAO/tests/InterOp-Naming/InterOp-Naming.dsw, InterOp Naming Client:
TAO/tests/NestedUpcall/Simple/Simple.dsw, Nested Upcall Simple Server:
TAO/tests/NestedUpcall/Simple/Simple.dsw, Nested Upcall Simple Client:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Server_A:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Server_B:
TAO/tests/NestedUpcall/Triangle_Test/Triangle_Test.dsw, Nested Upcall Triangle Initiator:
TAO/tests/NestedUpcall/MT_Client_Test/MT_Client_Test.dsw, Nested Upcall MT_Client Server:
TAO/tests/NestedUpcall/MT_Client_Test/MT_Client_Test.dsw, Nested Upcall MT_Client Client:
TAO/tests/Multiple_Inheritance/Multiple_Inheritance.dsw, Multiple Inheritance Server:
TAO/tests/Multiple_Inheritance/Multiple_Inheritance.dsw, Multiple Inheritance Client:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client ORB Creation:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Server:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Simple Client:
TAO/tests/MT_Client/MT_Client.dsw, MT_Client Client:
TAO/tests/MT_Server/MT_Server.dsw, MT_Server Server:
TAO/tests/MT_Server/MT_Server.dsw, MT_Server Client:
TAO/tests/POA/Destruction/Destruction.dsw, POA Destruction:
TAO/tests/POA/Single_Threaded_POA/Single_Threaded_POA.dsw, POA Single_Threaded_POA:
TAO/tests/POA/Non_Servant_Upcalls/Non_Servant_Upcalls.dsw, POA Non_Servant_Upcalls:
TAO/tests/POA/Deactivation/Deactivation.dsw, POA Deactivation:
TAO/tests/POA/Identity/Identity.dsw, POA Identity:
TAO/tests/IORManipulation/IORManipulation.dsw, IORTest:
TAO/tests/Connection_Purging/Connection_Purging.dsw, POA Connection Purging:
TAO/tests/ORB_init/ORB_init.dsw, ORB_init:
TAO/tests/ORB_destroy/ORB_destroy.dsw, ORB_destroy:
TAO/tests/Nested_Event_Loop/Nested_Event_Loop.dsw, Nested_Event_Loop Server:
TAO/tests/Nested_Event_Loop/Nested_Event_Loop.dsw, Nested_Event_Loop Client:
TAO/tests/Leader_Followers/Leader_Followers.dsw, Leader_Followers Server:
TAO/tests/Leader_Followers/Leader_Followers.dsw, Leader_Followers Client:
TAO/tests/Object_Loader/Object_Loader.dsw, Test_DLL:
TAO/tests/Object_Loader/Object_Loader.dsw, Object_Loader Driver:
TAO/tests/DynAny_Test/DynAny_Test.dsw, DynAny Basics:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Server:
TAO/tests/Param_Test/Param_Test.dsw, Param_Test Client:
TAO/tests/Faults/Faults.dsw, Faults Server:
TAO/tests/Faults/Faults.dsw, Faults Client:
TAO/tests/Faults/Faults.dsw, Faults Ping:
TAO/tests/Faults/Faults.dsw, Faults Pong:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Server:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Client:
TAO/tests/AMI/ami_test.dsw, AMI_Test_Simple_Client:
TAO/tests/RTCORBA/RTMutex/RTMutex.dsw, RTMutex Server:
TAO/tests/Timeout/Timeout.dsw, Timeout Server:
TAO/tests/Timeout/Timeout.dsw, Timeout Client:
TAO/tests/Policies/Policies.dsw, Manipulation:
TAO/tests/Timed_Buffered_Oneways/Timed_Buffered_Oneways.dsw, Timed_Buffered_Oneways Server:
TAO/tests/Timed_Buffered_Oneways/Timed_Buffered_Oneways.dsw, Timed_Buffered_Oneways Client:
TAO/tests/Smart_Proxies/Smart_Proxies.dsw, server:
TAO/tests/Smart_Proxies/Smart_Proxies.dsw, client:
TAO/utils/catior/catior.dsw, catior:
TAO/utils/IOR-Parser/IOR_Parser.dsw, IOR Parser:
TAO/utils/nslist/nslist.dsw, nslist:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE DLL:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE DLL:
EOD
                  );

%Alpha_DLL_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE DLL:
apps/gperf/src/gperf.dsw, gperf:
TAO/tao/TAO.dsw, TAO DLL:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services DLL:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
tests/tests.dsw, ALL:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE DLL:
apps/gperf/src/gperf.dsw, gperf:
TAO/tao/TAO.dsw, TAO DLL:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler:
TAO/orbsvcs/orbsvcs/orbsvcs.dsw, ORB Services DLL:
TAO/orbsvcs/Naming_Service/Naming_Service.dsw, Naming_Service:
netsvcs/lib/netsvcs.dsw, NETSVCS Library:
netsvcs/servers/servers.dsw, NETSVCS Servers:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE DLL:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE DLL:
EOD
                  );

%Win32_Lib_Collections = ( "$Win32_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE LIB:
TAO/tao/TAO.dsw, TAO LIB:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs_static.dsp, ORB Services Static:
EOD
                     "$Win32_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE LIB:
TAO/tao/TAO.dsw, TAO LIB:
TAO/TAO_IDL/tao_idl.dsw, TAO_IDL Compiler Static:
TAO/orbsvcs/orbsvcs/orbsvcs_static.dsp, ORB Services Static:
EOD
                     "$Win32_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE LIB:
EOD
                     "$Win32_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE LIB:
EOD
                  );

%Alpha_Lib_Collections = ( "$Alpha_Lists[0]" => <<EOD , # Debug
ace/ace.dsw, ACE LIB:
EOD
                     "$Alpha_Lists[1]" => <<EOD , # Release
ace/ace.dsw, ACE LIB:
EOD
                     "$Alpha_Lists[2]" => <<EOD , # Unicode Debug
ace/ace.dsw, ACE LIB:
EOD
                     "$Alpha_Lists[3]" => <<EOD # Unicode Release
ace/ace.dsw, ACE LIB:
EOD
                  );



sub Build_Config
{
    my $Config = shift;
    my $Collection = shift;
    my $Bname = "";
    my $Project_File = "";
    my $Project_Name = "";
    my $Status = -1;
    my $Cntr = 0;
    my $Command_Line = "";

    @Collection = split /:\s*/, $Collection;
    for ($Cntr = 0; $Cntr < scalar (@Collection); $Cntr++)
    {
        $Project_Dir = $Collection[$Cntr];
        $Project_Dir =~ s/(.*)\/[^\/]*/$1/;
        $Bname = $Collection[$Cntr];
        $Bname =~ s/.*\/([^\/]*)/$1/;
        ($Project_File, $Project_Name) = split /,\s*/, $Bname;
        chdir ("$ENV{ACE_ROOT}/$Project_Dir");
        $Command_Line =  "msdev.com $Project_File /MAKE \"$Project_Name - $Config\" /USEENV $Build_Cmd /Y3";
        print "Auto_compiling $Project_File : $Project_Name - $Config\n";
        if ( $Debug == 0 )
        {
            $Status =
                system "$Command_Line";
        }
        else
        {
            $Status = 0;
            print "*****************************************\n";
            print "Project_Dir: $Project_Dir -- Project_Name: $Project_Name\n";
            print "chdir (\"$ENV{ACE_ROOT}/$Project_Dir\");\n";
            print "$Status = $Command_Line\n\n";
        }

        if ($Ignore_error == 0)
        {
            return if $Status != 0;
        }
    }
}

sub Build_Version_Test
{
    my $Cntr = shift;

    print "Building Version Test $Version_Test_Target[$Cntr]\n" if ($Verbose == 1);

    chdir ("$ENV{ACE_ROOT}/tests/version_tests");
    $Command_Line =  "msdev.com version_tests.dsw /MAKE \"ALL - $Version_Test_Target[$Cntr]\" /USEENV $Build_Cmd /Y3";
    if ( $Debug == 0 )
    {
        $Status =
            system "$Command_Line";
    }
    else
    {
        $Status = 0;
        print "$Status = $Command_Line\n";
    }

    if ($Ignore_error == 0)
    {
        return if $Status != 0;
    }
}

sub Build_Collection
{
    print "Build_Collection\n" if ( $Verbose );
    my $Cntr = 0;
    for (; $Cntr < scalar(@Lists); $Cntr ++)
    {
        $Config = $Lists[$Cntr];
        print "Building $Config of: \n$Target{$Config}\n" if ( $Debug );
        Build_Config ($Config, $Target{$Config});
        if ($Build_Alpha == 0)
        {
            Build_Version_Test ($Cntr);
        }
    }
}


$Verbose = 0;
$Debug = 0;
$Ignore_error = 0;              # By default, bail out if an error occurs.
$Build_Alpha = 0;
$Build_DLL = 1;
$Build_LIB = 1;
$Build_Cmd = "/BUILD";
@Lists = @Win32_Lists;
%DLL_Collections = %Win32_DLL_Collections;
%Lib_Collections = %Win32_Lib_Collections;

## Parse command line argument
while ( $#ARGV >= 0  &&  $ARGV[0] =~ /^-/ )
{
    if ( $ARGV[0] eq '-k' )     # Ignore error.  Compile the whole thing
    {
        print "Ignore errors\n" if ( $Verbose );
        $Ignore_error = 1;      # in the same configuration.
    }
    elsif ( $ARGV[0] eq '-a' )  # Use Alpha
    {
        print "Build Alpha\n" if ( $Verbose );
        $Build_Alpha = 1;
        @Lists = @Alpha_Lists;
        %DLL_Collections = %Alpha_DLL_Collections;
        %Lib_Collections = %Alpha_Lib_Collections;
    }
    elsif ( $ARGV[0] eq '-d')   # Script debugging mode
    {
        print "Debug mode on\nVerbose mode on\n";
        $Debug = 1;
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-C')   # Script debugging mode
    {
        shift;
        $active_config = $ARGV[0];
        print "Select $Lists[$active_config]\n";
        @Lists = ( $Lists[$active_config] );
        @Win32_DLL_Version_Tests_Configs =
            ( $Win32_DLL_Version_Tests_Configs[$active_config] );
        @Win32_LIB_Version_Tests_Configs =
            ( $Win32_LIB_Version_Tests_Configs[$active_config] );
    }
    elsif ( $ARGV[0] eq '-D' )  # Build DLL only
    {
        print "Build DLL only\n" if ( $Verbose );
        $Build_LIB = 0;
    }
    elsif ( $ARGV[0] eq '-L' )  # Build LIB only
    {
        print "Build LIB only\n" if ( $Verbose );
        $Build_DLL = 0;
    }
    elsif ( $ARGV[0] eq '-v' )   # Verbose mode
    {
        $Verbose = 1;
    }
    elsif ( $ARGV[0] eq '-r' )  # Rebuild all
    {
        print "Rebuild all\n" if ( $Verbose );
        $Build_Cmd = "/REBUILD";
    }
    elsif ( $ARGV[0] eq '-c' )  # Rebuild all
    {
        print "Cleaning all\n" if ( $Verbose );
        $Build_Cmd = "/CLEAN";
    }
    else
    {
        warn "$0:  unknown option $ARGV[0]\n";
        die $usage;
    }
    shift;
}

if ( $Verbose )
{
    for ($II = 0; $II < scalar (@Lists); $II++)
    {
        printf "$Lists[$II]\n";
    }
}

if ( $Build_DLL )
{
    print "Building DLL\n" if ( $Verbose );
    %Target = %DLL_Collections;
    @Version_Test_Target = @Win32_DLL_Version_Tests_Configs;
    Build_Collection;
}

if ( $Build_LIB )
{
    print "Building LIB\n" if ( $Verbose );
    %Target = %Lib_Collections;
    @Version_Test_Target = @Win32_LIB_Version_Tests_Configs;
    Build_Collection;
}

print "End\n";





