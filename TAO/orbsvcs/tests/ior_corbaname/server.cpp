#include "status_i.h"
#include "orbsvcs/CosNamingC.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Get a reference to the RootPOA
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow down to the correct reference
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Set a POA Manager
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();

      // Activate the POA Manager
      poa_manager->activate ();

      // Create the servant
      corbaname_Status_i status_i;

      // Activate it to obtain the reference
      corbaname::Status_var const status = status_i._this ();

      // Get a reference to Naming Context
      CORBA::Object_var const naming_context_object =
        orb->resolve_initial_references ("NameService");

      // Narrow down the reference
      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      // Bind Status to the Naming Context
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      naming_context->bind (name, status.in ());

      // Run the orb
      orb->run ();

      // Destroy the POA, waiting until the destruction terminates
      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("CORBA exception raised in server!"));
    }

  return 0;
}
