// $Id$
#include "ServerApp.h"
#include "Foo_i.h"
#include "OrbTask.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"


ServerApp::ServerApp()
  : ior_filename_("ServerApp.default.ior"),
    num_clients_(1),
    num_orb_threads_(1)
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
  CORBA::PolicyList policies(0);
  policies.length(0);

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

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> csd_tp_strategy =
                                                 new TAO::CSD::TP_Strategy();

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa.in()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa.\n"));
      throw TestException();
    }

  // Create the servant object.
  Foo_i* servant = new Foo_i(this->num_clients_);

  // local smart pointer variable to deal with releasing the reference
  // to the servant object when the smart pointer object falls out of scope.
  PortableServer::ServantBase_var owner_transfer(servant);

  // Activate the servant using the Child POA.
  PortableServer::ObjectId_var oid
    = child_poa->activate_object(servant);

  // Obtain the object reference.
  obj = child_poa->servant_to_reference(servant);

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to activate servant (Foo_i).\n"));
      throw TestException();
    }

  // Stringify the object reference
  CORBA::String_var ior
    = orb->object_to_string(obj.in());

  // Write the stringified object reference to the ior file.
  FILE* ior_file = ACE_OS::fopen(this->ior_filename_.c_str(), "w");

  if (ior_file == 0)
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Cannot open output file for writing IOR: %s",
                 this->ior_filename_.c_str()));
      throw TestException();
    }

  ACE_OS::fprintf(ior_file, "%s", ior.in ());
  ACE_OS::fclose(ior_file);

  // Activate the POA Manager
  poa_manager->activate();

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.\n"));

  // If the num_orb_threads_ is exactly one, then just use the current
  // (mainline) thread to run the ORB event loop.
  if (this->num_orb_threads_ == 1)
    {
      // Since the num_orb_threads_ is exactly one, we just use the current
      // (mainline) thread to run the ORB event loop.
      orb->run();
    }
  else
    {
      // The num_orb_threads_ is greater than 1, so we will use an OrbTask
      // (active object) to run the ORB event loop in (num_orb_threads_ - 1)
      // threads.  We use the current (mainline) thread as the other thread
      // running the ORB event loop.
      OrbTask orb_task(orb.in(), this->num_orb_threads_ - 1);

      // Activate the OrbTask worker threads
      if (orb_task.open() != 0)
        {
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) Failed to open the OrbTask.\n"));
          throw TestException();
        }

      // This will use the current (mainline) thread to run the ORB event loop.
      orb->run();

      // Now that the current thread has unblocked from running the orb,
      // make sure to wait for all of the worker threads to complete.
      orb_task.wait();
    }

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is waiting for OrbShutdownTask.\n"));
  TheOrbShutdownTask::instance()->wait ();

  // Sleep for 2 second to let the done() two-way call complete
  // before cleanup.
  ACE_OS::sleep (2);

  // Tear-down the root poa and orb.
  root_poa->destroy(1, 1);
  orb->destroy();

  return 0;
}


int
ServerApp::parse_args(int argc, char* argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts(argc, argv, "o:n:t:");

  int c;
  int tmp;

  while ((c = get_opts()) != -1)
    {
      switch (c)
      {
        case 'o':
          this->ior_filename_ = get_opts.opt_arg();
          break;

        case 'n':
          tmp = ACE_OS::atoi(get_opts.opt_arg());
          if (tmp < 1)
            {
              this->usage_statement();
              return -1;
            }

          this->num_clients_ = tmp;
          break;

        case 't':
          tmp = ACE_OS::atoi(get_opts.opt_arg());
          if (tmp < 1)
            {
              this->usage_statement();
              return -1;
            }

          this->num_orb_threads_ = tmp;
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
             "\t[-t <num_orb_threads>]\n"
             "\t[-?]\n",
             "Default ior_filename_prefix is 'foo'.\n"
             "Default num_servants is 1.\n"
             "Default num_clients is 1.\n\n",
             this->exe_name_.c_str()));
}

