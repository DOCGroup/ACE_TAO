// $Id$
// server.cpp

#include "status_i.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNamingC.h"

int main (int argc, char* argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv,
                         "" /* the ORB name, it can be anything! */
                         );

      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG, "\nUsage:\n  %s [NAME to insert in Naming Service]\n", argv[0]));
          return -1;
        }
      // Get a reference to the RootPOA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      // narrow down to the correct reference
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Set a POA Manager
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      // Activate the POA Manager
      poa_manager->activate ();

      // Create the servant
      corbaloc_Status_i status_i;
      status_i.set_name (argv[1]);
      // Activate it to obtain the reference
      corbaloc::Status_var status =
        status_i._this ();

      // Get a reference to Naming Context
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService");

      // Narrow down the reference
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      // Bind Iterator_Factory to the Naming Context
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (argv[1]);

      naming_context->rebind (name, status.in ());

      // Run the orb
      orb->run ();

      // Destroy the POA, waiting until the destruction terminates
      poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (const CORBA::SystemException& ex){
    ex._tao_print_exception ("CORBA exception raised! ");
  }
  catch (const CORBA::Exception& ex){
    ex._tao_print_exception ("Exception caught in server");
  }

  return 0;
}
