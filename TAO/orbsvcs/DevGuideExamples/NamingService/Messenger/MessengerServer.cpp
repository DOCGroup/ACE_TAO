#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize orb
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // Get reference to Root POA
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

    // Activate POA Manager
    PortableServer::POAManager_var const manager = poa->the_POAManager ();
    manager->activate ();

    // Find the Naming Service
    CORBA::Object_var const naming_obj = orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var const root =
      CosNaming::NamingContext::_narrow (naming_obj.in ());
    if (CORBA::is_nil (root.in ()))
      {
        std::cerr << "Nil Naming Context reference" << std::endl;
        return 1;
      }

    // Bind the example Naming Context, if necessary
    CosNaming::Name name;
    name.length (1);
    name[0].id = CORBA::string_dup ("example");
    try
      {
        CORBA::Object_var const existing_context = root->resolve (name);
      }
    catch (CosNaming::NamingContext::NotFound const &)
      {
        CosNaming::NamingContext_var const new_context = root->bind_new_context (name);
      }

    // Bind the Messenger object
    name.length (2);
    name[1].id = CORBA::string_dup ("Messenger");

    // Create an object
    PortableServer::Servant_var<Messenger_i> const messenger_servant = new Messenger_i;
    PortableServer::ObjectId_var const oid = poa->activate_object (messenger_servant.in ());
    obj = poa->id_to_reference (oid.in ());
    root->rebind (name, obj.in ());

    std::cout << "Messenger object bound in Naming Service" << std::endl;

    // Accept requests
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
