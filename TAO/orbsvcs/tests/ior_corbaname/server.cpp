// $Id$

#include "status_i.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

int main (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv,
                         "" /* the ORB name, it can be anything! */
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get a reference to the RootPOA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down to the correct reference
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set a POA Manager
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate the POA Manager
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the servant
      corbaname_Status_i status_i;

      // Activate it to obtain the reference
      corbaname::Status_var status =
        status_i._this ();

      // Get a reference to Naming Context
      CORBA::Object_var naming_context_object =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down the reference
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Bind Status to the Naming Context
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("STATUS");

      naming_context->bind (name,
                            status.in ()
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run the orb
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the POA, waiting until the destruction terminates
      poa->destroy (1, 1);
      orb->destroy ();

    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "CORBA exception raised in server!");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
