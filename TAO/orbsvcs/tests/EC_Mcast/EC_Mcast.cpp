// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "EC_Mcast.h"

ACE_RCSID(EC_Mcast, EC_Mcast, "$Id$")

#define ECM_DEFAULT_SEND_MCAST_GROUP "224.9.9.1"
#define ECM_DEFAULT_RECV_MCAST_GROUP "224.9.9.2"

ECM_Driver::ECM_Driver (void)
  : lcl_name_ ("ECM"),
    short_circuit_ (0),
    n_suppliers_ (1),
    n_consumers_ (1),
    workload_ (10),
    event_period_ (25000),
    event_count_ (200),
    s_event_a_ (ACE_ES_EVENT_UNDEFINED),
    s_event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    c_event_a_ (ACE_ES_EVENT_UNDEFINED),
    c_event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    r_event_a_ (ACE_ES_EVENT_UNDEFINED),
    r_event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    schedule_file_ (0),
    pid_file_name_ (0),
    send_mcast_group_ (u_short(23000), ECM_DEFAULT_SEND_MCAST_GROUP),
    recv_mcast_group_ (u_short(23001), ECM_DEFAULT_RECV_MCAST_GROUP),
    mcast_eh_ (&receiver_),
    ready_ (0),
    ready_cnd_ (ready_mtx_)
{
}



int
ECM_Driver::run (int argc, char* argv[])
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
		  "  short circuit = <%d>\n"
                  "  suppliers = <%d>\n"
                  "  consumers = <%d>\n"
                  "  workload = <%d> (iterations)\n"
                  "  event period = <%d> (usecs)\n"
                  "  event count = <%d>\n"
                  "  supplier Event A = <%d>\n"
                  "  supplier Event B = <%d>\n"
                  "  consumer Event A = <%d>\n"
                  "  consumer Event B = <%d>\n"
                  "  remote Event A = <%d>\n"
                  "  remote Event B = <%d>\n"
                  "  schedule_file = <%s>\n"
                  "  pid file name = <%s>\n",
                  this->lcl_name_?this->lcl_name_:"nil",
		  this->short_circuit_,

                  this->n_suppliers_,
                  this->n_consumers_,
                  this->workload_,
                  this->event_period_,
                  this->event_count_,
                  this->s_event_a_,
                  this->s_event_b_,
                  this->c_event_a_,
                  this->c_event_b_,
                  this->r_event_a_,
                  this->r_event_b_,

                  this->schedule_file_?this->schedule_file_:"nil",
                  this->pid_file_name_?this->pid_file_name_:"nil") );

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

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Config_Scheduler scheduler_impl;
      RtecScheduler::Scheduler_var scheduler =
	scheduler_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // We use this buffer to generate the names of the local
      // services.
      const int bufsize = 512;
      char buf[bufsize];

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

      // Create the EventService implementation, but don't start its
      // internal threads.
      ACE_EventChannel ec_impl (CORBA::B_FALSE);

      // Register Event_Service with the Naming Service.
      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      str = orb->object_to_string (ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

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

      ACE_DEBUG ((LM_DEBUG, "starting....\n"));

      RtecEventChannelAdmin::EventChannel_var local_ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "located local EC\n"));

      this->connect_ecg (local_ec, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->connect_suppliers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected supplier\n"));

      this->connect_consumers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected consumer\n"));

      this->activate_suppliers (local_ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      this->running_suppliers_ = this->n_suppliers_;

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

      ACE_DEBUG ((LM_DEBUG, "shutdown the EC\n"));
      ec_impl.shutdown ();

      this->dump_results ();

      this->receiver_.close (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->receiver_.shutdown (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->sender_.close (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->sender_.shutdown (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->disconnect_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->disconnect_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      ACE_DEBUG ((LM_DEBUG, "shutdown grace period\n"));
      tv.set (5, 0);
      if (orb->run (&tv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      naming_context->unbind (schedule_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      naming_context->unbind (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

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

void
ECM_Driver::disconnect_suppliers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->close (_env);
      if (_env.exception () != 0) return;
    }
}

void
ECM_Driver::connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                             CORBA::Environment &_env)
{
  TAO_TRY
    {
      for (int i = 0; i < this->n_suppliers_; ++i)
        {
          // Limit the number of events sent by each supplier
          int mc = this->event_count_ / this->n_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "supplier_%02.2d@%s", i, this->lcl_name_);

          ACE_NEW (this->suppliers_[i],
                   ECM_Supplier (this, this->suppliers_ + i));

          this->suppliers_[i]->open (buf,
                                     this->s_event_a_,
                                     this->s_event_b_,
                                     mc,
                                     this->event_period_ * 10,
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
ECM_Driver::disconnect_consumers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->close (_env);
      if (_env.exception () != 0) return;
    }
}

void
ECM_Driver::activate_suppliers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                              CORBA::Environment &_env)
{
  TAO_TRY
    {
      for (int i = 0; i < this->n_suppliers_; ++i)
        {
          // Limit the number of events sent by each supplier
          int mc = this->event_count_ / this->n_suppliers_;
          if (mc == 0)
            mc = 1;

          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "supplier_%02.2d@%s", i, this->lcl_name_);

          this->suppliers_[i]->activate (buf,
                                         this->event_period_ * 10,
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
ECM_Driver::connect_consumers (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                             CORBA::Environment &_env)
{
  TAO_TRY
    {
      for (int i = 0; i < this->n_consumers_; ++i)
        {
          char buf[BUFSIZ];
          ACE_OS::sprintf (buf, "consumer_%02.2d@%s", i, this->lcl_name_);

          ACE_NEW (this->consumers_[i],
                   ECM_Consumer (this, this->consumers_ + i));

          this->consumers_[i]->open (buf,
                                     this->c_event_a_,
                                     this->c_event_b_,
                                     local_ec,
                                     TAO_TRY_ENV);
          TAO_CHECK_ENV;
          this->stats_[i].total_time_ = 0;
          this->stats_[i].lcl_count_ = 0;
          this->stats_[i].rmt_count_ = 0;
        }

      this->running_consumers_ = this->n_consumers_;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
ECM_Driver::connect_ecg (RtecEventChannelAdmin::EventChannel_ptr local_ec,
			 CORBA::Environment &_env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr local_sch =
        ACE_Scheduler_Factory::server ();

      // We could use the same name on the local and remote scheduler,
      // but that fails when using a global scheduler.
      const int bufsize = 512;

      char mcast_name[bufsize];
      ACE_OS::strcpy (mcast_name, "sender");
      ACE_OS::strcat (mcast_name, "@");
      ACE_OS::strcat (mcast_name, this->lcl_name_);

      this->sender_.init (local_ec,
			  local_sch,
			  mcast_name,
			  this->send_mcast_group_,
			  TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      ACE_INET_Addr ignore_from;
      this->sender_.get_local_addr (ignore_from);

      char recv_name[bufsize];
      ACE_OS::strcpy (recv_name, "receiver");
      ACE_OS::strcat (recv_name, "@");
      ACE_OS::strcat (recv_name, this->lcl_name_);

      this->receiver_.init (local_ec,
			    local_sch,
			    recv_name,
			    ignore_from,
			    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->mcast_eh_.reactor (TAO_ORB_Core_instance ()->reactor ());
      this->mcast_eh_.open (this->recv_mcast_group_);

      RtecEventChannelAdmin::ConsumerQOS sender_qos;
      sender_qos.is_gateway = CORBA::B_TRUE;
      sender_qos.dependencies.length (3);
      sender_qos.dependencies[0].event.type_ = ACE_ES_DISJUNCTION_DESIGNATOR;
      sender_qos.dependencies[0].event.source_ = 0;
      sender_qos.dependencies[0].event.creation_time_ = ORBSVCS_Time::zero;
      sender_qos.dependencies[0].rt_info = 0;
      sender_qos.dependencies[1].event.type_ = this->s_event_a_;
      sender_qos.dependencies[1].event.source_ = 0;
      sender_qos.dependencies[1].event.creation_time_ = ORBSVCS_Time::zero;
      sender_qos.dependencies[1].rt_info = 0;
      sender_qos.dependencies[2].event.type_ = this->s_event_b_;
      sender_qos.dependencies[2].event.source_ = 0;
      sender_qos.dependencies[2].event.creation_time_ = ORBSVCS_Time::zero;
      sender_qos.dependencies[2].rt_info = 0;

      this->sender_.open (sender_qos, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::SupplierQOS receiver_qos;
      receiver_qos.is_gateway = CORBA::B_TRUE;
      receiver_qos.publications.length (2);
      receiver_qos.publications[0].event.type_ = this->r_event_a_;
      receiver_qos.publications[0].event.source_ = 0;
      receiver_qos.publications[0].event.creation_time_ = ORBSVCS_Time::zero;
      receiver_qos.publications[0].dependency_info.dependency_type =
	RtecScheduler::TWO_WAY_CALL;
      receiver_qos.publications[0].dependency_info.number_of_calls = 1;
      receiver_qos.publications[0].dependency_info.rt_info = 0;
      receiver_qos.publications[1].event.type_ = this->r_event_b_;
      receiver_qos.publications[1].event.source_ = 0;
      receiver_qos.publications[1].event.creation_time_ = ORBSVCS_Time::zero;
      receiver_qos.publications[1].dependency_info.dependency_type =
	RtecScheduler::TWO_WAY_CALL;
      receiver_qos.publications[1].dependency_info.number_of_calls = 1;
      receiver_qos.publications[1].dependency_info.rt_info = 0;

      this->receiver_.open (receiver_qos, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
ECM_Driver::push_supplier (void * /* cookie */,
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
      for (int i = 0; i < this->n_consumers_ && !_env.exception (); ++i)
        {
          this->consumers_[i]->push (events, _env);
        }
    }
}

void
ECM_Driver::push_consumer (void *consumer_cookie,
                         ACE_hrtime_t arrival,
                         const RtecEventComm::EventSet &events,
                         CORBA::Environment &)
{
  int ID =
    (ACE_reinterpret_cast(ECM_Consumer**,consumer_cookie)
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
          int workload = this->workload_;
          int interval = this->event_period_;

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
          this->stats_[ID].laxity_[count] = 1 + tmp/1000.0F/interval;
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
ECM_Driver::wait_until_ready (void)
{
  ACE_GUARD (ACE_Thread_Mutex, ready_mon, this->ready_mtx_);
  while (!this->ready_)
    this->ready_cnd_.wait ();
}

void
ECM_Driver::shutdown_supplier (void* /* supplier_cookie */,
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
ECM_Driver::shutdown_consumer (int id)
{
  ACE_DEBUG ((LM_DEBUG, "Shutdown consumer %d\n", id));
  this->running_consumers_--;
  if (this->running_consumers_ == 0)
    TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

int
ECM_Driver::shutdown (CORBA::Environment& _env)
{
  ACE_DEBUG ((LM_DEBUG, "Shutting down the multiple EC test\n"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return 0;
}

void
ECM_Driver::dump_results (void)
{
  const int bufsize = 512;
  char buf[bufsize];

  for (int i = 0; i < this->n_consumers_; ++i)
    {
      ACE_OS::sprintf (buf, "CO%02.2d", i);
      this->dump_results (buf, this->stats_[i]);
    }
  // the cast is to workaround a msvc++ bug...
  CORBA::ULong tmp = ACE_static_cast(CORBA::ULong,
                                     this->test_stop_ - this->test_start_);
  double usec =  tmp / 1000.0;
  ACE_DEBUG ((LM_DEBUG, "Time[TOTAL]: %.3f\n", usec));
}

void
ECM_Driver::dump_results (const char* name, Stats& stats)
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
ECM_Driver::local_source (RtecEventComm::EventSourceID id) const
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      if (this->suppliers_[i]->supplier_id () == id)
        return 1;
    }
  return 0;
}

int
ECM_Driver::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "xl:s:r:h:p:d:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'x':
          this->short_circuit_ = 1;
          break;

	case 'l':
	  this->lcl_name_ = get_opt.optarg;
	  break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);

            this->n_suppliers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->n_consumers_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->workload_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->event_period_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->event_count_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->s_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->s_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->c_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->c_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->r_event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->r_event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.optarg;
          break;

        case 'd':
          this->schedule_file_ = get_opt.optarg;
          break;

	case 's':
	  this->send_mcast_group_.set (get_opt.optarg);
	  break;

	case 'r':
	  this->recv_mcast_group_.set (get_opt.optarg);
	  break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-x (short circuit EC) "
                      "-h <high priority args> "
                      "-p <pid file name> "
                      "-d <schedule file name> "
		      "-s <send_mcast group> "
		      "-r <recv_mcast group> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->event_count_ < 0
      || this->event_count_ >= ECM_Driver::MAX_EVENTS)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->event_count_,
                  160));
      this->event_count_ = 160;
    }

  if (this->n_consumers_ <= 0
      || this->n_consumers_ >= ECM_Driver::MAX_CONSUMERS
      || this->n_suppliers_ <= 0
      || this->n_suppliers_ >= ECM_Driver::MAX_SUPPLIERS)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: number of consumers or "
                         "suppliers out of range\n", argv[0]), -1);
    }

  return 0;
}

// ****************************************************************

ECM_Supplier::ECM_Supplier (ECM_Driver *test,
                              void *cookie)
  :  test_ (test),
     cookie_ (cookie),
     consumer_ (this)
{
}

void
ECM_Supplier::open (const char* name,
                     int event_a,
                     int event_b,
                     int event_count,
                     const RtecScheduler::Period& rate,
                     RtecEventChannelAdmin::EventChannel_ptr ec,
                     CORBA::Environment &_env)
{
  this->event_a_ = event_a;
  this->event_b_ = event_b;
  this->event_count_ = event_count;

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
ECM_Supplier::close (CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (_env);
  if (_env.exception () != 0) return;

  this->consumer_proxy_ = 0;
}

void
ECM_Supplier::activate (const char* name,
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
ECM_Supplier::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment& _env)
{
#if 0
   const int bufsize = 128;
   char buf[bufsize];
   ACE_OS::sprintf (buf, "Supplier %d receives event in thread: ",
                    this->supplier_id_);
#endif

  if (events.length () == 0 || this->event_count_ < 0)
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

      // ACE_DEBUG ((LM_DEBUG, "ECM_Supplier - timeout (%t)\n"));

      RtecEventComm::Event& s = sent[i];
      s.source_ = this->supplier_id_;
      s.ttl_ = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (s.creation_time_, t);
      s.ec_recv_time_ = ORBSVCS_Time::zero;
      s.ec_send_time_ = ORBSVCS_Time::zero;

      s.data_.x = 0;
      s.data_.y = 0;

      this->event_count_--;

      if (this->event_count_ < 0)
        {
          //this->supplier_proxy_->disconnect_push_supplier (_env);
          //if (_env.exception () != 0) return;
          this->test_->shutdown_supplier (this->cookie_,
                                          this->consumer_proxy_.in (),
                                          _env);
        }
      if (this->event_count_ % 2 == 0)
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
ECM_Supplier::disconnect_push_supplier (CORBA::Environment& _env)
{
  this->supplier_proxy_->disconnect_push_supplier (_env);
}

void
ECM_Supplier::disconnect_push_consumer (CORBA::Environment &)
{
}

int ECM_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

// ****************************************************************

ECM_Consumer::ECM_Consumer (ECM_Driver *test,
                              void *cookie)
  : test_ (test),
    cookie_ (cookie)
{
}

void
ECM_Consumer::open (const char* name,
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
ECM_Consumer::close (CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (_env);
  if (_env.exception () != 0) return;

  this->supplier_proxy_ = 0;
}

void
ECM_Consumer::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->test_->push_consumer (this->cookie_, arrival, events, _env);
}

void
ECM_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

int
main (int argc, char *argv [])
{
  ECM_Driver test;
  return test.run (argc, argv);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<ECM_Supplier>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<ECM_Supplier>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
