// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/ORB_Core.h"
#include "../check_supported_priorities.cpp"
#include "./readers.cpp"

static int iterations = 5;
static int shutdown_server = 0;

static const char *ior = "file://ior";

static const char *invocation_priorities_file = "invocation_priorities";
static const char *bands_file = "empty_file";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "b:p:k:i:x");
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
  Worker_Thread (test_ptr test,
                 RTCORBA::Current_ptr current,
                 CORBA::Short priority);

  int svc (void);

private:
  test_var test_;
  RTCORBA::Current_var current_;
  CORBA::Short priority_;
};

Worker_Thread::Worker_Thread (test_ptr test,
                              RTCORBA::Current_ptr current,
                              CORBA::Short priority)
  : test_ (test::_duplicate (test)),
    current_ (RTCORBA::Current::_duplicate (current)),
    priority_ (priority)
{
}

int
Worker_Thread::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->current_->the_priority (this->priority_
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i < iterations; i++)
        {
          this->test_->method (TAO_ENV_SINGLE_ARG_PARAMETER);
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

int
main (int argc, char **argv)
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      current->the_priority (0
                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->string_to_object (ior
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in ()
                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Short_Array priorities;
      result =
        get_values ("client",
                    invocation_priorities_file,
                    "invocation priorities",
                    priorities);
      if (result != 0)
        return result;

      CORBA::PolicyList policies;

      result =
        get_priority_bands ("client",
                            bands_file,
                            rt_orb.in (),
                            policies
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      u_long i = 0;

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
                          Worker_Thread (test.in (),
                                         current.in (),
                                         priorities[i]),
                          -1);

          long flags =
            THR_NEW_LWP |
            THR_JOINABLE |
            orb->orb_core ()->orb_params ()->scope_policy () |
            orb->orb_core ()->orb_params ()->sched_policy ();

          result =
            workers[i]->activate (flags);
          if (result != 0)
            return result;
        }

      ACE_Thread_Manager::instance ()->wait ();

      for (i = 0;
           i < priorities.size ();
           ++i)
        {
          delete workers[i];
        }
      delete[] workers;

      if (shutdown_server)
        {
          test->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
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
