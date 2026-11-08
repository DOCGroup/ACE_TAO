/* -*- C++ -*-  */

#include "Messenger_i.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include <fstream>
#include <iostream>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("Messenger.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      // Get reference to Root POA
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());

      // Activate POA Manager
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Create an object
      Messenger_i messenger_servant;

      // Register the servant with the RootPOA, obtain its object
      // reference, stringify it, and write it to a file.
      PortableServer::ObjectId_var oid =
        poa->activate_object (std::addressof (messenger_servant));
      CORBA::Object_var messenger_obj = poa->id_to_reference (oid.in ());
      CORBA::String_var str = orb->object_to_string (messenger_obj.in ());
      std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ()));
      if (!ior_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Unable to open %s for writing.\n"),
                           options.ior_output_file.c_str ()),
                          1);
      ior_file << str.in () << std::endl;
      if (!ior_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed writing IOR to %s.\n"),
                           options.ior_output_file.c_str ()),
                          1);
      std::cout << "IOR written to file "
                << ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ())
                << std::endl;

      // Accept requests
      orb->run ();
      orb->destroy ();
    }

  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
