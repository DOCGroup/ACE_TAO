// $Id$
#include "ServerApp.h"
#include "Foo_i.h"
#include "Callback_i.h"
#include "ClientTask.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
: synch_with_server_ (0)
{
}


ServerApp::~ServerApp()
{
}


int
ServerApp::run(int argc, char* argv[])
{
  this->orb_ = CORBA::ORB_init(argc, argv, "");

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args(argc, argv);
  if (result != 0)
    {
      return result;
    }

  TheOrbShutdownTask::instance()->orb (this->orb_.in ());

  if (synch_with_server_)
  {
    CORBA::Object_var manager_object =
      orb_->resolve_initial_references("ORBPolicyManager");

    CORBA::PolicyManager_var policy_manager
      = CORBA::PolicyManager::_narrow(manager_object.in());

    if (CORBA::is_nil (policy_manager.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Panic: nil PolicyManager\n"));
      throw TestException();
    }

    CORBA::Any policy_value;
    policy_value <<= Messaging::SYNC_WITH_SERVER;
    CORBA::PolicyList policies(1);
    policies.length(1);

    policies[0] =
      orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                          policy_value);

    policy_manager->set_policy_overrides (policies,
                                          CORBA::ADD_OVERRIDE);

    policies[0]->destroy ();
  }

  CORBA::Object_var obj
    = orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to resolve initial ref for 'RootPOA'.\n"));
      throw TestException();
    }

  PortableServer::POA_var root_poa
    = PortableServer::POA::_narrow(obj.in());

  if (CORBA::is_nil(root_poa.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to POA interface.\n"));
      throw TestException();
    }

  PortableServer::POAManager_var poa_manager
    = root_poa->the_POAManager();

  // Create the child POA.
  CORBA::PolicyList policies(1);
  policies.length(1);

  policies[0] = root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  PortableServer::POA_var child_poa_1 = root_poa->create_POA("ChildPoa_1",
                                                           poa_manager.in(),
                                                           policies);

  if (CORBA::is_nil(child_poa_1.in()))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to create the ChildPoa_1.\n"));
      throw TestException();
    }

  PortableServer::POA_var child_poa_2 = root_poa->create_POA("ChildPoa_2",
                                                           poa_manager.in(),
                                                           policies);

  if (CORBA::is_nil(child_poa_2.in()))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to create the ChildPoa_2.\n"));
      throw TestException();
    }

  policies[0]->destroy ();

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> csd_tp_strategy =
                                                 new TAO::CSD::TP_Strategy();

  // We need create multiple working threads otherwise it would deadlock
  // with the callback test.
  csd_tp_strategy->set_num_threads(2);

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa_1.in()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa 1.\n"));
      throw TestException();
    }

  Foo_var foo1 = this->create_foo(child_poa_1.in(),
                                  "foo_applied_strategy");

  Foo_var foo2 = this->create_foo(child_poa_2.in(),
                                  "foo_not_applied_strategy");

  Callback_var callback1
    = this->create_callback(child_poa_1.in(),
                            "callback_applied_strategy");

  Callback_var callback2
    = this->create_callback(child_poa_2.in(),
                            "callback_not_applied_strategy");

  // Activate the POA Manager
  poa_manager->activate();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.  Running the ORB event loop.\n"));

  // Wait for the custom stretegy launch the dispatching threads.
  ACE_OS::sleep (2);

  ClientTask client1 (foo1.in (), callback1.in ());
  if (client1.open() != 0)
    {
      ACE_ERROR((LM_ERROR,
        "(%P|%t) Failed to open the collocated client1.\n"));
      throw TestException();
    }

  ClientTask client2 (foo2.in (), callback2.in ());
  if (client2.open() != 0)
    {
      ACE_ERROR((LM_ERROR,
        "(%P|%t) Failed to open the collocated client2.\n"));
      throw TestException();
    }

  // Run the ORB event loop.
  orb_->run();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp ORB has stopped running.\n"));

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) Wait for the OrbShutdownTask.\n"));

  TheOrbShutdownTask::instance()->wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) Wait for the collocated client task.\n"));

  client1.wait ();
  client2.wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) Stop the CSD strategy.\n"));

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the Root POA.\n"));

  // Tear-down the root poa and orb_.
  root_poa->destroy(1, 1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the ORB.\n"));

  orb_->destroy();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp has completed running successfully.\n"));

  return 0;
}


int
ServerApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "s");

  int c;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 's':
          this->synch_with_server_ = 1;
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


Foo_ptr
ServerApp::create_foo (
  PortableServer::POA_ptr poa,
  const char* servant_name)
{
  PortableServer::ServantBase_var servant
    = new Foo_i(servant_name);

  PortableServer::ObjectId_var id =
                PortableServer::string_to_ObjectId(servant_name);

  poa->activate_object_with_id(id.in(), servant.in());

  CORBA::Object_var obj = poa->id_to_reference(id.in());

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                  "(%P|%t) Failed to activate servant (%s).\n",
                  servant_name));
      throw TestException();
    }

  Foo_var foo = Foo::_narrow (obj.in ());

  return foo._retn ();
}

Callback_ptr
ServerApp::create_callback(
  PortableServer::POA_ptr poa,
  const char* servant_name)
{
  PortableServer::ServantBase_var servant
    = new Callback_i();

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId("callback");

  poa->activate_object_with_id(id.in(), servant.in());

  CORBA::Object_var obj = poa->id_to_reference(id.in());

  if (CORBA::is_nil(obj.in()))
  {
    ACE_ERROR((LM_ERROR,
      "(%P|%t) Failed to activate servant (%s).\n",
      servant_name));
    throw TestException();
  }

  Callback_var callback = Callback::_narrow (obj.in ());

  return callback._retn ();
}

void
ServerApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-s <synch_with_server>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}

