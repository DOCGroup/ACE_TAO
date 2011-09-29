
//=============================================================================
/**
 *  @file     client.cpp
 *
 *  $Id$
 *
 *   This is a client that uses buffered AMI calls.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// Name of file contains ior.
static const ACE_TCHAR *IOR = ACE_TEXT ("file://ior");

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
  void method (CORBA::ULong reply_number)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "client: AMI Reply %d @ %T\n",
                  reply_number));

      // Last reply flips the flag.
      if (reply_number == iterations)
        received_all_replies = 1;
    }

  void method_excep (::Messaging::ExceptionHolder *holder)
  {
    try
      {
        holder->raise_exception ();
      }
    catch (const CORBA::SystemException& ex)
      {
        ex._tao_print_exception ("Reply_Handler::method_excep: ");
      }
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void)
  {
  }
  //FUZZ: enable check_for_lack_ACE_OS

  void shutdown_excep (::Messaging::ExceptionHolder *holder)
  {
    try
      {
        holder->raise_exception ();
      }
    catch (const CORBA::SystemException& ex)
      {
        ex._tao_print_exception ("Reply_Handler::shutdown_excep: ");
      }
  }
};

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:k:m:i:t:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'm':
        message_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'a':
        invoke_ami_style = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        setup_buffering = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        interval = ACE_OS::atoi (get_opts.opt_arg ());
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
                         "or the program should be changed to flush explicitly\n"),
                        -1);
    }

  // Indicates successful parsing of command line.
  return 0;
}

void
setup_buffering_constraints (CORBA::ORB_ptr orb)
{
  // Obtain PolicyCurrent.
  CORBA::Object_var object = orb->resolve_initial_references ("PolicyCurrent");

  // Narrow down to correct type.
  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ());

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
                        buffering_constraint_any);

  // Setup the constraints (at the ORB level).
  policy_current->set_policy_overrides (buffering_constraint_policy_list,
                                        CORBA::ADD_OVERRIDE);

  // We are done with the policy.
  buffering_constraint_policy_list[0]->destroy ();

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
                        sync_none_any);

  // Setup the none sync scope (at the ORB level).
  policy_current->set_policy_overrides (sync_none_policy_list,
                                        CORBA::ADD_OVERRIDE);

  // We are now done with these policies.
  sync_none_policy_list[0]->destroy ();
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
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var base =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (base.in ());

      // Get an object reference from the argument string.
      base = orb->string_to_object (IOR);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Try to narrow the object reference to a <test> reference.
      test_var test_object = test::_narrow (base.in ());

      Reply_Handler reply_handler_servant;
      AMI_testHandler_var reply_handler_object = reply_handler_servant._this ();

      if (setup_buffering)
        {
          setup_buffering_constraints (orb.in ());
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
                                         i);
            }
          else
            {
              CORBA::ULong reply_number = 0;

              // Invoke the regular method.
              test_object->method (i,
                                   reply_number);

              ACE_DEBUG ((LM_DEBUG,
                          "client: Regular Reply %d @ %T\n",
                          reply_number));
            }

          // Interval between successive calls.
          ACE_Time_Value sleep_interval (0,
                                         interval * 1000);

          orb->run (sleep_interval);
        }

      // Loop until all replies have been received.
      while (!received_all_replies)
        {
          orb->perform_work ();
        }

      // Shutdown server.
      if (shutdown_server)
        {
          test_object->shutdown ();
        }

      root_poa->destroy (1,
                         1);

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }


  return 0;
}
