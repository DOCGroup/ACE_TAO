#!/pkg/gnu/bin/perl -pi
s/ACE_Service_Config::run_reactor_event_loop/ACE_Reactor::run_event_loop/g;
s/ACE_Service_Config::end_reactor_event_loop/ACE_Reactor::end_event_loop/g;
s/ACE_Service_Config::reactor_event_loop_done/ACE_Reactor::event_loop_done/g;
s/ACE_Service_Config::run_proactor_event_loop/ACE_Proactor::run_proactor_event_loop/g;
s/ACE_Service_Config::end_proactor_event_loop/ACE_Proactor::end_proactor_event_loop/g;
s/ACE_Service_Config::proactor_event_loop_done/ACE_Proactor::event_loop_done/g;
s/ACE_Service_Config::run_reactorEx_event_loop/ACE_ReactorEx::run_event_loop/g;
s/ACE_Service_Config::end_reactorEx_event_loop/ACE_ReactorEx::end_event_loop/g;
s/ACE_Service_Config::reactorEx_event_loop_done/ACE_ReactorEx::event_loop_done/g;
s/ACE_Service_Config::reactor/ACE_Reactor::instance/g;
s/ACE_Service_Config::proactor/ACE_Proactor::instance/g;
s/ACE_Service_Config::reactorEx/ACE_ReactorEx::instance/g;
s/ACE_Service_Config::svc_rep/ACE_Service_Repository::instance/g;
s/ACE_Service_Config::thr_mgr/ACE_Thread_Manager::instance/g;
s/ACE_Service_Config::alloc/ACE_Allocator::instance/g;

