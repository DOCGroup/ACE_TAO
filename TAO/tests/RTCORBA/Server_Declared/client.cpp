#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"

struct Client_Options
{
  ACE_TCHAR const *ior1 {ACE_TEXT ("file://test1.ior")};
  ACE_TCHAR const *ior2 {ACE_TEXT ("file://test2.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'p':
        options.ior1 = get_opts.opt_arg ();
        break;

      case 'o':
        options.ior2 = get_opts.opt_arg ();
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
check_for_nil (CORBA::Object_ptr const object, char const *message)
{
  if (CORBA::is_nil (object))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       message),
                      -1);

  return 0;
}

CORBA::Short
check_policy (Test_ptr server)
{
  CORBA::Policy_var const policy = server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);
  RTCORBA::PriorityModelPolicy_var const priority_policy = RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

  if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
    return -1;

  RTCORBA::PriorityModel const priority_model = priority_policy->priority_model ();
  if (priority_model != RTCORBA::SERVER_DECLARED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: priority_model != RTCORBA::SERVER_DECLARED!\n"),
                      -1);

  return priority_policy->server_priority ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var object = orb->string_to_object (options.ior1);
      Test_var const server1 = Test::_narrow (object.in ());
      if (check_for_nil (server1.in (), "server1") == -1)
        return -1;

      object = orb->string_to_object (options.ior2);
      Test_var const server2 = Test::_narrow (object.in ());
      if (check_for_nil (server2.in (), "server2") == -1)
        return -1;

      CORBA::Short const server1_priority = check_policy (server1.in ());
      if (server1_priority == -1)
        return -1;

      CORBA::Short const server2_priority = check_policy (server2.in ());
      if (server2_priority == -1)
        return -1;

      for (int i {}; i < 5; ++i)
        {
          server1->test_method (server1_priority);
          server2->test_method (server2_priority);
        }

      server1->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception in Server_Declared test client:");
      return -1;
    }

  return 0;
}
