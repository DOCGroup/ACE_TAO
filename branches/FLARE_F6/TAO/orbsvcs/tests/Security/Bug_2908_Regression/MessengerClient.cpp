// $Id$

#include "MessengerC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv );

    const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
    CORBA::Object_var obj =
      orb->string_to_object (ior);

    // Narrow the Messenger object reference
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      ACE_ERROR((LM_ERROR, " ERROR: Client cannot get a Messenger reference\n"));
      return 1;
    }

    CORBA::String_var message = CORBA::string_dup("Hello!");

    // Send a message
    messenger->send_message("user", "TAO Test", message.inout());

    ACE_DEBUG((LM_DEBUG, "CLIENT: Message was sent\n"));

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
