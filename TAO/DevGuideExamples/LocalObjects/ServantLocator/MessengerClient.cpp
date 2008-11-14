// $Id$

#include "MessengerC.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Converts the contents of the file to an object reference
    CORBA::Object_var obj = orb->string_to_object ("file://Messenger.ior");
    if (CORBA::is_nil (obj.in())) {
      std::cerr << "Nill Messenger reference" << std::endl;
      return 1;
    }

    // Narrow the object reference to a Messenger object reference
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil (messenger.in())) {
      std::cerr << "Not a Messenger object reference" << std::endl;
      return 1;
    }

    // Create a message and send it to the Messenger
    CORBA::String_var message = CORBA::string_dup("Hello!");
    messenger->send_message ("TAO User", "TAO Test", message.inout());

    orb->destroy();
    std::cout << "Message was sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Client Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }
  std::cout << "Message was sent" << std::endl;
  return 0;
}
