#include "Messenger_i.h"
#include "MessengerS.h"
#include "ServerInitializer.h"

#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI_Server library is linked in when building statically
#include "tao/PI_Server/PI_Server.h"
#include "orbsvcs/CosNamingC.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_var const orb_initializer =
        new ServerInitializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      PortableServer::Servant_var<Messenger_i> const messenger_servant =
        new Messenger_i;

      CORBA::Object_var const naming_object = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var const root =
        CosNaming::NamingContext::_narrow (naming_object.in ());
      if (CORBA::is_nil (root.in ()))
        {
          std::cerr << "Nil Naming Context reference" << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("Messenger");

      PortableServer::ObjectId_var const oid =
        poa->activate_object (messenger_servant.in ());
      CORBA::Object_var const messenger_object = poa->id_to_reference (oid.in ());

      root->rebind (name, messenger_object.in ());

      std::cout << "Messenger bound in Naming Service" << std::endl;

      // Accept requests
      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
