// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"

static int iterations = 5;
static int shutdown_server = 0;
static int debug = 1;

static const char *ior = "file://ior";

static const char *invocation_priorities_file = "invocation_priorities";
static const char *bands_file = "empty_file";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "b:d:p:k:i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        debug = ::atoi (get_opts.opt_arg ());
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

  void validate_connection (ACE_ENV_SINGLE_ARG_DECL);

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
Worker_Thread::validate_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  // Try to validate the connection several times, ignoring transient
  // exceptions.  If the connection can still not be setup, return
  // failure.
  CORBA::PolicyList_var inconsistent_policies;
  int max_attempts = 10;
  int current_attempt = 0;
  for (;;)
    {
      ACE_TRY
        {
          ++current_attempt;
          this->test_->_validate_connection (inconsistent_policies.out ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // If successful, we are done.
          return;
        }
      ACE_CATCH (CORBA::TRANSIENT, exception)
        {
          // If we have reach our maximum number of tries, throw exception.
          if (current_attempt == max_attempts)
            ACE_RE_THROW;
          // Otherwise, ignore...
        }
      ACE_CATCHANY
        {
          // Rethrow any other exceptions.
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

int
Worker_Thread::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->current_->the_priority (this->priority_
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->validate_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i < iterations; i++)
        {
          this->test_->method (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Worker Thread exception:");
    }
  ACE_ENDTRY;
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
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->resolve_initial_references ("RTCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      current->the_priority (0
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->resolve_initial_references ("ORBPolicyManager"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->string_to_object (ior
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                            debug
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
          test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unexpected exception!");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in ());

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Unexpected exception!");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
