#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include <chrono>
#include <iostream>
#include <thread>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      TAO_Naming_Client naming_client;

      if (naming_client.init (orb.in ()) != 0)
        {
          std::cerr << "Could not initialize naming client." << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (2);
      name[0].id = CORBA::string_dup ("example");
      name[1].id = CORBA::string_dup ("Messenger");
      CORBA::Object_var object {};
      while (CORBA::is_nil (object.in ()))
        {
          try
            {
              object = naming_client->resolve (name);
            }
          catch (CosNaming::NamingContext::NotFound const &)
            {
              std::this_thread::sleep_for (std::chrono::seconds (1));
            }
        }

      Messenger_var const messenger = Messenger::_narrow (object.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "Not a Messenger reference" << std::endl;
          return 1;
        }

      CORBA::String_var message = CORBA::string_dup ("Hello!");
      messenger->send_message ("TAO User", "TAO Test", message.inout ());
      std::cout << "Message was sent" << std::endl;
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
