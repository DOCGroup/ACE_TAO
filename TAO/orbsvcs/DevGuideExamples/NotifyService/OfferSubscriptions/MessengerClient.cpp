#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var root_obj =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var root_context =
        CosNaming::NamingContext::_narrow (root_obj.in ());

    CosNaming::Name name;
    name.length (1);
      name[0].id = CORBA::string_dup ("MessengerService");

      CORBA::Object_var messenger_obj = root_context->resolve (name);

      Messenger_var messenger = Messenger::_narrow (messenger_obj.in ());

      if (CORBA::is_nil (messenger.in ()))
        std::cerr << "Argument is not a Messenger reference" << std::endl;

      CORBA::String_var message = CORBA::string_dup ("Where can I get TAO?");

      messenger->send_message ("person@company.com",
                               "OCI's Distribution of TAO",
                               message.inout ());
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught exception: " << ex << std::endl;
      return 1;
    }

  std::cout << "MessengerClient: success" << std::endl;
  return 0;
}

