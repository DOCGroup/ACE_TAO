// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_ccmC.h"
#include "ami_ccmS.h"
#include <vector>

using namespace std;

ACE_RCSID (AMI,
           client,
           "$Id$")

class RH_Manager;

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char * in_str = "Let's talk AMI.";
const int nthreads_ = 5;
const int niterations_ = 10;
int number_of_replies_ = 0;
int global_client_id = 0;
TAO_SYNCH_MUTEX lock_;

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
  Client (A::AMI_CCM_ptr server, int niterations, RH_Manager* manager);

  virtual int svc (void);

private:
  A::AMI_CCM_var ami_test_var_;
  int         niterations_;
  RH_Manager* manager_;
};

/**
 * @class Handler
 *
 * @brief The AMI (callback) reply handler
 *
 */
class Handler : public POA_A::AMI_AMI_CCMHandler
{
public:
  Handler (void) :
        done_ (false),
        client_id_ (0)
  {
  };

  //callback implementation
  void asynch_foo (CORBA::Long result,
                   const char * answer)
    {
      --number_of_replies_;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):\n\nCallback method called:")
                  ACE_TEXT ("Result <%d>\nNumber of replies ")
                  ACE_TEXT ("to go <%d>\nout_arg <%s>\n\n\n"),
                  result,
                  number_of_replies_,
                  answer));
      this->client_id_ = result;
      this->done_ = true;
    }

  void asynch_foo_excep (::Messaging::ExceptionHolder * excep_holder)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Callback method <foo_excep> called:\n")));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Caught exception:");
        }
      done_ = true;
    }

  bool done ()
    {
      return done_;
    }
  ~Handler (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P | %t): destructor called for <%d>\n"),
                    this->client_id_));
  };
private:
  bool done_;
  int client_id_;
};

/**
 * @class RH_Manager
 *
 * @brief Manager of created handlers
 *
 */
class RH_Manager
{
private:
  struct HandlerID {
    int id;
    Handler* hnd;
  };

public:
  RH_Manager ();
  ~RH_Manager ();

  void add (Handler* handler, int client_id);
  void clean_up ();

private:
  vector<HandlerID> handlers_;
  TAO_SYNCH_MUTEX   lock_;
};


RH_Manager::RH_Manager ()
{
}

RH_Manager::~RH_Manager ()
{
  /*
  for (vector<HandlerID>::iterator iter = handlers_.begin();
        iter != handlers_.end(); ++iter)
    {
      delete iter->hnd;
    }
*/
}

void RH_Manager::add (Handler* handler, int client_id)
{
  HandlerID tmp;
  tmp.hnd = handler;
  tmp.id = client_id;
  ACE_Guard<TAO_SYNCH_MUTEX> guard (this->lock_);
  handlers_.push_back (tmp);
}

void RH_Manager::clean_up ()
{
  for (vector<HandlerID>::iterator iter = handlers_.begin();
        iter != handlers_.end(); ++iter)
    {
      if (iter->hnd->done ())
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P | %t): Cleaning up <%d>\n"), iter->id));
          delete iter->hnd;
          ACE_Guard<TAO_SYNCH_MUTEX> guard (this->lock_);
          this->handlers_.erase (iter);
          break;
        }
    }
}


/**
 * @class Main
 *
 * @brief Main function
 *
 */
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  RH_Manager* manager = new RH_Manager ();
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
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             ior),
                            1);
        }

      // Activate POA to handle the call back.

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // Let the client perform the test in a separate thread

      Client client (server.in (), niterations_, manager);
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads_) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      number_of_replies_ = nthreads_ * niterations_;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) : Entering perform_work loop to receive <%d> replies\n"),
                  number_of_replies_));

      // ORB loop.

      while (number_of_replies_ > 0)
        {
          CORBA::Boolean pending = orb->work_pending();

          if (pending)
            {
              orb->perform_work();
              manager->clean_up ();
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) : Exited perform_work loop Received <%d> replies\n"),
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
  delete manager;
  return 0;
}

/**
 *
 * Client task implementation
 *
 */
Client::Client (A::AMI_CCM_ptr server,
                int niterations,
                RH_Manager* manager)
                : ami_test_var_ (A::AMI_CCM::_duplicate (server)),
                  niterations_ (niterations),
                  manager_(manager)
{
}

int
Client::svc (void)
{
  try
    {
      for (int i = 0; i < this->niterations_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P | %t):Start <%d>\n"),
                    i));
          ACE_Guard<TAO_SYNCH_MUTEX> guard (lock_);
          Handler* handler = new Handler ();
          manager_->add (handler, ++global_client_id);
          A::AMI_AMI_CCMHandler_var my_handler_var_ = handler->_this ();
          ami_test_var_->sendc_asynch_foo (my_handler_var_.in (), in_str, global_client_id);
        }
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P | %t):<%d> Asynchronous methods issued\n"),
                    this->niterations_));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P | %t) Thread ended\n")));
  return 0;
}
