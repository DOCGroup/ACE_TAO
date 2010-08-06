// $Id$

#include "Messenger_i.h"
#include <fstream>
#include <iostream>
#include <fstream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Set a wait time to an integer if it has been passed as a
    // command line argument. Otherwise, have
    // Messenger_i::send_message() throw an exception if e
    // has been passed as the command lin argument.
    unsigned int seconds_to_wait = 0;
    CORBA::Boolean servant_throws_exception = 0;
    if (argc == 2)
    {
      if (argv[1][0] == 'e')
      {
        servant_throws_exception = 1;
        std::cout << "Messenger_i::send_message() will throw an exception." << std::endl;
      }
      else
      {
        seconds_to_wait = ACE_OS::atoi(argv[1]);
        std::cout << "Messenger_i::send_message() will wait "
          << seconds_to_wait << " seconds" << std::endl;
      }
    }

    // Get reference to Root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create an object
    Messenger_i servant(seconds_to_wait, servant_throws_exception);

    // Write its stringified reference to stdout
    PortableServer::ObjectId_var oid =  poa->activate_object(&servant);
    obj = poa->id_to_reference(oid.in());
    Messenger_var messenger = Messenger::_narrow(obj.in());
    CORBA::String_var str = orb->object_to_string(messenger.in());
    std::ofstream fout("MessengerServer.ior");
    fout << str.in() << std::endl;
    fout.close();
    std::cout << "IOR written to file MessengerServer.ior" << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
