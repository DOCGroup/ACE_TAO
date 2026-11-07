#include "ServerApp.h"
#include "OrbTask.h"
#include "FooServantList.h"
#include "ClientTask.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"

int
ServerApp::run (int argc, ACE_TCHAR* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  // Parse the command-line args for this application.
  // * Raises -1 if problems are encountered.
  // * Returns 1 if the usage statement was explicitly requested.
  // * Returns 0 otherwise.
  int const result = this->parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  TheOrbShutdownTask::instance ()->orb (orb.in ());

  CORBA::Object_var obj
    = orb->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil(obj.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to resolve initial ref for 'RootPOA'.\n"));
      throw TestException();
    }

  PortableServer::POA_var root_poa
    = PortableServer::POA::_narrow (obj.in ());

  if (CORBA::is_nil(root_poa.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Failed to narrow obj ref to POA interface.\n"));
      throw TestException();
    }

  PortableServer::POAManager_var poa_manager
    = root_poa->the_POAManager ();

  // Create the child POA.
  CORBA::PolicyList policies(1);
  policies.length(1);

  policies[0]
    = root_poa->create_id_assignment_policy(PortableServer::USER_ID);

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

  csd_tp_strategy->set_num_threads (this->options_.num_csd_threads);

  // Tell the strategy to apply itself to the child poa.
  if (csd_tp_strategy->apply_to(child_poa.in()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                 "Failed to apply custom dispatching strategy to child poa.\n"));
      throw TestException();
    }

  FooServantList servants (this->options_.ior_filename.c_str (),
                          this->options_.num_servants,
                          this->options_.num_clients,
                          this->options_.collocated_test,
                          this->options_.servant_to_deactivate,
                          orb.in());

  // Activate the POA Manager before start the ClientTask thread so that
  // we do not need coordinate the ClientTask and main thread for the
  // collocated test.
  poa_manager->activate();

  servants.create_and_activate (child_poa.in ());

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ServerApp is ready.\n"));

  // If num_orb_threads is exactly one, then just use the current
  // (mainline) thread to run the ORB event loop.
  if (this->options_.num_orb_threads == 1)
    {
      // Since num_orb_threads is exactly one, we just use the current
      // (mainline) thread to run the ORB event loop.
      orb->run();
    }
  else
    {
      // num_orb_threads is greater than 1, so we will use an OrbTask
      // (active object) to run the ORB event loop in (num_orb_threads - 1)
      // threads.  We use the current (mainline) thread as the other thread
      // running the ORB event loop.
      OrbTask orb_task (orb.in (), this->options_.num_orb_threads - 1);

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

  if (this->options_.collocated_test)
    {
      servants.collocated_client ()->wait ();
    }

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
ServerApp::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:s:c:t:l:d:n:"));

  int c;
  while ((c = get_opts()) != -1)
    {
      int parse_error = 0;

      switch (c)
      {
        case 'p':
          this->options_.ior_filename = get_opts.opt_arg();
          break;

        case 's':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 1)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -s must be followed by an integer "
                           "value greater than 0.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.num_servants = static_cast<unsigned> (value);
              }
          }
          break;

        case 'c':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 1)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -c must be followed by an integer "
                           "value greater than 0.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.num_clients = static_cast<unsigned> (value);
              }
          }
          break;

        case 't':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 1)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -t must be followed by an integer "
                           "value greater than 0.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.num_orb_threads = static_cast<unsigned> (value);
              }
          }
          break;

        case 'n':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 1)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -n must be followed by an integer "
                           "value greater than 0.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.num_csd_threads = static_cast<unsigned> (value);
              }
          }
          break;

        case 'l':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 0)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -l must be followed by an integer "
                           "value greater than -1.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.collocated_test = value != 0;
              }
          }
          break;

        case 'd':
          {
            int const value = ACE_OS::atoi (get_opts.opt_arg ());
            if (value < 0)
              {
                ACE_ERROR((LM_ERROR,
                           "(%P|%t) Error.  -d must be followed by an integer "
                           "value >= 0.\n"));
                parse_error = 1;
              }
            else
              {
                this->options_.servant_to_deactivate = value;
              }
          }
          break;

        case '?':
          this->usage_statement();
          return 1;

        default:
          this->usage_statement();
          return -1;
      }

      if (parse_error != 0)
        {
          this->usage_statement();
          return parse_error;
        }
    }

  // The deadlock will happen with the collocated callback test
  // when we have one working thread, so create at least one more
  // working thread would resolve the deadlock.
  if (this->options_.collocated_test && this->options_.num_csd_threads == 1)
    {
      ACE_ERROR((LM_ERROR,
                 "(%P|%t) Error. Collocated testing requires more than one "
                 "CSD worker thread.\n"));
      return -1;
    }

  return 0;
}

void
ServerApp::usage_statement () const
{
  ACE_ERROR((LM_ERROR,
             ACE_TEXT("(%P|%t) usage:  %s\n")
             ACE_TEXT("\t[-p <ior_filename_prefix>]\n")
             ACE_TEXT("\t[-s <num_servants>]\n")
             ACE_TEXT("\t[-c <num_clients>]\n")
             ACE_TEXT("\t[-n <num_csd_threads>]\n")
             ACE_TEXT("\t[-t <num_orb_threads>]\n")
             ACE_TEXT("\t[-l <collocation_test>]\n")
             ACE_TEXT("\t[-d <servant_to_deactivate>]\n")
             ACE_TEXT("Default ior_filename_prefix is 'foo'.\n")
             ACE_TEXT("Default num_servants is 1.\n")
             ACE_TEXT("Default num_clients is 1.\n")
             ACE_TEXT("Default num_orb_threads is 1.\n")
             ACE_TEXT("Default collocation_test flag is 0.\n")
             ACE_TEXT("Default servant_to_deactivate is -1 means not deactivate servant.\n")
             ACE_TEXT("   0  means deactivate all servant.\n")
             ACE_TEXT("   >0 means the index (servant_to_deactivate-1) of the servant in the servant list.\n"),
             this->exe_name_.c_str ()));
}
