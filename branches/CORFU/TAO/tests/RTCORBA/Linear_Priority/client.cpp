// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_ManagerC.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"

static int iterations = 5;
static int shutdown_server = 0;
static int debug = 1;

static const ACE_TCHAR *ior = ACE_TEXT ("file://ior");

static const ACE_TCHAR *invocation_priorities_file = ACE_TEXT ("invocation_priorities");
static const ACE_TCHAR *bands_file = ACE_TEXT ("empty_file");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:d:p:k:i:x"));
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

      case 'd':
        debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case 'p':
        invocation_priorities_file = get_opts.opt_arg ();
        break;

      case 'b':
        bands_file = get_opts.opt_arg ();
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
                 CORBA::Short priority);

  int svc (void);

  void validate_connection (void);

private:
  test_var test_;
  RTCORBA::Current_var current_;
  CORBA::Short priority_;
};

Worker_Thread::Worker_Thread (ACE_Thread_Manager &thread_manager,
                              test_ptr test,
                              RTCORBA::Current_ptr current,
                              CORBA::Short priority)
  : ACE_Task_Base (&thread_manager),
    test_ (test::_duplicate (test)),
    current_ (RTCORBA::Current::_duplicate (current)),
    priority_ (priority)
{
}

void
Worker_Thread::validate_connection (void)
{
  // Try to validate the connection several times, ignoring transient
  // exceptions.  If the connection can still not be setup, return
  // failure.
  CORBA::PolicyList_var inconsistent_policies;
  int max_attempts = 10;
  int current_attempt = 0;
  for (;;)
    {
      try
        {
          ++current_attempt;
          this->test_->_validate_connection (inconsistent_policies.out ());

          // If successful, we are done.
          return;
        }
      catch (const CORBA::TRANSIENT& )
        {
          // If we have reach our maximum number of tries, throw exception.
          if (current_attempt == max_attempts)
            throw;
          // Otherwise, ignore...
        }
      catch (const CORBA::Exception&)
        {
          // Rethrow any other exceptions.
          throw;
        }
    }
}

int
Worker_Thread::svc (void)
{
  try
    {
      this->current_->the_priority (this->priority_);

      this->validate_connection ();

      for (int i = 0; i < iterations; i++)
        {
          this->test_->method ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Worker Thread exception:");
    }
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
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      current->the_priority (0);

      object =
        this->orb_->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      object =
        this->orb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior));

      test_var test =
        test::_narrow (object.in ());

      ULong_Array priorities;
      int result =
        get_values ("client",
                    invocation_priorities_file,
                    "invocation priorities",
                    priorities,
                    debug);
      if (result != 0)
        return result;

      CORBA::PolicyList policies;

      result =
        get_priority_bands ("client",
                            bands_file,
                            rt_orb.in (),
                            policies,
                            debug);
      if (result != 0)
        return result;

      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE);

      u_long i = 0;

      // Thread Manager for managing workers.
      ACE_Thread_Manager thread_manager;

      // Workers.
      Worker_Thread **workers = 0;

      ACE_NEW_RETURN (workers,
                      Worker_Thread *[priorities.size ()],
                      -1);

      for (i = 0;
           i < priorities.size ();
           ++i)
        {
          ACE_NEW_RETURN (workers[i],
                          Worker_Thread (thread_manager,
                                         test.in (),
                                         current.in (),
                                         priorities[i]),
                          -1);

          long flags =
            THR_NEW_LWP |
            THR_JOINABLE |
            this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

          result =
            workers[i]->activate (flags);
          if (result != 0)
            return result;
        }

      thread_manager.wait ();

      for (i = 0;
           i < priorities.size ();
           ++i)
        {
          delete workers[i];
        }
      delete[] workers;

      if (shutdown_server)
        {
          test->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception!");
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
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception!");
      return -1;
    }

  return 0;
}
