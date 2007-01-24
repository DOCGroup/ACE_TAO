// $Id$

//===========================================================================
//
// = LIBRARY
//     TAO/tests/POA/FindPOA
//
// = FILENAME
//     FindPOA.cpp
//
// = DESCRIPTION
//     This Program tests the find_POA method of a POA.
//
// = AUTHOR
//     Irfan Pyarali
//
//===========================================================================

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Adapter_Activator.h"

#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"

ACE_RCSID (FindPOA,
           FindPOA,
           "$Id$")

void
find_non_existant_POA (PortableServer::POA_ptr parent,
                       const char *child_poa_name,
                       int activate)
{
  bool expected_exception_raised = false;

  // New environment.
  ACE_TRY_NEW_ENV
    {
      // Try to find child poa.
      PortableServer::POA_var child_poa =
        parent->find_POA (child_poa_name,
                          activate
                          ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCH (PortableServer::POA::AdapterNonExistent, foo)
    {
      expected_exception_raised = true;
    }
  ACE_ENDTRY;

  if (!expected_exception_raised)
    ACE_ERROR ((LM_ERROR, "ERROR: Caught incorrect exception\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  // Initialize the ORB
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      // Narrow Object reference to RootPOA to a POA reference.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Try to find a non-existant POA.  Since the Adapter Activator
      // has not been installed yet, this call should fail.
      find_non_existant_POA (root_poa.in (),
                             "firstPOA",
                             1);

      // Get a TAO_Adapter_Activator reference
      PortableServer::AdapterActivator_var activator =
        new TAO::Portable_Server::Adapter_Activator (poa_manager.in ());

      // Register the TAO_Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.
      root_poa->the_activator (activator.in ()
                               ACE_ENV_ARG_PARAMETER);

      // Try to find a childPOA of RootPOA named firstPOA
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->find_POA (name.c_str (),
                            1
                            ACE_ENV_ARG_PARAMETER);

      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->find_POA (name.c_str (),
                             1
                             ACE_ENV_ARG_PARAMETER);

      // Try to find a non-existant POA.  Even though the Adapter
      // Activator has been installed, this call should fail because
      // the activate (if not found) flag is 0.
      find_non_existant_POA (root_poa.in (),
                             "thirdPOA",
                             0);

      // Get the names of all the POAs
      CORBA::String_var root_poa_name =
        root_poa->the_name ();

      CORBA::String_var first_poa_name =
        first_poa->the_name ();

      CORBA::String_var second_poa_name =
        second_poa->the_name ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n%s\n%s\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
