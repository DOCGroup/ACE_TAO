// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "tao/Messaging/Messaging.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior =
  "corbaloc:iiop:192.3.47/10007/RandomObject";

ACE_Profile_Timer profile_timer;
TimeBase::TimeT timeout_period = 1000000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;



      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE,
                            timeout_as_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      for (CORBA::ULong l = 0;
           l != policy_list.length ();
           ++l)
        {
          policy_list[l]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Start the timer
      profile_timer.start ();

      // First connection happens here..
      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::String_var the_string =
        hello->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Stop the timer
      profile_timer.stop ();

      // Get the elampsed time
      ACE_Profile_Timer::ACE_Elapsed_Time el;
      profile_timer.elapsed_time (el);

      // Give a 30% error margin for handling exceptions etc. It is a
      // high margin, though!. But the timeout is too small and wider
      // range would help.
#if (ACE_HAS_FLOATING_POINT == 1)
      // The elapsed time is in usecs
      if (el.real_time > 200000)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Too long to timeout \n"));

          return 1;
        }
#else

      // The elapsed time is in secs
      if (el.real_time > 0.200)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t)ERROR: Too long to timeout \n"));

          return 1;
        }

#endif/*ACE_HAS_FLOATING_POINT == 1*/
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}
