//
// $Id$
//

#if defined (sun) || defined (__osf__)
// @@ TODO This should be encapsulated in ACE...
#include <sys/types.h>
#include <sys/priocntl.h>
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>
#endif /* sun || __osf__ */

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "EC_Multiple.h"

#include "Scheduler_Runtime1.h"
#include "Scheduler_Runtime2.h"

Test_ECG::Test_ECG (void)
  : consumer_ (this),
    supplier_ (this),
    lcl_ec_name_ ("EC"),
    rmt_ec_name_ (0),
    lcl_sch_name_ ("ScheduleService"),
    rmt_sch_name_ (0),
    global_scheduler_ (0),
    short_circuit_ (0),
    interval_ (250000),
    message_count_ (Test_ECG::DEFAULT_EVENT_COUNT),
    event_a_ (0),
    event_b_ (0),
    event_c_ (0),
    lcl_count_ (0),
    rmt_count_ (0),
    scavenger_barrier_ (2),
    scavenger_count_ (0),
    scavenger_priority_ (0),
    push_count_ (1),
    schedule_file_ (0),
    pid_file_name_ (0)
{
}



void
print_priority_info (const char *const name)
{
#if defined (sun) || defined (__osf__)
  pcinfo_t pcinfo;
  id_t ts_id, rt_id;
  pcparms_t pcparms;
  rtparms_t rtparms;
  struct sched_param param;
  int policy, status;

  if ((status = pthread_getschedparam (pthread_self (), &policy,
                                       &param)) == 0) {
#   ifdef sun
    ACE_DEBUG ((LM_DEBUG,
		"%s (%lu|%u|%u); policy is %d, priority is %d\n",
		name,
		getpid (),
		_lwp_self (),
		pthread_self (),
		policy, param.sched_priority));
#   else  /* ! sun */
    ACE_DEBUG ((LM_DEBUG,
		"%s (%lu|%u); policy is %d, priority is %d\n",
		name,
		getpid (),
		0,
		pthread_self (),
		policy, param.sched_priority ));
#   endif /* ! sun */
  } else {
    ACE_DEBUG ((LM_DEBUG,"pthread_getschedparam failed: %d\n", status));
  }

  // Get the class TS and RT class IDs.

  memset (&pcinfo, 0, sizeof pcinfo);
  strcpy (pcinfo.pc_clname, "TS");
  if (priocntl (P_ALL /* ignored */,
                P_MYID /* ignored */,
                PC_GETCID,
                (char *) &pcinfo) == -1)
    return;
  ts_id = pcinfo.pc_cid;

  memset (&pcinfo, 0, sizeof pcinfo);
  strcpy (pcinfo.pc_clname, "RT");
  if (priocntl (P_ALL /* ignored */,
                P_MYID /* ignored */,
                PC_GETCID,
                (char *) &pcinfo) == -1)
    return;
  rt_id = pcinfo.pc_cid;

  /* The following is just to avoid Purify warnings about unitialized
     memory reads. */
  memset (&pcparms, 0, sizeof pcparms);
  pcparms.pc_cid = PC_CLNULL;

#ifdef sun
  if (priocntl (P_LWPID,
                P_MYID,
                PC_GETPARMS,
                (char *) &pcparms) == -1) {
    perror ("priocntl: PCGETPARMS");
  } else {
    ACE_DEBUG ((LM_DEBUG,
		"%s class: %s", name,
		pcparms.pc_cid == rt_id ? "RT" :
		(pcparms.pc_cid == ts_id ? "TS" : "UNKNOWN")));
    if (pcparms.pc_cid == rt_id) {
      /* RT class */
      memcpy (&rtparms, pcparms.pc_clparms, sizeof rtparms);

      ACE_DEBUG ((LM_DEBUG,
		  "; priority: %d, quantum: %lu sec, %ld nsec\n",
		  rtparms.rt_pri, rtparms.rt_tqsecs, rtparms.rt_tqnsecs));
    } else {
      ACE_DEBUG ((LM_DEBUG, "\n"));
    }
  }
#endif /* sun */
#endif /* sun || Digital Unix 4.0 */
}




int
Test_ECG::run (int argc, char* argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->parse_args (argc, argv))
        return 1;

      ACE_DEBUG ((LM_DEBUG,
		  "Execution paramaters:\n"
		  "  local EC name = <%s>\n"
		  "  remote EC name = <%s>\n"
		  "  local Scheduler name = <%s>\n"
		  "  remote Scheduler name = <%s>\n"
		  "  global scheduler = <%d>\n"
		  "  scavenger work = <%d>\n"
		  "  push work = <%d>\n" 
		  "  short circuit EC = <%d>\n"
		  "  interval between events = <%d> (usecs)\n"
		  "  message count = <%d>\n"
		  "  schedule_file = <%s>\n"
		  "  pid file name = <%s>\n"
		  "  event A = <%d>\n"
		  "  event B = <%d>\n"
		  "  event C = <%d>\n",
		  this->lcl_ec_name_,
		  this->rmt_ec_name_?this->rmt_ec_name_:"nil",
		  this->lcl_sch_name_,
		  this->rmt_sch_name_?this->rmt_sch_name_:"nil",
		  this->global_scheduler_,
		  this->scavenger_count_,
		  this->push_count_,
		  this->short_circuit_,
		  this->interval_,
		  this->message_count_,
		  this->schedule_file_?this->schedule_file_:"nil",
		  this->pid_file_name_?this->pid_file_name_:"nil",
		  this->event_a_,
		  this->event_b_,
		  this->event_c_) );

      print_priority_info ("Test_ECG::run (Main)");

      if (this->pid_file_name_ != 0)
	{
	  FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
	  if (pid != 0)
	    {
	      ACE_OS::fprintf (pid, "%d\n", ACE_OS::getpid ());
	      ACE_OS::fclose (pid);
	    }
	}

      this->scavenger_priority_ = 
	ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);

      // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
						  this->scavenger_priority_,
						  ACE_SCOPE_PROCESS)) != 0)
	{
	  if (ACE_OS::last_error () == EPERM)
	    ACE_DEBUG ((LM_DEBUG,
			"%s: user is not superuser, "
			"so remain in time-sharing class\n", argv[0]));
	  else
	    ACE_ERROR ((LM_ERROR,
			"%s: ACE_OS::sched_params failed\n", argv[0]));
	}

      int next_priority = 
	ACE_Sched_Params::next_priority (this->scavenger_priority_,
					 ACE_SCHED_FIFO,
					 ACE_SCOPE_THREAD);
      if (ACE_OS::thr_setprio (next_priority) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed\n"));
	}

      print_priority_info ("Test_ECG::run (Main)");

      if (this->scavenger_count_ != 0)
	{
	  if (this->activate (THR_BOUND|ACE_SCHED_FIFO,
			      1, 0, this->scavenger_priority_) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       " (%P|%t) Cannot activate scavenger.\n"),
			      1);
	}

      if (this->short_circuit_ != 0)
	{
	  if (this->run_short_circuit (orb.in (),
				       root_poa.in (),
				       poa_manager.in (),
				       TAO_TRY_ENV) == -1)
	    return 1;
	}
      else if (this->run_ec (orb.in (),
			     root_poa.in (),
			     poa_manager.in (),
			     TAO_TRY_ENV) == -1)
	{
	  return 1;
	}
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NON SYS EX");
    }
  TAO_ENDTRY;
  return 0;
}

int
Test_ECG::run_short_circuit (CORBA::ORB_ptr orb,
			     PortableServer::POA_ptr ,
			     PortableServer::POAManager_ptr ,
			     CORBA::Environment& _env)
{
  TAO_TRY
    {
      ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();
      ACE_Time_Value tv (this->interval_ / ACE_ONE_SECOND_IN_USECS,
			 (this->interval_ % ACE_ONE_SECOND_IN_USECS));
      reactor->schedule_timer (this, 0, tv);

      this->supplier_id_ = 0;

      ACE_DEBUG ((LM_DEBUG, "running short circuit test\n"));
      if (this->scavenger_count_ != 0)
	{
	  if (this->scavenger_barrier_.wait () == -1)
	    return -1;
	}

      while (this->message_count_ > 0 && orb->run () != -1)
	{
	  // ACE_DEBUG ((LM_DEBUG, "orb run returns\n"));
	}
      // ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      // This should be in the run method, but the EC crashes on
      // shutdown (sometimes).
      ACE_DEBUG ((LM_DEBUG, "waiting for scavenger\n"));
      if (this->wait () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) waiting for scavenger tasks\n"), 1);

      this->dump_results ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("run_short_circuit");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Test_ECG::run_ec (CORBA::ORB_ptr orb,
		  PortableServer::POA_ptr ,
		  PortableServer::POAManager_ptr poa_manager,
		  CORBA::Environment& _env)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      auto_ptr<ACE_Config_Scheduler> scheduler_impl;
      if (this->global_scheduler_)
	{
	  if (ACE_Scheduler_Factory::use_config (naming_context.in ()) == -1)
	    return -1;
	}
      else
	{
	  int config = 0;
	  RtecScheduler::Scheduler_var scheduler;
	  if (ACE_OS::strcmp (this->lcl_sch_name_, "RUNTIME1") == 0)
	    {
	      ACE_Scheduler_Factory::use_runtime
		(sizeof(runtime_infos_1)/sizeof(runtime_infos_1[0]),
		 runtime_infos_1);
	      scheduler = RtecScheduler::Scheduler::_duplicate (ACE_Scheduler_Factory::server ());
	    }
	  else if (ACE_OS::strcmp (this->lcl_sch_name_, "RUNTIME2") == 0)
	    {
	      ACE_Scheduler_Factory::use_runtime
		(sizeof(runtime_infos_2)/sizeof(runtime_infos_2[0]),
		 runtime_infos_2);
	      scheduler = RtecScheduler::Scheduler::_duplicate (ACE_Scheduler_Factory::server ());
	    }
	  else
	    {
	      scheduler_impl =
		auto_ptr<ACE_Config_Scheduler>(new ACE_Config_Scheduler);
	      if (scheduler_impl.get () == 0)
		return -1;
	      scheduler =
		scheduler_impl->_this (TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	      config = 1;
	    }
	  CORBA::String_var str =
	    orb->object_to_string (scheduler.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
		      str.in ()));

	  // Register the servant with the Naming Context....
	  CosNaming::Name schedule_name (1);
	  schedule_name.length (1);
	  schedule_name[0].id = CORBA::string_dup (this->lcl_sch_name_);
	  naming_context->bind (schedule_name, scheduler.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (config == 1)
	    {
	      if (ACE_Scheduler_Factory::use_config (naming_context.in (),
						     this->lcl_sch_name_) == -1)
		return -1;
	    }
	}

      if (ACE_Scheduler_Factory::use_config (naming_context.in (),
					     this->lcl_sch_name_) == -1)
	return -1;

      // Register Event_Service with Naming Service.
      ACE_EventChannel ec_impl;

      RtecEventChannelAdmin::EventChannel_var ec =
	ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "The (local) EC IOR is <%s>\n", str.in ()));

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (this->lcl_ec_name_);
      naming_context->bind (channel_name, ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "waiting to start\n"));
      ACE_Time_Value tv (15, 0);

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (orb->run (&tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      ACE_DEBUG ((LM_DEBUG, "starting....\n"));

      RtecEventChannelAdmin::EventChannel_var local_ec =
        this->get_ec (naming_context.in (),
                      this->lcl_ec_name_,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "located local EC\n"));

      if (this->connect_supplier (local_ec.in (), TAO_TRY_ENV) == -1)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "connected supplier\n"));

      tv.set (5, 0);
      if (orb->run (&tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      if (this->rmt_ec_name_ != 0)
	{
	  RtecEventChannelAdmin::EventChannel_var remote_ec =
	    this->get_ec (naming_context.in (),
			  this->rmt_ec_name_,
			  TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_DEBUG ((LM_DEBUG, "located remote EC\n"));

	  CosNaming::Name rsch_name (1);
	  rsch_name.length (1);
	  if (this->rmt_sch_name_ != 0
	      && !this->global_scheduler_)
	    {
	      rsch_name[0].id = CORBA::string_dup (this->rmt_sch_name_);
	    }
	  else
	    {
	      rsch_name[0].id = CORBA::string_dup (this->lcl_sch_name_);
	    }
	  CORBA::Object_var tmpobj =
	    naming_context->resolve (rsch_name, TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  RtecScheduler::Scheduler_var remote_sch =
	    RtecScheduler::Scheduler::_narrow (tmpobj.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (this->connect_ecg (local_ec.in (),
				 remote_ec.in (),
				 remote_sch.in (),
				 TAO_TRY_ENV) == -1)
	    return 1;

	  ACE_DEBUG ((LM_DEBUG, "connected proxy\n"));

	  tv.set (5, 0);
	  if (orb->run (&tv) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);
	}

      if (this->connect_consumer (local_ec.in (), TAO_TRY_ENV) == -1)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "connected consumer\n"));

      if (this->scavenger_count_ != 0)
	{
	  if (this->scavenger_barrier_.wait () == -1)
	    return -1;
	}

      ACE_DEBUG ((LM_DEBUG, "running EC test\n"));
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      // Wait for EC threads to terminate....
      ACE_DEBUG ((LM_DEBUG, "waiting for EC threads\n"));
      if (ACE_Thread_Manager::instance ()->wait () == -1)
	return -1;

      ACE_DEBUG ((LM_DEBUG, "shutdown grace period\n"));
      tv.set (5, 0);
      if (orb->run (&tv) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      // This should be in the run method, but the EC crashes on
      // shutdown (sometimes).
      ACE_DEBUG ((LM_DEBUG, "waiting for scavenger\n"));
      if (this->wait () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) waiting for scavenger tasks\n"), 1);

      this->dump_results ();

      if (this->schedule_file_ != 0)
	{
	  RtecScheduler::RT_Info_Set_var infos;

#if defined (__SUNPRO_CC)
	  // Sun C++ 4.2 warns with the code below:
	  //   Warning (Anachronism): Temporary used for non-const
	  //   reference, now obsolete.
	  //   Note: Type "CC -migration" for more on anachronisms.
	  //   Warning (Anachronism): The copy constructor for argument
	  //   infos of type RtecScheduler::RT_Info_Set_out should take
	  //   const RtecScheduler::RT_Info_Set_out&.
	  // But, this code is not CORBA conformant, because users should
	  // not define instances of _out types.

	  RtecScheduler::RT_Info_Set_out infos_out (infos);
	  ACE_Scheduler_Factory::server ()->compute_scheduling
	    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
					     ACE_SCOPE_THREAD),
	     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
					     ACE_SCOPE_THREAD),
	     infos_out, TAO_TRY_ENV);
#else  /* ! __SUNPRO_CC */
	  ACE_Scheduler_Factory::server ()->compute_scheduling
	    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
					     ACE_SCOPE_THREAD),
	     ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
					     ACE_SCOPE_THREAD),
	     infos.out (), TAO_TRY_ENV);
#endif /* ! __SUNPRO_CC */

	  TAO_CHECK_ENV;
	  ACE_Scheduler_Factory::dump_schedule (infos.in (),
						this->schedule_file_);
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("run_ec");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Test_ECG::svc (void)
{
  print_priority_info ("Test_ECG::svc (Scavenger)");

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger svc\n"));
  if (ACE_OS::thr_setprio (this->scavenger_priority_) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) scavenger thr_setprio failed\n"));
    }
  print_priority_info ("Test_ECG::svc (Scavenger)");

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger waiting\n"));
  if (this->scavenger_barrier_.wait () == -1)
    return -1;

  this->scavenger_timer_.start ();
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting scavenger\n"));

  for (int i = 0;
       i < this->scavenger_count_;
       ++i)
    {
      u_long n = 1279UL;
      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
      ACE::is_prime (n, 2, n / 2);
#if 0
      if (i % 100 == 0)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger iteration: %d\n",
		    i));
#endif
    }
  this->scavenger_timer_.stop ();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger finished its work\n"));
  return 0;
}

RtecEventChannelAdmin::EventChannel_ptr
Test_ECG::get_ec (CosNaming::NamingContext_ptr naming_context,
                  const char* ec_name,
                  CORBA::Environment &_env)
{
  CosNaming::Name channel_name (1);
  channel_name.length (1);
  channel_name[0].id = CORBA::string_dup (ec_name);

  CORBA::Object_ptr ec_ptr =
    naming_context->resolve (channel_name, _env);
  if (_env.exception () != 0 || CORBA::is_nil (ec_ptr))
    return RtecEventChannelAdmin::EventChannel::_nil ();

  return RtecEventChannelAdmin::EventChannel::_narrow (ec_ptr, _env);
}

int
Test_ECG::connect_supplier (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                            CORBA::Environment &_env)
{
  TAO_TRY
    {
      char buf[BUFSIZ];
      ACE_OS::strcpy (buf, "supplier@");
      ACE_OS::strcat (buf, this->lcl_ec_name_);

      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      RtecScheduler::handle_t rt_info =
        server->create (buf, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The worst case execution time is far less than 2
      // mislliseconds, but that is a safe estimate....
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   time, time, time,
		   // @@ Make it parametric; the number below is the
		   // maximum rate in the EC....
                   25000 * 10,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
		   time,
		   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_id_ = ACE::crc32 (buf);

      ACE_SupplierQOS_Factory qos;
      qos.insert (this->supplier_id_,
		  this->event_a_,
                  rt_info, 1);
      qos.insert (this->supplier_id_,
                  this->event_b_,
                  rt_info, 1);
      qos.insert (this->supplier_id_,
                  ACE_ES_EVENT_SHUTDOWN,
                  rt_info, 1);

      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        local_ec->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var objref =
        this->supplier_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                    qos.get_SupplierQOS (),
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Test_ECG::connect_consumer (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                            CORBA::Environment &_env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      char buf[BUFSIZ];
      ACE_OS::strcpy (buf, "consumer@");
      ACE_OS::strcat (buf, this->lcl_ec_name_);

      RtecScheduler::handle_t rt_info =
        server->create (buf, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The worst case execution time is far less than 2
      // mislliseconds, but that is a safe estimate....
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   time, time, time,
                   25000 * 10, // @@ Make it parametric
                   RtecScheduler::VERY_LOW_IMPORTANCE,
		   time,
		   0,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Time_Value tv_timeout (this->interval_ / ACE_ONE_SECOND_IN_USECS,
				 this->interval_ % ACE_ONE_SECOND_IN_USECS);
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout, tv_timeout);

      ACE_ConsumerQOS_Factory qos;
      qos.start_disjunction_group ();
      qos.insert_type (ACE_ES_EVENT_SHUTDOWN,
                       rt_info);
      qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                       timeout,
                       rt_info);
      qos.insert_type (this->event_a_, rt_info);
      qos.insert_type (this->event_c_, rt_info);

      // = Connect as a consumer.
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        local_ec->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var objref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                    qos.get_ConsumerQOS (),
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;

  return 0;
}

int
Test_ECG::connect_ecg (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                       RtecEventChannelAdmin::EventChannel_ptr remote_ec,
		       RtecScheduler::Scheduler_ptr remote_sch,
                       CORBA::Environment &_env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr local_sch =
        ACE_Scheduler_Factory::server ();

      // ECG name.
      char ecg_name[BUFSIZ];
      ACE_OS::strcpy (ecg_name, "ecg_");
      ACE_OS::strcat (ecg_name, this->lcl_ec_name_);

      // Generate its ConsumerQOS

      // We could use the same name on the local and remote scheduler,
      // but that fails when using a global scheduler.
      char rmt[BUFSIZ];
      ACE_OS::strcpy (rmt, ecg_name);
      ACE_OS::strcat (rmt, "@");
      ACE_OS::strcat (rmt, this->rmt_ec_name_);

      RtecScheduler::handle_t rmt_info =
        remote_sch->create (rmt, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The worst case execution time is far less than 500 usecs, but
      // that is a safe estimate.... 
      ACE_Time_Value tv (0, 500);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      remote_sch->set (rmt_info,
		       RtecScheduler::VERY_HIGH_CRITICALITY,
		       time, time, time,
		       25000 * 10, // @@ Make it parametric
		       RtecScheduler::VERY_LOW_IMPORTANCE,
		       time,
		       0,
		       RtecScheduler::OPERATION,
		       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_ConsumerQOS_Factory consumer_qos;
      consumer_qos.start_disjunction_group ();
      consumer_qos.insert_type (this->event_a_, rmt_info);
      consumer_qos.insert_type (this->event_c_, rmt_info);
      consumer_qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rmt_info);

      // Generate its SupplierQOS

      // We could use the same name on the local and remote scheduler,
      // but that fails when using a global scheduler.
      char lcl[BUFSIZ];
      ACE_OS::strcpy (lcl, ecg_name);
      ACE_OS::strcat (lcl, "@");
      ACE_OS::strcat (lcl, this->lcl_ec_name_);

      RtecScheduler::handle_t lcl_info =
        local_sch->create (lcl, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      local_sch->set (lcl_info,
		      RtecScheduler::VERY_HIGH_CRITICALITY,
		      time, time, time,
		      25000 * 10,
		      RtecScheduler::VERY_LOW_IMPORTANCE,
		      time,
		      1,
		      RtecScheduler::OPERATION,
		      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::ULong supplier_id = ACE::crc32 (lcl);

      ACE_SupplierQOS_Factory supplier_qos;
      supplier_qos.insert (supplier_id,
                           this->event_a_,
                           lcl_info, 1);
      supplier_qos.insert (supplier_id,
                           this->event_c_,
                           lcl_info, 1);
      supplier_qos.insert (supplier_id,
			   ACE_ES_EVENT_SHUTDOWN,
			   lcl_info, 1);

      this->ecg_.open (remote_ec, local_ec,
                       consumer_qos.get_ConsumerQOS (),
                       supplier_qos.get_SupplierQOS (),
                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;

  return 0;
}

void
Test_ECG::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier-consumer received disconnect from channel.\n"));
}

void
Test_ECG::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier received disconnect from channel.\n"));
}

void
Test_ECG::push (const RtecEventComm::EventSet &events,
                CORBA::Environment & _env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC_Proxy::push - "));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  this->push_timer_.start_incr ();
  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];
      if (e.type_ == ACE_ES_EVENT_INTERVAL_TIMEOUT)
        {
	  this->message_count_--;

          RtecEventComm::EventSet sent (1);
	  sent.length (1);

	  RtecEventComm::Event& s = sent[0];
	  s.source_ = this->supplier_id_;
	  s.ttl_ = 1;

	  ACE_hrtime_t t = ACE_OS::gethrtime ();
	  ORBSVCS_Time::hrtime_to_TimeT (s.creation_time_, t);
	  s.ec_recv_time_ = ORBSVCS_Time::zero;
	  s.ec_send_time_ = ORBSVCS_Time::zero;

	  s.data_.x = 0;
	  s.data_.y = 0;

	  if (this->message_count_ == 0)
	    {
	      s.type_ = ACE_ES_EVENT_SHUTDOWN;
	    }
	  else if (this->message_count_ % 2 == 0)
	    {
	      // Generate an A event...
	      s.type_ = this->event_a_;
	    }
	  else
	    {
	      s.type_ = this->event_b_;
	    }

	  this->consumer_proxy_->push (sent, _env);
          //ACE_DEBUG ((LM_DEBUG, "Sent %d events\n", n));
        }
      else if (e.type_ == ACE_ES_EVENT_SHUTDOWN)
        {
          this->shutdown (_env);
        }
      else
        {
          // Print out the events received...
	  // ACE_DEBUG ((LM_DEBUG, "Received event %d from %04.4x\n",
	  // e.type_, e.source_));

	  // @@ TODO Keep statistics....
	  ACE_hrtime_t r = ACE_OS::gethrtime ();
	  ACE_hrtime_t s;
	  ORBSVCS_Time::TimeT_to_hrtime (s, e.creation_time_);
	  ACE_hrtime_t nsec = r - s;
	  if (this->supplier_id_ == e.source_)
	    {
	      this->lcl_time_[this->lcl_count_] = nsec;
	      this->lcl_count_++;
	      // ACE_DEBUG ((LM_DEBUG, "Latency[LOCAL]: %d\n",
	      // nsec));
	    }
	  else
	    {
	      this->rmt_time_[this->rmt_count_] = nsec;
	      this->rmt_count_++;
	      // ACE_DEBUG ((LM_DEBUG, "Latency[REMOTE]: %d\n",
	      // nsec));
	    }

	  for (int j = 0; j < this->push_count_; ++j)
	    {
	      // Eat a little CPU so the Utilization test can measure the
	      // consumed time....
	      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
	      u_long n = 1279UL;
	      ACE::is_prime (n, 2, n / 2);
	    }
        }
    }
  this->push_timer_.stop_incr ();
}

int
Test_ECG::shutdown (CORBA::Environment& _env)
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the multiple EC test\n"));

  this->consumer_proxy_->disconnect_push_consumer (_env);
  if (_env.exception () != 0) return -1;

  this->supplier_proxy_->disconnect_push_supplier (_env);
  if (_env.exception () != 0) return -1;

  if (this->rmt_ec_name_ != 0)
    {
      this->ecg_.shutdown (_env);
      if (_env.exception () != 0) return -1;
    }

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return 0;
}

int
Test_ECG::handle_timeout (const ACE_Time_Value &,
			  const void*)
{
  this->message_count_--;
  if (this->message_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB core\n"));
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      return 0;
    }

  ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();
  ACE_Time_Value interval (this->interval_ / ACE_ONE_SECOND_IN_USECS,
			   this->interval_ % ACE_ONE_SECOND_IN_USECS);
  reactor->schedule_timer (this, 0, interval);
  RtecEventComm::EventSet sent (1);
  sent.length (1);

  RtecEventComm::Event& s = sent[0];
  s.source_ = this->supplier_id_;
  s.ttl_ = 1;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (s.creation_time_, t);
  s.ec_recv_time_ = ORBSVCS_Time::zero;
  s.ec_send_time_ = ORBSVCS_Time::zero;

  s.data_.x = 0;
  s.data_.y = 0;

  if (this->message_count_ % 2 == 0)
    {
      // Generate an A event...
      s.type_ = this->event_a_;
    }
  else
    {
      s.type_ = this->event_b_;
    }

#if 0
  if (this->message_count_ % 100 == 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) handle_timeout %d more msgs\n",
		this->message_count_));
#endif

  CORBA::Environment env;
  this->push (sent, env);
  if (env.exception () != 0)
    return -1;

  return 0;
}

void
Test_ECG::dump_results (void)
{
  int i;
  for (i = 0; i < this->lcl_count_; ++i)
    {
      double usec = this->lcl_time_[i] / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[LCL]: %.3f\n", usec));
    }
  for (i = 0; i < this->rmt_count_; ++i)
    {
      double usec = this->rmt_time_[i] / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[RMT]: %.3f\n", usec));
    }

  ACE_Time_Value tv;
  if (this->scavenger_count_ != 0)
    {
      this->scavenger_timer_.elapsed_time (tv);
      double usec = tv.sec () * ACE_ONE_SECOND_IN_USECS + tv.usec ();
      ACE_DEBUG ((LM_DEBUG, "Scavenger time: %.3f\n", usec));
    }

  this->push_timer_.elapsed_time_incr (tv);
  double usec = tv.sec () * ACE_ONE_SECOND_IN_USECS + tv.usec ();
  ACE_DEBUG ((LM_DEBUG, "Push time: %.3f\n", usec));
}

int
Test_ECG::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "tgxw:p:d:l:r:s:o:i:m:u:a:b:c:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'l':
          this->lcl_ec_name_ = get_opt.optarg;
          break;
        case 'r':
          this->rmt_ec_name_ = get_opt.optarg;
          break;
	case 's':
	  this->lcl_sch_name_ = get_opt.optarg;
	  break;
	case 'o':
	  this->rmt_sch_name_ = get_opt.optarg;
	  break;
	case 'g':
	  this->global_scheduler_ = 1;
	  break;
	case 'u':
	  this->scavenger_count_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'x':
	  this->short_circuit_ = 1;
	  break;
	case 'w':
	  this->push_count_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'p':
	  this->pid_file_name_ = get_opt.optarg;
	  break;
	case 'd':
	  this->schedule_file_ = get_opt.optarg;
	  break;
        case 'i':
          this->interval_ = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'm':
          this->message_count_ = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'a':
          this->event_a_ =
	    ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (get_opt.optarg);
          break;
        case 'b':
          this->event_b_ =
	    ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (get_opt.optarg);
          break;
        case 'c':
          this->event_c_ =
	    ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (get_opt.optarg);
          break;
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
		      "[ORB options] "
                      "-l <local_ec_name> "
                      "-r <remote_ec_name> "
		      "-g (use global scheduling service) "
		      "-s <scheduling service name> "
		      "-o <remote scheduling service name> "
		      "-x (short circuit EC) "
		      "-u <utilization test iterations> "
		      "-w <'work' iterations per push> "
                      "<-a event_type_a> "
                      "<-b event_type_b> "
                      "<-c event_type_c> "
                      "-i <event_interval> (usecs) "
                      "-m <message_count> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->message_count_ < 0
      || this->message_count_ > Test_ECG::MAX_EVENTS)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "%s: event count (%d) is out of range, "
		  "reset to default (%d)\n",
		  argv[0], this->message_count_,
		  Test_ECG::DEFAULT_EVENT_COUNT));
      this->message_count_ = Test_ECG::DEFAULT_EVENT_COUNT;
    }

  if (this->scavenger_count_ < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "%s: scavenger count < 0, test disabled\n"));
      this->scavenger_count_ = 0;
    }

  if (this->event_a_ <= 0
      || this->event_b_ <= 0
      || this->event_c_ <= 0 )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: you must specify the event types\n",
                  argv[0]));
      return -1;
    }

  return 0;
}

// ****************************************************************

int
main (int argc, char *argv [])
{
  Test_ECG test;
  return test.run (argc, argv);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<Test_ECG>;
template class ACE_PushSupplier_Adapter<Test_ECG>;
template class ACE_Auto_Basic_Ptr<ACE_Config_Scheduler>;
template class auto_ptr<ACE_Config_Scheduler>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<Test_ECG>
#pragma instantiate ACE_PushSupplier_Adapter<Test_ECG>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Config_Scheduler>
#pragma instantiate auto_ptr<ACE_Config_Scheduler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
