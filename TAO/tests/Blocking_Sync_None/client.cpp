// $Id$

#include "TestC.h"
#include "tao/Messaging.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Blocking_Sync_None, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 1000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Blocking_Sync_None_var blocking_sync_none =
        Test::Blocking_Sync_None::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (blocking_sync_none.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "ERROR: Nil reference in Blocking_Sync_None reference <%s>\n",
                           ior),
                          1);

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_NONE;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const int payload_length = 65536;
      const int sleep_milliseconds = 20;
      const int sleep_microseconds = sleep_milliseconds * 1000;

      Test::Payload payload(payload_length);
      payload.length (payload_length);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test running . . .\n"));
      int blocked_calls = 0;
      for (int i = 0; i != iterations; ++i)
        {
          ACE_Time_Value start = ACE_OS::gettimeofday ();

          blocking_sync_none->slow_operation (payload,
                                              sleep_microseconds
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_Time_Value elapsed = ACE_OS::gettimeofday ();
          elapsed -= start;

          if (elapsed.msec () >= sleep_milliseconds)
            {
              blocked_calls++;
            }
        }

      blocking_sync_none->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (blocked_calls > iterations / 20)
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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
