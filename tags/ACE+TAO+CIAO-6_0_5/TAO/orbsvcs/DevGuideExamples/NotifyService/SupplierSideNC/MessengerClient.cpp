// $Id$

#include "orbsvcs/CosNamingC.h"
#include "MessengerC.h"
#include <iostream>
int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var rootObj =
      orb->resolve_initial_references("NameService");

    CosNaming::NamingContext_var rootContext =
      CosNaming::NamingContext::_narrow(rootObj.in());

    CosNaming::Name name;
    name.length (1);
    name[0].id = CORBA::string_dup ("MessengerService");

    CORBA::Object_var messengerObj = rootContext->resolve(name);

    if (CORBA::is_nil(messengerObj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow(messengerObj.in());
    if (CORBA::is_nil(messenger.in ())) {
      std::cerr << "Argument is not a Messenger reference" << std::endl;
      return 1;
    }

    CORBA::String_var message =
      CORBA::string_dup("Where can I get TAO?");
    messenger->send_message ("person@company.com",
      "OCI's Distribution of TAO",
      message.inout());

    message = CORBA::string_dup("I need TAO now.");
    messenger->send_message ("person@company.com",
      "OCI's Distribution of TAO",
      message.inout());

  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }

  std::cout << "MessengerClient: success" << std::endl;
  return 0;
}


