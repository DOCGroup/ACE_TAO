
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  A client which uses the AMI callback model.
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ami_test_i.h"

#include <atomic>
#include <string>
  struct Client_Options
  {
    std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
    int thread_count {5};
    int iterations {10};
    bool debug {};
  };

  struct Client_State
  {
    std::atomic<long> replies {};
    std::atomic<bool> parameter_corruption {};
  };

  constexpr CORBA::Long in_number = 931232;
  char const in_str[] = "Let's talk AMI.";

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dk:n:i:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d "
                           "-k <ior> "
                           "-n <nthreads> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb, Client_State &state);
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;
  // ctor

  int svc () override;
  // The thread entry point.

private:
  CORBA::ORB_var const orb_;
  Client_State &state_;
};


/**
 * @class Client
 *
 * @brief Run the client thread
 *
 * Use the ACE_Task_Base class to run the client threads.
 */
class Client : public ACE_Task_Base
{
public:
  /// ctor
  Client (A::AMI_Test_ptr server,
          int iterations,
          A::AMI_AMI_TestHandler_ptr handler,
          bool debug);
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;
  /// dtor
  ~Client () override = default;

  /// The thread entry point.
  int svc () override;

private:
  /// Var for the AMI_Test object.
  A::AMI_Test_var const ami_test_var_;

  /// The number of iterations on each client thread.
  int const iterations_;

  /// Var for AMI_AMI_Test_ReplyHandler object.
  A::AMI_AMI_TestHandler_var const handler_var_;
  bool const debug_;
};

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  Handler (Client_State &state, bool debug)
    : state_ (state),
      debug_ (debug)
  {
  }
  Handler (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler &operator= (Handler &&) = delete;

  void set_ami_test (A::AMI_Test_ptr ami_test)
  {
    ami_test_var_ = A::AMI_Test::_duplicate (ami_test);
  }

  void foo (CORBA::Long result,
            CORBA::Long out_l) override
    {
      if (result == 0)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Callback method detected parameter corruption.\n"));
          state_.parameter_corruption.store (true);
        }

      if (debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t) : Callback method called: result <%d>, out_arg <%d>\n",
                      result,
                      out_l));
        }
      ACE_OS::sleep (1);
      ami_test_var_->sendc_set_yadda (0, 5);

      --state_.replies;
    }

   void foo_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"));
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
                  "Callback method <get_yadda> called: result <%d>\n",
                  result));
    }

  void get_yadda_excep (::Messaging::ExceptionHolder *) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda_excep> called:\n"));
    }

  void set_yadda () override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda> called:\n"));
    }

  void set_yadda_excep (::Messaging::ExceptionHolder *) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda_excep> called:\n"));
    }
  ~Handler () override = default;

  void inout_arg_test (char const *) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "Callback method <set_yadda_excep> called:\n"));
  }

  void inout_arg_test_excep (::Messaging::ExceptionHolder *) override
  {
  }

private:
  Client_State &state_;
  A::AMI_Test_var ami_test_var_;
  bool const debug_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_State state;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Client_Options options;

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());
      A::AMI_Test_var const server = A::AMI_Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             options.ior.c_str ()),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      poa_manager->activate ();

      // Let the client perform the test in a separate thread
      state.replies.store (options.thread_count * options.iterations);
      Handler handler (state, options.debug);
      PortableServer::ObjectId_var const id = root_poa->activate_object (&handler);

      CORBA::Object_var const hnd_object = root_poa->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var const the_handler_var =
        A::AMI_AMI_TestHandler::_narrow (hnd_object.in ());

      handler.set_ami_test (server.in ());

      Client client (server.in (), options.iterations, the_handler_var.in (), options.debug);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE, options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Entering perform_work loop to receive <%d> replies\n",
                      static_cast<int> (state.replies.load ())));
        }

      // ORB loop.

      Worker worker (orb.in (), state);
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE, options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      worker.thr_mgr ()->wait ();

      if (options.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Exited perform_work loop Received <%d> replies\n",
                      static_cast<int> ((options.thread_count * options.iterations)
                                        - state.replies.load ())));
        }

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      server->shutdown ();

      root_poa->destroy (true, false);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return state.parameter_corruption.load () ? 1 : 0;
}

// ****************************************************************

Client::Client (A::AMI_Test_ptr server,
                int iterations,
                A::AMI_AMI_TestHandler_ptr handler,
                bool debug)
  : ami_test_var_ (A::AMI_Test::_duplicate (server))
  , iterations_ (iterations)
  , handler_var_ (A::AMI_AMI_TestHandler::_duplicate (handler))
  , debug_ (debug)
{
}

int
Client::svc ()
{
  try
    {
      for (int iteration {}; iteration < this->iterations_; ++iteration)
        {
          this->ami_test_var_->sendc_foo (this->handler_var_.in (), in_number, in_str);
        }
      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) <%d> Asynchronous methods issued\n",
                      this->iterations_));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}

Worker::Worker (CORBA::ORB_ptr orb, Client_State &state)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    state_ (state)
{
}

int
Worker::svc ()
{
  while (this->state_.replies.load () > 0)
    {
      CORBA::Boolean const pending = this->orb_->work_pending ();

      if (pending)
        {
          ACE_Time_Value tm (1, 0);
          this->orb_->perform_work (tm);
        }
    }

  return 0;
}
