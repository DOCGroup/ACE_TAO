// $Id$

#include "Simple.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_errno.h"

int niterations = 10000;
int do_dump_history = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hi:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <niterations> "
                           "-h "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

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

      if (parse_args (argc, argv) != 0)
        return 1;

      Test::Simple_Sequence references (niterations);
      references.length (niterations);

      ACE_Sample_History activation (niterations);

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      ACE_DEBUG ((LM_DEBUG, "Activating %d objects\n", niterations));
      for (int i = 0; i != niterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          Simple *simple_impl;
          ACE_NEW_RETURN (simple_impl,
                          Simple,
                          1);
          PortableServer::ServantBase_var owner_transfer(simple_impl);

          references[i] =
            simple_impl->_this ();

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          activation.sample (now - start);
        }
      ACE_DEBUG ((LM_DEBUG, "Activations completed\n"));

      if (do_dump_history)
        {
          activation.dump_samples (ACE_TEXT("ACTIVATION_HISTORY"), gsf);
        }

      ACE_Basic_Stats activation_stats;
      activation.collect_basic_stats (activation_stats);
      activation_stats.dump_results (ACE_TEXT("Activation"), gsf);

      ACE_Sample_History destruction (niterations);

      ACE_DEBUG ((LM_DEBUG, "Destroying %d objects\n", niterations));
      for (int j = 0; j != niterations; ++j)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          references[j]->destroy ();

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          destruction.sample (now - start);
        }
      ACE_DEBUG ((LM_DEBUG, "Destructions completed\n"));

      if (do_dump_history)
        {
          destruction.dump_samples (ACE_TEXT("DESTRUCTION_HISTORY"), gsf);
        }

      ACE_Basic_Stats destruction_stats;
      destruction.collect_basic_stats (destruction_stats);
      destruction_stats.dump_results (ACE_TEXT("Destruction"), gsf);

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
