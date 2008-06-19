// $Id$
#include "ServerApp.h"
#include "Foo_i.h"
#include "FooS_T.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
: ior_filename_ ("server.ior")
{
}


ServerApp::~ServerApp()
{
}


int
ServerApp::run (int argc, char* argv[])
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
      throw TestException();;
    }

  PortableServer::POA_var root_poa
    = PortableServer::POA::_narrow(obj.in());

  if (CORBA::is_nil(root_poa.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to POA interface.\n"));
      throw TestException();;
    }

  PortableServer::POAManager_var poa_manager
    = root_poa->the_POAManager();

  // Create the child POA.
  CORBA::PolicyList policies(1);
  policies.length(1);

  policies[0] =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  PortableServer::POA_var child_poa
    = root_poa->create_POA("ChildPoa",
                           poa_manager.in(),
                           policies);

  if (CORBA::is_nil(child_poa.in()))
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to create the child POA.\n"));
      throw TestException();;
    }

  policies[0]->destroy ();

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> csd_tp_strategy =
                                                 new TAO::CSD::TP_Strategy();

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa.in()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa.\n"));
      throw TestException();;
    }

  // Create the Foo_i object.
  Foo_i foo_i (this->num_clients_);

  // Create tie object with the Foo_i object.
  POA_Foo_tie<Foo_i> foo_tie_i (foo_i, child_poa.in ());

  // Get Object Reference for the foo_tie_i object.
  Foo_var foo = foo_tie_i._this ();

  if (CORBA::is_nil(foo.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to activate servant foo_tie_i.\n"));
      throw TestException();;
    }

  // Stringify the object reference
  CORBA::String_var ior
    = orb->object_to_string(foo.in());

  // Write the stringified object reference to the ior file.
  FILE* ior_file = ACE_OS::fopen(this->ior_filename_.c_str(), "w");

  if (ior_file == 0)
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Cannot open output file for writing IOR: %s",
                 this->ior_filename_.c_str()));
      throw TestException();;
    }

  ACE_OS::fprintf(ior_file, "%s", ior.in ());
  ACE_OS::fclose(ior_file);

  // Activate the POA Manager
  poa_manager->activate();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.  Running the ORB event loop.\n"));

  // Run the ORB event loop.
  orb->run();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp ORB has stopped running. "
             "Stop the CSD strategy.\n"));

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is waiting for OrbShutdownTask.\n"));
  TheOrbShutdownTask::instance()->wait ();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is destroying the Root POA.\n"));

  // Sleep for 2 second to let the done() two-way call complete
  // before cleanup.
  ACE_OS::sleep (2);

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
ServerApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "o:n:");

  int c;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'o':
          this->ior_filename_ = get_opts.opt_arg();
          break;

        case 'n':
          {
            int tmp = ACE_OS::atoi(get_opts.opt_arg());
            if (tmp < 1)
              {
                this->usage_statement ();
                return -1;
              }

            this->num_clients_ = tmp;
          }
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
             "\t[-o <ior_filename>]\n"
             "\t[-n <num_clients>]\n"
             "\t[-?]\n\n",
             this->exe_name_.c_str()));
}

