// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#if (TAO_HAS_RT_CORBA == 1)

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        ior1 = get_opts.optarg;
        break;
      case 'o':
        ior2 = get_opts.optarg;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <ior> "
                           "-o <ior> "
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

CORBA::Short
check_policy (Test_ptr server,
              CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
    return -1;

  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  if (priority_model != RTCORBA::SERVER_DECLARED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: priority_model != "
                       "RTCORBA::SERVER_DECLARED!\n"),
                      -1);

  return priority_policy->server_priority (ACE_TRY_ENV);
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // First check that we have sufficient priority range to run the
      // test, i.e., more than 1 priority level.
      int max_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_OTHER);
      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);

      if (max_priority == min_priority)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Not enough priority levels on this platform"
                      "to run the test, aborting\n"));
          return 0;
        }

      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

	  // Test object 1.
      CORBA::Object_var object =
        orb->string_to_object (ior1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server1 = Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (server1.in (), "server1") == -1)
        return 1;

      // Test object 2.
      object = orb->string_to_object (ior2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

	  Test_var server2 = Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (server2.in (), "server2") == -1)
        return 1;

      // Check that test objects are configured with SERVER_DECLARED
      // PriorityModelPolicy, and get their server priorities.

      // Test object 1.
      CORBA::Short server1_priority =
        check_policy (server1.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

	  if (server1_priority == -1)
        return 1;

	  // Test object 2.
      CORBA::Short server2_priority =
        check_policy (server2.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (server2_priority == -1)
        return 1;

	  // Testing: make several invocations on test objects.
      for (int i = 0; i < 5; ++i)
        {
          server1->test_method (server1_priority, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          server2->test_method (server2_priority, ACE_TRY_ENV);
          ACE_TRY_CHECK;

	  }

      // Testing over. Shut down Server ORB.
      server1->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
       "Unexpected exception in Server_Declared test client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#else /* TAO_HAS_RT_CORBA == 1 */

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,
                     "\nRTCORBA must be enabled to run this test!\n"),
                    1);
}

#endif /* TAO_HAS_RT_CORBA == 1 */
