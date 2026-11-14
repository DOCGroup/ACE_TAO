
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  A client which uses the AMI callback model.
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_test_i.h"
#include <atomic>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  int thread_count {5};
  int iteration_count {5};
  bool debug {};
  bool shutdown {};
  bool perform_work {true};
};

struct Client_State
{
  std::atomic<int> number_of_replies {};
  std::atomic<bool> parameter_corruption {};
};

CORBA::Long constexpr in_number = 931232;
char constexpr in_str[] = "Let's talk AMI.";

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dk:n:i:xw"));
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
      case 'n':
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iteration_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown = true;
        break;
      case 'w':
        options.perform_work = false;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-d] [-k <ior>] [-n <threads>] "
                                     "[-i <iterations>] [-x] [-w]\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/**
 * @class Client
 *
 * @brief Run the client thread
 *
 * Use the ACE_Task_Base class to run the client threads.
 */
class Handler;

class Client : public ACE_Task_Base
{
public:
  /// ctor
  Client (A::AMI_Test_ptr server,
          int niterations,
          bool debug,
          Handler &handler);
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  /// The thread entry point.
  int svc () override;

private:
  /// Var for the AMI_Test object.
  A::AMI_Test_var ami_test_var_;

  /// The number of iterations on each client thread.
  int const niterations_;

  bool const debug_;

  /// Var for AMI_AMI_Test_ReplyHandler object.
  A::AMI_AMI_TestHandler_var const the_handler_var_;
};

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  Handler (Client_Options const &options, Client_State &state)
    : options_ (options)
    , state_ (state)
  {
  }

  ~Handler () override = default;
  Handler (Handler const &) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler &&) = delete;

  void foo (CORBA::Long result,
            CORBA::Long out_l) override
    {
      if (result == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("ERROR: Callback method detected parameter corruption.\n")));
          this->state_.parameter_corruption.store (true);
        }

      if (this->options_.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P | %t) : Callback method called: result <%d>, "
                                "out_arg <%d>\n"),
                      result,
                      out_l));
        }

      this->state_.number_of_replies.fetch_sub (1);
    }

  void foo_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <foo_excep> called:\n")));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Caught exception:");
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
      Client_Options options {};

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object (orb->string_to_object (options.ior));
      A::AMI_Test_var const server (A::AMI_Test::_narrow (object.in ()));

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var const poa_object (
        orb->resolve_initial_references ("RootPOA"));

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var const root_poa (
        PortableServer::POA::_narrow (poa_object.in ()));

      PortableServer::POAManager_var const poa_manager (root_poa->the_POAManager ());

      poa_manager->activate ();

      // Let the client perform the test in a separate thread

      Handler handler (options, state);
      Client client (server.in (), options.iteration_count, options.debug, handler);
      state.number_of_replies.store (options.thread_count * options.iteration_count);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      if (options.perform_work)
        {
          if (options.debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) : Entering perform_work loop to receive "
                                    "<%d> replies\n"),
                          state.number_of_replies.load ()));
            }

          // ORB loop.

          while (state.number_of_replies.load () > 0)
            {
              bool const pending (orb->work_pending ());

              if (pending)
                {
                  orb->perform_work();
                }
            }

          if (options.debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) : Exited perform_work loop\n")));
            }
        }

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("threads finished\n")));

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) : Received <%d> replies\n"),
                      (options.thread_count * options.iteration_count)
                        - state.number_of_replies.load ()));
        }

      if (options.shutdown)
        {
          server->shutdown ();
        }

      root_poa->destroy (true,  // ethernalize objects
                         false);  // wait for completion

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return state.parameter_corruption.load ();
}

// ****************************************************************

Client::Client (A::AMI_Test_ptr server,
                int niterations,
                bool debug,
                Handler &handler)
  : ami_test_var_ (A::AMI_Test::_duplicate (server))
  , niterations_ (niterations)
  , debug_ (debug)
  , the_handler_var_ (handler._this ())
{
}

int
Client::svc ()
{
  try
    {
      for (int i {}; i < this->niterations_; ++i)
        {
          this->ami_test_var_->sendc_foo (this->the_handler_var_.in (), in_number, in_str);
        }
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P | %t):<%d> Asynchronous methods issued\n"),
                      this->niterations_));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
