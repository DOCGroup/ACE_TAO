// $Id$

#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Find the Naming Service
    TAO_Naming_Client naming_client;

    if (naming_client.init (orb.in ()) != 0) {
      std::cerr << "Could not initialize naming client." << std::endl;
      return 1;
    }

    // Resolve the Messenger object
    CosNaming::Name name;
    name.length( 2 );
    name[0].id = CORBA::string_dup( "example" );
    name[1].id = CORBA::string_dup( "Messenger" );
    CORBA::Object_var obj = CORBA::Object::_nil();
    while (CORBA::is_nil(obj.in())) {
      try {
        obj = naming_client->resolve(name);
      } catch (const CosNaming::NamingContext::NotFound&) {
        // Sleep for a second and try again
        ACE_OS::sleep(1);
      }
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup( "Hello!" );
    messenger->send_message( "TAO User", "TAO Test", message.inout() );

    std::cout << "Message was sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
