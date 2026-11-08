#include "bidir_giop_pch.h"

#include "simple_i.h"
#include "callbackC.h"

#include "ace/Get_Opt.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include <fstream>
#include <iostream>

struct Server_Options
{
  ACE_TString ior_output_file;
  int callback_count {10};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:i:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        options.callback_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        std::cerr << "usage: " << argv[0]
                  << " -o <iorfile> -i <no_iterations>" << std::endl;
        return -1;
      }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object (orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var const root_poa (
        PortableServer::POA::_narrow (object.in ()));
      PortableServer::POAManager_var const poa_manager (
        root_poa->the_POAManager ());

      CORBA::PolicyList policies (1);
      policies.length (1);
      CORBA::Any policy_value;
      policy_value <<= BiDirPolicy::BOTH;
      policies[0] = orb->create_policy (
        BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, policy_value);
      PortableServer::POA_var const poa (
        root_poa->create_POA ("bidirPOA", poa_manager.in (), policies));
      policies[0]->destroy ();
      poa_manager->activate ();

      PortableServer::Servant_var<Simple_i> const servant (
        new Simple_i (orb.in (), options.callback_count));
      PortableServer::ObjectId_var const id (poa->activate_object (servant.in ()));
      object = poa->id_to_reference (id.in ());
      Simple_var const server (Simple::_narrow (object.in ()));

      CORBA::String_var const ior (orb->object_to_string (server.in ()));
      if (!options.ior_output_file.empty ())
        {
          std::ofstream outfile (
            ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ()));
          if (!outfile)
            {
              std::cerr << "Cannot open IOR output file." << std::endl;
              return 1;
            }
          outfile << ior.in () << std::endl;
          if (!outfile)
            {
              std::cerr << "Cannot write IOR output file." << std::endl;
              return 1;
            }
        }
      std::cout << "Activated as " << ior.in () << std::endl;

      while (!servant->call_client ())
        orb->perform_work ();

      std::cout << "Event loop finished." << std::endl;
      CORBA::Boolean const etherealize = true;
      CORBA::Boolean const wait_for_completion = true;
      poa->destroy (etherealize, wait_for_completion);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught CORBA::Exception: " << std::endl << ex << std::endl;
      return 1;
    }

  return 0;
}
