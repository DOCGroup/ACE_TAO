#include "orbsvcs/CosNamingC.h"
#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include <fstream>
#include <iostream>

struct Server_Options
{
  ACE_TString ior_file { ACE_TEXT ("Messenger.ior") };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <ior>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      Messenger_i messenger_servant (orb.in ());
      PortableServer::ObjectId_var const oid = poa->activate_object (&messenger_servant);
      CORBA::Object_var const messenger_obj = poa->id_to_reference (oid.in ());
      CORBA::String_var const str = orb->object_to_string (messenger_obj.in ());

      {
        std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (options.ior_file.c_str ()));
        if (!ior_file)
          {
            std::cerr << "Cannot open IOR file" << std::endl;
            return 1;
          }
        ior_file << str.in () << std::endl;
        if (!ior_file)
          {
            std::cerr << "Cannot write IOR file" << std::endl;
            return 1;
          }
      }
      std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR (options.ior_file.c_str ())
                << std::endl;

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}

