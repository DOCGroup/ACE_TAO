// $Id$

#include "Consumer.h"
#include "orbsvcs/CosEventChannelAdminS.h"

ACE_RCSID(CosEC_Examples, Consumer, "$Id$")

int
main (int argc, char* argv[])
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
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the Consumer object.
      this->orb_ = orb.in ();

      if (argc <= 1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Consumer <event_channel_ior>\n"));
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...
      object =
        orb->string_to_object (argv[1] TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (object.in ()
                                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The canonical protocol to connect to the EC
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::ProxyPushSupplier_var supplier =
        consumer_admin->obtain_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventComm::PushConsumer_var consumer =
        this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier->connect_push_consumer (consumer.in () TAO_ENV_ARG_PARAMETER);
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
Consumer::push (const CORBA::Any &
                TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->event_count_ ++;
  if (this->event_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d events received\n",
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
