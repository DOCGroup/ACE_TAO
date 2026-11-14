#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_stdlib.h"
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  int iterations {1000};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <iterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior.c_str ());

      Test::Blocking_Sync_None_var const blocking_sync_none =
        Test::Blocking_Sync_None::_narrow (tmp.in ());

      if (CORBA::is_nil (blocking_sync_none.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("ERROR: Nil reference in Blocking_Sync_None reference <%s>\n"),
                           options.ior.c_str ()),
                          1);

      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_NONE;

      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      int const payload_length = 65536;
      unsigned int const sleep_milliseconds = 20;
      unsigned int const sleep_microseconds = sleep_milliseconds * 1000;

      Test::Payload payload (payload_length);
      payload.length (payload_length);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test running . . .\n"));
      int blocked_calls {};
      for (int iteration {}; iteration != options.iterations; ++iteration)
        {
          ACE_Time_Value const start = ACE_OS::gettimeofday ();

          blocking_sync_none->slow_operation (payload,
                                              sleep_microseconds);

          ACE_Time_Value elapsed = ACE_OS::gettimeofday ();
          elapsed -= start;

          if (elapsed.msec () >= sleep_milliseconds)
            {
              ++blocked_calls;
            }
        }

      blocking_sync_none->shutdown ();

      orb->destroy ();

      if (blocked_calls > options.iterations / 20)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: More than 5% (%d) of the calls blocked\n",
                      blocked_calls));
          return 1;
        }

      if (blocked_calls != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Warning: Some (%d) SYNC_NONE calls blocked\n",
                      blocked_calls));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
