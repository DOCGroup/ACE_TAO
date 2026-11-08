
//=============================================================================
/**
 *  @file    simple_client.cpp
 *
 *  A very simple client which uses the AMI callback model.
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_testS.h"
#include <atomic>
#include <cstring>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int iteration_count {5};
  bool shutdown {};
  bool debug {};
};

struct Client_State
{
  std::atomic<int> result {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dk:i:x"));
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
        options.iteration_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-d] [-k <ior>] [-i <iterations>] [-x]\n"),
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
  Handler (Client_Options const &options, Client_State &state)
    : options_ (options)
    , state_ (state)
  {
  }

  /// Destructor.
  ~Handler () override = default;
  Handler (Handler const &) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler &&) = delete;

  void foo (CORBA::Long ami_return_val,
            CORBA::Long out_l) override
    {
      if (this->options_.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Callback method <foo> called: result <%d>, out_arg <%d>\n"),
                      ami_return_val,
                      out_l));
        }
      if (out_l != 931233)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: out_l not 931233: %d\n"),
                      out_l));
          this->state_.result.store (1);
        }
      if (ami_return_val != 931234)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: ami_return_val not 931234: %d\n"),
                      ami_return_val));
          this->state_.result.store (1);
        }
    }

  void foo_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <foo_excep> called:\n")
                  ACE_TEXT ("Testing proper exception handling ...\n")));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (A::DidTheRightThing const &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("... exception received successfully\n")));
          if (ex.id != 42)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR: ex.id not 42: %d\n"),
                          ex.id));
              this->state_.result.store (1);
            }
          if (std::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR: ex.whatDidTheRightThing not ok: <%C>\n"),
                          ex.whatDidTheRightThing.in ()));
              this->state_.result.store (1);
            }
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("ERROR");
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("... caught the wrong exception -> ERROR\n")));
        }
    }

  void get_yadda (CORBA::Long result) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <get_yadda> called: result <%d>\n"),
                  result));
    }

  void get_yadda_excep (::Messaging::ExceptionHolder *) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <get_yadda_excep> called:\n")));
    }

  void set_yadda () override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <set_yadda> called:\n")));
    }

  void set_yadda_excep (::Messaging::ExceptionHolder *) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <set_yadda_excep> called:\n")));
    }

  void inout_arg_test (char const *) override
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Callback method <inout_arg_test> called:\n")));
  }

  void inout_arg_test_excep (::Messaging::ExceptionHolder *) override
  {
  }

private:
  Client_Options const &options_;
  Client_State &state_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_State state {};
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));

      CORBA::Object_var const poa_object (
        orb->resolve_initial_references ("RootPOA"));

      PortableServer::POA_var const poa_var (
        PortableServer::POA::_narrow (poa_object.in ()));

      PortableServer::POAManager_var const poa_manager_var (
        poa_var->the_POAManager ());

      poa_manager_var->activate ();

      Client_Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object (orb->string_to_object (options.ior));

      A::AMI_Test_var const ami_test_var (A::AMI_Test::_narrow (object.in ()));

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler (options, state);
      PortableServer::ObjectId_var const id (poa_var->activate_object (&handler));

      CORBA::Object_var const handler_object (poa_var->id_to_reference (id.in ()));

      A::AMI_AMI_TestHandler_var const the_handler_var (
        A::AMI_AMI_TestHandler::_narrow (handler_object.in ()));

      // Try out sending asynchronous messages without a reply handler
      // registered. Things fail if we get an exception.
      ami_test_var->sendc_foo (A::AMI_AMI_TestHandler::_nil (), 0, "");

      // Trigger the DidTheRightThing exception on the server side
      // by sending 0 to it.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sending asynch message\n")));

      ami_test_var->sendc_foo (the_handler_var.in (),
                               0,
                               "Let's talk AMI.");

      CORBA::Long l {931247};

      for (ssize_t ni {}; ni < options.iteration_count; ++ni)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sending asynch message: %d\n"),
                      ni));

          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.");
        }

      // Begin test of attributes
      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ami_test_var->sendc_set_yadda (the_handler_var.in (), 4711);

      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      // End test of attributes

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("<%d> Asynchronous methods issued\n"),
                      options.iteration_count));
        }

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Issuing a synchronous method to collect the AMI replies\n")));
        }

      CORBA::Long number (ami_test_var->foo (l, l, "Let's talk SMI."));

      for (ssize_t ni {}; ni < options.iteration_count; ++ni)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sending another asynch message: %d\n"),
                      ni));

          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.");
        }

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("<%d> more asynchronous methods issued\n"),
                      options.iteration_count));
        }

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Issuing a synchronous method to collect the AMI replies again\n")));
        }

      number = ami_test_var->foo (l, l, "Let's talk SMI.");

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Received the following number: %d\n"),
                      number));
        }

      if (options.shutdown)
        {
          ami_test_var->shutdown ();
        }

      poa_var->destroy (true,  // ethernalize objects
                        false); // wait for completion

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return state.result.load ();
}
