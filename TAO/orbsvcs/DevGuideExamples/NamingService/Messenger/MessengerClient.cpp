#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"

#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // Find the Naming Service
    CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContextExt_var const root =
      CosNaming::NamingContextExt::_narrow (naming_obj.in ());
    if (CORBA::is_nil (root.in ()))
      {
        std::cerr << "Nil Naming Context reference" << std::endl;
        return 1;
      }

    // Resolve the Messenger object
    CORBA::Object_var obj {};
    while (CORBA::is_nil (obj.in ()))
      {
        try
          {
            obj = root->resolve_str ("example/Messenger");
          }
        catch (CosNaming::NamingContext::NotFound const &)
          {
            ACE_OS::sleep (1);
          }
      }

    // Narrow the Messenger object reference
    Messenger_var const messenger = Messenger::_narrow (obj.in ());
    if (CORBA::is_nil (messenger.in ()))
      {
        std::cerr << "Not a Messenger reference" << std::endl;
        return 1;
      }

    CORBA::String_var message = CORBA::string_dup ("Hello!");

    // Send a message
    messenger->send_message ("TAO User", "TAO Test", message.inout ());

    std::cout << "Message was sent" << std::endl;

    orb->destroy ();
  }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
