// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTCORBA_Setup.h"
#include "Send_Task.h"
#include "Client_Group.h"
#include "ORB_Task.h"

#include "orbsvcs/Event_Service_Constants.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Continuous_Priority_Mapping.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Sched_Params.h"

ACE_RCSID(TAO_RTEC_PERF_Roundtrip, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 10000;
int nthreads   = 0;
int do_dump_history = 0;
int high_priority_period = 0;
int high_priority_workload = 0;
int low_priority_period  = 0;
int low_priority_workload = 0;
int disable_low_priority = 0;
int use_rt_corba = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:n:l:h:w:v:zdr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'h':
        high_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        low_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        high_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'v':
        low_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        do_dump_history = 1;
        break;

      case 'z':
        disable_low_priority = 1;
        break;

      case 'r':
        use_rt_corba = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <IOR> "
                           "-i iterations (iterations) "
                           "-h high_priority_period (usecs) "
                           "-l low_priority_period (usecs) "
                           "-w high_priority_workload (usecs) "
                           "-v low_priority_workload (usecs) "
                           "-r (enable RT-CORBA) "
                           "-n nthreads (low priority thread) "
                           "-d (dump history) "
                           "-z (disable low priority) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  const CORBA::Long experiment_id = 1;

  RT_Class test_scheduling;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      auto_ptr<RTCORBA_Setup> rtcorba_setup;
      if (use_rt_corba)
        {
          rtcorba_setup = 
            auto_ptr<RTCORBA_Setup> (new RTCORBA_Setup (orb,
                                                        test_scheduling
                                                        ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;
        }

      CORBA::PolicyManager_var policy_manager =
        RIR_Narrow<CORBA::PolicyManager>::resolve (orb,
                                                   "ORBPolicyManager"
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      // @@ We need a helper class that automatically calls the
      //    destroy() method on each policy...
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_list[0]->destroy (ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished EC configuration and activation\n"));

      ORB_Task orb_task (orb);
      orb_task.activate (test_scheduling.thr_sched_class ()
                         | THR_NEW_LWP
                         | THR_JOINABLE,
                         1, 1,
                         test_scheduling.priority_high ());

      ACE_Thread_Manager my_thread_manager;

      int thread_count = 1;
      if (disable_low_priority == 0)
        thread_count += nthreads;
      ACE_Barrier barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      ACE_Auto_Basic_Array_Ptr<Client_Group> low_priority_group (
          new Client_Group[nthreads]
          );
      ACE_Auto_Basic_Array_Ptr<Send_Task> low_priority_tasks (new Send_Task[nthreads]);
      for (int i = 0; i != nthreads; ++i)
        {
          int low_priority_iterations = iterations;
          if (low_priority_period == 0)
            low_priority_iterations = INT_MAX;

          int per_consumer_workload =
            low_priority_workload / nthreads;
          if (per_consumer_workload == 0)
            per_consumer_workload = 1;

          CORBA::Long base_event_type =
            ACE_ES_EVENT_UNDEFINED + 2 * (i + 1);
          low_priority_group[i].init (experiment_id,
                                      base_event_type,
                                      iterations,
                                      per_consumer_workload,
                                      gsf);
          low_priority_group[i].connect (ec.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (disable_low_priority == 0)
            {
              low_priority_tasks[i].init (low_priority_iterations,
                                          low_priority_period,
                                          base_event_type,
                                          1,
                                          low_priority_group[i].supplier (),
                                          &barrier);
              low_priority_tasks[i].thr_mgr (&my_thread_manager);
              if (low_priority_tasks[i].activate (
                      test_scheduling.thr_sched_class ()
                      | THR_NEW_LWP
                      | THR_JOINABLE,
                      1, 1,
                      test_scheduling.priority_low ()) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "Panic - cannot activate "
                                     "low priority task\n"),
                                    1);
                }
            }
        }

      Client_Group high_priority_group;
      high_priority_group.init (experiment_id,
                                ACE_ES_EVENT_UNDEFINED,
                                iterations,
                                high_priority_workload,
                                gsf);
      high_priority_group.connect (ec.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Send_Task high_priority_task;
      high_priority_task.init (iterations,
                               high_priority_period,
                               ACE_ES_EVENT_UNDEFINED,
                               1,
                               high_priority_group.supplier (),
                               &barrier);
      high_priority_task.thr_mgr (&my_thread_manager);
      if (high_priority_task.activate (
             test_scheduling.thr_sched_class ()
             | THR_NEW_LWP
             | THR_JOINABLE,
             1, 1,
             test_scheduling.priority_high ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Panic - cannot activate high priority task\n"),
                            1);
        }

      high_priority_task.wait ();

      if (disable_low_priority == 0)
        {
          // Stop all the low priority threads (could be running in
          // continuous mode
          for (int i = 0; i != nthreads; ++i)
            {
              low_priority_tasks[i].stop ();
            }
        }

      my_thread_manager.wait ();

      ACE_Sample_History &history =
        high_priority_group.consumer ()->sample_history ();
      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results ("High Priority", gsf);

      ACE_Basic_Stats low_priority_stats;
      for (int k = 0; k != nthreads; ++k)
        {
          low_priority_group[k].consumer ()->sample_history ().collect_basic_stats (low_priority_stats);
        }
      low_priority_stats.dump_results ("Low Priority", gsf);

      high_priority_group.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int j = 0; j != nthreads; ++j)
        {
          low_priority_group[j].disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ec->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      orb->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb_task.wait ();

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<Supplier>;
template class Servant_var<Consumer>;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<Supplier> >;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<Consumer> >;
template class ACE_Auto_Basic_Array_Ptr<Send_Task>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<Supplier>
#pragma instantiate Servant_var<Consumer>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<Supplier> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<Consumer> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Send_Task>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
