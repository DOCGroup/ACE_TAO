// $Id$

#include "EC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event/EC_Gateway.h"
#include "ace/Arg_Shifter.h"

ACE_RCSID(EC_Examples, Supplier, "$Id$")

static const char* ecname = 0;

int
main (int argc, char* argv[])
{
  EC channel;

  return channel.run (argc, argv);
}

// ****************************************************************

EC::EC (void)
{
}

int
EC::run (int argc, char* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
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

      PortableServer::POA_var rootpoa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var root_poa_manager =
        rootpoa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create persistent POA
      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        rootpoa->create_id_assignment_policy (PortableServer::USER_ID
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        rootpoa->create_lifespan_policy (PortableServer::PERSISTENT
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_CString poaname = "POA";
      PortableServer::POA_var child_poa_ =
         rootpoa->create_POA (poaname.c_str (),
                          root_poa_manager.in (),
                          policies
                          ACE_ENV_ARG_PARAMETER);

      // Create a local event channel and register it with the RootPOA.
      TAO_EC_Event_Channel_Attributes attributes (rootpoa.in (), rootpoa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate ();

      PortableServer::ObjectId_var ecId = PortableServer::string_to_ObjectId(ecname);

      child_poa_->activate_object_with_id(ecId.in(), &ec_impl);
      ACE_TRY_CHECK;

      CORBA::Object_var ec_obj = child_poa_->id_to_reference(ecId.in());
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow(ec_obj.in());
      ACE_TRY_CHECK;

      // Find the Naming Service.
      object = orb->resolve_initial_references("NameService");
      ACE_TRY_CHECK;

      CosNaming::NamingContextExt_var naming_context =
        CosNaming::NamingContextExt::_narrow(object.in());
      ACE_TRY_CHECK;

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup (ecname);
      name[0].kind = CORBA::string_dup ("");

      // Register with the name server
      naming_context->rebind (name, ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "EC::run");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

int
EC::parse_args (int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-e") == 0)
        {
          arg_shifter.consume_arg ();
          ecname = arg_shifter.get_current ();
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
