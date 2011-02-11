// $Id$

#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "../check_supported_priorities.cpp"

static const ACE_TCHAR *ior = ACE_TEXT("file://ior_1");
static int iterations = 12;
static CORBA::ULong work = 2;
static int make_banded_invocations = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:k:i:w:"));
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

      case 'b':
        make_banded_invocations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        work = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-b <make banded invocations> (default to %d)\n"
                           "\t-k <ior> (default to %s)\n"
                           "\t-i <iterations> (default to %d)\n"
                           "\t-w <work> (default to %d)\n"
                           "\n",
                           argv [0],
                           make_banded_invocations,
                           ior,
                           iterations,
                           work),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (ior);

      test_var test =
        test::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var rt_current =
        RTCORBA::Current::_narrow (object.in ());

      RTCORBA::Priority default_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      RTCORBA::Priority low_priority =
        default_thread_priority;

      RTCORBA::Priority high_priority =
        default_thread_priority + 1;

      if (make_banded_invocations)
        {
          RTCORBA::PriorityBands bands;
          bands.length (2);
          bands[0].low = low_priority;
          bands[0].high = low_priority;
          bands[1].low = high_priority;
          bands[1].high = high_priority;

          CORBA::PolicyList policies;
          policies.length (1);
          policies[0] =
            rt_orb->create_priority_banded_connection_policy (bands);

          object =
            test->_set_policy_overrides (policies,
                                         CORBA::SET_OVERRIDE);

          test =
            test::_narrow (object.in ());
        }

      rt_current->the_priority (low_priority);

      test->initialize (iterations * 2);

      int i = 0;
      char iteration_description[BUFSIZ];
      for (i = 0; i != iterations; ++i)
        {
          ACE_OS::sprintf (iteration_description, "L:%02d", i);
          test->method (work,
                        iteration_description);
        }

      rt_current->the_priority (high_priority);

      for (i = 0; i != iterations; ++i)
        {
          ACE_OS::sprintf (iteration_description, "H:%02d", i);
          test->method (work,
                        iteration_description);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
