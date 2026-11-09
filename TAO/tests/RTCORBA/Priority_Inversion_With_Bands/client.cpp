#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "../check_supported_priorities.cpp"

#include <cstdio>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT("file://ior_1")};
  int iterations {12};
  CORBA::ULong work {2};
  bool make_banded_invocations {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:k:i:w:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        options.make_banded_invocations = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case 'w':
        options.work = ACE_OS::atoi (get_opts.opt_arg ());
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
                           options.make_banded_invocations,
                           options.ior,
                           options.iterations,
                           options.work),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Client_Options const &options);

  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

  Client_Options const options_;
  CORBA::ORB_var orb_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Client_Options const &options)
  : ACE_Task_Base (&thread_manager),
    options_ (options),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (this->options_.ior);

      test_var test =
        test::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var const rt_current =
        RTCORBA::Current::_narrow (object.in ());

      RTCORBA::Priority const default_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      RTCORBA::Priority const low_priority =
        default_thread_priority;

      RTCORBA::Priority const high_priority =
        default_thread_priority + 1;

      if (this->options_.make_banded_invocations)
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

      test->initialize (this->options_.iterations * 2);

      for (int i {}; i != this->options_.iterations; ++i)
        {
          char iteration_description[BUFSIZ];
          std::snprintf (iteration_description,
                         sizeof (iteration_description),
                         "L:%02d",
                         i);
          test->method (this->options_.work,
                        iteration_description);
        }

      rt_current->the_priority (high_priority);

      for (int i {}; i != this->options_.iterations; ++i)
        {
          char iteration_description[BUFSIZ];
          std::snprintf (iteration_description,
                         sizeof (iteration_description),
                         "H:%02d",
                         i);
          test->method (this->options_.work,
                        iteration_description);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      int result = parse_args (argc, argv, options);
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
                 orb.in (),
                 options);

      // Task activation flags.
      long const flags =
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
            ACE_ASSERT (false);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
