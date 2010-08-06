// $Id$

#include "MessengerC.h"
#include <iostream>
int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Read and destringify the Messenger object's IOR.
    CORBA::Object_var obj = orb->string_to_object( "file://Messenger.ior" );
    if( CORBA::is_nil( obj.in() ) ) {
      std::cerr << "Could not get Messenger IOR." << std::endl;
      return 1;
    }

    // Narrow the IOR to a Messenger object reference.
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if( CORBA::is_nil( messenger.in() ) ) {
      std::cerr << "IOR was not a Messenger object reference." << std::endl;
      return 1;
    }

    // Send a message the the Messenger object.
    CORBA::String_var message = CORBA::string_dup( "Hello!" );
    messenger->send_message( "TAO User", "TAO Test", message.inout() );

    // Print the Messenger's reply.
    std::cout << "Reply: " << message.in() << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
