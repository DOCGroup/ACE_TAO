// $Id$

#include "Messenger_i.h"
#include <iostream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("MessengerServer.ior");

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
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    // Get reference to Root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create an object
    PortableServer::Servant_var<Messenger_i> messenger_servant =
      new Messenger_i;

    // Write its stringified reference to a file
    PortableServer::ObjectId_var oid = poa->activate_object(messenger_servant.in());
    obj = poa->id_to_reference(oid.in());
    Messenger_var messenger = Messenger::_narrow(obj.in());
    CORBA::String_var str = orb->object_to_string(messenger.in());
    std::ofstream fout(ACE_TEXT_ALWAYS_CHAR(ior_output_file));
    fout << str.in() << std::endl;
    fout.close();
    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA::Exception " << ex << std::endl;
    return 1;
  }

  return 0;
}
