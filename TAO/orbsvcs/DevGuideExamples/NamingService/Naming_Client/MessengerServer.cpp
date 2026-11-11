#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      TAO_Naming_Client naming_client;
      if (naming_client.init (orb.in ()) != 0)
        {
          std::cerr << "Could not initialize naming client." << std::endl;
          return 1;
        }

      CosNaming::Name name {};
      name.length (1);
      name[0].id = CORBA::string_dup ("example");
      try
        {
          CORBA::Object_var const context_object = naming_client->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var const context = naming_client->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("Messenger");
      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const object = poa->id_to_reference (object_id.in ());
      naming_client->rebind (name, object.in ());
      std::cout << "Messenger object bound in Naming Service" << std::endl;

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught a CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
