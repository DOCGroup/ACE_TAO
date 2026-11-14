#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"
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
      name[0].id = CORBA::string_dup ("example");
      try
        {
          CORBA::Object_var const context_object = root->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          CosNaming::NamingContext_var const context = root->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("Messenger");
      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;
      PortableServer::ObjectId_var const object_id = poa->activate_object (servant.in ());
      CORBA::Object_var const object = poa->id_to_reference (object_id.in ());
      Messenger_var const messenger = Messenger::_narrow (object.in ());
      root->rebind (name, messenger.in ());
      std::cout << "Messenger object bound in Naming Service" << std::endl;

      orb->run ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "server: Caught a CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
