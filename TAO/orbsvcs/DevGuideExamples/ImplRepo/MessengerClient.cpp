// $Id$

#include "MessengerC.h"
#include <iostream>
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Destringify ior
    CORBA::Object_var obj = orb->string_to_object( "file://Messenger.ior" );
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Argument is not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup( "Hello!" );
    messenger->send_message( "TAO User", "TAO Test", message.inout() );

    std::cout << "message was sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Client main() Caught CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
