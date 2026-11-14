#include "amh_ami_pch.h"

#include "middle_i.h"
#include "ace/Get_Opt.h"
#include <iostream>
#include <fstream>
#include <string>

struct Middle_Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("middle.ior")};
  ACE_TString ior_input_file {ACE_TEXT ("file://inner.ior")};
  bool use_sync {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Middle_Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:i:n"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        options.ior_input_file = get_opts.opt_arg ();
        break;
      case 'n':
        options.use_sync = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior_output_file> "
                           "-i <ior_input_file> "
                           "-n"
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
      Middle_Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object (orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var const poa (
        PortableServer::POA::_narrow (object.in ()));
      PortableServer::POAManager_var const manager (poa->the_POAManager ());
      manager->activate ();

      object = orb->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (options.ior_input_file.c_str ()));
      Inner_var const peer (Inner::_narrow (object.in ()));
      if (CORBA::is_nil (peer.in ()))
        {
          std::cerr << "Could not initialize peer object reference" << std::endl;
          return 1;
        }

      PortableServer::ServantBase_var servant;
      if (options.use_sync)
        servant = new Middle_i (peer.in ());
      else
        servant = new Asynch_Middle_i (poa.in (), peer.in ());

      PortableServer::ObjectId_var const oid (poa->activate_object (servant.in ()));
      object = poa->id_to_reference (oid.in ());
      CORBA::String_var const ior (orb->object_to_string (object.in ()));

      std::string const ior_name (
        ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ()));
      std::ofstream ior_file (ior_name);
      if (!ior_file)
        {
          std::cerr << "Cannot open IOR output file." << std::endl;
          return 1;
        }
      ior_file << ior.in () << std::endl;
      ior_file.close ();
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
