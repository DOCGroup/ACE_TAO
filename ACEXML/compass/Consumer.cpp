// $Id$

#include "Consumer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

extern "C"
int
createConsumer (int argc, char* argv[])
{
  Consumer consumer;
  return consumer.run (argc, argv);
}

Consumer::Consumer (void)
  : event_count_ (0), shutdown_ (0)
{
}

int
Consumer::run (int argc, char* argv[])
{
//   int register_with_object_manager = 0;
//   if (TAO_Singleton_Manager::instance ()->init (
//         register_with_object_manager) == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Consumer::init -- ORB pre-initialization "
//                        "failed.\n"),
//                       -1);  // No exceptions available yet, so return
//                             // an error status.

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Prepend a "dummy" program name argument to the Service
      // Configurator argument vector.
      int new_argc = argc + 1;

      CORBA::StringSeq new_argv (new_argc);
      new_argv.length (new_argc);

      // Prevent the ORB from opening the Service Configurator file
      // again since the Service Configurator file is already in the
      // process of being opened.
      new_argv[0] = CORBA::string_dup ("dummy");

      // Copy the remaining arguments into the new argument vector.
      for (int i = new_argc - argc, j = 0;
           j < argc;
           ++i, ++j)
        new_argv[i] = CORBA::string_dup (argv[j]);

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (new_argc,
                                            new_argv.get_buffer (),
                                            "Consumer"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the run() method.
      this->orb_ = orb.in ();

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Naming_Client naming_client;

      // Initialization of the naming service.
      if (naming_client.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name ec_name (1);
      ec_name.length (1);
      ec_name[0].id = CORBA::string_dup ("EventChannel");

      CORBA::Object_var ec_obj =
        naming_client->resolve (ec_name
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
        consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushConsumer_var consumer =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Simple subscription, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::ConsumerQOS qos;
      qos.dependencies.length (2);
      RtecEventComm::EventHeader& h0 =
        qos.dependencies[0].event.header;
      h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
      h0.source = ACE_ES_EVENT_SOURCE_ANY;

      RtecEventComm::EventHeader& h1 =
        qos.dependencies[1].event.header;
      h1.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      h1.source = ACE_ES_EVENT_SOURCE_ANY;

      supplier->connect_push_consumer (consumer.in (), qos
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Consumer::run");
      return 1;
    }
  ACE_ENDTRY;
//   if (TAO_Singleton_Manager::instance ()->fini () == -1)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "Consumer::fini -- ORB pre-termination failed."),
//                       -1);
  ACE_DEBUG ((LM_DEBUG, "Consumer exiting...\n"));
  return 0;
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  this->event_count_ += events.length ();
  if (this->event_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d events received\n",
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->shutdown_ = 1;
  this->orb_->destroy (ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
