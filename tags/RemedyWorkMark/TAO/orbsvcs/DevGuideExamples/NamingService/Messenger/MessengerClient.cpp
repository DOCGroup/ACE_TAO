// $Id$

#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Find the Naming Service
    CORBA::Object_var naming_obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root =
      CosNaming::NamingContextExt::_narrow(naming_obj.in());
    if (CORBA::is_nil(root.in())) {
      std::cerr << "Nil Naming Context reference" << std::endl;
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
        obj = root->resolve_str("example/Messenger");
      } catch (const CosNaming::NamingContext::NotFound&) {
        // Sleep for a second and try again
        ACE_OS::sleep(1);
      }
    }

    // Narrow the Messenger object reference
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup("Hello!");

    // Send a message
    messenger->send_message("TAO User", "TAO Test", message.inout());

    std::cout << "Message was sent" << std::endl;

    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
