// $Id$

#include "Messenger_i.h"
#include <iostream>
int ACE_TMAIN (int, ACE_TCHAR **)
{
  try {
    // Construct a Messenger object and use it "as if" it's a corba object.
    // Put it into CORBA object reference
    // comparable to activation, narrow, etc.
    Messenger_var messenger(new Messenger_i);

    // Send a message the the Messenger object.
    CORBA::String_var message = CORBA::string_dup ("Hello!");
    messenger->send_message("TAO User", "TAO Test", message.inout());

    // Print the Messenger's reply.
    std::cout << "Reply: " << message.in() << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught CORBA::Exception : " << ex << std::endl;
    return 1;
  }

  return 0;
}


