#include "ServerApp.h"
#include "Foo_i.h"
#include "OrbTask.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include <memory>


int
ServerApp::run (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

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

  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Failed to resolve initial ref for 'RootPOA'.\n"));
      throw TestException ();
    }

  PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (obj.in ());

  if (CORBA::is_nil (root_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Failed to narrow obj ref to POA interface.\n"));
      throw TestException ();
    }

  PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

  // Create the child POA.
  CORBA::PolicyList const policies;

  PortableServer::POA_var const child_poa = root_poa->create_POA (
    "ChildPoa",
    poa_manager.in (),
    policies);

  if (CORBA::is_nil (child_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                  "Failed to create the child POA.\n"));
      throw TestException ();
    }

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> const csd_tp_strategy (
    new TAO::CSD::TP_Strategy ());

  // Tell the strategy to apply itself to the child poa.
  if (!csd_tp_strategy->apply_to (child_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR [ServerApp::run()]: "
                  "Failed to apply custom dispatching strategy to child poa.\n"));
      throw TestException ();
    }

  // POA servant APIs use a raw pointer; owner_transfer keeps the servant alive.
  Foo_i *const servant = new Foo_i (this->options_.num_clients);

  PortableServer::ServantBase_var const owner_transfer (servant);

  // Activate the servant using the Child POA.
  PortableServer::ObjectId_var const oid = child_poa->activate_object (servant);

  // Obtain the object reference.
  obj = child_poa->servant_to_reference (servant);

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Failed to activate servant (Foo_i).\n"));
      throw TestException ();
    }

  // Stringify the object reference
  CORBA::String_var const ior = orb->object_to_string (obj.in ());

  // Write the stringified object reference to the ior file.
    {
    std::unique_ptr<FILE, int (*) (FILE *)> const ior_file (
      ACE_OS::fopen (this->options_.ior_filename.c_str (), "w"), ACE_OS::fclose);
    if (!ior_file)
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Cannot open output file for writing IOR: %s",
                    this->options_.ior_filename.c_str ()));
        throw TestException ();
      }

    ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
  }

  // Activate the POA Manager
  poa_manager->activate ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) ServerApp is ready.\n"));

  // If the configured ORB thread count is one, use the current
  // (mainline) thread to run the ORB event loop.
  if (this->options_.num_orb_threads == 1)
    {
      // The main thread runs the ORB event loop.
      orb->run ();
    }
  else
    {
      // OrbTask runs the ORB event loop in the remaining configured worker
      // threads while the main thread also runs the event loop.
      OrbTask orb_task (orb.in (), this->options_.num_orb_threads - 1);

      // Activate the OrbTask worker threads
      if (orb_task.open () != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Failed to open the OrbTask.\n"));
          throw TestException ();
        }

      // This will use the current (mainline) thread to run the ORB event loop.
      orb->run ();

      // Now that the current thread has unblocked from running the orb,
      // make sure to wait for all of the worker threads to complete.
      orb_task.wait ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) ServerApp is waiting for OrbShutdownTask.\n"));
  TheOrbShutdownTask::instance ()->wait ();

  // Sleep for 2 second to let the done() two-way call complete
  // before cleanup.
  ACE_OS::sleep (2);

  // Tear-down the root poa and orb.
  root_poa->destroy (1, 1);
  orb->destroy ();

  return 0;
}


int
ServerApp::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:n:t:"));

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          this->options_.ior_filename = get_opts.opt_arg ();
          break;

        case 'n':
          {
            int const num_clients = ACE_OS::atoi (get_opts.opt_arg ());
            if (num_clients < 1)
              {
                this->usage_statement ();
                return -1;
              }

            this->options_.num_clients = static_cast<unsigned> (num_clients);
          }
          break;

        case 't':
          {
            int const num_orb_threads = ACE_OS::atoi (get_opts.opt_arg ());
            if (num_orb_threads < 1)
              {
                this->usage_statement ();
                return -1;
              }

            this->options_.num_orb_threads = static_cast<unsigned> (num_orb_threads);
          }
          break;

        case '?':
          this->usage_statement ();
          return 1;

        default:
          this->usage_statement ();
          return -1;
        }
    }

  return 0;
}


void
ServerApp::usage_statement () const
{
  ACE_ERROR ((LM_ERROR,
              "Usage:  %s [options]\n\n"
              "OPTIONS:\n\n"
              "\t[-o <ior_filename>]\n"
              "\t[-n <num_clients>]\n"
              "\t[-t <num_orb_threads>]\n"
              "\t[-?]\n\n"
              "Default ior_filename is 'ServerApp.default.ior'.\n"
              "Default num_clients is 1.\n"
              "Default num_orb_threads is 1.\n\n",
              this->exe_name_.c_str ()));
}

