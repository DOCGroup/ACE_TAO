// $Id$

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ImR_Client/ImR_Client.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

#include "bug_689_regression_i.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-o <ior file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_Argv_Type_Converter argcon (argc, argv);
      CORBA::ORB_var orb = CORBA::ORB_init (argcon.get_argc (),
                                            argcon.get_TCHAR_argv ());

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (5);
      policies.length (5);
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      policies[2] =
        root_poa->create_request_processing_policy (
         PortableServer::USE_DEFAULT_SERVANT);

      policies[3] =
        root_poa->create_servant_retention_policy (
          PortableServer::NON_RETAIN);

      policies[4] =
        root_poa->create_id_uniqueness_policy (
          PortableServer::MULTIPLE_ID);

      PortableServer::POA_var new_poa =
        root_poa->create_POA ("test_server",
                              poa_manager.in (),
                              policies);

      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          policies[i]->destroy ();
        }

      PortableServer::ObjectId_var id =
              PortableServer::string_to_ObjectId ("server");

      CORBA::Object_var object =
        new_poa->create_reference_with_id (id.in (),
                                           "IDL:bug_689_regression:1.0");

      bug_689_regression_var bug_689_regression =
        bug_689_regression::_unchecked_narrow (object.in ());

      bug_689_regression_i impl (orb.in ());
      new_poa->set_servant (&impl);

      poa_manager->activate ();

      CORBA::String_var ior = orb->object_to_string (bug_689_regression.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      orb->run ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
