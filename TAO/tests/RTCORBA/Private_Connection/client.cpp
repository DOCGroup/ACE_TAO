// $Id$

#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Get_Opt.h"

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior1 = get_opts.opt_arg ();
        break;
      case 'p':
        ior2 = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior1> "
                           "-p <ior2> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 1;

      // PolicyCurrent.
      object = orb->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (policy_current.in (), "PolicyCurrent")
          == -1)
        return 1;

      // Test object 1.
      object = orb->string_to_object (ior1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test_var server1 = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (server1.in (), "server1") == -1)
        return 1;

      // Test object 2.
      object = orb->string_to_object (ior2 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test_var server2 = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (server2.in (), "server2") == -1)
        return 1;

      // Make four invocations on test objects.  Expected: connection
      // established on the first invocation, and reused in the
      // following three.
      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 1 --> new connection\n"));
      server1->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 2 --> use connection from invocation 1\n"));
      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 3 --> use connection from invocation 1\n"));
      server1->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 4 --> use connection from invocation 1\n"));
      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set RTCORBA::PrivateConnectionPolicy on PolicyCurrent.
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_private_connection_policy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);

      // Make four invocations on test objects again.  This time,
      // since RTCORBA::PrivateConnectionPolicy is set, we expect a
      // connection to be established for <server1> during the first
      // invocation, a connection to be established for <server2> during
      // the second invocation, <server1>'s connection reused on
      // third, and <server2>'s reused on fourth.
      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 5 --> new connection\n"));
      server1->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 6 --> new connection\n"));
      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 7 --> use connection from invocation 5\n"));
      server1->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Invocation 8 --> use connection from invocation 6\n"));
      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Testing over.  Shut down Server ORB.
      ACE_DEBUG ((LM_DEBUG,
                  "\n  Testing over - shutting down\n"));
      server1->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "Unexpected exception caught in Private_Connection test client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

