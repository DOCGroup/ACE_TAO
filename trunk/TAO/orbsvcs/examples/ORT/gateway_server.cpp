// $Id$

#include "Object_Factory_i.h"
#include "Gateway_i.h"

#include "ace/Get_Opt.h"

const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-o <iorfile>"
                           "\n",
                           argv[0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "gateway_server_orb"
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      /// Resolve reference to RootPOA
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Narrow it down correctly.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Check for nil references
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      /// Get poa_manager reference
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Activate it.
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ///@}

      CORBA::PolicyList policies (3);
      policies.length (3);

      policies [0] =
        root_poa->create_servant_retention_policy (PortableServer::RETAIN
                                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies [1] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT
                                                    TAO_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      policies [2] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID
                                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var gateway_poa =
        root_poa->create_POA ("Gateway_POA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i != policies.length (); ++i) {
        policies[i]->destroy ();
      }

      // Get the POA Current object reference
      obj =
        orb->resolve_initial_references ("POACurrent"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference to a POA Current reference
      PortableServer::Current_var poa_current =
        PortableServer::Current::_narrow (obj.in ()
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Gateway_i *gateway;

      ACE_NEW_THROW_EX (gateway,
                        Gateway_i (orb.in (),
                                   poa_current.in ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      gateway_poa->set_servant (gateway TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Get the ObjectID
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("Object_Factory");

      /// This class is used to create a object reference.
      Object_Factory_i *object_factory;

      ACE_NEW_THROW_EX (object_factory,
                        Object_Factory_i (orb.in (),
                                          gateway_poa.in ()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      /// Activate the Object_Factory_i Object
      gateway_poa->activate_object_with_id (oid.in (),
                                            object_factory
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the object reference.
      CORBA::Object_var gateway_object_factory =
        gateway_poa->id_to_reference (oid.in ());

      /// Convert the object reference to a string format.
      CORBA::String_var ior =
        orb->object_to_string (gateway_object_factory.in ()
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// If the ior_output_file exists, output the IOR to it.
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing "
                               "IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ORT test (gateway_server):");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
