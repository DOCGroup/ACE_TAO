
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a client that uses buffered AMI calls.
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

#include <limits>
#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://ior")};
  CORBA::ULong iterations {20};
  CORBA::Long message_count {5};
  long interval {1000};
  bool shutdown_server {};
  bool invoke_ami_style {true};
  bool setup_buffering {true};
};

class Reply_Handler : public POA_AMI_testHandler
{
public:
  Reply_Handler (Reply_Handler const&) = delete;
  Reply_Handler (Reply_Handler&&) = delete;
  Reply_Handler& operator= (Reply_Handler const&) = delete;
  Reply_Handler& operator= (Reply_Handler&&) = delete;

  Reply_Handler (CORBA::ULong iterations, bool invoke_ami_style)
    : iterations_ (iterations),
      all_replies_received_ (!invoke_ami_style)
  {
  }

  bool all_replies_received () const
  {
    return this->all_replies_received_;
  }

  void method (CORBA::ULong reply_number) override
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("client: AMI Reply %u @ %T\n"),
                reply_number));

    // Last reply flips the flag.
    if (reply_number == this->iterations_)
      this->all_replies_received_ = true;
  }

  void method_excep (::Messaging::ExceptionHolder *holder) override
  {
    try
      {
        holder->raise_exception ();
      }
    catch (CORBA::SystemException const &ex)
      {
        ex._tao_print_exception ("Reply_Handler::method_excep: ");
      }
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override
  {
  }
  //FUZZ: enable check_for_lack_ACE_OS

  void shutdown_excep (::Messaging::ExceptionHolder *holder) override
  {
    try
      {
        holder->raise_exception ();
      }
    catch (CORBA::SystemException const &ex)
      {
        ex._tao_print_exception ("Reply_Handler::shutdown_excep: ");
      }
  }

private:
  CORBA::ULong const iterations_;
  bool all_replies_received_;
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:k:m:i:t:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'm':
        {
          int const message_count = ACE_OS::atoi (get_opts.opt_arg ());
          if (message_count <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Message count must be greater than zero.\n")),
                              -1);
          options.message_count = message_count;
        }
        break;

      case 'a':
        options.invoke_ami_style = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case 'b':
        options.setup_buffering = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case 'i':
        {
          int const iterations = ACE_OS::atoi (get_opts.opt_arg ());
          if (iterations <= 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Iterations must be greater than zero.\n")),
                              -1);
          options.iterations = static_cast<CORBA::ULong> (iterations);
        }
        break;

      case 't':
        {
          int const interval = ACE_OS::atoi (get_opts.opt_arg ());
          if (interval < 0 || interval > std::numeric_limits<long>::max () / 1000)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Interval must be nonnegative and fit in microseconds.\n")),
                              -1);
          options.interval = interval;
        }
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k IOR ")
                           ACE_TEXT ("-m message count ")
                           ACE_TEXT ("-a invoke AMI style [0/1] ")
                           ACE_TEXT ("-b setup buffering [0/1] ")
                           ACE_TEXT ("-i iterations ")
                           ACE_TEXT ("-t interval between calls ")
                           ACE_TEXT ("-x shutdown server\n"),
                           argv [0]),
                          -1);
      }

  if (options.ior.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Please specify the IOR for the servant\n")), -1);

  // Iterations must be a multiple of message count; otherwise we'll
  // have some unsent messages left in the buffered queue.  Even
  // though we can explicitly flush the queue, I am being lazy and
  // forcing the user to give the right numbers.
  if ((options.iterations % options.message_count) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("<iterations> must be a multiple of ")
                         ACE_TEXT ("<message_count>, or the program must ")
                         ACE_TEXT ("flush explicitly\n")),
                        -1);
    }

  // Indicates successful parsing of command line.
  return 0;
}

static void
setup_buffering_constraints (CORBA::ORB_ptr orb, CORBA::Long message_count)
{
  // Obtain PolicyCurrent.
  CORBA::Object_var const object = orb->resolve_initial_references ("PolicyCurrent");

  // Narrow down to correct type.
  CORBA::PolicyCurrent_var const policy_current =
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
  Messaging::SyncScope const sync_none = Messaging::SYNC_NONE;

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
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      Client_Options options;
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var base =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (base.in ());

      // Get an object reference from the argument string.
      base = orb->string_to_object (options.ior.c_str ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Try to narrow the object reference to a <test> reference.
      test_var const test_object = test::_narrow (base.in ());

      Reply_Handler reply_handler_servant (options.iterations,
                                           options.invoke_ami_style);
      AMI_testHandler_var const reply_handler_object = reply_handler_servant._this ();

      if (options.setup_buffering)
        {
          setup_buffering_constraints (orb.in (), options.message_count);
        }

      for (CORBA::ULong i = 1; i <= options.iterations; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("client: Iteration %u @ %T\n"),
                      i));

          if (options.invoke_ami_style)
            {
              // Invoke the AMI method.
              test_object->sendc_method (reply_handler_object.in (),
                                         i);
            }
          else
            {
              CORBA::ULong reply_number {};

              // Invoke the regular method.
              test_object->method (i,
                                   reply_number);

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("client: Regular Reply %u @ %T\n"),
                          reply_number));
            }

          // Interval between successive calls.
          ACE_Time_Value sleep_interval (0, options.interval * 1000);

          orb->run (sleep_interval);
        }

      // Loop until all replies have been received.
      while (!reply_handler_servant.all_replies_received ())
        {
          orb->perform_work ();
        }

      // Shutdown server.
      if (options.shutdown_server)
        {
          test_object->shutdown ();
        }

      root_poa->destroy (true, true);

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
