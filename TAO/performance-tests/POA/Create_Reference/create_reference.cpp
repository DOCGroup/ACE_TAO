// $Id$

#include "tao/PortableServer/PortableServer.h"
#include "tao/corba.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

ACE_RCSID(Activation, server, "$Id$")

int niterations = 10000;
int do_dump_history = 0;
ACE_UINT32 gsf;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hi:");
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
                      int create_reference_with_id
                      TAO_ENV_ARG_DECL)
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
                                           "IDL:Test/Simple:1.0"
                                           TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          object =
            poa->create_reference ("IDL:Test/Simple:1.0"
                                   TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      creation.sample (now - start);
    }

  if (do_dump_history)
    {
      creation.dump_samples ("HISTORY", gsf);
    }

  ACE_Basic_Stats creation_stats;
  creation.collect_basic_stats (creation_stats);
  creation_stats.dump_results ("Creation", gsf);
}

int
main (int argc, char *argv[])
{
  set_rt_scheduling ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::PolicyList policies(1); policies.length (1);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("TestPOA",
                              poa_manager.in (),
                              policies
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      gsf = ACE_High_Res_Timer::global_scale_factor ();

      object_creation_test (root_poa.in (),
                            0  // POA::create_reference
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object_creation_test (child_poa.in (),
                            1  // POA::create_reference_with_id
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
