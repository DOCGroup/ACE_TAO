//=============================================================================
/**
 *  @file    client.cpp
 *
 *  A very simple client which uses the AMI callback model.
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ami_testS.h"

#include <atomic>

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int iterations {5};
  bool shutdown {};
  bool debug {true};
  CORBA::ULong payload_size {128000};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("db:k:i:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        options.debug = true;
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown = true;
        break;
      case 'b':
        options.payload_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-b <payload size> "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  explicit Handler (bool debug)
    : debug_ (debug)
  {
  }

  /// Destructor.
  ~Handler () override = default;
  Handler (Handler const &) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler &&) = delete;

  /// Get the reply count
  long reply_count () const
  {
    return reply_count_.load ();
  }

  void foo (CORBA::Long ami_return_val,
            CORBA::Long out_l) override
    {
      ++reply_count_;
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Callback method <foo> called: result <%d>, out_arg <%d>\n",
                      ami_return_val,
                      out_l));
        }
    };

  void foo_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ++reply_count_;

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"
                                  "Testing proper exception handling ...\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (CORBA::Exception const &)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };

private:
  bool const debug_;
  std::atomic<long> reply_count_ {};
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa_var =
        PortableServer::POA::_narrow (object_var.in ());

      PortableServer::POAManager_var poa_manager_var =
        poa_var->the_POAManager ();

      poa_manager_var->activate ();

      Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // We reuse the object_var smart pointer!
      CORBA::Object_var const ior_object = orb->string_to_object (options.ior);

      A::AMI_Test_var const ami_test_var =
        A::AMI_Test::_narrow (ior_object.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler (options.debug);
      PortableServer::ObjectId_var id =
        poa_var->activate_object (&handler);

      CORBA::Object_var object = poa_var->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var the_handler_var =
        A::AMI_AMI_TestHandler::_narrow (object.in ());

      CORBA::Long l {931247};
      A::Payload payload (options.payload_size);
      payload.length (options.payload_size);

      for (CORBA::ULong j = 0; j != options.payload_size; ++j)
        {
          payload[j] = j % 256;
        }

      for (ssize_t ni = 0; ni < options.iterations; ni++)
        {
          if (options.debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Sending asynch message: %b\n",
                          ni));
            }

          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.",
                                   payload);
        }

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "<%d> Asynchronous methods issued\n",
                      options.iterations));
        }

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }

      CORBA::Long const number = ami_test_var->foo (l,
                                                    l,
                                                    "Let's talk SMI.",
                                                    payload);

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Received the following number: %d\n",
                      number));
        }

      if (options.shutdown)
        {
          ami_test_var->shutdown ();
        }

      poa_var->destroy (true,  // ethernalize objects
                        false);  // wait for completion

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
