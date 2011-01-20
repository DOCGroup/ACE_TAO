// $Id$

#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int test_sync_with_target    = 0;
int test_sync_with_server    = 0;
int test_sync_with_transport = 0;
int test_sync_none           = 0;
int iterations = 500;

const int ALIVE     = 0;
const int DESTROYED = 1;
const int SHUTDOWN  = 2;
int server_status = ALIVE;

int successful_calls = 0;
int failed_calls = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:ntsr"));
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

      case 'n':
        test_sync_none = 1;
        break;

      case 't':
        test_sync_with_transport = 1;
        break;

      case 's':
        test_sync_with_server = 1;
        break;

      case 'r':
        test_sync_with_target = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n -t -s -r "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

void run_test_sync_with_target (Test::Oneway_Receiver_ptr oneway_receiver);
void run_test_sync_with_server (Test::Oneway_Receiver_ptr oneway_receiver);
void run_test_sync_with_transport (Test::Oneway_Receiver_ptr oneway_receiver);
void run_test_sync_none (Test::Oneway_Receiver_ptr oneway_receiver);


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Oneway_Receiver_var oneway_receiver =
        Test::Oneway_Receiver::_narrow(tmp.in ());

      if (CORBA::is_nil (oneway_receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil oneway receiver reference <%s>\n",
                             ior),
                            1);
        }

      if (test_sync_with_target)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_TARGET\n"));
          run_test_sync_with_target (oneway_receiver.in ());
        }
      else if (test_sync_with_server)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_SERVER\n"));
          run_test_sync_with_server (oneway_receiver.in ());
        }
      else if (test_sync_with_transport)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_TRANSPORT\n"));
          run_test_sync_with_transport (oneway_receiver.in ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_NONE\n"));
          run_test_sync_none (oneway_receiver.in ());
        }

      //FUZZ: disable check_for_lack_ACE_OS
      // Run the orb for 3 seconds, this way we make sure things are flushed
      // to the transport.
      ACE_Time_Value time (3, 0);
      //FUZZ: enable check_for_lack_ACE_OS

      orb->run (time);

      if (successful_calls == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: No requests were successful\n"));

      if (failed_calls != 0)
        ACE_ERROR ((LM_ERROR, "ERROR: At least %d requests failed\n",
                    failed_calls));

      if (failed_calls + successful_calls != iterations)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Mismatched number of calls (%d + %d != %d)\n",
                    successful_calls, failed_calls, iterations));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

void
set_sync_scope_policy (Messaging::SyncScope sync_scope)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  CORBA::Object_var object =
    orb->resolve_initial_references ("PolicyCurrent");

  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ());

  if (CORBA::is_nil (policy_current.in ()))
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
      return;
    }
  CORBA::Any scope_as_any;
  scope_as_any <<= sync_scope;

  CORBA::PolicyList policies(1); policies.length (1);
  policies[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        scope_as_any);

  policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

  policies[0]->destroy ();
}

void
run_one_iteration (int i,
                   Test::Oneway_Receiver_ptr oneway_receiver,
                   Test::Shutdown_Helper_ptr shutdown_helper)
{
  try
    {
      if (i == iterations / 3)
        {
          ACE_DEBUG ((LM_DEBUG, "Destroying object\n"));
          server_status = DESTROYED;
          oneway_receiver->destroy ();
        }
      if (i == 2 * iterations / 3)
        {
          ACE_DEBUG ((LM_DEBUG, "Shutting down server process\n"));
          server_status = SHUTDOWN;
          shutdown_helper->shutdown ();
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  oneway_receiver->raise_no_permission ();
}

/// Helper routine to set breakpoints during failures
void
call_failed (const CORBA::SystemException &ex)
{
  failed_calls++;

  ex._tao_print_exception ("Exception raised");
}

void
run_test_sync_with_target (Test::Oneway_Receiver_ptr oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TARGET);

  Test::Shutdown_Helper_var shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  int had_comm_failure = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
        {
          run_one_iteration (i, oneway_receiver, shutdown_helper.in ());
        }
      catch (const CORBA::NO_PERMISSION& ex)
        {
          if (server_status == ALIVE)
            successful_calls++;
          else
            call_failed (ex);
        }
      catch (const CORBA::OBJECT_NOT_EXIST& ex)
        {
          if (server_status == DESTROYED)
            successful_calls++;
          else
            call_failed (ex);
        }
      catch (const CORBA::COMM_FAILURE& ex)
        {
          if (server_status == SHUTDOWN && had_comm_failure == 0)
            successful_calls++;
          else
            call_failed (ex);
          had_comm_failure = 1;
        }
      catch (const CORBA::TRANSIENT& ex)
        {
          if (server_status == SHUTDOWN)
            successful_calls++;
          else
            call_failed (ex);
        }
      catch (const CORBA::SystemException& ex)
        {
          call_failed (ex);
        }
    }
}

void
run_test_sync_with_server (Test::Oneway_Receiver_ptr oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_SERVER);

  Test::Shutdown_Helper_var shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  int had_comm_failure = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
        {
          run_one_iteration (i, oneway_receiver, shutdown_helper.in ());

          if (server_status == DESTROYED
              || server_status == SHUTDOWN)
            failed_calls++;
          else
            successful_calls++;
        }
      catch (const CORBA::NO_PERMISSION& ex)
        {
          call_failed (ex);
        }
      catch (const CORBA::OBJECT_NOT_EXIST& ex)
        {
          if (server_status == DESTROYED)
            successful_calls++;
          else
            call_failed (ex);
        }
      catch (const CORBA::COMM_FAILURE& ex)
        {
          if (server_status == SHUTDOWN && had_comm_failure == 0)
            successful_calls++;
          else
            call_failed (ex);
          had_comm_failure = 1;
        }
      catch (const CORBA::TRANSIENT& ex)
        {
          if (server_status == SHUTDOWN)
            successful_calls++;
          else
            call_failed (ex);
        }
      catch (const CORBA::SystemException& ex)
        {
          call_failed (ex);
        }
    }
}

/// Helper routine to run the sync_with_transport and sync_none tests.
void
run_test_unreliable (Test::Oneway_Receiver_ptr oneway_receiver);

void
run_test_sync_with_transport (Test::Oneway_Receiver_ptr oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TRANSPORT);

  run_test_unreliable (oneway_receiver);
}

void
run_test_sync_none (Test::Oneway_Receiver_ptr oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_NONE);

  run_test_unreliable (oneway_receiver);
}


void
run_test_unreliable (Test::Oneway_Receiver_ptr oneway_receiver)
{
  Test::Shutdown_Helper_var shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  int undetected = 0;
  int had_transient = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
        {
          run_one_iteration (i, oneway_receiver, shutdown_helper.in ());

          // A few failures can go undetected...
          if (server_status == SHUTDOWN)
            undetected++;

          // Once a TRANSIENT is raised all future calls should fail.
          if (had_transient)
            failed_calls++;
          else
            successful_calls++;
        }
      catch (const CORBA::NO_PERMISSION& ex)
        {
          call_failed (ex);
        }
      catch (const CORBA::OBJECT_NOT_EXIST& ex)
        {
          call_failed (ex);
        }
      catch (const CORBA::COMM_FAILURE& ex)
        {
          call_failed (ex);
        }
      catch (const CORBA::TRANSIENT& ex)
        {
          if (server_status == SHUTDOWN)
            successful_calls++;
          else
            call_failed (ex);
          had_transient = 1;
        }
      catch (const CORBA::SystemException& ex)
        {
          call_failed (ex);
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Undetected TRANSIENT count = %d\n",
              undetected));
}
