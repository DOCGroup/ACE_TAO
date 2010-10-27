// $Id$

#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    const ACE_TCHAR *url = ACE_TEXT("corbaname:rir:#example/Messenger"); // default URL to InitRef
    if ( argc < 2 ) {
      std::cout << "Defaulting URL to " << url << std::endl;
      std::cout << "Usage: " << argv[0]
           << " [-ORB options] [corbaname URL for message server]" << std::endl;
    }
    else {
      url = argv[1];
    }

    // Find the Naming Service  & the Message Server name in it
    CORBA::Object_var obj = orb->string_to_object(url);

    // Narrow
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if( CORBA::is_nil( messenger.in() ) ) {
      std::cerr << "Not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup( "Hello!" );
    messenger->send_message( "TAO User", "TAO Test", message.inout() );

    std::cout << "Message was sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "client:Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
