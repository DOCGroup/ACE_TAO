// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/BiDir_Multiple_ORB
//
// = FILENAME
//     destroy.cpp
//
// = DESCRIPTION
//     Modified ORB destruction test.
//
// = AUTHOR
//     Andrew Schnable <Andrew.Schnable@veritas.com>
//     Iliyan Jeliazkov <jeliazkov_i@ociweb.com>
//
//=========================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

ACE_RCSID(BiDir_Multiple_ORB, destroy, "$Id$")

int
test_with_bidir_poa (int argc,
                     char **argv,
                     const char *orb_name,
                     int destroy_orb)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (destroy_orb)
        {
          orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised");
      ACE_CHECK_RETURN (-1);
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char **argv)
{
  int result = 0;

  for (int i=0; i<10; i++)
  {
    result = test_with_bidir_poa (argc, argv, "poa_1", 1);
    ACE_ASSERT (result == 0);

    result = test_with_bidir_poa (argc, argv, "poa_2", 1);
    ACE_ASSERT (result == 0);
  }
  ACE_DEBUG ((LM_DEBUG, "Completed OK\n"));
  return result;
}
