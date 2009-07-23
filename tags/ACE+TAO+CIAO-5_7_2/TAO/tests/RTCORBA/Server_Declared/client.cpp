// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"

const ACE_TCHAR *ior1 = ACE_TEXT("file://test1.ior");
const ACE_TCHAR *ior2 = ACE_TEXT("file://test2.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        ior1 = get_opts.opt_arg ();
        break;
      case 'o':
        ior2 = get_opts.opt_arg ();
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
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

CORBA::Short
check_policy (Test_ptr server)
{
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

  if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
    return -1;

  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model ();
  if (priority_model != RTCORBA::SERVER_DECLARED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: priority_model != "
                       "RTCORBA::SERVER_DECLARED!\n"),
                      -1);

  return priority_policy->server_priority ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // Test object 1.
      CORBA::Object_var object =
        orb->string_to_object (ior1);

      Test_var server1 = Test::_narrow (object.in ());
      if (check_for_nil (server1.in (), "server1") == -1)
        return -1;

      // Test object 2.
      object = orb->string_to_object (ior2);

      Test_var server2 = Test::_narrow (object.in ());
      if (check_for_nil (server2.in (), "server2") == -1)
        return -1;

      // Check that test objects are configured with SERVER_DECLARED
      // PriorityModelPolicy, and get their server priorities.

      // Test object 1.
      CORBA::Short server1_priority =
        check_policy (server1.in ());

      if (server1_priority == -1)
        return -1;

      // Test object 2.
      CORBA::Short server2_priority =
        check_policy (server2.in ());
      if (server2_priority == -1)
        return -1;

      // Testing: make several invocations on test objects.
      for (int i = 0; i < 5; ++i)
        {
          server1->test_method (server1_priority);

          server2->test_method (server2_priority);
        }

      // Testing over. Shut down Server ORB.
      server1->shutdown ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception in Server_Declared test client:");
      return -1;
    }

  return 0;
}
