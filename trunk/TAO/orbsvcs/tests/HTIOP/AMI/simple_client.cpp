
//=============================================================================
/**
 *  @file    simple_client.cpp
 *
 *  $Id$
 *
 *  A very simple client which uses the AMI callback model.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "../test_config.h"

#include "ace/OS_NS_sys_socket.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ami_testC.h"
#include "ami_testS.h"



const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int shutdown_flag = 0;
int debug = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:i:x"));
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
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
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
  Handler (void) {};

  /// Destructor.
    ~Handler (void) {};

  void foo (CORBA::Long ami_return_val,
            CORBA::Long out_l)
    {
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

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"
                                  "Testing proper exception handling ...\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const A::DidTheRightThing&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... exception received successfully\n"));
                    }
      catch (const CORBA::Exception&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };


  void get_yadda (CORBA::Long result)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda> called: result <%d>\n",
                  result));
    };

  void get_yadda_excep (::Messaging::ExceptionHolder *)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda_excep> called:\n"));
    };

  void set_yadda (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda> called:\n"));
    };

  void set_yadda_excep (::Messaging::ExceptionHolder *)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda_excep> called:\n"));
    };

};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("HTIOP_AMI_simple_client"));


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
      object_var = orb->string_to_object (ior);

      A::AMI_Test_var ami_test_var =
        A::AMI_Test::_narrow (object_var.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      ACE_OS::socket_init ();

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler;
      A::AMI_AMI_TestHandler_var the_handler_var =
        handler._this ();

      // Try out sending asynchronous messages without a reply handler
      // registered. Things fail if we get an exception.

      ami_test_var->sendc_foo (A::AMI_AMI_TestHandler::_nil (),
                               0,
                               "");


      // Trigger the DidTheRightThing exception on the server side
      // by sending 0 to it.
      ACE_DEBUG ((LM_DEBUG,
                  "Sending asynch message\n"));

      ami_test_var->sendc_foo (the_handler_var.in (),
                               0,
                               "Let's talk AMI.");

      CORBA::Long l = 931247;

      for (ssize_t ni = 0; ni < niterations; ni++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Sending asynch message: %d\n",
                      ni));

          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.");
        }

      // Begin test of attributes
      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ami_test_var->sendc_set_yadda (the_handler_var.in (),
                                     4711);

      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      // End test of attributes

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

      //while (orb->work_pending())
      //  orb->perform_work ();


      CORBA::Long number = ami_test_var->foo (l,
                                              l,
                                              "Let's talk SMI.");

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Received the following number: %d\n",
                      number));
        }

      if (shutdown_flag)
        {
          ACE_DEBUG ((LM_DEBUG, "invoking shutdown\n"));
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

  ACE_END_TEST;
  return 0;
}
