#include "amh_ami_pch.h"

#include "inner_i.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>
#include <string>

struct Inner_Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("inner.ior")};
  bool crash_on_answer {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Inner_Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("co:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'c':
        options.crash_on_answer = true;
        break;
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-c "
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
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Inner_Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const root_object (
        orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var const poa (
        PortableServer::POA::_narrow (root_object.in ()));
      PortableServer::POAManager_var const manager (poa->the_POAManager ());
      manager->activate ();

      int const divisor = options.crash_on_answer ? 0 : 1;
      PortableServer::Servant_var<Inner_i> const servant (new Inner_i (divisor));
      PortableServer::ObjectId_var const oid (poa->activate_object (servant.in ()));
      CORBA::Object_var const object (poa->id_to_reference (oid.in ()));
      CORBA::String_var const ior (orb->object_to_string (object.in ()));

      std::string const ior_name (ACE_TEXT_ALWAYS_CHAR (options.ior_output_file));
      std::ofstream ior_file (ior_name);
      if (!ior_file)
        {
          std::cerr << "Cannot open IOR output file." << std::endl;
          return 1;
        }
      ior_file << ior.in () << std::endl;
      if (!ior_file)
        {
          std::cerr << "Cannot write IOR output file." << std::endl;
          return 1;
        }
      std::cout << "IOR written to " << ior_name << std::endl;

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
