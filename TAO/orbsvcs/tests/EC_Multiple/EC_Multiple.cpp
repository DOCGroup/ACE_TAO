//
// $Id$
//

#include "ace/Get_Opt.h"

#include "ace/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "EC_Multiple.h"

// ************************************************************

EC_Proxy::EC_Proxy (void)
  :  consumer_ (this),
     supplier_ (this)
{
}

EC_Proxy::~EC_Proxy (void)
{
}

int
EC_Proxy::open (RtecEventChannelAdmin::EventChannel_ptr remote_ec,
                RtecEventChannelAdmin::EventChannel_ptr local_ec,
                const RtecEventChannelAdmin::ConsumerQOS& subscriptions,
                const RtecEventChannelAdmin::SupplierQOS& publications,
                CORBA::Environment &_env)
{
  TAO_TRY
    {
      // = Connect as a supplier to the local EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        local_ec->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                    publications,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        remote_ec->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var consumer_ref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                    subscriptions,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("EC_Proxy::open");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;

  return 0;
}

void
EC_Proxy::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier-consumer received disconnect from channel.\n"));
}

void
EC_Proxy::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier received disconnect from channel.\n"));
}

void
EC_Proxy::push (const RtecEventComm::EventSet &events,
                CORBA::Environment & _env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC_Proxy::push - "));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // ACE_DEBUG ((LM_DEBUG, "ECP: %d event(s)\n", events.length ()));

  // @@ TODO, there is an extra data copy here, we should do the event
  // modification without it and only compact the necessary events.
  int count = 0;
  RtecEventComm::EventSet out (events.length ());
  for (u_int i = 0; i < events.length (); ++i)
    {
      if (events[i].ttl_ > 0)
        {
          count++;
          out.length (count);
          out[count - 1] = events[i];
          out[count - 1].ttl_--;
        }
    }

  if (count > 0)
    {
      this->consumer_proxy_->push (out, _env);
    }
}

int
EC_Proxy::shutdown (CORBA::Environment& _env)
{
  TAO_TRY
    {
      this->consumer_proxy_->disconnect_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Disconnect from the push supplier.
      this->supplier_proxy_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN(-1);
    }
  TAO_ENDTRY;

  return 0;
}

// ****************************************************************

Test_ECP::Test_ECP (void)
  : consumer_ (this),
    supplier_ (this),
    event_a_ (0),
    event_b_ (0),
    event_c_ (0),
    interval_ (250),
    message_count_ (100)
{
}

int
Test_ECP::run (int argc, char* argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
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

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      // Register Event_Service with Naming Service.
      ACE_EventChannel ec_impl;

      RtecEventChannelAdmin::EventChannel_var ec =
	ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (ec.in (), TAO_TRY_ENV);

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
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), 1);

      ACE_DEBUG ((LM_DEBUG, "starting....\n"));

      RtecEventChannelAdmin::EventChannel_var remote_ec =
        this->get_ec (naming_context.in (),
                      this->rmt_ec_name_,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "located remote EC\n"));

      RtecEventChannelAdmin::EventChannel_var local_ec =
        this->get_ec (naming_context.in (),
                      this->lcl_ec_name_,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "located local EC\n"));

      if (this->connect_supplier (local_ec.in (),
                                  TAO_TRY_ENV) == -1)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "connected supplier\n"));

      if (this->connect_consumer (local_ec.in (),
                                  TAO_TRY_ENV) == -1)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "connected consumer\n"));

      if (this->connect_ecp (local_ec.in (),
                             remote_ec.in (),
                             TAO_TRY_ENV) == -1)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "connected proxy\n"));

      ACE_DEBUG ((LM_DEBUG, "running multiple EC test\n"));
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), 1);
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;
  return 0;
}

RtecEventChannelAdmin::EventChannel_ptr
Test_ECP::get_ec (CosNaming::NamingContext_ptr naming_context,
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
Test_ECP::connect_supplier (RtecEventChannelAdmin::EventChannel_ptr local_ec,
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
                   this->interval_ * 10000, // @@ Make it parametric
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
Test_ECP::connect_consumer (RtecEventChannelAdmin::EventChannel_ptr local_ec,
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
                   this->interval_ * 10000, // @@ Make it parametric
                   RtecScheduler::VERY_LOW_IMPORTANCE,
		   ORBSVCS_Time::zero,
                   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Time_Value tv_timeout (0, this->interval_ * 1000);
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
Test_ECP::connect_ecp (RtecEventChannelAdmin::EventChannel_ptr local_ec,
                       RtecEventChannelAdmin::EventChannel_ptr remote_ec,
                       CORBA::Environment &_env)
{
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      // Generate its ConsumerQOS
      char rmt[BUFSIZ];
      ACE_OS::strcpy (rmt, "ecp@");
      ACE_OS::strcat (rmt, this->rmt_ec_name_);

      RtecScheduler::handle_t rmt_info =
        server->create (rmt, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      server->set (rmt_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
                   this->interval_ * 10000,
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
        server->create (lcl, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      server->set (lcl_info,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
		   ORBSVCS_Time::zero,
                   this->interval_ * 10000,
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

      this->ecp_.open (remote_ec, local_ec,
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
Test_ECP::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier-consumer received disconnect from channel.\n"));
}

void
Test_ECP::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier received disconnect from channel.\n"));
}

void
Test_ECP::push (const RtecEventComm::EventSet &events,
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
	  int nsec = r - s;
	  ACE_DEBUG ((LM_DEBUG, "Latency[%d]: %d (from %d)\n",
		      this->supplier_id_, nsec, e.source_));
        }
    }
}

int
Test_ECP::shutdown (CORBA::Environment& _env)
{
  this->consumer_proxy_->disconnect_push_consumer (_env);
  if (_env.exception () != 0) return -1;

  this->supplier_proxy_->disconnect_push_supplier (_env);
  if (_env.exception () != 0) return -1;

  this->ecp_.shutdown (_env);
  if (_env.exception () != 0) return -1;

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return 0;
}

int
Test_ECP::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "l:r:a:b:c:t:m:");
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
        case 't':
          this->interval_ = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'm':
          this->message_count_ = ACE_OS::atoi (get_opt.optarg);
          break;
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-l local_ec_name "
                      "-r remote_ec_name "
                      "<-a event_type_a> "
                      "<-b event_type_b> "
                      "<-c event_type_c> "
                      "-t event_interval "
                      "-m message_count "
                      "\n",
                      argv[0]));
          return -1;
        }
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
  Test_ECP test;
  return test.run (argc, argv);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<Test_ECP>;
template class ACE_PushSupplier_Adapter<Test_ECP>;
template class ACE_PushConsumer_Adapter<EC_Proxy>;
template class ACE_PushSupplier_Adapter<EC_Proxy>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<Test_ECP>
#pragma instantiate ACE_PushSupplier_Adapter<Test_ECP>
#pragma instantiate ACE_PushConsumer_Adapter<EC_Proxy>
#pragma instantiate ACE_PushSupplier_Adapter<EC_Proxy>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
