/**
 * @file client.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Consumer.h"
#include "Supplier.h"
#include "Client_Task.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "tao/Messaging.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Sched_Params.h"
#include "ace/Task.h"

ACE_RCSID(EC_Scalability, client, "$Id$")

const char *ior = "file://test.ior";
int consumer_count = 10;
int iterations = 10000;
int do_dump_history = 0;

/// Parse the arguments.
static int parse_args (int argc, char *argv[]);

int
main (int argc, char *argv [])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
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

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the event channel object reference
      CORBA::Object_var ec_object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_object.in ()
                                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (ec.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Invalid or nil event channel\n"));
          return 1;
        }

      ECS_Consumer **consumer_impl;
      ACE_NEW_RETURN (consumer_impl,
                      ECS_Consumer*[consumer_count],
                      1);
      int i;
      for (i = 0; i != consumer_count; ++i)
        {
          ACE_NEW_RETURN (consumer_impl[i],
                          ECS_Consumer (iterations),
                          1);
          consumer_impl[i]->connect (ec.in () TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ECS_Supplier *supplier_impl;
      ACE_NEW_RETURN (supplier_impl,
                      ECS_Supplier,
                      1);
      PortableServer::ServantBase_var supplier_owner (supplier_impl);

      supplier_impl->connect (ec.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Connected consumer & supplier\n"));

      ECS_Client_Task task (orb.in ());
      task.activate ();

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = 1;
      event[0].header.ttl    = 1;
      // event[0].data.payload.length(1024);

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      for (i = 0; i != iterations; ++i)
        {
          ACE_hrtime_t creation = ACE_OS::gethrtime ();
          ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                         creation);
          // push one event...
          supplier_impl->push (event TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_hrtime_t end = ACE_OS::gethrtime ();

      for (i = 0; i != consumer_count; ++i)
        {
          consumer_impl[i]->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      supplier_impl->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      ACE_Sample_History history (iterations);
      for (int j = 0; j != iterations; ++j)
        {
          ACE_UINT64 value = 0;
          for (int i = 0; i != consumer_count; ++i)
            {
              ACE_Sample_History &consumer_history =
                consumer_impl[i]->sample_history ();

              ACE_UINT64 consumer_sample =
                consumer_history.get_sample (j);
              if (consumer_sample > value)
                value = consumer_sample;
            }
          history.sample (value);
        }

      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             end - start,
                                             stats.samples_count ());


      orb->shutdown (0 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Thread_Manager::instance ()->wait ();

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      ACE_PRINT_EXCEPTION (ex, argv[0]);
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hc:i:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'c':
        consumer_count = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h (dump full sample history) "
                           "-c <consumer_count> "
                           "-i <iterations> "
                           "-k <IOR> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
