//
// $Id$
//

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
    lcl_cnt_ (0),
    rmt_cnt_ (0),
    scavenger_start_ (0),
    scavenger_end_ (0),
    scavenger_barrier_ (2),
    scavenger_cnt_ (0),
    scavenger_priority_ (0)
{
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

      if (this->scavenger_cnt_ != 0)
	{
	  this->scavenger_priority_ =
	    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
					    ACE_SCOPE_THREAD);
	  if (this->activate (THR_BOUND|THR_SCHED_FIFO,
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
			     PortableServer::POA_ptr root_poa,
			     PortableServer::POAManager_ptr poa_manager,
			     CORBA::Environment& _env)
{
  TAO_TRY
    {
      ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();
      ACE_Time_Value tv (this->interval_ / ACE_ONE_SECOND_IN_USECS,
			 (this->interval_ % ACE_ONE_SECOND_IN_USECS));
      reactor->schedule_timer (this, 0, tv, tv);

      this->supplier_id_ = 0;

      ACE_DEBUG ((LM_DEBUG, "running short circuit test\n"));
      if (this->scavenger_barrier_.wait () == -1)
	return -1;

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
		  PortableServer::POA_ptr root_poa,
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
	  scheduler_impl = new ACE_Config_Scheduler;
	  if (scheduler_impl.get () == 0)
	    return -1;

	  RtecScheduler::Scheduler_var scheduler = 
	    scheduler_impl->_this (TAO_TRY_ENV);
	  TAO_CHECK_ENV;

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

	  if (ACE_Scheduler_Factory::use_config (naming_context.in (),
						 this->lcl_sch_name_) == -1)
	    return -1;
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
	    RtecScheduler::Scheduler::_narrow (tmpobj, TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (this->connect_ecg (local_ec.in (),
				 remote_ec.in (),
				 remote_sch,
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

      if (this->scavenger_cnt_ != 0)
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
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger svc\n"));
  if (ACE_OS::thr_setprio (this->scavenger_priority_) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) scavenger thr_setprio failed\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger waiting\n"));
  if (this->scavenger_barrier_.wait () == -1)
    return -1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting scavenger\n"));

  this->scavenger_start_ = ACE_OS::gethrtime ();
  for (int i = 0;
       i < this->scavenger_cnt_;
       ++i)
    {
      u_long n = 1279ul;
      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
      ACE::is_prime (n, 2, n / 2);
#if 0
      if (i % 100 == 0)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger iteration: %d\n",
		    i));
#endif
    }
  this->scavenger_end_ = ACE_OS::gethrtime ();
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

      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
                   this->interval_ * 10, // @@ Make it parametric
                   RtecScheduler::VERY_LOW_IMPORTANCE,
		   ORBSVCS_Time::zero,
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

      server->set (rt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
                   this->interval_ * 10, // @@ Make it parametric
                   RtecScheduler::VERY_LOW_IMPORTANCE,
		   ORBSVCS_Time::zero,
                   1,
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

      // Generate its ConsumerQOS
      char rmt[BUFSIZ];
      ACE_OS::strcpy (rmt, "ecp@");
      ACE_OS::strcat (rmt, this->rmt_ec_name_);

      RtecScheduler::handle_t rmt_info =
        remote_sch->create (rmt, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      remote_sch->set (rmt_info,
		       RtecScheduler::VERY_HIGH_CRITICALITY,
		       ORBSVCS_Time::zero,
		       ORBSVCS_Time::zero,
		       ORBSVCS_Time::zero,
		       this->interval_ * 10,
		       RtecScheduler::VERY_LOW_IMPORTANCE,
		       ORBSVCS_Time::zero,
		       1,
		       RtecScheduler::OPERATION,
		       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_ConsumerQOS_Factory consumer_qos;
      consumer_qos.start_disjunction_group ();
      consumer_qos.insert_type (this->event_a_, rmt_info);
      consumer_qos.insert_type (this->event_c_, rmt_info);
      consumer_qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rmt_info);

      // Generate its SupplierQOS
      char lcl[BUFSIZ];
      ACE_OS::strcpy (lcl, "ecp@");
      ACE_OS::strcat (lcl, this->lcl_ec_name_);

      RtecScheduler::handle_t lcl_info =
        local_sch->create (lcl, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      local_sch->set (lcl_info,
		      RtecScheduler::VERY_HIGH_CRITICALITY,
		      ORBSVCS_Time::zero,
		      ORBSVCS_Time::zero,
		      ORBSVCS_Time::zero,
		      this->interval_ * 10,
		      RtecScheduler::VERY_LOW_IMPORTANCE,
		      ORBSVCS_Time::zero,
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
	      this->lcl_time_[this->lcl_cnt_] = nsec;
	      this->lcl_cnt_++;
	      // ACE_DEBUG ((LM_DEBUG, "Latency[LOCAL]: %d\n",
	      // nsec));
	    }
	  else
	    {
	      this->rmt_time_[this->rmt_cnt_] = nsec;
	      this->rmt_cnt_++;
	      // ACE_DEBUG ((LM_DEBUG, "Latency[REMOTE]: %d\n",
	      // nsec));
	    }
        }
    }
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
Test_ECG::handle_timeout (const ACE_Time_Value &tv,
			  const void*)
{
  this->message_count_--;
  if (this->message_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB core\n"));
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      return 0;
    }

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

  ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();
  ACE_Time_Value interval (this->interval_ / ACE_ONE_SECOND_IN_USECS,
			   this->interval_ % ACE_ONE_SECOND_IN_USECS);
  return reactor->schedule_timer (this, 0, interval);
}

void
Test_ECG::dump_results (void)
{
  int i;
  for (i = 0; i < this->lcl_cnt_; ++i)
    {
      double usec = this->lcl_time_[i] / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[LCL]: %.3f\n", usec));
    }
  for (i = 0; i < this->rmt_cnt_; ++i)
    {
      double usec = this->rmt_time_[i] / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Latency[RMT]: %.3f\n", usec));
    }
  if (this->scavenger_cnt_ != 0)
    {
      double usec = (this->scavenger_end_ - this->scavenger_start_) / 1000.0;
      ACE_DEBUG ((LM_DEBUG, "Scavenger time: %.3f\n", usec));
    }
}

int
Test_ECG::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "gxl:r:s:o:t:m:u:a:b:c:");
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
	  this->scavenger_cnt_ = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'x':
	  this->short_circuit_ = 1;
	  break;
        case 't':
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
                      "-l local_ec_name "
                      "-r remote_ec_name "
		      "-d (use local scheduling service) "
		      "-s scheduling service name "
		      "-x (short circuit EC) "
		      "-u utilization test iterations "
                      "<-a event_type_a> "
                      "<-b event_type_b> "
                      "<-c event_type_c> "
                      "-t event_interval (usecs) "
                      "-m message_count "
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

  if (this->scavenger_cnt_ < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "%s: scavenger count < 0, test disabled\n"));
      this->scavenger_cnt_ = 0;
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
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_DEBUG,
		    "%s: user is not superuser, "
                    "so remain in time-sharing class\n", argv[0]));
      else
        ACE_ERROR_RETURN ((LM_ERROR,
			   "%s: ACE_OS::sched_params failed\n", argv[0]),
                          1);
    }

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
