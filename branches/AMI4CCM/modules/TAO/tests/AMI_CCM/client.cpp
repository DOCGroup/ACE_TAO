// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_ccmC.h"
#include "ami_ccmS.h"

ACE_RCSID (AMI,
           client,
           "$Id$")

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char * in_str = "Let's talk AMI.";
const int nthreads_ = 2;
const int niterations_ = 10;
int number_of_replies_ = 0;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
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
  Client (A::AMI_CCM_ptr server, int niterations);

  /// The thread entry point.
  virtual int svc (void);

  // private:
  /// Var for the AMI_CCM object.
  A::AMI_CCM_var ami_test_var_;

  /// The number of iterations on each client thread.
  int niterations_;

  /// Var for AMI_AMI_CCM_ReplyHandler object.
  A::AMI_AMI_CCMHandler_var the_handler_var_;
};


//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++ REPLY HANDLER+++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++

class Handler : public POA_A::AMI_AMI_CCMHandler
{
public:
  Handler (void)
  {
  };

  //callback implementation
  void asynch_foo (const char * answer)
    {
      --number_of_replies_;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P | %t) : Callback method called: Replies to go <%d>, out_arg <%s>\n",
                  number_of_replies_,
                  answer));
    };

  void asynch_foo_excep (::Messaging::ExceptionHolder * excep_holder)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Caught exception:");
        }
    };
  ~Handler (void)
  {
  };
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

      A::AMI_CCM_var server;

      CORBA::Object_var object =
        orb->string_to_object (ior);
      server =  A::AMI_CCM::_narrow (object.in ());

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

      // Let the client perform the test in a separate thread

      Client client (server.in (), niterations_);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads_) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      number_of_replies_ = nthreads_ *niterations_;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) : Entering perform_work loop to receive <%d> replies\n",
                  number_of_replies_));

      // ORB loop.

      while (number_of_replies_ > 0)
        {
          CORBA::Boolean pending = orb->work_pending();

          if (pending)
            {
              orb->perform_work();
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) : Exited perform_work loop Received <%d> replies\n",
                  (nthreads_*niterations_) - number_of_replies_));

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

  return 0;
}

// ****************************************************************

Client::Client (A::AMI_CCM_ptr server,
                int niterations)
                :  ami_test_var_ (A::AMI_CCM::_duplicate (server)),
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
          ACE_DEBUG ((LM_DEBUG,
                    "(%P | %t):Start <%d>\n",
                    i));
          ami_test_var_->sendc_asynch_foo (the_handler_var_.in (), in_str);
        }
        ACE_DEBUG ((LM_DEBUG,
                    "(%P | %t):<%d> Asynchronous methods issued\n",
                    this->niterations_));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
