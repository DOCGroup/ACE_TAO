
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  A client which uses the AMI callback model.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ami_test_i.h"
#include "tao/ORB_Core.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int nthreads = 1;
int niterations = 1;
int debug = 1;
int number_of_replies = 0;

CORBA::Long in_number = 931232;
const char * in_str = "Let's talk AMI.";
int parameter_corruption = 0;

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
  Client (A::AMI_Test_ptr server, int niterations, A::AMI_AMI_TestHandler_ptr handler);

  /// The thread entry point.
  virtual int svc (void);

  /// Set all members to nil
  void clear ();

private:
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
  Handler (void)
  {
  };

  void foo (CORBA::Long result,
            CORBA::Long out_l)
    {
      if (result == 0)
        {
          ACE_ERROR((LM_ERROR, "ERROR: Callback method detected parameter corruption.\n"));
          parameter_corruption = 1;
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P | %t) : Callback method called: result <%d>, out_arg <%d>\n",
                      result,
                      out_l));
        }

      --number_of_replies;
    };

   void foo_excep (::Messaging::ExceptionHolder * excep_holder)
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);
      A::AMI_Test_var server =  A::AMI_Test::_narrow (object.in ());

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
      Handler* handler = 0;
      ACE_NEW_RETURN (handler,
                      Handler,
                      1);
      PortableServer::ServantBase_var owner_transfer(handler);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (handler);

      CORBA::Object_var object2 = root_poa->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var hello = A::AMI_AMI_TestHandler::_narrow (object2.in ());
      object2 = CORBA::Object::_nil ();

      server->shutdown (); // oneway, so returns here immediately but server waits 5 sec

      Client client (server.in (), niterations, hello.in ());
      if (client.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      // Main thread collects replies. It needs to collect
      // <nthreads*niterations> replies.
      number_of_replies = nthreads *niterations;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Entering perform_work loop to receive <%d> replies\n",
                      number_of_replies));
        }

      // ORB loop.
      ACE_Time_Value tv (1,0);
      orb->run (tv);

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) : Exited perform_work loop Received <%d> replies\n",
                      (nthreads*niterations) - number_of_replies));
        }

      ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      client.wait ();

      tv = ACE_Time_Value (1,0);
      orb->run (tv);

      root_poa->deactivate_object (id.in ());
      root_poa->destroy (1,  // ethernalize objects
                         0);  // wait for completion

      hello = A::AMI_AMI_TestHandler::_nil ();
      root_poa = PortableServer::POA::_nil ();
      poa_object = CORBA::Object::_nil ();
      object = CORBA::Object::_nil ();
      server = A::AMI_Test::_nil ();
      poa_manager = PortableServer::POAManager::_nil ();
      client.clear ();

      orb->shutdown ();
      orb->destroy ();

      CORBA::ULong ref_count  = orb->_refcount();

      if (ref_count > 1)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Refcount orb %d\n"), ref_count));
          ++parameter_corruption;
        }
      else
        {
          TAO_ORB_Core* core = orb->orb_core ();
          if (core != 0)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Core <> null\n")));
              ++parameter_corruption;
            }
        }
      orb = CORBA::ORB::_nil ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;

    }

  return parameter_corruption;
}

// ****************************************************************

void  Client::clear ()
{
  ami_test_var_ = A::AMI_Test::_nil ();
  the_handler_var_ = A::AMI_AMI_TestHandler::_nil ();
}

Client::Client (A::AMI_Test_ptr server,
                int niterations,
                A::AMI_AMI_TestHandler_ptr handler)
                :  ami_test_var_ (A::AMI_Test::_duplicate (server)),
     niterations_ (niterations),
     the_handler_var_ (A::AMI_AMI_TestHandler::_duplicate (handler))
{
}

int
Client::svc (void)
{
  try
    {
      for (int i = 0; i < this->niterations_; ++i)
        {
          ami_test_var_->sendc_foo (the_handler_var_.in ());
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
    }
  return 0;
}
