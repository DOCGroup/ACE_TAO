// $Id$

#include "Gateway.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "ace/Arg_Shifter.h"
#include "ace/Dynamic_Service.h"

static const ACE_TCHAR *supplierec = 0;
static const ACE_TCHAR *consumerec = 0;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Gateway gateway;

  return gateway.run (argc, argv);
}

// ****************************************************************

Gateway::Gateway (void)
{
}

int
Gateway::run (int argc, ACE_TCHAR* argv[])
{
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  try
    {
      // First parse our command line options
      if (this->parse_args(argc, argv) != 0)
      {
         return -1;
      }

      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel from the naming service
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name supplierecname (1);
      supplierecname.length (1);
      supplierecname[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(supplierec));

      CORBA::Object_var supplierec_obj =
        naming_context->resolve (supplierecname);

      CosNaming::Name consumerecname (1);
      consumerecname.length (1);
      consumerecname[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(consumerec));

      CORBA::Object_var consumerec_obj =
        naming_context->resolve (consumerecname);

      RtecEventChannelAdmin::EventChannel_var supplier_event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (supplierec_obj.in ());

      if (CORBA::is_nil (supplier_event_channel.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the supplier event channel.\n"),
                          1);

      RtecEventChannelAdmin::EventChannel_var consumer_event_channel =
        RtecEventChannelAdmin::EventChannel::_narrow (consumerec_obj.in ());

      if (CORBA::is_nil (consumer_event_channel.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the consumer event channel.\n"),
                          1);

      TAO_EC_Gateway_IIOP gateway;

      gateway.init(supplier_event_channel.in(), consumer_event_channel.in());

      PortableServer::ObjectId_var gateway_oid =
         poa->activate_object(&gateway);

      CORBA::Object_var gateway_obj =
         poa->id_to_reference(gateway_oid.in());

       RtecEventChannelAdmin::Observer_var obs =
         RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in());

       RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
         consumer_event_channel->append_observer (obs.in ());

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      consumer_event_channel->remove_observer (local_ec_obs_handle);

      poa->deactivate_object (gateway_oid.in ());

      // Destroy the POA
      poa->destroy (1, 0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Gateway::run");
      return 1;
    }

  return 0;
}

int
Gateway::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-s")) == 0)
        {
          arg_shifter.consume_arg ();
          supplierec = arg_shifter.get_current ();
        }
      if (ACE_OS::strcmp (arg, ACE_TEXT("-c")) == 0)
        {
          arg_shifter.consume_arg ();
          consumerec = arg_shifter.get_current ();
        }

      arg_shifter.ignore_arg ();
    }
  // Indicates sucessful parsing of the command line
  return 0;
}

