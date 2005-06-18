// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/PSS
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//      This class implements a simple CORBA server which uses the
//      simple_naming.idl to bind and find a object reference and uses
//      Persistent State Service to save the information persistently.
//      Serves like a mini naming service.
//
// = AUTHOR
//     Priyanka Gontla <gontla_p@ociweb.com>
// ============================================================================

#include "Simple_Naming_i.h"
#include "Server_i.h"
#include "ace/Get_Opt.h"

ACE_RCSID (PSS, client, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
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

      Server_i server_i;

      Simple_Server::Server_var server = server_i._this ();

      CORBA::String_var string_obj_ref =
        orb->object_to_string (server.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get a reference to Simple_Naming
      CORBA::Object_var simple_naming_object =
        orb->resolve_initial_references ("Simple_Naming"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down the reference
      Simple_Naming::Naming_Context_var simple_naming =
        Simple_Naming::Naming_Context::_narrow (simple_naming_object.in()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Naming::Name name = CORBA::string_dup ("Server");

      // Bind the name to stringified objecte refernce
      simple_naming->bind (CORBA::string_dup (name),
                           string_obj_ref.in ()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected excpeption in PSS Test");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
