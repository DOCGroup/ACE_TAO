// $Id$

#include "MessengerC.h"
#include "Smart_Messenger_Proxy.h"

#include <iostream>
int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Create a smart proxy factory.  It will register itself with the
    // smart proxy factory adapter so it can be used to create
    // Messenger proxies.  It must be created on the heap, but is
    // otherwise unused here.
    new Smart_Messenger_Proxy_Factory(orb.in());

    // Convert the contents of the Messenger.ior file to an object reference.
    CORBA::Object_var obj = orb->string_to_object("file://Messenger.ior");
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow the object reference to a Messenger object reference.
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Not a Messenger object reference" << std::endl;
      return 1;
    }

    // Create a message and send it to the Messenger.
    CORBA::String_var message = CORBA::string_dup("Hello!");
    messenger->send_message ("TAO User", "TAO Test", message.inout());
    std::cout << "Message was sent" << std::endl;

    // Release resources.
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Caught an unknown exception type" << std::endl;
    return 1;
  }

  return 0;
}
