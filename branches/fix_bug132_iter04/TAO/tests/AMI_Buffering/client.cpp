// $Id$

#include "Reply_Handler.h"
#include "Client_Task.h"
#include "tao/Messaging.h"
#include "tao/TAOC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(AMI_Buffering, client, "$Id$")

const char *server_ior = "file://server.ior";
const char *admin_ior = "file://admin.ior";
int iterations = 200;

int run_message_count_test = 0;
int run_timeout_test = 0;
int run_timeout_reactive_test = 0;
int run_buffer_size_test = 0;

const int PAYLOAD_LENGTH = 1024;
const int BUFFERED_MESSAGES_COUNT = 50;
const int TIMEOUT_MILLISECONDS = 50;
const int BUFFER_SIZE = 64 * PAYLOAD_LENGTH;

/// Check that no more than 10% of the messages are not sent.
const double PROGRESS_TOLERANCE = 0.9;

/// Factor in GIOP overhead in the buffer size test
const double GIOP_OVERHEAD = 0.9;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:a:i:ctbr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        server_ior = get_opts.optarg;
        break;

      case 'a':
        admin_ior = get_opts.optarg;
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
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
                   Test::AMI_Buffering_ptr ami_buffering,
                   Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                   CORBA::Environment &ACE_TRY_ENV);
int
run_timeout (CORBA::ORB_ptr orb,
             Test::AMI_Buffering_ptr ami_buffering,
             Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
             CORBA::Environment &ACE_TRY_ENV);

int
run_timeout_reactive (CORBA::ORB_ptr orb,
                      Test::AMI_Buffering_ptr oneway_buffering,
                      Test::AMI_Buffering_Admin_ptr oneway_buffering_admin,
                      CORBA::Environment &ACE_TRY_ENV);

int
run_buffer_size (CORBA::ORB_ptr orb,
                 Test::AMI_Buffering_ptr ami_buffering,
                 Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                 CORBA::Environment &ACE_TRY_ENV);

int
main (int argc, char *argv[])
{
  int test_failed = 0;
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(server_ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::AMI_Buffering_var ami_buffering =
        Test::AMI_Buffering::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ami_buffering.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::AMI_Buffering reference <%s>\n",
                             server_ior),
                            1);
        }

      tmp =
        orb->string_to_object(admin_ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::AMI_Buffering_Admin_var ami_buffering_admin =
        Test::AMI_Buffering_Admin::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ami_buffering_admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::AMI_Buffering_Admin reference <%s>\n",
                             admin_ior),
                            1);
        }

      Client_Task client_task (orb.in ());
      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }

      if (run_message_count_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running message count flushing test\n"));
          test_failed =
            run_message_count (orb.in (),
                               ami_buffering.in (),
                               ami_buffering_admin.in (),
                               ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else if (run_timeout_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running timeout flushing test\n"));
          test_failed =
            run_timeout (orb.in (),
                         ami_buffering.in (),
                         ami_buffering_admin.in (),
                         ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else if (run_timeout_reactive_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running timeout (reactive) flushing test\n"));
          test_failed =
            run_timeout_reactive (orb.in (),
                                  ami_buffering.in (),
                                  ami_buffering_admin.in (),
                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else if (run_buffer_size_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Running buffer size flushing test\n"));
          test_failed =
            run_buffer_size (orb.in (),
                             ami_buffering.in (),
                             ami_buffering_admin.in (),
                             ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: No test was configured\n"));
        }

      client_task.thr_mgr ()->wait ();
      
      ami_buffering->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ami_buffering_admin->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return test_failed;
}

int
configure_policies (CORBA::ORB_ptr orb,
                    const TAO::BufferingConstraint &buffering_constraint,
                    Test::AMI_Buffering_ptr ami_buffering,
                    Test::AMI_Buffering_out flusher,
                    CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var object =
    orb->resolve_initial_references ("PolicyCurrent", ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

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
                        scope_as_any,
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  policies[1] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_as_any,
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  policies[0]->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  policies[1]->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  TAO::BufferingConstraint flush_constraint;
  flush_constraint.mode = TAO::BUFFER_FLUSH;
  flush_constraint.message_count = 0;
  flush_constraint.message_bytes = 0;
  flush_constraint.timeout = 0;

  buffering_as_any <<= flush_constraint;
  policies.length (1);
  policies[0] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_as_any,
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  object =
    ami_buffering->_set_policy_overrides (policies,
                                             CORBA::ADD_OVERRIDE,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  policies[0]->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  flusher =
    Test::AMI_Buffering::_narrow (object.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
run_progress_test (Test::AMI_AMI_BufferingHandler_ptr reply_handler,
                   Test::AMI_Buffering_ptr ami_buffering,
                   Test::AMI_Buffering_ptr flusher,
                   Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                   CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "Liveness test\n"));

  int test_failed = 0;

  // Get back in sync with the server...
  flusher->flush (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  flusher->sync (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::ULong send_count =
    ami_buffering_admin->request_count (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  int progress_test_iterations = int(send_count);

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  for (int i = 0; i != progress_test_iterations; ++i)
    {
      ami_buffering->sendc_receive_data (reply_handler,
                                         payload,
                                         ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      send_count++;

      CORBA::ULong receive_count =
        ami_buffering_admin->request_count (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      // Once the system has sent enough messages we don't
      // expect it to fall too far behind, i.e. at least 90% of the
      // messages should be delivered....
      CORBA::ULong expected =
        CORBA::ULong (PROGRESS_TOLERANCE * send_count);

      if (receive_count < expected)
        {
          test_failed = 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Iteration %d "
                      "not enough messages received %u "
                      "expected %u\n",
                      i, receive_count, expected));
        }
    }

  return test_failed;
}

int
run_message_count (CORBA::ORB_ptr orb,
                   Test::AMI_Buffering_ptr ami_buffering,
                   Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                   CORBA::Environment &ACE_TRY_ENV)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_MESSAGE_COUNT;
  buffering_constraint.message_count = BUFFERED_MESSAGES_COUNT;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = 0;

  Test::AMI_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        ami_buffering, flusher.out (),
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  Reply_Handler *reply_handler_impl;
  ACE_NEW_RETURN (reply_handler_impl,
                  Reply_Handler,
                  1);
  PortableServer::ServantBase_var owner_transfer(reply_handler_impl);

  Test::AMI_AMI_BufferingHandler_var reply_handler =
    reply_handler_impl->_this (ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      // Get back in sync with the server...
      flusher->flush (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      flusher->sync (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      CORBA::ULong initial_receive_count =
        ami_buffering_admin->request_count (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      while (1)
        {
          ami_buffering->sendc_receive_data (reply_handler.in (),
                                             payload,
                                             ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          send_count++;

          CORBA::ULong receive_count =
            ami_buffering_admin->request_count (ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          CORBA::ULong iteration_count =
            send_count - initial_receive_count;
          if (receive_count != initial_receive_count)
            {
              if (iteration_count < CORBA::ULong(BUFFERED_MESSAGES_COUNT))
                {
                  test_failed = 1;
                  ACE_ERROR ((LM_ERROR,
                              "ERROR: Iteration %d flush before "
                              "message count reached. "
                              "Iteration count = %u, Threshold = %u\n",
                              i,
                              iteration_count, BUFFERED_MESSAGES_COUNT));
                }
              break;
            }

          if (iteration_count > 2 * BUFFERED_MESSAGES_COUNT)
            {
              test_failed = 1;
              ACE_ERROR ((LM_ERROR,
                          "ERROR: Iteration %d no flush past "
                          "message count threshold. "
                          "Iteration count = %u, Threshold = %u\n",
                          i,
                          iteration_count, BUFFERED_MESSAGES_COUNT));
              break;
            }
        }
    }

  int progress_test_failed =
    run_progress_test (reply_handler.in (),
                       ami_buffering,
                       flusher.in (),
                       ami_buffering_admin,
                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (progress_test_failed)
    test_failed = 1;

  return test_failed;
}

int
run_timeout (CORBA::ORB_ptr orb,
              Test::AMI_Buffering_ptr ami_buffering,
              Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
              CORBA::Environment &ACE_TRY_ENV)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_TIMEOUT;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = TIMEOUT_MILLISECONDS * 10000;

  Test::AMI_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        ami_buffering, flusher.out (),
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  Reply_Handler *reply_handler_impl;
  ACE_NEW_RETURN (reply_handler_impl,
                  Reply_Handler,
                  1);
  PortableServer::ServantBase_var owner_transfer(reply_handler_impl);

  Test::AMI_AMI_BufferingHandler_var reply_handler =
    reply_handler_impl->_this (ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      // Get back in sync with the server...
      flusher->flush (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      flusher->sync (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      CORBA::ULong initial_receive_count =
        ami_buffering_admin->request_count (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      ACE_Time_Value start = ACE_OS::gettimeofday ();
      while (1)
        {
          ami_buffering->sendc_receive_data (reply_handler.in (),
                                             payload,
                                             ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          send_count++;

          CORBA::ULong receive_count =
            ami_buffering_admin->request_count (ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

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

          if (elapsed.msec () > 2 * TIMEOUT_MILLISECONDS)
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

  int progress_test_failed =
    run_progress_test (reply_handler.in (),
                       ami_buffering,
                       flusher.in (),
                       ami_buffering_admin,
                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (progress_test_failed)
    test_failed = 1;

  return test_failed;
}

int
run_timeout_reactive (CORBA::ORB_ptr orb,
                      Test::AMI_Buffering_ptr ami_buffering,
                      Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                      CORBA::Environment &ACE_TRY_ENV)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_TIMEOUT;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = TIMEOUT_MILLISECONDS * 10000;

  Test::AMI_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        ami_buffering, flusher.out (),
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);
  for (int j = 0; j != PAYLOAD_LENGTH; ++j)
    payload[j] = CORBA::Octet(j % 256);

  Reply_Handler *reply_handler_impl;
  ACE_NEW_RETURN (reply_handler_impl,
                  Reply_Handler,
                  1);
  PortableServer::ServantBase_var owner_transfer(reply_handler_impl);

  Test::AMI_AMI_BufferingHandler_var reply_handler =
    reply_handler_impl->_this (ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::ULong send_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      // Get back in sync with the server...
      flusher->flush (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      flusher->sync (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      CORBA::ULong initial_receive_count =
        ami_buffering_admin->request_count (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (initial_receive_count != send_count)
        {
          test_failed = 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Iteration %d message lost (%u != %u)\n",
                      i, initial_receive_count, send_count));
        }

      ACE_Time_Value start = ACE_OS::gettimeofday ();
      for (int j = 0; j != 20; ++j)
        {
          ami_buffering->sendc_receive_data (reply_handler.in (),
                                             payload,
                                             ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          send_count++;
        }
      while (1)
        {
          CORBA::ULong receive_count =
            ami_buffering_admin->request_count (ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          ACE_Time_Value sleep (0, 10000);
          orb->run (sleep, ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

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

          if (elapsed.msec () > 2 * TIMEOUT_MILLISECONDS)
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

  int progress_test_failed =
    run_progress_test (reply_handler.in (),
                       ami_buffering,
                       flusher.in (),
                       ami_buffering_admin,
                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (progress_test_failed)
    test_failed = 1;


  return test_failed;
}

int
run_buffer_size (CORBA::ORB_ptr orb,
                 Test::AMI_Buffering_ptr ami_buffering,
                 Test::AMI_Buffering_Admin_ptr ami_buffering_admin,
                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_MESSAGE_BYTES;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = BUFFER_SIZE;
  buffering_constraint.timeout = 0;

  Test::AMI_Buffering_var flusher;
  int test_failed =
    configure_policies (orb, buffering_constraint,
                        ami_buffering, flusher.out (),
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (test_failed != 0)
    return test_failed;

  Test::Payload payload (PAYLOAD_LENGTH);
  payload.length (PAYLOAD_LENGTH);

  Reply_Handler *reply_handler_impl;
  ACE_NEW_RETURN (reply_handler_impl,
                  Reply_Handler,
                  1);
  PortableServer::ServantBase_var owner_transfer(reply_handler_impl);

  Test::AMI_AMI_BufferingHandler_var reply_handler =
    reply_handler_impl->_this (ACE_TRY_ENV);
  ACE_TRY_CHECK;

  CORBA::ULong bytes_sent = 0;
  for (int i = 0; i != iterations; ++i)
    {
      // Get back in sync with the server...
      flusher->flush (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      flusher->sync (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      CORBA::ULong initial_bytes_received =
        ami_buffering_admin->bytes_received_count (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (initial_bytes_received != bytes_sent)
        {
          test_failed = 1;
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Iteration %d data lost (%u != %u)\n",
                      i, initial_bytes_received, bytes_sent));
        }

      while (1)
        {
          ami_buffering->sendc_receive_data (reply_handler.in (),
                                             payload,
                                             ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
          bytes_sent += PAYLOAD_LENGTH;

          CORBA::ULong bytes_received =
            ami_buffering_admin->bytes_received_count (ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          CORBA::ULong payload_delta =
            bytes_sent - initial_bytes_received;
          if (bytes_received != initial_bytes_received)
            {
              // The queue has been flushed, check that enough data
              // has been sent.  The check cannot be precise because
              // the ORB counts the GIOP message overhead, in this
              // test we assume the overhead to be less than 10%

              if (payload_delta < CORBA::ULong (GIOP_OVERHEAD * BUFFER_SIZE))
                {
                  test_failed = 1;
                  ACE_ERROR ((LM_ERROR,
                              "ERROR: Iteration %d flush before "
                              "minimum buffer size was reached. "
                              "Sent = %u, Minimum buffer = %u bytes\n",
                              i,
                              payload_delta, BUFFER_SIZE));
                }
              break;
            }

          if (payload_delta > 2 * BUFFER_SIZE)
            {
              test_failed = 1;
              ACE_ERROR ((LM_ERROR,
                          "ERROR: Iteration %d no flush past "
                          "buffer size threshold. "
                          "Sent = %u, Minimum buffer = %u bytes\n",
                          i,
                          payload_delta, BUFFER_SIZE));
              break;
            }
        }
    }

  int progress_test_failed =
    run_progress_test (reply_handler.in (),
                       ami_buffering,
                       flusher.in (),
                       ami_buffering_admin,
                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (progress_test_failed)
    test_failed = 1;

  return test_failed;
}
