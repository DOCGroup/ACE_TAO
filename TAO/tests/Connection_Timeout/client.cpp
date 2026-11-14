#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

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
  // Indicates successful parsing of the command line
  return 0;
}

int
test_timeout (CORBA::Object_ptr object, ACE_TString const &ior)
{
  ACE_Profile_Timer profile_timer;

  // Start the timer
  profile_timer.start ();

  try
    {
      // First connection happens here..
      Test::Hello_var const hello =
        Test::Hello::_narrow (object);

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      CORBA::String_var const the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();
    }
  catch (CORBA::Exception const&)
    {
      // Stop the timer
      profile_timer.stop ();

      // Get the elampsed time
      ACE_Profile_Timer::ACE_Elapsed_Time el {};
      profile_timer.elapsed_time (el);

      // Give a 30% error margin for handling exceptions etc. It is a
      // high margin, though!. But the timeout is too small and wider
      // range would help.
      // The elapsed time is in secs
      if (el.real_time > 0.200)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) ERROR: Too long to timeout: %F\n",
                             el.real_time),
                             1);
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Success, timeout: %F\n",
                    el.real_time));
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval {1};

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TString ior (ACE_TEXT("corbaloc:iiop:192.3.47/10007/RandomObject"));
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior.c_str ());

      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      constexpr TimeBase::TimeT timeout_period = 1000000;
      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE,
                            timeout_as_any);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE);

      for (CORBA::ULong l = 0;
           l != policy_list.length ();
           ++l)
        {
          policy_list[l]->destroy ();
        }

      retval = test_timeout (tmp.in (), ior);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return retval;
}
