// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Interoperable_Naming/
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//     This class implements a simple CORBA server for the
//     CosNaming::NamingContextExt example
//
// = AUTHORS
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================


#include "Iterator_Factory_i.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

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
       
       CORBA::Object_var poa_object =
         orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
       ACE_TRY_CHECK;
       
       if (CORBA::is_nil (poa_object.in ()))
         ACE_ERROR_RETURN ((LM_ERROR,
                            "server: unable to initialize the POA.\n"),
                           -1);
       
       PortableServer::POA_var poa =
         PortableServer::POA::_narrow (poa_object.in ());
       
       
       PortableServer::POAManager_var poa_manager =
         poa->the_POAManager (ACE_TRY_ENV);
       ACE_TRY_CHECK; 
       
       poa_manager->activate (ACE_TRY_ENV );
       ACE_TRY_CHECK; 
       
       // Create the servant
       Web_Server_Iterator_Factory_i iterator_factory_i;
       
       // Activate it to obtain the reference
       Web_Server::Iterator_Factory_var iterator_factory =
         iterator_factory_i._this ();
       
       // Get a reference to Naming Context
       CORBA::Object_var naming_context_object = 
         orb->resolve_initial_references ("NameService", ACE_TRY_ENV);
       ACE_TRY_CHECK;
       
       // Narrow
       CosNaming::NamingContext_var naming_context = 
         CosNaming::NamingContext::_narrow (naming_context_object.in (),
                                            ACE_TRY_ENV);
       ACE_TRY_CHECK;
       
       CosNaming::Name name;
       name.length (1);
       name[0].id = CORBA::string_dup ("content");
       name[0].kind = CORBA::string_dup ("ctxt");
       
       CosNaming::NamingContext_var content =
         naming_context->bind_new_context (name);
       
       name.length (2);
       name[0].id = CORBA::string_dup ("content");
       name[1].id = CORBA::string_dup ("Iterator_Factory");
       name[1].kind = CORBA::string_dup ("factory");
       
       naming_context->bind (name, iterator_factory.in ());
       
       orb->run ();
       
       // Destroy the POA, waiting until the destruction terminates
       poa->destroy (1, 1);
       orb->destroy ();
     }
   ACE_CATCH (CORBA::SystemException, ex)
     {
       ACE_PRINT_EXCEPTION (ex, "A system exception");
       return -1;
     }
   ACE_ENDTRY;
   ACE_CHECK_RETURN (-1);
   
   return 0;
}


