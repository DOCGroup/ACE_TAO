// $Id$

//

// ============================================================================
//
// = LIBRARY
//    orbsvcs/examples/PSS
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//     This implements a simple CORBA client for the
//     Server.idl
//
// = AUTHOR
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================

#include "ServerC.h"
#include "Simple_Naming_i.h"

int main (int argc, char *argv [])
{

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
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

      // Get a reference to Simple_Naming
      CORBA::Object_var simple_naming_object =
        orb->resolve_initial_references ("Simple_Naming" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow down the reference
      Simple_Naming::Naming_Context_var simple_naming =
        Simple_Naming::Naming_Context::_narrow (simple_naming_object.in()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Naming::Name name = CORBA::string_dup ("Server");

      CORBA::String_var ior_string =
        simple_naming->find (name
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Connect to the server
      CORBA::Object_var tmp =
        orb->string_to_object(ior_string.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server::Server_var server =
        Simple_Server::Server::_narrow (tmp.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var status =
        server->get_status (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (status.in (), "active") != 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR: The server didnt return the active status\n"));
          return -1;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client raised an exception:\n");
      ACE_CHECK_RETURN (-1);
    }
  ACE_ENDTRY;

  return 0;
}
