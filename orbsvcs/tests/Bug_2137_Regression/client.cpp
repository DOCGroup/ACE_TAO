// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "tao/corba.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

CORBA::ORB_var orb = CORBA::ORB::_nil ();

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT(""));
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
  // Indicates successful parsing of the command line
  return 0;
}

// Returns false if calling is_equivalent in both directions on the two supplied
// IORs does not produce expected result.
CORBA::Boolean
check_is_equivalent (const char* ior1, const char * ior2, CORBA::Boolean expected_result)
{
  CORBA::Boolean result = 1;
  try
    {
      CORBA::Object_var object1 =
        orb->string_to_object(ior1);

      CORBA::Object_var object2 =
        orb->string_to_object (ior2);

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
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Vastly unexpected exception comparing %s and %s:\n", ior1, ior2));
      ex._tao_print_exception ("Exception:");
      result = 0;
    }
  return result;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::Boolean result = 0;
  try
    {
      orb =
        CORBA::ORB_init (argc, argv);

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

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return result;
}
