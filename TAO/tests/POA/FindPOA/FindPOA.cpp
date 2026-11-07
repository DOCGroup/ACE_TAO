
//=============================================================================
/**
 *  @file     FindPOA.cpp
 *
 *   This Program tests the find_POA method of a POA.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"
#include "ace/Log_Msg.h"
#include "Adapter_Activator.h"

void
find_non_existant_POA (PortableServer::POA_ptr parent,
                       char const *child_poa_name,
                       bool activate)
{
  bool expected_exception_raised {};

  // New environment.
  try
    {
      // Try to find child poa.
      PortableServer::POA_var child_poa =
        parent->find_POA (child_poa_name,
                          activate);
    }
  catch (PortableServer::POA::AdapterNonExistent const &)
    {
      expected_exception_raised = true;
    }

  if (!expected_exception_raised)
    ACE_ERROR ((LM_ERROR, "ERROR: Caught incorrect exception\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow Object reference to RootPOA to a POA reference.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Try to find a non-existent POA.  Since the Adapter Activator
      // has not been installed yet, this call should fail.
      find_non_existant_POA (root_poa.in (),
                             "firstPOA",
                             true);

      // Get a TAO_Adapter_Activator reference
      Adapter_Activator activator (poa_manager.in ());

      // Register the TAO_Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.
      root_poa->the_activator (&activator);

      // Try to find a childPOA of RootPOA named firstPOA
      PortableServer::POA_var first_poa =
        root_poa->find_POA ("firstPOA",
                            true);

      PortableServer::POA_var second_poa =
        first_poa->find_POA ("secondPOA",
                             true);

      // Try to find a non-existent POA.  Even though the Adapter
      // Activator has been installed, this call should fail because
      // the activate (if not found) flag is 0.
      find_non_existant_POA (root_poa.in (),
                             "thirdPOA",
                             false);

      // Get the names of all the POAs
      CORBA::String_var root_poa_name =
        root_poa->the_name ();

      CORBA::String_var first_poa_name =
        first_poa->the_name ();

      CORBA::String_var second_poa_name =
        second_poa->the_name ();

      ACE_DEBUG ((LM_DEBUG,
                  "%C\n%C\n%C\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in ()));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
