// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/ORB_destroy
//
// = FILENAME
//     ORB_destroy.cpp
//
// = DESCRIPTION
//     Simple ORB destruction test.
//
// = AUTHOR
//     Irfan Pyarali <irfan@cs.wustl.edu>
//
//=========================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID(ORB_destroy, ORB_destroy, "$Id$")

int
test_with_regular_poa_manager (int argc,
                               char **argv,
                               const char *orb_name,
                               int destroy_orb,
                               int destroy_poa)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (destroy_poa)
        {
          root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (destroy_orb)
        {
          orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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
test_with_funky_poa_manager (int argc,
                             char **argv,
                             const char *orb_name,
                             int destroy_orb,
                             int destroy_poa,
                             int funky_poa_manager)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (funky_poa_manager)
        {
          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (destroy_poa)
        {
          root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (destroy_orb)
        {
          orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_nothing_with_poa_manager",
                                          0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_poa_with_poa_manager",
                                          0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_orb_with_poa_manager",
                                          1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_regular_poa_manager (argc, argv,
                                          "destroy_poa_and_orb_with_poa_manager",
                                          1, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_nothing_without_poa_manager",
                                          0, 0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_without_poa_manager",
                                          0, 1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_orb_without_poa_manager",
                                          1, 0, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_and_orb_without_poa_manager",
                                          1, 1, 0);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_nothing_with_funky_poa_manager",
                                          0, 0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_with_funky_poa_manager",
                                          0, 1, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_orb_with_funky_poa_manager",
                                          1, 0, 1);
  ACE_ASSERT (result == 0);

  result = test_with_funky_poa_manager   (argc, argv,
                                          "destroy_poa_and_orb_with_funky_poa_manager",
                                          1, 1, 1);
  ACE_ASSERT (result == 0);

  return result;
}
