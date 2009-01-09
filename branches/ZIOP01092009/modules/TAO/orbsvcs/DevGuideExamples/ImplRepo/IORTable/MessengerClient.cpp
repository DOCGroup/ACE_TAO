// $Id$

#include "MessengerC.h"
#include "ace/SString.h"
#include <iostream>


int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  try {
    if (argc <= 1) {
      std::cerr << "Error: Must specify the name of an IOR file." << std::endl;
      return 1;
    }
    ACE_TString ior = ACE_TEXT("file://");
    ior += argv[1];

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->string_to_object(ior.c_str());

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
