// $Id$

#include "Gateway.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "ace/Arg_Shifter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(EC_Examples, Supplier, "$Id$")

static const char* supplierec = 0;
static const char* consumerec = 0;

int
main (int argc, char* argv[])
{
  Gateway gateway;

  return gateway.run (argc, argv);
}

// ****************************************************************

Gateway::Gateway (void)
{
}

int
Gateway::run (int argc, char* argv[])
{
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
//      TAO_EC_Gateway_IIOP* gateway = 0;

      // First parse our command line options
      if (this->parse_args(argc, argv) != 0)
      {
         return -1;
      }

      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      // Obtain the event channel from the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name supplierecname (1);
      supplierecname.length (1);
      supplierecname[0].id = CORBA::string_dup (supplierec);

      CORBA::Object_var supplierec_obj =
        naming_context->resolve (supplierecname ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::Name consumerecname (1);
      consumerecname.length (1);
      consumerecname[0].id = CORBA::string_dup (consumerec);

      CORBA::Object_var consumerec_obj =
        naming_context->resolve (consumerecname ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var supplier_event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (supplierec_obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (supplier_event_channel.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the supplier event channel.\n"),
                          1);

      RtecEventChannelAdmin::EventChannel_var consumer_event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (consumerec_obj.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (consumer_event_channel.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the consumer event channel.\n"),
                          1);

      TAO_EC_Gateway_IIOP gateway;

      gateway.init(supplier_event_channel.in(), consumer_event_channel.in());

      PortableServer::ObjectId_var gateway_oid =
         poa->activate_object(&gateway);
      ACE_TRY_CHECK;

      CORBA::Object_var gateway_obj =
         poa->id_to_reference(gateway_oid.in());
      ACE_TRY_CHECK;

       RtecEventChannelAdmin::Observer_var obs =
         RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in());
      ACE_TRY_CHECK;

       RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
         consumer_event_channel->append_observer (obs.in ());
      ACE_TRY_CHECK;

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();
      ACE_TRY_CHECK;

      // Destroy the POA
      poa->destroy (1, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Gateway::run");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Gateway::parse_args (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-s") == 0)
        {
          arg_shifter.consume_arg ();
          supplierec = arg_shifter.get_current ();
        }
      if (ACE_OS::strcmp (arg, "-c") == 0)
        {
          arg_shifter.consume_arg ();
          consumerec = arg_shifter.get_current ();
        }

      arg_shifter.ignore_arg ();
    }
  // Indicates sucessful parsing of the command line
  return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
