#include "RTEvent_Consumer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID (RTEvent,
           RTEvent_Consumer,
           "$Id$")


#define NAMING_SERVICE_NAME "NameService"
#define EVENT_TLS_LOG_FACTORY_NAME "RTEventLogFactory"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

// ****************************************************************

Consumer::Consumer (void)
  : event_count_ (0)
{
}

int
Consumer::run (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the run() method.
      this->orb_ = orb.in ();
/*
      if (argc <= 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Consumer <event_channel_ior>\n"));
          return 1;
        }
*/
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

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
/*      object =
        orb->string_to_object (argv[1] ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
*/

     // Obtain the event channel
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references (NAMING_SERVICE_NAME
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_THROW_RETURN (CORBA::UNKNOWN (), 0);

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (EVENT_TLS_LOG_FACTORY_NAME);

      CORBA::Object_var obj =
        this->naming_context_->resolve (name
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->event_log_factory_ =
        RTEventLogAdmin::EventLogFactory::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // The canonical protocol to connect to the EC

      this->supplier_ =
        this->event_log_factory_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
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

      this->supplier_->connect_push_consumer (consumer.in (), qos
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Consumer::run");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (events);
  
  this->event_count_ ++;

      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d log generated events received\n",
                  this->event_count_));
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
