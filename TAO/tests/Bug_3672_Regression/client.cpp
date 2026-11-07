
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
#include "tao/ORB_Core.h"

#include <atomic>
#include <string>

struct Client_State
{
  Client_State () = default;
  Client_State (Client_State const &) = delete;
  Client_State (Client_State &&) = delete;
  Client_State &operator= (Client_State const &) = delete;
  Client_State &operator= (Client_State &&) = delete;

  std::atomic<long> number_of_replies {};
  std::atomic<bool> parameter_corruption {};
  bool debug {true};
};

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            int &nthreads,
            int &niterations,
            bool &debug)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dk:n:i:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'd':
        debug = true;
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
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
  Client (A::AMI_Test_ptr const server,
          int const niterations,
          A::AMI_AMI_TestHandler_ptr const handler,
          Client_State &state);
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  /// The thread entry point.
  int svc () override;

  /// Set all members to nil
  void clear ();

private:
  /// Var for the AMI_Test object.
  A::AMI_Test_var ami_test_var_;

  /// The number of iterations on each client thread.
  int const niterations_;

  /// Var for AMI_AMI_Test_ReplyHandler object.
  A::AMI_AMI_TestHandler_var the_handler_var_;

  Client_State &state_;
};

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  explicit Handler (Client_State &state)
    : state_ (state)
  {
  }
  Handler (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler &operator= (Handler &&) = delete;

  void foo (CORBA::Long result,
            CORBA::Long out_l) override
    {
      if (result == 0)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Callback method detected parameter corruption.\n"));
          state_.parameter_corruption.store (true);
        }

      if (state_.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t) : Callback method called: result <%d>, out_arg <%d>\n",
                      result,
                      out_l));
        }

      --state_.number_of_replies;
    };

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
    };

  ~Handler () override = default;

private:
  Client_State &state_;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  int nthreads {1};
  int niterations {1};
  Client_State state;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior, nthreads, niterations, state.debug) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior.c_str ());
      A::AMI_Test_var server = A::AMI_Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior.c_str ()),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Let the client perform the test in a separate thread
      Handler *handler {};
      ACE_NEW_RETURN (handler,
                      Handler (state),
                      1);
      // ServantBase_var takes ownership of the allocated callback servant.
      PortableServer::ServantBase_var owner_transfer (handler);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (handler);

      CORBA::Object_var object2 = root_poa->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var hello = A::AMI_AMI_TestHandler::_narrow (object2.in ());
      object2 = CORBA::Object::_nil ();

      server->shutdown (); // oneway, so returns here immediately but server waits 5 sec

      Client client (server.in (), niterations, hello.in (), state);
      state.number_of_replies = nthreads * niterations;
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      if (state.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Entering perform_work loop to receive <%d> replies\n",
                      state.number_of_replies.load ()));
        }

      // ORB loop.
      ACE_Time_Value tv (1, 0);
      orb->run (tv);

      if (state.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Exited perform_work loop Received <%d> replies\n",
                      (nthreads * niterations) - state.number_of_replies.load ()));
        }

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      client.wait ();

      tv = ACE_Time_Value (1, 0);
      orb->run (tv);

      root_poa->deactivate_object (id.in ());
      root_poa->destroy (true,  // ethernalize objects
                         false);  // wait for completion

      hello = A::AMI_AMI_TestHandler::_nil ();
      root_poa = PortableServer::POA::_nil ();
      poa_object = CORBA::Object::_nil ();
      object = CORBA::Object::_nil ();
      server = A::AMI_Test::_nil ();
      poa_manager = PortableServer::POAManager::_nil ();
      client.clear ();

      orb->shutdown ();
      orb->destroy ();

      CORBA::ULong const ref_count = orb->_refcount ();

      if (ref_count > 1)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Refcount orb %d\n"), ref_count));
          state.parameter_corruption.store (true);
        }
      else
        {
          TAO_ORB_Core *core = orb->orb_core ();
          if (core != nullptr)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Core <> null\n")));
              state.parameter_corruption.store (true);
            }
        }
      orb = CORBA::ORB::_nil ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return state.parameter_corruption.load ();
}

// ****************************************************************

void
Client::clear ()
{
  ami_test_var_ = A::AMI_Test::_nil ();
  the_handler_var_ = A::AMI_AMI_TestHandler::_nil ();
}

Client::Client (A::AMI_Test_ptr const server,
                int const niterations,
                A::AMI_AMI_TestHandler_ptr const handler,
                Client_State &state)
  : ami_test_var_ (A::AMI_Test::_duplicate (server))
  , niterations_ (niterations)
  , the_handler_var_ (A::AMI_AMI_TestHandler::_duplicate (handler))
  , state_ (state)
{
}

int
Client::svc ()
{
  try
    {
      for (int i = 0; i < this->niterations_; ++i)
        {
          ami_test_var_->sendc_foo (the_handler_var_.in ());
        }
      if (this->state_.debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t):<%d> Asynchronous methods issued\n",
                      this->niterations_));
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
