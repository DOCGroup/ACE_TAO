// $Id$

#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/TAOC.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *server_ior = ACE_TEXT("file://server.ior");
const ACE_TCHAR *admin_ior = ACE_TEXT("file://admin.ior");
int iterations = 20;

int run_message_count_test = 0;
int run_timeout_test = 0;
int run_timeout_reactive_test = 0;
int run_buffer_size_test = 0;

const int PAYLOAD_LENGTH = 1024;
const int BUFFERED_MESSAGES_COUNT = 10;
const unsigned int TIMEOUT_MILLISECONDS = 50;
const int BUFFER_SIZE = 10 * PAYLOAD_LENGTH;

/// Check that no more than 10% of the messages are not sent.
const double LIVENESS_TOLERANCE = 0.9;

/// Limit the depth of the liveness test, avoid blowing up the stack
/// on the server
const int LIVENESS_MAX_DEPTH = 256;

/// Factor in GIOP overhead in the buffer size test
const double GIOP_OVERHEAD = 0.9;

const ACE_Time_Value TRANSIENT_HOLDOFF (0, 500); // 0.5ms delay
const int TRANSIENT_LIMIT = 10;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:a:i:ctbr"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.opt_arg ();
        break;

      case 'a':
        admin_ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'c':
        run_message_count_test = 1;
        break;

      case 't':
        run_timeout_test = 1;
        break;

      case 'b':
        run_buffer_size_test = 1;
        break;

      case 'r':
        run_timeout_reactive_test = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <server_ior> "
                           "-a <admin_ior> "
                           "-i <iterations> "
                           "<-c|-t|-b|-r> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
run_message_count (CORBA::ORB_ptr orb,
                   Test::Oneway_Buffering_ptr oneway_buffering,
                   Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin);

int
run_timeout (CORBA::ORB_ptr orb,
             Test::Oneway_Buffering_ptr oneway_buffering,
             Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin);

int
run_timeout_reactive (CORBA::ORB_ptr orb,
                      Test::Oneway_Buffering_ptr oneway_buffering,
                      Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin);

int
run_buffer_size (CORBA::ORB_ptr orb,
                 Test::Oneway_Buffering_ptr oneway_buffering,
                 Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int test_failed = 0;
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(server_ior);

      Test::Oneway_Buffering_var oneway_buffering =
        Test::Oneway_Buffering::_narrow(tmp.in ());

      if (CORBA::is_nil (oneway_buffering.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Oneway_Buffering reference <%s>\n",
                             server_ior),
                            1);
        }

      tmp =
        orb->string_to_object(admin_ior);

      Test::Oneway_Buffering_Admin_var oneway_buffering_admin =
        Test::Oneway_Buffering_Admin::_narrow(tmp.in ());

      if (CORBA::is_nil (oneway_buffering_admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Oneway_Buffering_Admin reference <%s>\n",
                             admin_ior),
                            1);
        }

      if (run_message_count_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running message count flushing test\n"));
          test_failed =
            run_message_count (orb.in (),
                               oneway_buffering.in (),
                               oneway_buffering_admin.in ());
        }
      else if (run_timeout_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running timeout flushing test\n"));
          test_failed =
            run_timeout (orb.in (),
                         oneway_buffering.in (),
                         oneway_buffering_admin.in ());
        }
      else if (run_timeout_reactive_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running timeout (reactive) flushing test\n"));
          test_failed =
            run_timeout_reactive (orb.in (),
                                  oneway_buffering.in (),
                                  oneway_buffering_admin.in ());
        }
      else if (run_buffer_size_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running buffer size flushing test\n"));
          test_failed =
            run_buffer_size (orb.in (),
                             oneway_buffering.in (),
                             oneway_buffering_admin.in ());
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: No test was configured\n"));
        }

      oneway_buffering->shutdown ();
      oneway_buffering_admin->shutdown ();
      ACE_OS::sleep(1);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Client: "));
      ex._tao_print_exception ("CORBA Exception caught:");
      return 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Client caught unknown exception\n"));
      return 1;
    }

  return test_failed;
}

int
configure_policies (CORBA::ORB_ptr orb,
                    const TAO::BufferingConstraint &buffering_constraint,
                    Test::Oneway_Buffering_ptr oneway_buffering,
                    Test::Oneway_Buffering_out flusher)
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("PolicyCurrent");

  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ());

  if (CORBA::is_nil (policy_current.in ()))
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
      return 1;
    }
  CORBA::Any scope_as_any;
  scope_as_any <<= Messaging::SYNC_NONE;

  CORBA::Any buffering_as_any;
  buffering_as_any <<= buffering_constraint;

  CORBA::PolicyList policies (2); policies.length (2);
  policies[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        scope_as_any);
  policies[1] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_as_any);

  policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

  policies[0]->destroy ();
  policies[1]->destroy ();

  TAO::BufferingConstraint flush_constraint;
  flush_constraint.mode = TAO::BUFFER_FLUSH;
  flush_constraint.message_count = 0;
  flush_constraint.message_bytes = 0;
  flush_constraint.timeout = 0;

  buffering_as_any <<= flush_constraint;
  policies.length (1);
  policies[0] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_as_any);

  object =
    oneway_buffering->_set_policy_overrides (policies,
                                             CORBA::ADD_OVERRIDE);

  policies[0]->destroy ();

  flusher =
    Test::Oneway_Buffering::_narrow (object.in ());

  return 0;
}

void
sync_server (Test::Oneway_Buffering_ptr flusher)
{
  // Get back in sync with the server...
  int transient_count= 0;
  while (true)
    {
      try
        {
          flusher->flush ();
          break;
        }
      catch (const CORBA::TRANSIENT &)
        {
          if (++transient_count < TRANSIENT_LIMIT)
            {
              if (transient_count == TRANSIENT_LIMIT / 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    "(%P) Client large TRANSIENTS encountered calling flush().\n"));
                }
              ACE_OS::sleep (TRANSIENT_HOLDOFF);
            }
          else
            {
              throw; // Abort the message sending.
            }
        }
    }
  while (true)
    {
      try
        {
          flusher->sync ();
          break;
        }
      catch (const CORBA::TRANSIENT &)
        {
          if (++transient_count < TRANSIENT_LIMIT)
            {
              if (transient_count == TRANSIENT_LIMIT / 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    "(%P) Client large TRANSIENTS encountered calling sync().\n"));
                }
              ACE_OS::sleep (TRANSIENT_HOLDOFF);
            }
          else
            {
              throw; // Abort the message sending.
            }
        }
    }
}

CORBA::ULong
request_count (
  const Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin,
  const CORBA::ULong expected_request_count= static_cast<CORBA::ULong>(0))
{
  CORBA::ULong count;
  int transient_count= 0;
  while (true)
    {
      try
        {
          count= oneway_buffering_admin->request_count (expected_request_count);
          break;
        }
      catch (const CORBA::TRANSIENT &)
        {
          if (++transient_count < TRANSIENT_LIMIT)
            {
              if (transient_count == TRANSIENT_LIMIT / 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    "(%P) Client large TRANSIENTS encountered calling request_count().\n"));
                }
              ACE_OS::sleep (TRANSIENT_HOLDOFF);
            }
          else
            {
              throw; // Abort the message sending.
            }
        }
    }

  return count;
}

void
receive_data (
  const Test::Oneway_Buffering_ptr oneway_buffering,
  const Test::Payload &payload)
{
  int transient_count= 0;
  while (true)
    {
      try
        {
          return oneway_buffering->receive_data (payload);
        }
      catch (const CORBA::TRANSIENT &)
        {
          if (++transient_count < TRANSIENT_LIMIT)
            {
              if (transient_count == TRANSIENT_LIMIT / 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    "(%P) Client large TRANSIENTS encountered calling receive_data().\n"));
                }
              ACE_OS::sleep (TRANSIENT_HOLDOFF);
            }
          else
            {
              throw; // Abort the message sending.
            }
        }
    }
}

CORBA::ULong
bytes_received_count (
  const Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin,
  const CORBA::ULong expected_bytes_received_count= static_cast<CORBA::ULong>(0))
{
  CORBA::ULong count;
  int transient_count= 0;
  while (true)
    {
      try
        {
          count= oneway_buffering_admin->bytes_received_count (expected_bytes_received_count);
          break;
        }
      catch (const CORBA::TRANSIENT &)
        {
          if (++transient_count < TRANSIENT_LIMIT)
            {
              if (transient_count == TRANSIENT_LIMIT / 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    "(%P) Client large TRANSIENTS encountered calling bytes_received_count().\n"));
                }
              ACE_OS::sleep (TRANSIENT_HOLDOFF);
            }
          else
            {
              throw; // Abort the message sending.
            }
        }
    }

  return count;
}

int
run_liveness_test (CORBA::ORB_ptr orb,
                   Test::Oneway_Buffering_ptr oneway_buffering,
                   Test::Oneway_Buffering_ptr flusher,
                   Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin)
{
  ACE_DEBUG ((LM_DEBUG, ".... checking for liveness\n"));
  int test_failed = 0;

  sync_server (flusher);

  CORBA::ULong send_count =
    request_count (oneway_buffering_admin);

  int liveness_test_iterations = static_cast<int> (send_count);

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  int depth = 0;
  for (int i = 0; i != liveness_test_iterations; ++i)
    {
      receive_data (oneway_buffering, payload);
      ++send_count;

      ACE_Time_Value tv (0, 1000);
      orb->run (tv);

      CORBA::ULong receive_count =
        request_count (oneway_buffering_admin);

      // Once the system has sent enough messages we don't
      // expect it to fall too far behind, i.e. at least 90% of the
      // messages should be delivered....
      CORBA::ULong expected =
        CORBA::ULong (LIVENESS_TOLERANCE * send_count);

      if (receive_count < expected)
        {
          test_failed = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "DEBUG: Iteration %d "
                      "not enough messages received %u "
                      "expected %u\n",
                      i, receive_count, expected));

          sync_server (flusher);
        }

      if (depth++ == LIVENESS_MAX_DEPTH)
        {
          sync_server (flusher);

          depth = 0;
        }

    }

  return test_failed;
}

int
run_message_count (CORBA::ORB_ptr orb,
                   Test::Oneway_Buffering_ptr oneway_buffering,
                   Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_MESSAGE_COUNT;
  buffering_constraint.message_count = BUFFERED_MESSAGES_COUNT;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = 0;

  Test::Oneway_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        oneway_buffering, flusher.out ());

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      sync_server (flusher.in ());

      CORBA::ULong initial_receive_count =
        request_count (oneway_buffering_admin, send_count);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "DEBUG: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      while (1)
        {
          receive_data (oneway_buffering, payload);
          ++send_count;

          ACE_Time_Value tv (0, 1000);
          orb->run (tv);

          CORBA::ULong iteration_count =
            send_count - initial_receive_count;

          const bool
            too_few= (iteration_count < static_cast<CORBA::ULong> (BUFFERED_MESSAGES_COUNT));

          CORBA::ULong receive_count =
            request_count (
              oneway_buffering_admin,
              static_cast<CORBA::ULong> (too_few ?
                                         initial_receive_count
                                         : (initial_receive_count
                                            + static_cast<CORBA::ULong> (BUFFERED_MESSAGES_COUNT))));

          if (receive_count != initial_receive_count)
            {
              if (too_few)
                {
                  test_failed = 1;
                  ACE_DEBUG ((LM_DEBUG,
                              "DEBUG: Iteration %d flush before "
                              "message count reached. "
                              "Iteration count = %u, Threshold = %u\n",
                              i,
                              iteration_count, BUFFERED_MESSAGES_COUNT));
                }
              break;
            }

          if (iteration_count > 3 * BUFFERED_MESSAGES_COUNT)
            {
              test_failed = 1;
              ACE_DEBUG ((LM_DEBUG,
                          "DEBUG: Iteration %d no flush past "
                          "message count threshold. "
                          "Iteration count = %u, Threshold = %u\n",
                          i,
                          iteration_count, BUFFERED_MESSAGES_COUNT));
              break;
            }
        }
    }

  int liveness_test_failed =
    run_liveness_test (orb,
                       oneway_buffering,
                       flusher.in (),
                       oneway_buffering_admin);

  if (liveness_test_failed)
    test_failed = 1;

  return test_failed;
}

int
run_timeout (CORBA::ORB_ptr orb,
              Test::Oneway_Buffering_ptr oneway_buffering,
              Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_TIMEOUT;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = TIMEOUT_MILLISECONDS * 10000;

  Test::Oneway_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        oneway_buffering, flusher.out ());

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      sync_server (flusher.in ());

      CORBA::ULong initial_receive_count =
        request_count (oneway_buffering_admin, send_count);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "DEBUG: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      ACE_Time_Value start = ACE_OS::gettimeofday ();
      while (1)
        {
          receive_data (oneway_buffering, payload);
          ++send_count;

          ACE_Time_Value tv (0, 1000);
          orb->run (tv);

          CORBA::ULong receive_count =
            request_count (oneway_buffering_admin);

          ACE_Time_Value elapsed = ACE_OS::gettimeofday () - start;
          if (receive_count != initial_receive_count)
            {
              if (elapsed.msec () < TIMEOUT_MILLISECONDS)
                {
                  test_failed = 1;
                  ACE_ERROR ((LM_ERROR,
                              "ERROR: Iteration %d flush before "
                              "timeout expired. "
                              "Elapsed = %d, Timeout = %d msecs\n",
                              i,
                              elapsed.msec (), TIMEOUT_MILLISECONDS));
                }
              // terminate the while loop.
              break;
            }

          if (elapsed.msec () > 3 * TIMEOUT_MILLISECONDS)
            {
              test_failed = 1;
              ACE_ERROR ((LM_ERROR,
                          "ERROR: Iteration %d no flush past "
                          "timeout threshold. "
                          "Elapsed = %d, Timeout = %d msecs\n",
                          i,
                          elapsed.msec (), TIMEOUT_MILLISECONDS));
              break;
            }
        }
    }

  int liveness_test_failed =
    run_liveness_test (orb, oneway_buffering,
                       flusher.in (),
                       oneway_buffering_admin);

  if (liveness_test_failed)
    test_failed = 1;


  return test_failed;
}

int
run_timeout_reactive (CORBA::ORB_ptr orb,
                      Test::Oneway_Buffering_ptr oneway_buffering,
                      Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_TIMEOUT;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = TIMEOUT_MILLISECONDS * 10000;

  Test::Oneway_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        oneway_buffering, flusher.out ());

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      sync_server (flusher.in ());

      CORBA::ULong initial_receive_count =
        request_count (oneway_buffering_admin, send_count);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "DEBUG: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      ACE_Time_Value start = ACE_OS::gettimeofday ();
      for (int j = 0; j != 20; ++j)
        {
          receive_data (oneway_buffering, payload);
          ++send_count;
        }

      ACE_Time_Value tv (0, 1000);
      orb->run (tv);

      while (1)
        {
          CORBA::ULong receive_count =
            request_count (oneway_buffering_admin);

          //FUZZ: disable check_for_lack_ACE_OS
          ACE_Time_Value sleep (0, 10000);
          orb->run (sleep);
          //FUZZ: enable check_for_lack_ACE_OS

          ACE_Time_Value elapsed = ACE_OS::gettimeofday () - start;
          if (receive_count != initial_receive_count)
            {
              if (elapsed.msec () < TIMEOUT_MILLISECONDS)
                {
                  test_failed = 1;
                  ACE_DEBUG ((LM_DEBUG,
                              "DEBUG: Iteration %d flush before "
                              "timeout expired. "
                              "Elapsed = %d, Timeout = %d msecs\n",
                              i,
                              elapsed.msec (), TIMEOUT_MILLISECONDS));
                }
              // terminate the while loop.
              break;
            }

          if (elapsed.msec () > 3 * TIMEOUT_MILLISECONDS)
            {
              test_failed = 1;
              ACE_DEBUG ((LM_DEBUG,
                          "DEBUG: Iteration %d no flush past "
                          "timeout threshold. "
                          "Elapsed = %d, Timeout = %d msecs\n",
                          i,
                          elapsed.msec (), TIMEOUT_MILLISECONDS));
              break;
            }
        }
    }

  int liveness_test_failed =
    run_liveness_test (orb, oneway_buffering,
                       flusher.in (),
                       oneway_buffering_admin);

  if (liveness_test_failed)
    test_failed = 1;


  return test_failed;
}

int
run_buffer_size (CORBA::ORB_ptr orb,
                 Test::Oneway_Buffering_ptr oneway_buffering,
                 Test::Oneway_Buffering_Admin_ptr oneway_buffering_admin)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_MESSAGE_BYTES;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = BUFFER_SIZE;
  buffering_constraint.timeout = 0;

  Test::Oneway_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        oneway_buffering, flusher.out ());

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  CORBA::ULong bytes_sent = 0;
  const CORBA::ULong
    expected_size= static_cast<CORBA::ULong>(GIOP_OVERHEAD * BUFFER_SIZE);

  for (int i = 0; i != iterations; ++i)
    {
      sync_server (flusher.in ());

      CORBA::ULong initial_bytes_received =
        bytes_received_count (oneway_buffering_admin, bytes_sent);

      if (initial_bytes_received != bytes_sent)
        {
          test_failed = 1;
          ACE_DEBUG ((LM_DEBUG,
                      "DEBUG: Iteration %d data lost (%u != %u)\n",
                      i, initial_bytes_received, bytes_sent));
        }

      while (1)
        {
          receive_data (oneway_buffering, payload);
          bytes_sent += PAYLOAD_LENGTH;

          ACE_Time_Value tv (0, 1000);
          orb->run (tv);

          CORBA::ULong payload_delta =
            bytes_sent - initial_bytes_received;

          CORBA::ULong bytes_received =
            bytes_received_count (oneway_buffering_admin);

          if (bytes_received != initial_bytes_received)
            {
              // The queue has been flushed, check that enough data
              // has been sent.  The check cannot be precise because
              // the ORB counts the GIOP message overhead, in this
              // test we assume the overhead to be less than 10%

              if (payload_delta < expected_size)
                {
                  test_failed = 1;
                  ACE_DEBUG ((LM_DEBUG,
                              "DEBUG: Iteration %d flush before "
                              "minimum buffer size was reached. "
                              "Sent = %u, Minimum buffer = %u bytes\n",
                              i,
                              payload_delta, BUFFER_SIZE));
                }
              break;
            }

          if (payload_delta > 3 * BUFFER_SIZE)
            {
              test_failed = 1;
              ACE_DEBUG ((LM_DEBUG,
                          "DEBUG: Iteration %d no flush past "
                          "buffer size threshold. "
                          "Sent = %u, Minimum buffer = %u bytes\n",
                          i,
                          payload_delta, BUFFER_SIZE));
              break;
            }
        }
    }

  int liveness_test_failed =
    run_liveness_test (orb, oneway_buffering,
                       flusher.in (),
                       oneway_buffering_admin);

  if (liveness_test_failed)
    test_failed = 1;

  return test_failed;
}
