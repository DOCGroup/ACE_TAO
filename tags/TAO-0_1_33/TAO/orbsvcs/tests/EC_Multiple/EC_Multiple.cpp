// $Id$

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
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "EC_Multiple.h"

#include "Scheduler_Runtime1.h"
#include "Scheduler_Runtime2.h"

#if defined (sun)
# include <sys/lwp.h> /* for _lwp_self */
#endif /* sun */

Test_ECG::Test_ECG (void)
  : lcl_name_ ("Test_ECG"),
    rmt_name_ (0),
    scheduling_type_ (Test_ECG::ss_runtime),
    consumer_disconnects_ (0),
    supplier_disconnects_ (0),
    short_circuit_ (0),
    hp_suppliers_ (1),
    hp_consumers_ (1),
    hp_workload_ (10),
    hp_interval_ (25000),
    hp_message_count_ (200),
    hps_event_a_ (ACE_ES_EVENT_UNDEFINED),
    hps_event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    hpc_event_a_ (ACE_ES_EVENT_UNDEFINED),
    hpc_event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    lp_suppliers_ (0),
    lp_consumers_ (0),
    lp_workload_ (0),
    lp_interval_ (100000),
    lp_message_count_ (50),
    lps_event_a_ (0),
    lps_event_b_ (0),
    lpc_event_a_ (0),
    lpc_event_b_ (0),
    schedule_file_ (0),
    pid_file_name_ (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_)
{
}



void
print_priority_info (const char *const name)
{
#if defined (ACE_HAS_PTHREADS) || defined (sun)
#if defined (ACE_HAS_PTHREADS)
  struct sched_param param;
  int policy, status;

  if ((status = pthread_getschedparam (pthread_self (), &policy,
                                       &param)) == 0) {
#   ifdef sun
    ACE_DEBUG ((LM_DEBUG,
                "%s (%lu|%u); policy is %d, priority is %d\n",
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
#endif /* ACE_HAS_PTHREADS */

#ifdef sun
  // Find what scheduling class the thread's LWP is in.
  ACE_Sched_Params sched_params (ACE_SCHED_OTHER, 0);
  if (ACE_OS::lwp_getparams (sched_params) == -1)
    {
      ACE_OS::perror ("ACE_OS::lwp_getparams");
      return;
    }
  else if (sched_params.policy () == ACE_SCHED_FIFO  ||
           sched_params.policy () == ACE_SCHED_RR)
    {
      // This thread's LWP is in the RT class.
      ACE_DEBUG ((LM_DEBUG,
                  "RT class; priority: %d, quantum: %u msec\n",
                  sched_params.priority (),
                  sched_params.quantum ().msec ()));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TS class; priority: %d\n",
                  sched_params.priority ()));
    }
#endif /* sun */
#endif /* ACE_HAS_PTHREADS */
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
                  "Execution parameters:\n"
                  "  lcl name = <%s>\n"
                  "  rmt name = <%s>\n"
                  "  scheduler type = <%d>\n"
                  "  consumer disconnects = <%d>\n"
                  "  supplier disconnects = <%d>\n"
                  "  short circuit EC = <%d>\n"
                  "  HP suppliers = <%d>\n"
                  "  HP consumers = <%d>\n"
                  "  HP workload = <%d> (iterations)\n"
                  "  HP interval between events = <%d> (usecs)\n"
                  "  HP message count = <%d>\n"
                  "  HP supplier Event A = <%d>\n"
                  "  HP supplier Event B = <%d>\n"
                  "  HP consumer Event A = <%d>\n"
                  "  HP consumer Event B = <%d>\n"
                  "  LP suppliers = <%d>\n"
                  "  LP consumers = <%d>\n"
                  "  LP workload = <%d> (iterations)\n"
                  "  LP interval between events = <%d> (usecs)\n"
                  "  LP message count = <%d>\n"
                  "  LP supplier Event A = <%d>\n"
                  "  LP supplier Event B = <%d>\n"
                  "  LP consumer Event A = <%d>\n"
                  "  LP consumer Event B = <%d>\n"
                  "  schedule_file = <%s>\n"
                  "  pid file name = <%s>\n",
                  this->lcl_name_?this->lcl_name_:"nil",
                  this->rmt_name_?this->rmt_name_:"nil",
                  this->scheduling_type_,
		  this->consumer_disconnects_,
		  this->supplier_disconnects_,
                  this->short_circuit_,

                  this->hp_suppliers_,
                  this->hp_consumers_,
                  this->hp_workload_,
                  this->hp_interval_,
                  this->hp_message_count_,
                  this->hps_event_a_,
                  this->hps_event_b_,
                  this->hpc_event_a_,
                  this->hpc_event_b_,

                  this->lp_suppliers_,
                  this->lp_consumers_,
                  this->lp_workload_,
                  this->lp_interval_,
                  this->lp_message_count_,
                  this->lps_event_a_,
                  this->lps_event_b_,
                  this->lpc_event_a_,
                  this->lpc_event_b_,

                  this->schedule_file_?this->schedule_file_:"nil",
                  this->pid_file_name_?this->pid_file_name_:"nil") );

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

      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
        // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  min_priority,
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

      if (ACE_OS::thr_setprio (min_priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed\n"));
        }

      print_priority_info ("Test_ECG::run (Main after thr_setprio)");

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      auto_ptr<POA_RtecScheduler::Scheduler> scheduler_impl;
      RtecScheduler::Scheduler_var scheduler;

      switch (this->scheduling_type_)
        {
        default:
          ACE_ERROR ((LM_WARNING, "Unknown scheduling type %d\n",
                      this->scheduling_type_));
          /*FALLTHROUGH*/
        case Test_ECG::ss_global:
          break;

        case Test_ECG::ss_local:
          scheduler_impl =
            auto_ptr<POA_RtecScheduler::Scheduler>(new ACE_Config_Scheduler);
          if (scheduler_impl.get () == 0)
                return -1;
          scheduler = scheduler_impl->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;
          break;

        case Test_ECG::ss_runtime:
          if (ACE_OS::strcmp (this->lcl_name_, "ECM1") == 0)
            {
              scheduler_impl =
                auto_ptr<POA_RtecScheduler::Scheduler>
                    (new ACE_Runtime_Scheduler (runtime_infos_1_size,
                                                runtime_infos_1));
              if (scheduler_impl.get () == 0)
                return -1;
              scheduler = scheduler_impl->_this (TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
          else if (ACE_OS::strcmp (this->lcl_name_, "ECM2") == 0)
            {
              scheduler_impl =
                auto_ptr<POA_RtecScheduler::Scheduler>
                    (new ACE_Runtime_Scheduler (runtime_infos_2_size,
                                                runtime_infos_2));
              if (scheduler_impl.get () == 0)
                return -1;
              scheduler = scheduler_impl->_this (TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
          else
            {
              ACE_ERROR ((LM_WARNING,
                          "Unknown name <%s> defaulting to "
                          "config scheduler\n", this->lcl_name_));
              scheduler_impl =
                auto_ptr<POA_RtecScheduler::Scheduler>(new ACE_Config_Scheduler);
              if (scheduler_impl.get () == 0)
                return -1;
              scheduler = scheduler_impl->_this (TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
          break;

        }

      // We use this buffer to generate the names of the local
      // services.
      const int bufsize = 512;
      char buf[bufsize];

      // Register the scheduler with the naming service.
      switch (this->scheduling_type_)
        {
        default:
        case Test_ECG::ss_global:
          break;

        case Test_ECG::ss_local:
        case Test_ECG::ss_runtime:
          {
            CORBA::String_var str =
              orb->object_to_string (scheduler.in (), TAO_TRY_ENV);
            TAO_CHECK_ENV;
            ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
                        str.in ()));

            ACE_OS::strcpy (buf, "ScheduleService@");
            ACE_OS::strcat (buf, this->lcl_name_);

            // Register the servant with the Naming Context....
            CosNaming::Name schedule_name (1);
            schedule_name.length (1);
            schedule_name[0].id = CORBA::string_dup (buf);
            naming_context->bind (schedule_name, scheduler.in (), TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (ACE_Scheduler_Factory::use_config (naming_context.in (),
                                                   buf) == -1)
              return -1;
          }
          break;
        }

      // Create the EventService implementation, but don't start its
      // internal threads.
      ACE_EventChannel ec_impl (CORBA::B_FALSE);

      // Register Event_Service with the Naming Service.
      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
        orb->object_to_string (ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_OS::sleep (5);
      ACE_DEBUG ((LM_DEBUG, "The (local) EC IOR is <%s>\n", str.in ()));

      ACE_OS::strcpy (buf, "EventChannel@");
      ACE_OS::strcat (buf, this->lcl_name_);

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (buf);
      naming_context->bind (channel_name, ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "waiting to start\n"));

      ACE_Time_Value tv (15, 0);

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->rmt_name_ != 0)
        {
          if (orb->run (&tv) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);
        }

      ACE_DEBUG ((LM_DEBUG, "starting....\n"));

      RtecEventChannelAdmin::EventChannel_var local_ec =
        this->get_ec (naming_context.in (),
                      this->lcl_name_,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "located local EC\n"));

      for (int sd = 0; sd < this->supplier_disconnects_; ++sd)
	{
	  this->connect_suppliers (local_ec.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  this->disconnect_suppliers (TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_OS::sleep (5);
	  ACE_DEBUG ((LM_DEBUG, "Supplier disconnection %d\n", sd));
	}

      this->connect_suppliers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected supplier\n"));

      if (this->rmt_name_ != 0)
        {
          tv.set (5, 0);
          if (orb->run (&tv) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

          RtecEventChannelAdmin::EventChannel_var remote_ec =
            this->get_ec (naming_context.in (),
                          this->rmt_name_,
                          TAO_TRY_ENV);
          TAO_CHECK_ENV;
          ACE_DEBUG ((LM_DEBUG, "located remote EC\n"));

          CosNaming::Name rsch_name (1);
          rsch_name.length (1);
          ACE_OS::strcpy (buf, "ScheduleService");
          if (this->scheduling_type_ != Test_ECG::ss_global)
            {
              ACE_OS::strcat (buf, "@");
              ACE_OS::strcat (buf, this->rmt_name_);
            }
          rsch_name[0].id = CORBA::string_dup (buf);
          CORBA::Object_var tmpobj =
            naming_context->resolve (rsch_name, TAO_TRY_ENV);
          TAO_CHECK_ENV;

          RtecScheduler::Scheduler_var remote_sch =
            RtecScheduler::Scheduler::_narrow (tmpobj.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->connect_ecg (local_ec.in (),
                             remote_ec.in (),
                             remote_sch.in (),
                             TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_DEBUG ((LM_DEBUG, "connected proxy\n"));

          tv.set (5, 0);
          if (orb->run (&tv) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

	  ec_impl.add_gateway (&this->ecg_);
        }

      for (int cd = 0; cd < this->consumer_disconnects_; ++cd)
	{
	  this->connect_consumers (local_ec.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  this->disconnect_consumers (TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_OS::sleep (5);
	  ACE_DEBUG ((LM_DEBUG, "Consumer disconnection %d\n", cd));
	}
      this->connect_consumers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected consumer\n"));

      this->activate_suppliers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      this->running_suppliers_ = this->hp_suppliers_ + this->lp_suppliers_;

      // Acquire the mutex for the ready mutex, blocking any supplier
      // that may start after this point.
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, this->ready_mtx_, 1);
      this->ready_ = 1;
      this->test_start_ = ACE_OS::gethrtime ();
      ready_mon.release ();
      this->ready_cnd_.broadcast ();

      ACE_DEBUG ((LM_DEBUG, "activate the  EC\n"));

      // Create the EC internal threads
      ec_impl.activate ();
      
      ACE_DEBUG ((LM_DEBUG, "running the test\n"));
      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      this->test_stop_ = ACE_OS::gethrtime ();

      // Wait for EC threads to terminate....
      ACE_DEBUG ((LM_DEBUG, "waiting for EC threads\n"));
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG, "shutdown grace period\n"));
      tv.set (5, 0);
      if (orb->run (&tv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

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

RtecEventChannelAdmin::EventChannel_ptr
Test_ECG::get_ec (CosNaming::NamingContext_ptr naming_context,
                  const char* process_name,
                  CORBA::Environment &_env)
{
  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, "EventChannel@");
  ACE_OS::strcat (buf, process_name);

  CosNaming::Name channel_name (1);
  channel_name.length (1);
  channel_name[0].id = CORBA::string_dup (buf);

  CORBA::Object_var ec_ptr =
    naming_context->resolve (channel_name, _env);
  if (_env.exception () != 0 || CORBA::is_nil (ec_ptr.in ()))
    return RtecEventChannelAdmin::EventChannel::_nil ();

  return RtecEventChannelAdmin::EventChannel::_narrow (ec_ptr.in (),
                                                       _env);
}

void
Test_ECG::disconnect_suppliers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->hp_suppliers_ + this->lp_suppliers_; ++i)
    {
      this->suppliers_[i]->close (_env);
      if (_env.exception () != 0) return;
    }
}

void
Test_ECG::connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                             CORBA::Environment &_env)
{
  TAO_TRY
    {
      int i;
      for (i = 0; i < this->hp_suppliers_; ++i)
        {
          // Limit the number of messages sent by each supplier
          int mc = this->hp_message_count_ / this->hp_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "hp_supplier_%02.2d@%s", i, this->lcl_name_);

          ACE_NEW (this->suppliers_[i],
                   Test_Supplier (this, this->suppliers_ + i));

          this->suppliers_[i]->open (buf,
                                     this->hps_event_a_,
                                     this->hps_event_b_,
                                     mc,
                                     this->hp_interval_ * 10,
                                     local_ec,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

      for (; i < this->hp_suppliers_ + this->lp_suppliers_; ++i)
        {
          // Limit the number of messages sent by each supplier
          int mc = this->lp_message_count_ / this->lp_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "lp_supplier_%02.2d@%s",
                           i - this->hp_suppliers_, this->lcl_name_);

          ACE_NEW (this->suppliers_[i],
                   Test_Supplier (this, this->suppliers_ + i));

          this->suppliers_[i]->open (buf,
                                     this->lps_event_a_,
                                     this->lps_event_b_,
                                     mc,
                                     this->lp_interval_ * 10,
                                     local_ec,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_ECG::disconnect_consumers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->hp_consumers_ + this->lp_consumers_; ++i)
    {
      this->consumers_[i]->close (_env);
      if (_env.exception () != 0) return;
    }
}

void
Test_ECG::activate_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                              CORBA::Environment &_env)
{
  TAO_TRY
    {
      int i;
      for (i = 0; i < this->hp_suppliers_; ++i)
        {
          // Limit the number of messages sent by each supplier
          int mc = this->hp_message_count_ / this->hp_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "hp_supplier_%02.2d@%s", i, this->lcl_name_);

          this->suppliers_[i]->activate (buf,
                                         this->hp_interval_ * 10,
                                         local_ec,
                                         TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

      for (; i < this->hp_suppliers_ + this->lp_suppliers_; ++i)
        {
          // Limit the number of messages sent by each supplier
          int mc = this->lp_message_count_ / this->lp_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "lp_supplier_%02.2d@%s",
                           i - this->hp_suppliers_, this->lcl_name_);

          this->suppliers_[i]->activate (buf,
                                         this->lp_interval_ * 10,
                                         local_ec,
                                         TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_ECG::connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                             CORBA::Environment &_env)
{
  TAO_TRY
    {
      int i;
      for (i = 0; i < this->hp_consumers_; ++i)
        {
          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "hp_consumer_%02.2d@%s", i, this->lcl_name_);

          ACE_NEW (this->consumers_[i],
                   Test_Consumer (this, this->consumers_ + i));

          this->consumers_[i]->open (buf,
                                     this->hpc_event_a_,
                                     this->hpc_event_b_,
                                     local_ec,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;
          this->stats_[i].total_time_ = 0;
          this->stats_[i].lcl_count_ = 0;
          this->stats_[i].rmt_count_ = 0;
        }

      for (; i < this->hp_consumers_ + this->lp_consumers_; ++i)
        {
          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "lp_consumer_%02.2d@%s",
                           i - this->hp_consumers_, this->lcl_name_);

          ACE_NEW (this->consumers_[i],
                   Test_Consumer (this, this->consumers_ + i));

          this->consumers_[i]->open (buf,
                                     this->lpc_event_a_,
                                     this->lpc_event_b_,
                                     local_ec,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;
          this->stats_[i].total_time_ = 0;
          this->stats_[i].lcl_count_ = 0;
          this->stats_[i].rmt_count_ = 0;
        }
      this->running_consumers_ = this->hp_consumers_ + this->lp_consumers_;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
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
      const int bufsize = 512;
      char ecg_name[bufsize];
      ACE_OS::strcpy (ecg_name, "ecg_");
      ACE_OS::strcat (ecg_name, this->lcl_name_);

      // We could use the same name on the local and remote scheduler,
      // but that fails when using a global scheduler.
      char rmt[BUFSIZ];
      ACE_OS::strcpy (rmt, ecg_name);
      ACE_OS::strcat (rmt, "@");
      ACE_OS::strcat (rmt, this->rmt_name_);

      // We could use the same name on the local and remote scheduler,
      // but that fails when using a global scheduler.
      char lcl[bufsize];
      ACE_OS::strcpy (lcl, ecg_name);
      ACE_OS::strcat (lcl, "@");
      ACE_OS::strcat (lcl, this->lcl_name_);

      this->ecg_.init (remote_ec, local_ec, remote_sch, local_sch,
		       rmt, lcl, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_ECG::push_supplier (void * /* cookie */,
                         RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
                         const RtecEventComm::EventSet &events,
                         CORBA::Environment & _env)
{
  this->wait_until_ready ();
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) events sent by supplier\n"));
  // @@ TODO we could keep somekind of stats here...
  if (!this->short_circuit_)
    {
      consumer->push (events, _env);
    }
  else
    {
      int i = 0;
      for (; i < this->hp_consumers_ && !_env.exception (); ++i)
        {
          this->consumers_[i]->push (events, _env);
        }
      for (; i < this->hp_consumers_ + this->lp_consumers_ && !_env.exception (); ++i)
        {
          this->consumers_[i]->push (events, _env);
        }
    }
}

void
Test_ECG::push_consumer (void *consumer_cookie,
                         ACE_hrtime_t arrival,
                         const RtecEventComm::EventSet &events,
                         CORBA::Environment &)
{
  int ID =
    (ACE_reinterpret_cast(Test_Consumer**,consumer_cookie)
     - this->consumers_);

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) events received by consumer %d\n", ID));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

#if 0
  const int bufsize = 128;
  char buf[bufsize];
  ACE_OS::sprintf (buf, "Consumer %d receives event in thread: ", ID);
  print_priority_info (buf);
#endif

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];

      if (e.type_ == ACE_ES_EVENT_SHUTDOWN)
        {
          this->shutdown_consumer (ID);
          continue;
        }

      ACE_hrtime_t s;
      ORBSVCS_Time::TimeT_to_hrtime (s, e.creation_time_);
      ACE_hrtime_t nsec = arrival - s;
      if (this->local_source (e.source_))
        {
          int& count = this->stats_[ID].lcl_count_;

          this->stats_[ID].lcl_latency_[count] = nsec;
          int workload = this->hp_workload_;
          int interval = this->hp_interval_;
          if (ID >= this->hp_consumers_)
            {
              workload = this->lp_workload_;
              interval = this->lp_interval_;
            }

          for (int j = 0; j < workload; ++j)
            {
              // Eat a little CPU so the Utilization test can measure the
              // consumed time....
              /* takes about 40.2 usecs on a 167 MHz Ultra2 */
              u_long n = 1279UL;
              ACE::is_prime (n, 2, n / 2);
            }
          // Increment the elapsed time on this consumer.
          ACE_hrtime_t now = ACE_OS::gethrtime ();
          this->stats_[ID].total_time_ += now - arrival;
          this->stats_[ID].end_[count] = now;

          // We estimate our laxity based on the event creation
          // time... it may not be very precise, but will do; other
          // strategies include:
          // + Keep track of the "current frame", then then deadline
          // is the end of the frame.
          // + Use the start of the test to keep the current frame.
          // + Use the last execution.

          // Work around MSVC++ bug, it does not not how to convert an
          // unsigned 64 bit int into a long....
          CORBA::ULong tmp = ACE_static_cast(CORBA::ULong,(s - now));
          this->stats_[ID].laxity_[count] = 1 + tmp/1000.0/interval;
          count++;
        }
      else
        {
          int& count = this->stats_[ID].rmt_count_;
          this->stats_[ID].rmt_latency_[count] = nsec;
          count++;
        }
    }
}

void
Test_ECG::wait_until_ready (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ready_mon, this->ready_mtx_);
  while (!this->ready_)
    this->ready_cnd_.wait ();
}

void
Test_ECG::shutdown_supplier (void* /* supplier_cookie */,
                             RtecEventComm::PushConsumer_ptr consumer,
                             CORBA::Environment& _env)
{

  this->running_suppliers_--;
  if (this->running_suppliers_ == 0)
    {
      // We propagate a shutdown event through the system...
      RtecEventComm::EventSet shutdown (1);
      shutdown.length (1);
      RtecEventComm::Event& s = shutdown[0];

      s.source_ = 0;
      s.ttl_ = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (s.creation_time_, t);
      s.ec_recv_time_ = ORBSVCS_Time::zero;
      s.ec_send_time_ = ORBSVCS_Time::zero;
      s.data_.x = 0;
      s.data_.y = 0;
      s.type_ = ACE_ES_EVENT_SHUTDOWN;
      consumer->push (shutdown, _env);
    }
}

void
Test_ECG::shutdown_consumer (int id)
{
  ACE_DEBUG ((LM_DEBUG, "Shutdown consumer %d\n", id));
  this->running_consumers_--;
  if (this->running_consumers_ == 0)
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

int
Test_ECG::shutdown (CORBA::Environment& _env)
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the multiple EC test\n"));

  if (this->rmt_name_ != 0)
    {
      this->ecg_.shutdown (_env);
      if (_env.exception () != 0) return -1;
    }

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return 0;
}

void
Test_ECG::dump_results (void)
{
  const int bufsize = 512;
  char buf[bufsize];

  int i;
  for (i = 0; i < this->hp_consumers_; ++i)
    {
      ACE_OS::sprintf (buf, "HP%02.2d", i);
      this->dump_results (buf, this->stats_[i]);
    }
  for (i = 0; i < this->lp_consumers_; ++i)
    {
      ACE_OS::sprintf (buf, "LP%02.2d", i);
      this->dump_results (buf, this->stats_[i + this->hp_consumers_]);
    }
  // the cast is to workaround a msvc++ bug...
  CORBA::ULong tmp = ACE_static_cast(CORBA::ULong,
                                     this->test_stop_ - this->test_start_);
  double usec =  tmp / 1000.0;
  ACE_DEBUG ((LM_DEBUG, "Time[TOTAL]: %.3f\n", usec));
}

void
Test_ECG::dump_results (const char* name, Stats& stats)
{
  // @@ We are reporting the information without specifics about
  // the cast is to workaround a msvc++ bug...
  double usec = ACE_static_cast(CORBA::ULong,stats.total_time_) / 1000.0;
  ACE_DEBUG ((LM_DEBUG, "Time[LCL,%s]: %.3f\n", name, usec));
  int i;
  for (i = 1; i < stats.lcl_count_ - 1; ++i)
    {
      // the cast is to workaround a msvc++ bug...
      usec = ACE_static_cast(CORBA::ULong,stats.lcl_latency_[i]) / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[LCL,%s]: %.3f\n", name, usec));

      double percent = stats.laxity_[i] * 100.0;
      ACE_DEBUG ((LM_DEBUG, "Laxity[LCL,%s]: %.3f\n", name, percent));

      // the cast is to workaround a msvc++ bug...
      usec = ACE_static_cast(CORBA::ULong,stats.end_[i] - this->test_start_) / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Completion[LCL,%s]: %.3f\n", name, usec));
    }
  for (i = 1; i < stats.rmt_count_ - 1; ++i)
    {
      // the cast is to workaround a msvc++ bug...
      double usec = ACE_static_cast(CORBA::ULong,stats.rmt_latency_[i]) / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[RMT,%s]: %.3f\n", name, usec));
    }
}

int
Test_ECG::local_source (RtecEventComm::EventSourceID id) const
{
  for (int i = 0; i < this->hp_suppliers_ + this->lp_suppliers_; ++i)
    {
      if (this->suppliers_[i]->supplier_id () == id)
        return 1;
    }
  return 0;
}

int
Test_ECG::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "l:r:s:i:xh:w:p:d:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'l':
          this->lcl_name_ = get_opt.optarg;
          break;

        case 'r':
          this->rmt_name_ = get_opt.optarg;
          break;

        case 's':
          if (ACE_OS::strcasecmp (get_opt.optarg, "global") == 0)
            {
              this->scheduling_type_ = Test_ECG::ss_global;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "local") == 0)
            {
              this->scheduling_type_ = Test_ECG::ss_local;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "runtime") == 0)
            {
              this->scheduling_type_ = Test_ECG::ss_runtime;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown scheduling type <%s> "
                          "defaulting to local\n",
                          get_opt.optarg));
              this->scheduling_type_ = Test_ECG::ss_local;
            }
          break;

        case 'x':
          this->short_circuit_ = 1;
          break;

	case 'i':
	  {
            char* aux;
	    char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);
	    this->consumer_disconnects_ = ACE_OS::atoi (arg);
	    arg = ACE_OS::strtok_r (0, ",", &aux);
	    this->supplier_disconnects_ = ACE_OS::atoi (arg);
	  }
	  break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);

            this->hp_suppliers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hp_consumers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hp_workload_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hp_interval_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hp_message_count_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hps_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hps_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hpc_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->hpc_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'w':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);

            this->lp_suppliers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lp_consumers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lp_workload_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lp_interval_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lp_message_count_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lps_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lps_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lpc_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->lpc_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.optarg;
          break;
        case 'd':
          this->schedule_file_ = get_opt.optarg;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-l <local_name> "
                      "-r <remote_name> "
                      "-s <global|local|runtime> "
		      "-i <consumer disc.,supplier disc.> "
                      "-x (short circuit EC) "
                      "-h <high priority args> "
                      "-w <low priority args> "
                      "-p <pid file name> "
                      "-d <schedule file name> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->hp_message_count_ < 0
      || this->hp_message_count_ >= Test_ECG::MAX_EVENTS)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: HP event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->lp_message_count_,
                  160));
      this->hp_message_count_ = 160;
    }

  if (this->lp_message_count_ < 0
      || this->lp_message_count_ >= Test_ECG::MAX_EVENTS)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: LP event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->lp_message_count_,
                  4));
      this->lp_message_count_ = 4;
    }

  if (this->hp_consumers_ <= 0
      || this->lp_consumers_ < 0
      || this->hp_consumers_ + this->lp_consumers_ >= Test_ECG::MAX_CONSUMERS
      || this->hp_suppliers_ <= 0
      || this->lp_suppliers_ < 0
      || this->hp_suppliers_ + this->lp_suppliers_ >= Test_ECG::MAX_SUPPLIERS)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: number of consumers or "
                         "suppliers out of range\n", argv[0]), -1);
    }

  return 0;
}

// ****************************************************************

Test_Supplier::Test_Supplier (Test_ECG *test,
                              void *cookie)
  :  test_ (test),
     cookie_ (cookie),
     consumer_ (this)
{
}

void
Test_Supplier::open (const char* name,
                     int event_a,
                     int event_b,
                     int message_count,
                     const RtecScheduler::Period& rate,
                     RtecEventChannelAdmin::EventChannel_ptr ec,
                     CORBA::Environment &_env)
{
  this->event_a_ = event_a;
  this->event_b_ = event_b;
  this->message_count_ = message_count;

  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      RtecScheduler::handle_t rt_info =
        server->create (name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The execution times are set to reasonable values, but
      // actually they are changed on the real execution, i.e. we
      // lie to the scheduler to obtain right priorities; but we
      // don't care if the set is schedulable.
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
                   time, time, time,
                   rate,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   time,
                   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_id_ = ACE::crc32 (name);
      ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
                  this->supplier_id_));

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
        ec->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var objref = this->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                    qos.get_SupplierQOS (),
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_Supplier::close (CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (_env);
  if (_env.exception () != 0) return;

  this->consumer_proxy_ = 0;
}

void
Test_Supplier::activate (const char* name,
                         const RtecScheduler::Period& rate,
                         RtecEventChannelAdmin::EventChannel_ptr ec,
                         CORBA::Environment &_env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      const int bufsize = 512;
      char buf[bufsize];
      ACE_OS::strcpy (buf, "consumer_");
      ACE_OS::strcat (buf, name);
      RtecScheduler::handle_t rt_info =
        server->create (buf, TAO_TRY_ENV);
      TAO_CHECK_ENV;


      // The execution times are set to reasonable values, but
      // actually they are changed on the real execution, i.e. we
      // lie to the scheduler to obtain right priorities; but we
      // don't care if the set is schedulable.
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
                   time, time, time,
                   rate,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   time,
                   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Also connect our consumer for timeout events from the EC.
      int interval = rate / 10;
      ACE_Time_Value tv_timeout (interval / ACE_ONE_SECOND_IN_USECS,
                                 interval % ACE_ONE_SECOND_IN_USECS);
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout, tv_timeout);

      ACE_ConsumerQOS_Factory consumer_qos;
      consumer_qos.start_disjunction_group ();
      consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                timeout,
                                rt_info);

      // = Connect as a consumer.
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var cref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_->connect_push_consumer (cref.in (),
                                                    consumer_qos.get_ConsumerQOS (),
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_Supplier::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment& _env)
{
#if 0
   const int bufsize = 128;
   char buf[bufsize];
   ACE_OS::sprintf (buf, "Supplier %d receives event in thread: ",
                    this->supplier_id_);
   print_priority_info (buf);
#endif

  if (events.length () == 0 || this->message_count_ < 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  RtecEventComm::EventSet sent (events.length ());
  sent.length (events.length ());

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];
      if (e.type_ != ACE_ES_EVENT_INTERVAL_TIMEOUT)
        continue;

      // ACE_DEBUG ((LM_DEBUG, "Test_Supplier - timeout (%t)\n"));

      RtecEventComm::Event& s = sent[i];
      s.source_ = this->supplier_id_;
      s.ttl_ = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (s.creation_time_, t);
      s.ec_recv_time_ = ORBSVCS_Time::zero;
      s.ec_send_time_ = ORBSVCS_Time::zero;

      s.data_.x = 0;
      s.data_.y = 0;

      this->message_count_--;

      if (this->message_count_ < 0)
        {
          //this->supplier_proxy_->disconnect_push_supplier (_env);
          //if (_env.exception () != 0) return;
          this->test_->shutdown_supplier (this->cookie_,
                                          this->consumer_proxy_.in (),
                                          _env);
        }
      if (this->message_count_ % 2 == 0)
        {
          // Generate an A event...
          s.type_ = this->event_a_;
        }
      else
        {
          s.type_ = this->event_b_;
        }
    }
  this->test_->push_supplier (this->cookie_,
                              this->consumer_proxy_.in (),
                              sent,
                              _env);
}

void
Test_Supplier::disconnect_push_supplier (CORBA::Environment& _env)
{
  this->supplier_proxy_->disconnect_push_supplier (_env);
}

void
Test_Supplier::disconnect_push_consumer (CORBA::Environment &)
{
}

int Test_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

// ****************************************************************

Test_Consumer::Test_Consumer (Test_ECG *test,
                              void *cookie)
  : test_ (test),
    cookie_ (cookie)
{
}

void
Test_Consumer::open (const char* name,
                     int event_a, int event_b,
                     RtecEventChannelAdmin::EventChannel_ptr ec,
                     CORBA::Environment& _env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      RtecScheduler::handle_t rt_info =
        server->create (name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The worst case execution time is far less than 2
      // milliseconds, but that is a safe estimate....
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
                   time, time, time,
                   0,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   time,
                   0,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_ConsumerQOS_Factory qos;
      qos.start_disjunction_group ();
      qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info);
      qos.insert_type (event_a, rt_info);
      qos.insert_type (event_b, rt_info);

      // = Connect as a consumer.
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var objref = this->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                    qos.get_ConsumerQOS (),
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
Test_Consumer::close (CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (_env);
  if (_env.exception () != 0) return;

  this->supplier_proxy_ = 0;
}

void
Test_Consumer::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->test_->push_consumer (this->cookie_, arrival, events, _env);
}

void
Test_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

int
main (int argc, char *argv [])
{
  Test_ECG test;
  return test.run (argc, argv);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>;
template class ACE_Condition<ACE_Thread_Mutex>;
template class ACE_PushConsumer_Adapter<Test_Supplier>;
template class auto_ptr<POA_RtecScheduler::Scheduler>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate ACE_Condition<ACE_Thread_Mutex>
#pragma instantiate ACE_PushConsumer_Adapter<Test_Supplier>
#pragma instantiate auto_ptr<POA_RtecScheduler::Scheduler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
