#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      ACE_TCHAR const *url = ACE_TEXT ("corbaloc:rir:/NameService");
      if (argc < 2)
        {
          std::cout << "Defaulting URL to " << ACE_TEXT_ALWAYS_CHAR (url) << std::endl;
          std::cout << "Usage: " << ACE_TEXT_ALWAYS_CHAR (argv[0])
                    << " [corbaloc URL] [-ORB options]" << std::endl;
        }
      else
        url = argv[1];

      CORBA::Object_var object = orb->string_to_object (url);
      CosNaming::NamingContextExt_var root =
        CosNaming::NamingContextExt::_narrow (object.in ());
      if (CORBA::is_nil (root.in ()))
        {
          std::cerr << "Nil Naming Context reference" << std::endl;
          return 1;
        }

      object = CORBA::Object::_nil ();
      while (CORBA::is_nil (object.in ()))
        {
          try
            {
              object = root->resolve_str ("example/Messenger");
            }
          catch (CosNaming::NamingContext::NotFound const &)
            {
              ACE_OS::sleep (1);
            }
        }

      Messenger_var messenger = Messenger::_narrow (object.in ());
      if (CORBA::is_nil (messenger.in ()))
        {
          std::cerr << "Not a Messenger reference" << std::endl;
          return 1;
        }

      CORBA::String_var message = CORBA::string_dup ("Hello!");
      messenger->send_message ("TAO User", "TAO Test", message.inout ());
      std::cout << "Message was sent" << std::endl;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "client: Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
