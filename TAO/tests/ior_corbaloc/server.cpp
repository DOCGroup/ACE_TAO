// $Id$
// server.cpp
//

#include "status_i.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include <iostream>

int main (int argc, char* argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv,
                         "" /* the ORB name, it can be anything! */,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get a reference to the RootPOA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // narrow down to the correct reference
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Set a POA Manager
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Activate the POA Manager
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create the servant
      corbaloc_Status_i status_i;

      // Activate it to obtain the reference
      corbaloc::Status_var status =
        status_i._this ();

      // Get a reference to Naming Context
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Narrow down the reference
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in(),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Bind Iterator_Factory to the Naming Context
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      naming_context->bind (name, status.in ());

      // Run the orb
      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Destroy the POA, waiting until the destruction terminates
      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, ex) {
    std::cerr << "CORBA exception raised! " << ex << std::endl;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
