//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a client implementation.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Strategies/advanced_resource.h"
#include "../check_supported_priorities.cpp"
#include "../common_args.cpp"
#include "testC.h"

#include <cstddef>
#include <memory>
#include <new>
#include <vector>

struct Client_Options
{
  int iterations { 1 };
  int debug { 1 };
  bool shutdown_server {};
  ACE_TCHAR const *ior { ACE_TEXT ("file://ior") };
  ACE_TCHAR const *invocation_priorities_file { ACE_TEXT ("empty_file") };
  ACE_TCHAR const *protocols_file { ACE_TEXT ("empty_file") };
  ACE_TCHAR const *bands_file { ACE_TEXT ("empty_file") };
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:d:i:k:p:t:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':
        options.bands_file = get_opts.opt_arg ();
        break;

      case 'd':
        options.debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'p':
        options.protocols_file = get_opts.opt_arg ();
        break;

      case 't':
        options.invocation_priorities_file = get_opts.opt_arg ();
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s usage:\n"
                           "\t-b <bands file> (defaults to %s)\n"
                           "\t-i <iterations> (defaults to %d)\n"
                           "\t-k <ior> (defaults to %s)\n"
                           "\t-p <protocol file> (defaults to %s)\n"
                           "\t-t <invocation priorities file> (defaults to %s)\n"
                           "\t-x <shutdown server> (defaults to %d)\n"
                           "\n",
                           argv[0],
                           options.bands_file,
                           options.iterations,
                           options.ior,
                           options.protocols_file,
                           options.invocation_priorities_file,
                           options.shutdown_server),
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
          CORBA::PolicyManager_ptr policy_manager,
          Client_Options const &options);
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  void vanilla_invocations ();

  void priority_invocations (int debug);

  void set_client_protocols_policies (int debug);

  void set_priority_bands (int debug);

  void set_private_connection_policies ();

  void reset_policies ();

private:
  test_var const test_;
  CORBA::ORB_var const orb_;
  RTCORBA::Current_var const current_;
  RTCORBA::RTORB_var const rt_orb_;
  CORBA::PolicyManager_var const policy_manager_;
  Client_Options const options_;
};

class Worker_Thread : public ACE_Task_Base
{
public:
  Worker_Thread (ACE_Thread_Manager &thread_manager,
                 Client &client,
                 test_ptr test,
                 RTCORBA::Current_ptr current,
                 CORBA::Short priority);
  Worker_Thread (Worker_Thread const &) = delete;
  Worker_Thread (Worker_Thread &&) = delete;
  Worker_Thread &operator= (Worker_Thread const &) = delete;
  Worker_Thread &operator= (Worker_Thread &&) = delete;

  int svc () override;

  void validate_connection ();

private:
  // The client outlives the worker because its thread manager is joined first.
  Client &client_;
  test_var const test_;
  RTCORBA::Current_var const current_;
  CORBA::Short const priority_;
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

      this->client_.vanilla_invocations ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Worker Thread exception:");
    }
  return 0;
}

Client::Client (test_ptr test,
                CORBA::ORB_ptr orb,
                RTCORBA::Current_ptr current,
                RTCORBA::RTORB_ptr rt_orb,
                CORBA::PolicyManager_ptr policy_manager,
                Client_Options const &options)
  : test_ (test::_duplicate (test)),
    orb_ (CORBA::ORB::_duplicate (orb)),
    current_ (RTCORBA::Current::_duplicate (current)),
    rt_orb_ (RTCORBA::RTORB::_duplicate (rt_orb)),
    policy_manager_ (CORBA::PolicyManager::_duplicate (policy_manager)),
    options_ (options)
{
}

void
Client::vanilla_invocations ()
{
  for (int i = 0; i < this->options_.iterations; ++i)
    {
      this->test_->method ();
    }
}

void
Client::priority_invocations (int debug)
{
  ULong_Array priorities;
  int result =
    get_values ("client",
                this->options_.invocation_priorities_file,
                "invocation priorities",
                priorities,
                debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing invocation priorities data file: %s\n",
                  this->options_.invocation_priorities_file));
      return;
    }

  std::vector<std::unique_ptr<Worker_Thread>> workers;
  workers.reserve (priorities.size ());

  ACE_Thread_Manager thread_manager;

  for (size_t i = 0; i < priorities.size (); ++i)
    {
      workers.push_back (std::make_unique<Worker_Thread> (
                           thread_manager,
                           *this,
                           this->test_.in (),
                           this->current_.in (),
                           priorities[i]));
    }

  for (size_t i = 0; i < priorities.size (); ++i)
    {
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      int const activate_result = workers[i]->activate (flags);
      if (activate_result != 0)
        {
          thread_manager.wait ();
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate thread\n"));
          return;
        }
    }

  thread_manager.wait ();
}

void
Client::set_client_protocols_policies (int debug)
{
  CORBA::PolicyList policies;

  int const result =
    get_protocols ("client",
                   this->options_.protocols_file,
                   this->rt_orb_.in (),
                   policies,
                   debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing protocols data file: %s\n",
                  this->options_.protocols_file));
      return;
    }

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE);
}

void
Client::set_priority_bands (int debug)
{
  CORBA::PolicyList policies;

  int const result =
    get_priority_bands ("client",
                        this->options_.bands_file,
                        this->rt_orb_.in (),
                        policies,
                        debug);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error in parsing bands data file: %s\n",
                  this->options_.bands_file));
      return;
    }

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE);
}

void
Client::set_private_connection_policies ()
{
  CORBA::PolicyList policies;
  policies.length (1);

  policies[0] =
    this->rt_orb_->create_private_connection_policy ();

  this->policy_manager_->set_policy_overrides (policies,
                                               CORBA::ADD_OVERRIDE);
}

void
Client::reset_policies ()
{
  CORBA::PolicyList empty_policies;

  this->policy_manager_->set_policy_overrides (empty_policies,
                                               CORBA::SET_OVERRIDE);
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
        this->orb_->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var const policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());

      current->the_priority (0);

      object =
        this->orb_->string_to_object (this->options_.ior);

      test_var test =
        test::_narrow (object.in ());

      Client client (test.in (),
                     this->orb_.in (),
                     current.in (),
                     rt_orb.in (),
                     policy_manager.in (),
                     this->options_);

      client.vanilla_invocations ();

      client.set_private_connection_policies ();

      client.vanilla_invocations ();

      client.reset_policies ();

      client.set_client_protocols_policies (this->options_.debug);

      client.vanilla_invocations ();

      client.set_private_connection_policies ();

      client.vanilla_invocations ();

      client.reset_policies ();

      client.set_priority_bands (this->options_.debug);

      client.priority_invocations (this->options_.debug);

      client.set_private_connection_policies ();

      client.priority_invocations (this->options_.debug);

      client.reset_policies ();

      client.set_priority_bands (this->options_.debug);

      client.set_client_protocols_policies (0);

      client.priority_invocations (0);

      client.set_private_connection_policies ();

      client.priority_invocations (this->options_.debug);

      client.reset_policies ();

      if (this->options_.shutdown_server)
        {
          test->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Error!");
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
      ex._tao_print_exception ("Error!");
      return -1;
    }

  return 0;
}
