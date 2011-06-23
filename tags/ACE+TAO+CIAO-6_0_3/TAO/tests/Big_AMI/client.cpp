
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  A very simple client which uses the AMI callback model.
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ami_testS.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int shutdown_flag = 0;
int debug = 1;
CORBA::ULong payload_size  = 128000;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("db:k:i:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = 1;
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        shutdown_flag = 1;
        break;
      case 'b':
        payload_size  = ACE_OS::atoi (get_opts.opt_arg ());
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
  /// Constructor.
  Handler (void) : reply_count_ (0) {};

  /// Destructor.
  ~Handler (void) {};

  /// Get the reply count
  long reply_count (void) const
  {
    return reply_count_.value ();
  }

  void foo (CORBA::Long ami_return_val,
            CORBA::Long out_l)
    {
      ++reply_count_;
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Callback method <foo> called: result <%d>, out_arg <%d>\n",
                      ami_return_val,
                      out_l));
        }
    };

  void foo_excep (::Messaging::ExceptionHolder * excep_holder)
    {
      ++reply_count_;

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"
                                  "Testing proper exception handling ...\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const CORBA::Exception&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };

private:
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, long> reply_count_;
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

      if (parse_args (argc, argv) != 0)
        return 1;

      // We reuse the object_var smart pointer!
      CORBA::Object_var ior_object = orb->string_to_object (ior);

      A::AMI_Test_var ami_test_var =
        A::AMI_Test::_narrow (ior_object.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler;
      PortableServer::ObjectId_var id =
        poa_var->activate_object (&handler);

      CORBA::Object_var object = poa_var->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var the_handler_var =
        A::AMI_AMI_TestHandler::_narrow (object.in ());

      CORBA::Long l = 931247;
      A::Payload payload (payload_size);
      payload.length (payload_size);

      for (CORBA::ULong j = 0; j != payload_size; ++j)
        {
          payload[j] = j % 256;
        }

      for (ssize_t ni = 0; ni < niterations; ni++)
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Sending asynch message: %d\n",
                          ni));
            }

          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.",
                                   payload);
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "<%d> Asynchronous methods issued\n",
                      niterations));
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }

      CORBA::Long number = ami_test_var->foo (l,
                                              l,
                                              "Let's talk SMI.",
                                              payload);

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Received the following number: %d\n",
                      number));
        }

      if (shutdown_flag)
        {
          ami_test_var->shutdown ();
        }

      poa_var->destroy (1,  // ethernalize objects
                        0  // wait for completion
                       );

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
