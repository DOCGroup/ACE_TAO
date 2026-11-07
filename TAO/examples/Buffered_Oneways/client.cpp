
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a client that uses buffered oneways.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://ior")};
  CORBA::ULong iterations {20};
  CORBA::Long message_bytes {-1};
  CORBA::Long message_count {5};
  CORBA::Long flush_count {-1};
  long timeout {-1};
  long interval {1000};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:f:c:b:i:z:t:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'f':
        options.flush_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'c':
        options.message_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        options.message_bytes = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        options.interval = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        options.timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-f flush count "
                           "-c message count "
                           "-b message bytes "
                           "-i iterations "
                           "-z interval between calls "
                           "-t implicit flush timeout "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      Client_Options options;
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var base =
        orb->string_to_object (options.ior.c_str ());

      // Try to narrow the object reference to a <test> reference.
      test_var test_object = test::_narrow (base.in ());

      // Obtain PolicyCurrent.
      base = orb->resolve_initial_references ("PolicyCurrent");

      // Narrow down to correct type.
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (base.in ());

      // Setup the none sync scope policy, i.e., the ORB will buffer
      // oneways.
      Messaging::SyncScope const sync_none {Messaging::SYNC_NONE};

      // Setup the none sync scope any.
      CORBA::Any sync_none_any;
      sync_none_any <<= sync_none;

      // Setup the none sync scope policy list.
      CORBA::PolicyList sync_none_policy_list (1);
      sync_none_policy_list.length (1);

      // Setup the none sync scope policy.
      sync_none_policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_none_any);

      // Setup the none sync scope.
      policy_current->set_policy_overrides (sync_none_policy_list,
                                            CORBA::ADD_OVERRIDE);

      // We are now done with this policy.
      sync_none_policy_list[0]->destroy ();

      // Start off with no constraints.
      TAO::BufferingConstraint buffering_constraint;
      buffering_constraint.mode = TAO::BUFFER_FLUSH;
      buffering_constraint.message_count = 0;
      buffering_constraint.message_bytes = 0;
      buffering_constraint.timeout = 0;

      // If valid <message_count>, set the implicit flushing to
      // account for queued messages.
      if (options.message_count != -1)
        {
          buffering_constraint.mode |= TAO::BUFFER_MESSAGE_COUNT;
          buffering_constraint.message_count = options.message_count;
        }

      // If valid <message_bytes>, set the implicit flushing to
      // account for queued bytes.
      if (options.message_bytes != -1)
        {
          buffering_constraint.mode |= TAO::BUFFER_MESSAGE_BYTES;
          buffering_constraint.message_bytes = options.message_bytes;
        }

      // If valid <timeout>, set the implicit flushing to account for
      // timeouts.
      if (options.timeout != -1)
        {
          buffering_constraint.mode |= TAO::BUFFER_TIMEOUT;
          buffering_constraint.timeout = options.timeout * 10000;
        }

      // Setup the buffering constraint any.
      CORBA::Any buffering_constraint_any;
      buffering_constraint_any <<= buffering_constraint;

      // Setup the buffering constraint policy list.
      CORBA::PolicyList buffering_constraint_policy_list (1);
      buffering_constraint_policy_list.length (1);

      // Setup the buffering constraint policy.
      buffering_constraint_policy_list[0] =
        orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                            buffering_constraint_any);

      if (buffering_constraint.mode == TAO::BUFFER_FLUSH)
        {
          ACE_ERROR((LM_ERROR, "Error : Must specify a timeout, message"
            " count, or message bytes constraint.\n"));
          return 1;
        }

      // Setup the constraints.
      policy_current->set_policy_overrides (buffering_constraint_policy_list,
                                            CORBA::ADD_OVERRIDE);

      //
      // We use this policy again later. Therefore, we don't destroy
      // it right away.
      //

      // Setup the explicit flushing policy.
      TAO::BufferingConstraint buffering_flush;
      buffering_flush.mode = TAO::BUFFER_FLUSH;
      buffering_flush.message_count = 0;
      buffering_flush.message_bytes = 0;
      buffering_flush.timeout = 0;

      // Setup the buffering flush any.
      CORBA::Any buffering_flush_any;
      buffering_flush_any <<= buffering_flush;

      // Setup the buffering flush policy list.
      CORBA::PolicyList buffering_flush_policy_list (1);
      buffering_flush_policy_list.length (1);

      // Setup the buffering flush policy.
      buffering_flush_policy_list[0] =
        orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                            buffering_flush_any);

      //
      // Explicit flushing policy will be used later.
      //

      for (CORBA::ULong i = 1; i <= options.iterations; ++i)
        {
          // Explicit flushing (is specified).
          if (options.flush_count != -1 &&
              i % options.flush_count == 0)
            {
              // Setup explicit flushing.
              policy_current->set_policy_overrides (buffering_flush_policy_list,
                                                    CORBA::ADD_OVERRIDE);

              ACE_DEBUG ((LM_DEBUG,
                          "client: Iteration %d @ %T\n",
                          i));

              // Invoke the oneway method.
              test_object->method (i);

              // Reset buffering policy.
              policy_current->set_policy_overrides (buffering_constraint_policy_list,
                                                    CORBA::ADD_OVERRIDE);
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "client: Iteration %d @ %T\n",
                          i));

              // Invoke the oneway method.
              test_object->method (i);
            }

          // Interval between successive calls.
          ACE_Time_Value sleep_interval (0,
                                         options.interval * 1000);

          orb->run (sleep_interval);
        }

      // Shutdown server.
      if (options.shutdown_server)
        {
          test_object->shutdown ();
        }

      // We are done with the policy.
      buffering_constraint_policy_list[0]->destroy ();

      // We are done with the policy.
      buffering_flush_policy_list[0]->destroy ();

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
