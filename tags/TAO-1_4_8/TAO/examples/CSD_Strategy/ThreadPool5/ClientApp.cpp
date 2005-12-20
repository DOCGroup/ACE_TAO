// $Id$
#include "ClientApp.h"
#include "Callback_i.h"
#include "ClientTask.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ClientApp::ClientApp()
{
}


ClientApp::~ClientApp()
{
}


int
ClientApp::run(int argc, char* argv[] ACE_ENV_ARG_DECL)
{
  CORBA::ORB_var orb 
    = CORBA::ORB_init(argc, argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);
  if (result != 0)
    {
      return result;
    }

  CORBA::Object_var obj 
    = orb->string_to_object(this->ior_.c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to convert IOR string to obj ref.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  Foo_var foo = Foo::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(foo.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to Foo interface.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  // Create the callback object using the child poa with the custom
  // strategy.
  obj = orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to resolve initial ref for 'RootPOA'.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  PortableServer::POA_var root_poa 
    = PortableServer::POA::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(root_poa.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to POA interface.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  PortableServer::POAManager_var poa_manager 
    = root_poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create the child POA.
  CORBA::PolicyList policies(0);
  policies.length(0);

  PortableServer::POA_var child_poa 
    = root_poa->create_POA("ChildPoa",
                           poa_manager.in(),
                           policies 
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(child_poa.in()))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to create the child POA.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> csd_tp_strategy =
                                                 new TAO::CSD::TP_Strategy();

  csd_tp_strategy->set_num_threads(1);

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa.in() ACE_ENV_ARG_PARAMETER) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa.\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }
  ACE_CHECK_RETURN (-1);

  // Create the servant object.
  Callback_i* servant = new Callback_i ();

  // local smart pointer variable to deal with releasing the reference
  // to the servant object when the smart pointer object falls out of scope.
  PortableServer::ServantBase_var owner_transfer(servant);

  // Activate the servant using the Child POA.
  PortableServer::ObjectId_var oid 
    = child_poa->activate_object(servant 
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Obtain the object reference.
  obj = child_poa->servant_to_reference(servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to activate servant (Callback_i).\n"));
      ACE_THROW_RETURN (TestException(), -1);
    }

  Callback_var callback = Callback::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ClientTask client_task(orb.in (), foo.in (), callback.in ());

  if (client_task.open () != 0)
    {
      ACE_THROW_RETURN (TestException(), -1);
    }

    // Activate the POA Manager
  poa_manager->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientApp is ready.\n"));

  orb->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  client_task.wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientApp is destroying the Root POA.\n"));

  // Tear-down the root poa and orb.
  root_poa->destroy(1, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientApp is destroying the ORB.\n"));

  orb->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientApp has completed running successfully.\n"));

  return 0;
}


int
ClientApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "i:");

  int c;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'i':
          this->ior_ = get_opts.opt_arg();
          break;

        case '?':
          this->usage_statement();
          return 1;
        default:
          this->usage_statement();
          return -1;
      }
    }

  return 0;
}


void
ClientApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-i <ior>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}

