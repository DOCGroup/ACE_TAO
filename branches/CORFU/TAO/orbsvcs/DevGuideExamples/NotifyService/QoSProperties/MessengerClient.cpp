// $Id$

#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>

int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

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

    CORBA::String_var message = CORBA::string_dup(
      "Where can I get TAO?");
    for (int i=0; i<3; i++) {
      messenger->send_message ("person@company.com",
                               "OCI's Distribution of TAO",
                               message.inout());
    }
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "Caught exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
