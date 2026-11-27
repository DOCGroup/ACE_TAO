#include "ServerApp.h"
#include "Foo_i.h"
#include "FooS_T.h"
#include "OrbShutdownTask.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include <memory>
// To force static load the service.
#include "tao/PI/PI.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"

int
ServerApp::run (int argc, ACE_TCHAR* argv[])
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

  CORBA::Object_var const obj
    = orb->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to resolve initial ref for 'RootPOA'.\n")));
      throw TestException ();
    }

  PortableServer::POA_var const root_poa
    = PortableServer::POA::_narrow (obj.in ());

  if (CORBA::is_nil (root_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to narrow obj ref to POA interface.\n")));
      throw TestException ();
    }

  PortableServer::POAManager_var const poa_manager
    = root_poa->the_POAManager ();

  // Create the child POA.
  CORBA::PolicyList policies (1);
  policies.length (1);

  policies[0] =
    root_poa->create_implicit_activation_policy (
      PortableServer::IMPLICIT_ACTIVATION);

  PortableServer::POA_var const child_poa
    = root_poa->create_POA ("ChildPoa", poa_manager.in (), policies);

  if (CORBA::is_nil (child_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) ERROR [ServerApp::run()]: ")
                  ACE_TEXT ("Failed to create the child POA.\n")));
      throw TestException ();
    }

  policies[0]->destroy ();

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
  TAO_Intrusive_Ref_Count_Handle<TAO::CSD::TP_Strategy> const csd_tp_strategy =
    new TAO::CSD::TP_Strategy ();

  // Tell the strategy to apply itself to the child poa.
  if (!csd_tp_strategy->apply_to (child_poa.in ()))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) ERROR [ServerApp::run()]: ")
                  ACE_TEXT ("Failed to apply custom dispatching strategy to child POA.\n")));
      throw TestException ();
    }

  // Create the Foo_i object.
  Foo_i foo_i (this->options_.num_clients);

  // Create tie object with the Foo_i object.
  POA_Foo_tie<Foo_i> foo_tie_i (foo_i, child_poa.in ());

  // Get Object Reference for the foo_tie_i object.
  Foo_var const foo = foo_tie_i._this ();

  if (CORBA::is_nil (foo.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failed to activate servant foo_tie_i.\n")));
      throw TestException ();
    }

  // Stringify the object reference
  CORBA::String_var const ior
    = orb->object_to_string (foo.in ());

  // Write the stringified object reference to the ior file.
  {
    std::unique_ptr<FILE, int (*) (FILE *)> const ior_file (
      ACE_OS::fopen (this->options_.ior_filename.c_str (), "w"), ACE_OS::fclose);
    if (!ior_file)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Cannot open output file for writing IOR: %s"),
                    this->options_.ior_filename.c_str ()));
        throw TestException ();
      }

    ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
  }

  // Activate the POA Manager
  poa_manager->activate ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp is ready.  Running the ORB event loop.\n")));

  // Run the ORB event loop.
  orb->run ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp ORB has stopped running. ")
              ACE_TEXT ("Stop the CSD strategy.\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp is waiting for OrbShutdownTask.\n")));
  TheOrbShutdownTask::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp is destroying the Root POA.\n")));

  // Sleep for 2 second to let the done() two-way call complete
  // before cleanup.
  ACE_OS::sleep (2);

  // Tear-down the root poa and orb.
  root_poa->destroy (true, true);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp is destroying the ORB.\n")));

  orb->destroy ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) ServerApp has completed running successfully.\n")));

  return 0;
}

int
ServerApp::parse_args (int argc, ACE_TCHAR *argv[])
{
  this->exe_name_ = argv[0];

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:n:"));

  int c {};

  while ((c = get_opts()) != -1)
    {
      switch (c)
        {
        case 'o':
          this->options_.ior_filename = get_opts.opt_arg ();
          break;

        case 'n':
          {
            int const tmp = ACE_OS::atoi (get_opts.opt_arg ());
            if (tmp < 1)
              {
                this->usage_statement ();
                return -1;
              }

            this->options_.num_clients = static_cast<unsigned> (tmp);
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
              ACE_TEXT ("Usage:  %s [options]\n\n")
              ACE_TEXT ("OPTIONS:\n\n")
              ACE_TEXT ("\t[-o <ior_filename>]\n")
              ACE_TEXT ("\t[-n <num_clients>]\n")
              ACE_TEXT ("\t[-?]\n\n"),
              this->exe_name_.c_str ()));
}

