// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "tao/corba.h"
#include "tao/PI/PI.h"

CORBA::ORB_var orb = CORBA::ORB::_nil ();

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

// Returns false if calling is_equivalent in both directions on the two supplied
// IORs does not produce expected result.
CORBA::Boolean
check_is_equivalent (const char* ior1, const char * ior2, CORBA::Boolean expected_result)
{
  CORBA::Boolean result = 1;
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object1 =
        orb->string_to_object(ior1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object2 =
        orb->string_to_object (ior2 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char* expected_result_string = (expected_result ? "true" : "false");

      if ((object1->_is_equivalent (object2.in ())) != expected_result)
        {
          ACE_DEBUG ((LM_ERROR, "Error: Result of <%s>->_is_equivalent (<%s>) is not %s when it should be...\n",
                                ior1, ior2, expected_result_string));
          result = 0;
        }

      if ((object2->_is_equivalent (object1.in ())) != expected_result)
        {
          ACE_DEBUG ((LM_ERROR, "Error: Result of <%s>->_is_equivalent (<%s>) is not %s when it should be...\n",
                                ior2, ior1, expected_result_string));
          result = 0;
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_ERROR, "Vastly unexpected exception comparing %s and %s:\n", ior1, ior2));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception:");
      result = 0;
    }
  ACE_ENDTRY;
  return result;
}

int
main (int argc, char *argv[])
{
  CORBA::Boolean result = 0;
  ACE_TRY_NEW_ENV
    {
      orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // ep1 is equivalent to no other IORs - it is not an IOGR, and ep2 is a different endpoint.
      // The next nine should all be false.
      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d1_g1_v2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because these are IORs with different endpoints.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep2_d1_g1_v2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1.ior", "file://ep1_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep1_d1_g1_v2.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep1_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep1_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep2_d1_g1_v1.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep2_d1_g1_v2.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep2_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v1.ior", "file://ep2_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep1_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep1_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep2_d1_g1_v1.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep2_d1_g1_v2.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep2_d1_g2_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g1_v2.ior", "file://ep2_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep1_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep2_d1_g1_v2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep2_d1_g2_v1.ior", 1))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be true because both are IOGRs and the domain and group ids match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d1_g2_v1.ior", "file://ep2_d2_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d2_g1_v1.ior", "file://ep2.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because an IOR is never equivalent to an IOGR\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      if (! check_is_equivalent ("file://ep1_d2_g1_v1.ior", "file://ep2_d1_g1_v1.ior", 0))
        {
          ACE_DEBUG ((LM_ERROR, "... it should be false because both are IOGRs but the domain and group ids don't match.\n\n"));
          result = 1;
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return result;
}
