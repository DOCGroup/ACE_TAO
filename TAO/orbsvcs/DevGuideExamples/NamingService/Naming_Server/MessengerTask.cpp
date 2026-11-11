#include "MessengerTask.h"
#include "Messenger_i.h"
#include "orbsvcs/CosNamingC.h"

#include <iostream>

static CORBA::ORB_ptr
init_messenger_orb ()
{
  ACE_TCHAR arg0[] = ACE_TEXT ("Messenger");
  ACE_TCHAR *argv[] = {arg0, nullptr};
  int argc = 1;
  return CORBA::ORB_init (argc, argv, "NamingORB");
}

MessengerTask::MessengerTask ()
  : orb_ (init_messenger_orb ())
{
}

int MessengerTask::svc ()
{
  try
    {
      // Get reference to Root POA
      CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      // Activate POA Manager
      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      // Find the Naming Service
      obj = orb_->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var root =
        CosNaming::NamingContext::_narrow (obj.in ());

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
      PortableServer::Servant_var<Messenger_i> const servant = new Messenger_i;
      PortableServer::ObjectId_var const oid = poa->activate_object (servant.in ());
      obj = poa->id_to_reference (oid.in ());
      root->rebind (name, obj.in ());

      std::cout << "Messenger object bound in Naming Service" << std::endl;

      // Let this task finish after a short period while the naming task keeps
      // the shared ORB running.
      ACE_Time_Value timeout (1);
      orb_->run (timeout);

      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA exception: " << ex << std::endl;
    }

  return -1;
}


