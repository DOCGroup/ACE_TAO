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

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/POA.h"

ACE_RCSID(FindPOA, FindPOA, "$Id$")

int
main (int argc, char **argv)
{
  //  CORBA::Environment env;
  ACE_DECLARE_NEW_CORBA_ENV;

  // Initialize the ORB
  char str[256]; // Exception message
  ACE_TRY
    {
      ACE_OS::strcpy (str, "CORBA::ORB_init");

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get Object reference to RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      ACE_OS::strcpy (str, "PortableServer::POA::_narrow");
      // Narrow Object reference to RootPOA to a POA reference.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get a TAO_Adapter_Activator reference
      PortableServer::AdapterActivator_var activator =
        new TAO_Adapter_Activator (poa_manager.in ());

      // Register the TAO_Adapter_Activator reference to be the RootPOA's
      // Adapter Activator.
      ACE_OS::strcpy (str,"PortableServer::POA::the_activator");
      root_poa->the_activator (activator.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Try to find a childPOA of RootPOA named firstPOA
      ACE_CString name = "firstPOA";
      ACE_OS::strcpy (str,"PortableServer::POA::find_POA");
      PortableServer::POA_var first_poa =
        root_poa->find_POA (name.c_str (),
                            1,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->find_POA (name.c_str (),
                             1,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the names of all the POAs
      ACE_OS::strcpy (str, "PortableServer::POA::_narrow");

      CORBA::String_var root_poa_name =
        root_poa->the_name (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var first_poa_name =
        first_poa->the_name (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var second_poa_name =
        second_poa->the_name (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n%s\n%s\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, str);
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
