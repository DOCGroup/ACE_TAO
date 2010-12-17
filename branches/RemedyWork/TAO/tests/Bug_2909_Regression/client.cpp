
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 * A client which uses the AMI callback model and tests whether exceptions
 * are redirected to the reply handler when client and servant are
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"
#include "ami_test_i.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int nthreads = 1;
int niterations = 2;
int debug = 0;
ACE_Atomic_Op<TAO_SYNCH_MUTEX, int> number_of_replies = 0;

int invalid_exception = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:n:i:"));
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
  Client (A::AMI_Test_ptr server, int niterations);

  /// The thread entry point.
  virtual int svc (void);

  // private:
  /// Var for the AMI_Test object.
  A::AMI_Test_var ami_test_var_;

  /// The number of iterations on each client thread.
  int niterations_;

  /// Var for AMI_AMI_Test_ReplyHandler object.
  A::AMI_AMI_TestHandler_var the_handler_var_;
};

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  Handler (void) : request_ (0)
  {
  };

  void foo (void)
    {
      int const reply = --number_of_replies;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t) : Callback method %d called\n",
                      (nthreads * niterations) - reply));
        }
      ACE_UNUSED_ARG (reply);
      ++request_;
    };

   void foo_excep (::Messaging::ExceptionHolder * excep_holder)
    {
      int const reply = --number_of_replies;
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t) : Callback excep method %d called\n",
                      (nthreads * niterations) - reply));
        }
      ACE_UNUSED_ARG (reply);
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const A::DidTheRightThing& ex)
        {
          if (request_ != 0)
            {
              ++invalid_exception;
              ex._tao_print_exception ("Caught exception:");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
            }
        }
      catch (const CORBA::UNKNOWN& ex)
        {
          if (request_ != 1)
            {
              ++invalid_exception;
              ex._tao_print_exception ("Caught exception:");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
            }
        }
      ++request_;
    };

  ~Handler (void)
  {
  };
private:
  int request_;
};

// ReplyHandler.
Handler handler;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      A::AMI_Test_var server;

      AMI_Test_i * servant =
        new AMI_Test_i(orb.in());
      PortableServer::ServantBase_var safe (servant);

      server = servant->_this();

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      number_of_replies = nthreads * niterations;

      // Let the client perform the test in a separate thread

      Client client (server.in (), niterations);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Entering perform_work loop to receive <%d> replies\n",
                      number_of_replies.value ()));
        }

      // ORB loop.

      while (number_of_replies > 0)
        {
          CORBA::Boolean pending = orb->work_pending();

          if (pending)
            {
              orb->perform_work();
            }

          // On some systems this loop must yield or else the other threads
          // will not get a chance to run.
          ACE_OS::thr_yield();
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Exited perform_work loop Received <%d> replies\n",
                      (nthreads*niterations) - number_of_replies.value ()));
        }

      client.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                        );

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return invalid_exception;
}

// ****************************************************************

Client::Client (A::AMI_Test_ptr server,
                int niterations)
                :  ami_test_var_ (A::AMI_Test::_duplicate (server)),
     niterations_ (niterations)
{
  the_handler_var_ = handler._this (/* */);
}

int
Client::svc (void)
{
  try
    {
      for (int i = 0; i < this->niterations_; ++i)
        {
          ami_test_var_->sendc_foo (the_handler_var_.in (), i);
        }
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t):<%d> Asynchronous methods issued\n",
                      niterations));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
      invalid_exception = 1;
    }
  return 0;
}
