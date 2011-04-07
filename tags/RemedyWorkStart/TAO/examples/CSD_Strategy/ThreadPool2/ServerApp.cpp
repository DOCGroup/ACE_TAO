// $Id$
#include "ServerApp.h"
#include "FooServantList.h"
#include "FooC.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
  : ior_filename_(ACE_TEXT("foo")),
    num_servants_(1),
    num_clients_(1)
{
}


ServerApp::~ServerApp()
{
}


int
ServerApp::run (int argc, ACE_TCHAR* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int result = this->parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  TheOrbShutdownTask::instance()->orb (orb.in ());

  CORBA::Object_var obj
    = orb->resolve_initial_references("RootPOA");

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

  PortableServer::POA_var child_poa
    = root_poa->create_POA("ChildPoa",
                           poa_manager.in(),
                           policies);

  if (CORBA::is_nil(child_poa.in()))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to create the child POA.\n"));
      throw TestException();
    }

  policies[0]->destroy ();

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> csd_tp_strategy =
                                                 new TAO::CSD::TP_Strategy();

  csd_tp_strategy->set_num_threads(this->num_servants_);

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa.in()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa.\n"));
      throw TestException();
    }

  FooServantList servants(this->ior_filename_.c_str(),
                          this->num_servants_,
                          this->num_clients_,
                          orb.in());

  servants.create_and_activate(child_poa.in());

  // Activate the POA Manager
  poa_manager->activate();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.  Running the ORB event loop.\n"));

  // Run the ORB event loop.
  orb->run();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp ORB has stopped running. "
             "Stop the CSD strategy.\n"));

  // Sleep for 2 second to let the done() two-way call complete
  // before cleanup.
  ACE_OS::sleep (2);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is waiting for OrbShutdownTask.\n"));
  TheOrbShutdownTask::instance()->wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the Root POA.\n"));

  // Tear-down the root poa and orb.
  root_poa->destroy(1, 1);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the ORB.\n"));

  orb->destroy();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp has completed running successfully.\n"));

  return 0;
}


int
ServerApp::parse_args(int argc, ACE_TCHAR* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("p:s:c:"));

  int c;
  int tmp;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'p':
          this->ior_filename_ = get_opts.opt_arg();
          break;

        case 's':
          tmp = ACE_OS::atoi(get_opts.opt_arg());
          if (tmp < 1)
            {
              this->usage_statement();
              return -1;
            }

          this->num_servants_ = tmp;
          break;

        case 'c':
          tmp = ACE_OS::atoi(get_opts.opt_arg());
          if (tmp < 1)
            {
              this->usage_statement();
              return -1;
            }

          this->num_clients_ = tmp;
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
ServerApp::usage_statement()
{
  ACE_ERROR((LM_ERROR,
             "Usage:  %s [options]\n\n"
             "OPTIONS:\n\n"
             "\t[-p <ior_filename_prefix>]\n"
             "\t[-s <num_servants>]\n"
             "\t[-c <num_clients>]\n"
             "\t[-?]\n",
             "Default ior_filename_prefix is 'foo'.\n"
             "Default num_servants is 1.\n"
             "Default num_clients is 1.\n\n",
             this->exe_name_.c_str()));
}

