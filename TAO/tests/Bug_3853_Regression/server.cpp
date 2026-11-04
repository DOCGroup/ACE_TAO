#include "Hello_i.h"
#include "HelloC.h"

#include "ace/Task.h"
#include "ace/Get_Opt.h"

#include <fstream>
#include <iostream>
#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior_output_file,
            std::basic_string<ACE_TCHAR> &shutdown_ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:s:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        shutdown_ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-s <shutdown_iorfile>"
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
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};
      std::basic_string<ACE_TCHAR> ior_output_file {};
      std::basic_string<ACE_TCHAR> shutdown_ior_output_file {};
      if (parse_args (argc, argv, ior_output_file, shutdown_ior_output_file) != 0)
        return 1;

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      CORBA::PolicyList policies (5);
      policies.length (5);
      policies[0] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      policies[1] = root_poa->create_servant_retention_policy (PortableServer::RETAIN);
      policies[2] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);
      policies[3] =
        root_poa->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
      policies[4] = root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);

      PortableServer::POA_var const demo_poa =
        root_poa->create_POA ("HelloWorldServer", poa_manager.in (), policies);

      for (CORBA::ULong i {}; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      {
        Demo_HelloWorld_i * const hello {new Demo_HelloWorld_i (orb.in ())};
        // The servant var takes ownership of the new servant reference.
        PortableServer::ServantBase_var owner_transfer (hello);
        PortableServer::ObjectId_var const oid = PortableServer::string_to_ObjectId ("shutdown");
        demo_poa->activate_object_with_id (oid.in (), hello);
        CORBA::Object_var const object = demo_poa->id_to_reference (oid.in ());
        CORBA::String_var const ior = orb->object_to_string (object.in ());
        std::ofstream out (ACE_TEXT_ALWAYS_CHAR (shutdown_ior_output_file.c_str ()));
        out << ior.in ();
      }

      {
        Demo_HelloWorld_i * const hello {new Demo_HelloWorld_i (orb.in ())};
        // The servant var takes ownership of the new servant reference.
        PortableServer::ServantBase_var owner_transfer (hello);
        PortableServer::ObjectId_var const oid = PortableServer::string_to_ObjectId ("hello");
        demo_poa->activate_object_with_id (oid.in (), hello);
        CORBA::Object_var const object = demo_poa->id_to_reference (oid.in ());
        CORBA::String_var const ior = orb->object_to_string (object.in ());
        std::ofstream out (ACE_TEXT_ALWAYS_CHAR (ior_output_file.c_str ()));
        out << ior.in ();
      }

      poa_manager->activate ();
      std::cout << ACE_TEXT_ALWAYS_CHAR (ior_output_file.c_str ()) << " is ready\n";
      orb->run ();
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Unexpected exception: " << ex << '\n';
      return 1;
    }

  return 0;
}
