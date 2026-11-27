#include "MessengerC.h"
#include "ClientInitializer.h"

#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Keep this non-owning pointer to call the derived API after ownership
      // transfers to ORBInitializer_var.
      ClientInitializer * const temp_initializer = new ClientInitializer;

      PortableInterceptor::ORBInitializer_var const orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, "Client ORB");

      // Now that the ORB is initialized (and subsequently the
      // PICurrent), we can set the slot data on the PICurrent for our
      // interceptor initializer.
      temp_initializer->set_slot_data ();

      CORBA::Object_var const naming_object = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const root =
        CosNaming::NamingContext::_narrow (naming_object.in ());
      if (CORBA::is_nil (root.in ()))
        {
          std::cerr << "Couldn't find Naming Service." << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("Messenger");

      CORBA::Object_var object {};
      while (CORBA::is_nil (object.in ()))
        {
          try
            {
              object = root->resolve (name);
            }
          catch (CosNaming::NamingContext::NotFound const &)
            {
              ACE_OS::sleep (1);
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
    }

  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
