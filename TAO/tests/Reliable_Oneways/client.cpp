#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

enum class Server_Status
{
  alive,
  destroyed,
  shutdown
};

struct Test_Context
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  bool sync_with_target {};
  bool sync_with_server {};
  bool sync_with_transport {};
  int iterations {500};
  Server_Status server_status {Server_Status::alive};
  int successful_calls {};
  int failed_calls {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Test_Context &context)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:ntsr"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        context.ior = get_opts.opt_arg ();
        break;

      case 'i':
        context.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        break;

      case 't':
        context.sync_with_transport = true;
        break;

      case 's':
        context.sync_with_server = true;
        break;

      case 'r':
        context.sync_with_target = true;
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
  return 0;
}

void run_test_sync_with_target (Test::Oneway_Receiver_ptr oneway_receiver,
                                Test_Context &context);
void run_test_sync_with_server (Test::Oneway_Receiver_ptr oneway_receiver,
                                Test_Context &context);
void run_test_sync_with_transport (Test::Oneway_Receiver_ptr oneway_receiver,
                                   Test_Context &context);
void run_test_sync_none (Test::Oneway_Receiver_ptr oneway_receiver,
                         Test_Context &context);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Test_Context context;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, context) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (context.ior.c_str ());
      Test::Oneway_Receiver_var const oneway_receiver =
        Test::Oneway_Receiver::_narrow (tmp.in ());

      if (CORBA::is_nil (oneway_receiver.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil oneway receiver reference <%s>\n",
                           context.ior.c_str ()),
                          1);

      if (context.sync_with_target)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_TARGET\n"));
          run_test_sync_with_target (oneway_receiver.in (), context);
        }
      else if (context.sync_with_server)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_SERVER\n"));
          run_test_sync_with_server (oneway_receiver.in (), context);
        }
      else if (context.sync_with_transport)
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_WITH_TRANSPORT\n"));
          run_test_sync_with_transport (oneway_receiver.in (), context);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Running SYNC_NONE\n"));
          run_test_sync_none (oneway_receiver.in (), context);
        }

      //FUZZ: disable check_for_lack_ACE_OS
      // Run the orb for 3 seconds, this way we make sure things are flushed
      // to the transport.
      ACE_Time_Value time (3, 0);
      //FUZZ: enable check_for_lack_ACE_OS

      orb->run (time);

      if (context.successful_calls == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: No requests were successful\n"));

      if (context.failed_calls != 0)
        ACE_ERROR ((LM_ERROR, "ERROR: At least %d requests failed\n",
                    context.failed_calls));

      if (context.failed_calls + context.successful_calls != context.iterations)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Mismatched number of calls (%d + %d != %d)\n",
                    context.successful_calls,
                    context.failed_calls,
                    context.iterations));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}

void
set_sync_scope_policy (Messaging::SyncScope sync_scope)
{
  int argc {};
  ACE_TCHAR **argv {};
  CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var const object =
    orb->resolve_initial_references ("PolicyCurrent");
  CORBA::PolicyCurrent_var const policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ());

  if (CORBA::is_nil (policy_current.in ()))
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
      return;
    }

  CORBA::Any scope_as_any;
  scope_as_any <<= sync_scope;
  CORBA::PolicyList policies (1);
  policies.length (1);
  policies[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        scope_as_any);
  policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
  policies[0]->destroy ();
}

void
run_one_iteration (int iteration,
                   Test::Oneway_Receiver_ptr oneway_receiver,
                   Test::Shutdown_Helper_ptr shutdown_helper,
                   Test_Context &context)
{
  try
    {
      if (iteration == context.iterations / 3)
        {
          ACE_DEBUG ((LM_DEBUG, "Destroying object\n"));
          context.server_status = Server_Status::destroyed;
          oneway_receiver->destroy ();
        }
      if (iteration == 2 * context.iterations / 3)
        {
          ACE_DEBUG ((LM_DEBUG, "Shutting down server process\n"));
          context.server_status = Server_Status::shutdown;
          shutdown_helper->shutdown ();
        }
    }
  catch (CORBA::Exception const &)
    {
    }

  oneway_receiver->raise_no_permission ();
}

/// Helper routine to set breakpoints during failures.
void
call_failed (CORBA::SystemException const &ex,
             Test_Context &context)
{
  ++context.failed_calls;
  ex._tao_print_exception ("Exception raised");
}

void
run_test_sync_with_target (Test::Oneway_Receiver_ptr oneway_receiver,
                           Test_Context &context)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TARGET);
  Test::Shutdown_Helper_var const shutdown_helper =
    oneway_receiver->get_shutdown_helper ();
  bool had_comm_failure {};

  for (int iteration {}; iteration != context.iterations; ++iteration)
    {
      try
        {
          run_one_iteration (iteration,
                             oneway_receiver,
                             shutdown_helper.in (),
                             context);
        }
      catch (CORBA::NO_PERMISSION const &ex)
        {
          if (context.server_status == Server_Status::alive)
            ++context.successful_calls;
          else
            call_failed (ex, context);
        }
      catch (CORBA::OBJECT_NOT_EXIST const &ex)
        {
          if (context.server_status == Server_Status::destroyed)
            ++context.successful_calls;
          else
            call_failed (ex, context);
        }
      catch (CORBA::COMM_FAILURE const &ex)
        {
          if (context.server_status == Server_Status::shutdown && !had_comm_failure)
            ++context.successful_calls;
          else
            call_failed (ex, context);
          had_comm_failure = true;
        }
      catch (CORBA::TRANSIENT const &ex)
        {
          if (context.server_status == Server_Status::shutdown)
            ++context.successful_calls;
          else
            call_failed (ex, context);
        }
      catch (CORBA::SystemException const &ex)
        {
          call_failed (ex, context);
        }
    }
}

void
run_test_sync_with_server (Test::Oneway_Receiver_ptr oneway_receiver,
                           Test_Context &context)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_SERVER);
  Test::Shutdown_Helper_var const shutdown_helper =
    oneway_receiver->get_shutdown_helper ();
  bool had_comm_failure {};

  for (int iteration {}; iteration != context.iterations; ++iteration)
    {
      try
        {
          run_one_iteration (iteration,
                             oneway_receiver,
                             shutdown_helper.in (),
                             context);
          if (context.server_status == Server_Status::destroyed
              || context.server_status == Server_Status::shutdown)
            ++context.failed_calls;
          else
            ++context.successful_calls;
        }
      catch (CORBA::NO_PERMISSION const &ex)
        {
          call_failed (ex, context);
        }
      catch (CORBA::OBJECT_NOT_EXIST const &ex)
        {
          if (context.server_status == Server_Status::destroyed)
            ++context.successful_calls;
          else
            call_failed (ex, context);
        }
      catch (CORBA::COMM_FAILURE const &ex)
        {
          if (context.server_status == Server_Status::shutdown && !had_comm_failure)
            ++context.successful_calls;
          else
            call_failed (ex, context);
          had_comm_failure = true;
        }
      catch (CORBA::TRANSIENT const &ex)
        {
          if (context.server_status == Server_Status::shutdown)
            ++context.successful_calls;
          else
            call_failed (ex, context);
        }
      catch (CORBA::SystemException const &ex)
        {
          call_failed (ex, context);
        }
    }
}

void
run_test_unreliable (Test::Oneway_Receiver_ptr oneway_receiver,
                     Test_Context &context);

void
run_test_sync_with_transport (Test::Oneway_Receiver_ptr oneway_receiver,
                              Test_Context &context)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TRANSPORT);
  run_test_unreliable (oneway_receiver, context);
}

void
run_test_sync_none (Test::Oneway_Receiver_ptr oneway_receiver,
                    Test_Context &context)
{
  set_sync_scope_policy (Messaging::SYNC_NONE);
  run_test_unreliable (oneway_receiver, context);
}

void
run_test_unreliable (Test::Oneway_Receiver_ptr oneway_receiver,
                     Test_Context &context)
{
  Test::Shutdown_Helper_var const shutdown_helper =
    oneway_receiver->get_shutdown_helper ();
  int undetected {};
  bool had_transient {};

  for (int iteration {}; iteration != context.iterations; ++iteration)
    {
      try
        {
          run_one_iteration (iteration,
                             oneway_receiver,
                             shutdown_helper.in (),
                             context);

          // A few failures can go undetected...
          if (context.server_status == Server_Status::shutdown)
            ++undetected;

          // Once a TRANSIENT is raised all future calls should fail.
          if (had_transient)
            ++context.failed_calls;
          else
            ++context.successful_calls;
        }
      catch (CORBA::NO_PERMISSION const &ex)
        {
          call_failed (ex, context);
        }
      catch (CORBA::OBJECT_NOT_EXIST const &ex)
        {
          call_failed (ex, context);
        }
      catch (CORBA::COMM_FAILURE const &ex)
        {
          call_failed (ex, context);
        }
      catch (CORBA::TRANSIENT const &ex)
        {
          if (context.server_status == Server_Status::shutdown)
            ++context.successful_calls;
          else
            call_failed (ex, context);
          had_transient = true;
        }
      catch (CORBA::SystemException const &ex)
        {
          call_failed (ex, context);
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Undetected TRANSIENT count = %d\n", undetected));
}
