// $Id$

#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>
#include <fstream>
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    char url[200];
    std::ifstream iorFile( "url.ior" );
    iorFile >> url;
    iorFile.close();

    std::cout << "url: " << url << std::endl;

        // Find the Naming Service  & the Message Server name in it
    CORBA::Object_var obj = orb->string_to_object(url);

    // Narrow
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup("Hello!");
    messenger->send_message( "TAO User", "TAO Test", message.inout());

    std::cout << "Message was sent" << std::endl;

    std::cout << "Now try the same thing with the simple name." << std::endl;
    obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContextExt_var root =
      CosNaming::NamingContextExt::_narrow(obj.in());
    obj = root->resolve_str("Simple/Messenger");
    messenger = Messenger::_narrow(obj.in());
    messenger->send_message( "ACE User", "TAO Test", message.inout());

    std::cout << "Message was sent" << std::endl;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
