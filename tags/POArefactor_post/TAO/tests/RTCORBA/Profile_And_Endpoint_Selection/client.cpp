// $Id$

// ================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a client implementation.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Strategies/advanced_resource.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"
#include "testC.h"

ACE_RCSID(Profile_And_Endpoint_Selection, client, "$Id$")

static int iterations = 1;
static int debug = 1;
static int shutdown_server = 0;
static const char *ior = "file://ior";
static const char *invocation_priorities_file = "empty_file";
static const char *protocols_file = "empty_file";
static const char *bands_file = "empty_file";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "b:d:i:k:p:t:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        bands_file = get_opts.opt_arg ();
        break;

      case 'd':
        debug = ::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'p':
        protocols_file = get_opts.opt_arg ();
        break;

      case 't':
        invocation_priorities_file = get_opts.opt_arg ();
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s usage: \n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-i <iterations> (defaults to %d)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\t-p <protocol file> (defaults to %s)\n"
                           "\t-t <invocation priorities file> (defaults to %s)\n"
                           "\t-x <shutdown server> (defaults to %d)\n"
                           "\n",
                           argv[0],
                           bands_file,
                           iterations,
                           ior,
                           protocols_file,
                           invocation_priorities_file,
                           shutdown_server),
                          -1);
      }

  return 0;
}

class Client
{

public:

  Client (test_ptr test,
          CORBA::ORB_ptr orb,
          RTCORBA::Current_ptr current,
          RTCORBA::RTORB_ptr rt_orb,
          CORBA::PolicyManager_ptr policy_manager);

  void vanilla_invocations (ACE_ENV_SINGLE_ARG_DECL);

  void priority_invocations (int debug
                             ACE_ENV_ARG_DECL);

  void set_client_protocols_policies (int debug
                                      ACE_ENV_ARG_DECL);

  void set_priority_bands (int debug
                           ACE_ENV_ARG_DECL);

  void set_private_connection_policies (ACE_ENV_SINGLE_ARG_DECL);

  void reset_policies (ACE_ENV_SINGLE_ARG_DECL);


private:

  test_var test_;
  CORBA::ORB_var orb_;
  RTCORBA::Current_var current_;
  RTCORBA::RTORB_var rt_orb_;
  CORBA::PolicyManager_var policy_manager_;

};

class Worker_Thread : public ACE_Task_Base
{
public:
  Worker_Thread (ACE_Thread_Manager &thread_manager,
                 Client &client,
                 test_ptr test,
                 RTCORBA::Current_ptr current,
                 CORBA::Short priority);

  int svc (void);

  void validate_connection (ACE_ENV_SINGLE_ARG_DECL);

private:
  Client client_;
  test_var test_;
  RTCORBA::Current_var current_;
  CORBA::Short priority_;
};

Worker_Thread::Worker_Thread (ACE_Thread_Manager &thread_manager,
                              Client &client,
                              test_ptr test,
                              RTCORBA::Current_ptr current,
                              CORBA::Short priority)
  : ACE_Task_Base (&thread_manager),
    client_ (client),
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

      this->client_.vanilla_invocations (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Worker Thread exception:");
    }
  ACE_ENDTRY;
  return 0;
}

Client::Client (test_ptr test,
                CORBA::ORB_ptr orb,
                RTCORBA::Current_ptr current,
                RTCORBA::RTORB_ptr rt_orb,
                CORBA::PolicyManager_ptr policy_manager)
  : test_ (test::_duplicate (test)),
    orb_ (CORBA::ORB::_duplicate (orb)),
    current_ (RTCORBA::Current::_duplicate (current)),
    rt_orb_ (RTCORBA::RTORB::_duplicate (rt_orb)),
    policy_manager_ (CORBA::PolicyManager::_duplicate (policy_manager))
{
}

void
Client::vanilla_invocations (ACE_ENV_SINGLE_ARG_DECL)
{
  for (int i = 0; i < iterations; i++)
    {
      this->test_->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Client::priority_invocations (int debug
                              ACE_ENV_ARG_DECL)
{
  ULong_Array priorities;
  int result =
    get_values ("client",
                invocation_priorities_file,
                "invocation priorities",
                priorities,
                debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing invocation priorities data file: %s\n",
                  invocation_priorities_file));
      return;
    }

  u_long i = 0;

  Worker_Thread **workers = 0;

  ACE_NEW_THROW_EX (workers,
                    Worker_Thread *[priorities.size ()],
                    CORBA::NO_MEMORY ());

  ACE_Thread_Manager thread_manager;

  for (i = 0;
       i < priorities.size ();
       ++i)
    {
      ACE_NEW_THROW_EX (workers[i],
                        Worker_Thread (thread_manager,
                                       *this,
                                       this->test_.in (),
                                       this->current_.in (),
                                       priorities[i]),
                        CORBA::NO_MEMORY ());

      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      result =
        workers[i]->activate (flags);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate thread\n"));
          return;
        }
    }

  thread_manager.wait ();

  for (i = 0;
       i < priorities.size ();
       ++i)
    {
      delete workers[i];
    }
  delete[] workers;
}

void
Client::set_client_protocols_policies (int debug
                                       ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policies;

  int result =
    get_protocols ("client",
                   protocols_file,
                   this->rt_orb_.in (),
                   policies,
                   debug
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing protocols data file: %s\n",
                  protocols_file));
      return;
    }

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Client::set_priority_bands (int debug
                            ACE_ENV_ARG_DECL)
{
  CORBA::PolicyList policies;

  int result =
    get_priority_bands ("client",
                        bands_file,
                        this->rt_orb_.in (),
                        policies,
                        debug
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing bands data file: %s\n",
                  bands_file));
      return;
    }

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Client::set_private_connection_policies (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::PolicyList policies;
  policies.length (1);

  policies[0] =
    this->rt_orb_->create_private_connection_policy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Client::reset_policies (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::PolicyList empty_policies;

  this->policy_manager_->set_policy_overrides (empty_policies,
                                               CORBA::SET_OVERRIDE
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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
        this->orb_->resolve_initial_references ("ORBPolicyManager"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ()
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
        this->orb_->string_to_object (ior
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Client client (test.in (),
                     this->orb_.in (),
                     current.in (),
                     rt_orb.in (),
                     policy_manager.in ());

      client.vanilla_invocations (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_private_connection_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.vanilla_invocations (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.reset_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_client_protocols_policies (debug
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.vanilla_invocations (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_private_connection_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.vanilla_invocations (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.reset_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_priority_bands (debug
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.priority_invocations (debug
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_private_connection_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.priority_invocations (debug
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.reset_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_priority_bands (debug
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_client_protocols_policies (0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.priority_invocations (0
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.set_private_connection_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.priority_invocations (debug
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.reset_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (shutdown_server)
        {
          test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error!");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in());

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
      int result =
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error!");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
