// $Id$

// ================================================================
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a client that uses buffered AMI calls.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "testS.h"
#include "tao/Messaging.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Buffered_AMI, client, "$Id$")

// Name of file contains ior.
static const char *IOR = "file://ior";

// Default iterations.
static CORBA::ULong iterations = 20;

// Default number of invocations to buffer before flushing.
static CORBA::Long message_count = iterations / 4;

// Time interval between invocation (in milli seconds).
static long interval = 1000;

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static int shutdown_server = 0;

// AMI call or regular call.
static int invoke_ami_style = 1;

// Setup buffering or not.
static int setup_buffering = 1;

// Flag indicates that all replies have been received
static int received_all_replies = 0;

class Reply_Handler : public POA_AMI_testHandler
{
public:
  void method (CORBA::ULong reply_number
               TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "client: AMI Reply %d @ %T\n",
                  reply_number));

      // Last reply flips the flag.
      if (reply_number == iterations)
        received_all_replies = 1;
    }

  void method_excep (AMI_testExceptionHolder *holder
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        holder->raise_method (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCH(CORBA::SystemException, ex)
      {
        ACE_PRINT_EXCEPTION (ex, "Reply_Handler::method_excep: ");
      }
    ACE_ENDTRY;
  }

  void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void shutdown_excep (AMI_testExceptionHolder *holder
                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        holder->raise_shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCH(CORBA::SystemException, ex)
      {
        ACE_PRINT_EXCEPTION (ex, "Reply_Handler::shutdown_excep: ");
      }
    ACE_ENDTRY;
  }
};

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "a:b:k:m:i:t:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'm':
        message_count = ::atoi (get_opts.opt_arg ());
        break;

      case 'a':
        invoke_ami_style = ::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        setup_buffering = ::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 't':
        interval = ::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-m message count "
                           "-a invoke AMI style [0/1] "
                           "-b setup buffering [0/1] "
                           "-i iterations "
                           "-t interval between calls "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Without AMI, replies are immediate.
  if (!invoke_ami_style)
    received_all_replies = 1;

  // Message count must be a multiple of iterations; otherwise we'll
  // have some unsent messages left in the buffered queue.  Even
  // though we can explicitly flush the queue, I am being lazy and
  // forcing the user to give the right numbers.
  if ((iterations % message_count) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "<message_count> must be a multiple <iterations> "
                         "or the program should be changed to flush explicitly \n"),
                        -1);
    }

  // Indicates successful parsing of command line.
  return 0;
}

void
setup_buffering_constraints (CORBA::ORB_ptr orb
                             TAO_ENV_ARG_DECL)
{
  // Obtain PolicyCurrent.
  CORBA::Object_var object = orb->resolve_initial_references ("PolicyCurrent"
                                                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Narrow down to correct type.
  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Start off with no constraints.
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_MESSAGE_COUNT;
  buffering_constraint.message_count = message_count;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = 0;

  // Setup the buffering constraint any.
  CORBA::Any buffering_constraint_any;
  buffering_constraint_any <<= buffering_constraint;

  // Setup the buffering constraint policy list.
  CORBA::PolicyList buffering_constraint_policy_list (1);
  buffering_constraint_policy_list.length (1);

  // Setup the buffering constraint policy.
  buffering_constraint_policy_list[0] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_constraint_any
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the constraints (at the ORB level).
  policy_current->set_policy_overrides (buffering_constraint_policy_list,
                                        CORBA::ADD_OVERRIDE
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // We are done with the policy.
  buffering_constraint_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the none sync scope policy, i.e., the ORB will buffer AMI
  // calls.
  Messaging::SyncScope sync_none = Messaging::SYNC_NONE;

  // Setup the none sync scope any.
  CORBA::Any sync_none_any;
  sync_none_any <<= sync_none;

  // Setup the none sync scope policy list.
  CORBA::PolicyList sync_none_policy_list (1);
  sync_none_policy_list.length (1);

  // Setup the none sync scope policy.
  sync_none_policy_list[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_none_any
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the none sync scope (at the ORB level).
  policy_current->set_policy_overrides (sync_none_policy_list,
                                        CORBA::ADD_OVERRIDE
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // We are now done with these policies.
  sync_none_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var base =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (base.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get an object reference from the argument string.
      base = orb->string_to_object (IOR
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to narrow the object reference to a <test> reference.
      test_var test_object = test::_narrow (base.in ()
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Reply_Handler reply_handler_servant;
      AMI_testHandler_var reply_handler_object = reply_handler_servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (setup_buffering)
        {
          setup_buffering_constraints (orb.in ()
                                       TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      for (CORBA::ULong i = 1; i <= iterations; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client: Iteration %d @ %T\n",
                      i));

          if (invoke_ami_style)
            {
              // Invoke the AMI method.
              test_object->sendc_method (reply_handler_object.in (),
                                         i
                                         TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              CORBA::ULong reply_number = 0;

              // Invoke the regular method.
              test_object->method (i,
                                   reply_number
                                   TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "client: Regular Reply %d @ %T\n",
                          reply_number));
            }

          // Interval between successive calls.
          ACE_Time_Value sleep_interval (0,
                                         interval * 1000);

          orb->run (sleep_interval TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Loop until all replies have been received.
      while (!received_all_replies)
        {
          orb->perform_work ();
        }

      // Shutdown server.
      if (shutdown_server)
        {
          test_object->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      root_poa->destroy (1,
                         1
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}
