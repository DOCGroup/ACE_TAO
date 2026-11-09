#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_ManagerC.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"

#include <cstddef>
#include <memory>
#include <new>
#include <vector>

struct Client_Options
{
  int iterations { 5 };
  bool shutdown_server {};
  int debug { 1 };
  ACE_TCHAR const *ior { ACE_TEXT ("file://ior") };
  ACE_TCHAR const *invocation_priorities_file { ACE_TEXT ("invocation_priorities") };
  ACE_TCHAR const *bands_file { ACE_TEXT ("empty_file") };
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:d:p:k:i:x"));
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

      case 'd':
        options.debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case 'p':
        options.invocation_priorities_file = get_opts.opt_arg ();
        break;

      case 'b':
        options.bands_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-b <bands_file> "
                           "-d <debug> "
                           "-p <invocation_priorities_file> "
                           "-k ior "
                           "-i iterations "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

class Worker_Thread : public ACE_Task_Base
{
public:
  Worker_Thread (ACE_Thread_Manager &thread_manager,
                 test_ptr test,
                 RTCORBA::Current_ptr current,
                 CORBA::Short priority,
                 int iterations);
  Worker_Thread (Worker_Thread const &) = delete;
  Worker_Thread (Worker_Thread &&) = delete;
  Worker_Thread &operator= (Worker_Thread const &) = delete;
  Worker_Thread &operator= (Worker_Thread &&) = delete;

  int svc () override;

  void validate_connection ();

private:
  test_var const test_;
  RTCORBA::Current_var const current_;
  CORBA::Short const priority_;
  int const iterations_;
};

Worker_Thread::Worker_Thread (ACE_Thread_Manager &thread_manager,
                              test_ptr test,
                              RTCORBA::Current_ptr current,
                              CORBA::Short priority,
                              int iterations)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    current_ (RTCORBA::Current::_duplicate (current)),
    priority_ (priority),
    iterations_ (iterations)
{
}

void
Worker_Thread::validate_connection ()
{
  // Try to validate the connection several times, ignoring transient
  // exceptions.  If the connection can still not be setup, return
  // failure.
  CORBA::PolicyList_var inconsistent_policies;
  int const max_attempts = 10;
  int current_attempt {};
  for (;;)
    {
      try
        {
          ++current_attempt;
          this->test_->_validate_connection (inconsistent_policies.out ());

          // If successful, we are done.
          return;
        }
      catch (CORBA::TRANSIENT const &)
        {
          // If we have reach our maximum number of tries, throw exception.
          if (current_attempt == max_attempts)
            throw;
          // Otherwise, ignore...
        }
      catch (CORBA::Exception const &)
        {
          // Rethrow any other exceptions.
          throw;
        }
    }
}

int
Worker_Thread::svc ()
{
  try
    {
      this->current_->the_priority (this->priority_);

      this->validate_connection ();

      for (int i = 0; i < this->iterations_; ++i)
        {
          this->test_->method ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Worker Thread exception:");
    }
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

private:
  CORBA::ORB_var const orb_;
  Client_Options const options_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Client_Options const &options)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());

      current->the_priority (0);

      object =
        this->orb_->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var const policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      object =
        this->orb_->string_to_object (this->options_.ior);

      test_var const test =
        test::_narrow (object.in ());

      ULong_Array priorities;
      int result =
        get_values ("client",
                    this->options_.invocation_priorities_file,
                    "invocation priorities",
                    priorities,
                    this->options_.debug);
      if (result != 0)
        return result;

      CORBA::PolicyList policies;

      result =
        get_priority_bands ("client",
                            this->options_.bands_file,
                            rt_orb.in (),
                            policies,
                            this->options_.debug);
      if (result != 0)
        return result;

      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE);

      // Thread Manager for managing workers.
      ACE_Thread_Manager thread_manager;

      std::vector<std::unique_ptr<Worker_Thread>> workers;
      workers.reserve (priorities.size ());

      for (std::size_t i = 0; i < priorities.size (); ++i)
        {
          workers.push_back (std::make_unique<Worker_Thread> (
                               thread_manager,
                               test.in (),
                               current.in (),
                               priorities[i],
                               this->options_.iterations));
        }

      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      for (std::unique_ptr<Worker_Thread> const &worker : workers)
        {
          int const activate_result = worker->activate (flags);
          if (activate_result != 0)
            {
              thread_manager.wait ();
              return activate_result;
            }
        }

      int const wait_result = thread_manager.wait ();
      ACE_ASSERT (wait_result != -1);

      if (this->options_.shutdown_server)
        {
          test->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception!");
      return -1;
    }
  catch (std::bad_alloc const &)
    {
      ACE_ERROR ((LM_ERROR, "Unable to allocate priority worker storage\n"));
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

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
      int const activate_result = task.activate (flags);
      if (activate_result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %C\n"
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
      int const wait_result = thread_manager.wait ();
      ACE_ASSERT (wait_result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception!");
      return -1;
    }

  return 0;
}
