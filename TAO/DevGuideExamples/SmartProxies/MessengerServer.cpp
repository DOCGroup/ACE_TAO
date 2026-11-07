#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

ACE_TCHAR const *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
                           "usage:  %s "
                           "-o <iorfile>"
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
  try {
    // Initialize orb
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    //Get reference to Root POA
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

    // Activate POA Manager
    PortableServer::POAManager_var const mgr = poa->the_POAManager ();
    mgr->activate();

    // Create an object
    PortableServer::Servant_var<Messenger_i> const messenger_servant = new Messenger_i;

    // Write its strigified reference to a file
    PortableServer::ObjectId_var const oid =
      poa->activate_object (messenger_servant.in ());
    CORBA::Object_var const messenger_obj = poa->id_to_reference (oid.in ());
    CORBA::String_var const str = orb->object_to_string (messenger_obj.in ());
    std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (ior_output_file));
    ior_file << str.in () << std::endl;
    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }

  catch (CORBA::Exception const &ex) {
    std::cerr << "Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
