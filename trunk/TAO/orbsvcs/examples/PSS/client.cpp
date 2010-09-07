// $Id$

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Get a reference to the RootPOA
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow down to the correct reference
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Set a POA Manager
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      // Activate the POA Manager
      poa_manager->activate ();

      // Get a reference to Simple_Naming
      CORBA::Object_var simple_naming_object =
        orb->resolve_initial_references ("Simple_Naming");

      // Narrow down the reference
      Simple_Naming::Naming_Context_var simple_naming =
        Simple_Naming::Naming_Context::_narrow (simple_naming_object.in());

      Simple_Naming::Name name = CORBA::string_dup ("Server");

      CORBA::String_var ior_string =
        simple_naming->find (name);

      // Connect to the server
      CORBA::Object_var tmp =
        orb->string_to_object(ior_string.in ());

      Simple_Server::Server_var server =
        Simple_Server::Server::_narrow (tmp.in ());

      CORBA::String_var status =
        server->get_status ();

      if (ACE_OS::strcmp (status.in (), "active") != 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR: The server didnt return the active status\n"));
          return -1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client raised an exception:\n");
    }

  return 0;
}
