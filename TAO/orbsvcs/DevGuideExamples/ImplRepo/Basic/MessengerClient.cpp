// $Id$

#include "MessengerC.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->string_to_object("file://messenger.ior");

    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Unable to get a Messenger reference." << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup("Hello!");
    messenger->send_message("TAO User", "TAO Test", message.inout());
    std::cout << "message was sent" << std::endl;
    std::cout << "Reply was : " << message.in() << std::endl;

    return 0;
  } catch(const CORBA::Exception& ex) {
    std::cerr << "Client main() Caught Exception: " << ex << std::endl;
  }
  return 1;
}
