// $Id$

#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"

int niterations = 10000;
int do_dump_history = 0;
ACE_UINT32 gsf;

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
  // Indicates successful parsing of the command line
  return 0;
}

void
set_rt_scheduling (void)
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
}

void
object_creation_test (PortableServer::POA_ptr poa,
                      int create_reference_with_id)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nCreating %d object references with %s\n",
              niterations,
              create_reference_with_id ?
              "create_reference_with_id" :
              "create_reference"));

  ACE_Sample_History creation (niterations);

  for (int i = 0; i != niterations; ++i)
    {
      char buf[16];
      ACE_OS::sprintf (buf, "Object_%8.8x", i);
      PortableServer::ObjectId_var oid
        (PortableServer::string_to_ObjectId (buf));

      ACE_hrtime_t start =
        ACE_OS::gethrtime ();

      CORBA::Object_var object;
      if (create_reference_with_id)
        {
          object =
            poa->create_reference_with_id (oid.in (),
                                           "IDL:Test/Simple:1.0");
        }
      else
        {
          object =
            poa->create_reference ("IDL:Test/Simple:1.0");
        }

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      creation.sample (now - start);
    }

  if (do_dump_history)
    {
      creation.dump_samples (ACE_TEXT("HISTORY"), gsf);
    }

  ACE_Basic_Stats creation_stats;
  creation.collect_basic_stats (creation_stats);
  creation_stats.dump_results (ACE_TEXT("Creation"), gsf);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  set_rt_scheduling ();

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

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

      CORBA::PolicyList policies(1); policies.length (1);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("TestPOA",
                              poa_manager.in (),
                              policies);

      gsf = ACE_High_Res_Timer::global_scale_factor ();

      object_creation_test (root_poa.in (),
                            0  // POA::create_reference
                            );

      object_creation_test (child_poa.in (),
                            1  // POA::create_reference_with_id
                            );

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
