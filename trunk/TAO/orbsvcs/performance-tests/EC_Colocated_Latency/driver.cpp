// $Id$

#include "Supplier.h"
#include "Consumer.h"
#include "Send_Task.h"
#include "Receive_Task.h"
#include "POA_Destroyer.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_RTCORBA_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "tao/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping.h"
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

ACE_RCSID(EC_Colocated_Latency, server, "$Id$")

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
  ACE_Get_Opt get_opts (argc, argv, "i:n:l:h:w:v:zdr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
  TAO_EC_Default_Factory::init_svcs ();

  int thr_sched_class = THR_SCHED_FIFO;
  int prc_sched_class = ACE_SCHED_FIFO;

  int low_priority =
    ACE_Sched_Params::priority_min (prc_sched_class);
  int prc_priority =
    ACE_Sched_Params::next_priority (prc_sched_class,
                                     low_priority);
  int high_priority =
    ACE_Sched_Params::next_priority (prc_sched_class,
                                     prc_priority);

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (prc_sched_class,
                                              prc_priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "WARNING (%P|%t) user does not have "
                      "permissions to run real-time tests.\n"
                      "The test will run in the time-shared "
                      "class, it may fail or produce unpredictable "
                      "results\n"));
          prc_sched_class = ACE_SCHED_OTHER;
          thr_sched_class = THR_SCHED_DEFAULT;

          low_priority =
            ACE_Sched_Params::priority_min (prc_sched_class);
          prc_priority =
            ACE_Sched_Params::next_priority (prc_sched_class,
                                             low_priority);
          high_priority =
            ACE_Sched_Params::next_priority (prc_sched_class,
                                             prc_priority);

          (void) ACE_OS::sched_params (
                     ACE_Sched_Params (prc_sched_class,
                                       prc_priority,
                                       ACE_SCOPE_PROCESS));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;


      RTCORBA::PriorityMappingManager_var priority_mapping_manager;
      auto_ptr<RTCORBA::PriorityMapping> priority_mapping;
      if (use_rt_corba)
        {
          CORBA::Object_var object =
            orb->resolve_initial_references ("PriorityMappingManager"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTCORBA::PriorityMappingManager_var priority_mapping_manager =
            RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (priority_mapping_manager.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Panic: nil priority mapping manager\n"),
                              1);

          priority_mapping =
            new TAO_Continuous_Priority_Mapping (prc_sched_class);

          priority_mapping_manager->mapping (priority_mapping.get ());

          object =
            orb->resolve_initial_references ("RTCurrent"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTCORBA::Current_var current =
            RTCORBA::Current::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTCORBA::Priority corba_prc_priority;
          priority_mapping->to_CORBA (prc_priority,
                                      corba_prc_priority);

          current->the_priority (corba_prc_priority
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      POA_Destroyer destroy_poa (root_poa.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());

      TAO_EC_Default_Factory default_factory;
      auto_ptr<TAO_EC_Factory> factory_holder;
      TAO_EC_Factory *factory = 0;

      if (!use_rt_corba)
        {
          factory = &default_factory;
        }
      else
        {
          RTCORBA::ThreadpoolLanes lanes (2);
          lanes.length (2);

          if (!priority_mapping->to_CORBA (high_priority,
                                           lanes[0].lane_priority))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Panic: cannot map high priority\n"),
                              1);
          lanes[0].static_threads = 1;
          lanes[0].dynamic_threads = 0;

          if (!priority_mapping->to_CORBA (low_priority,
                                           lanes[1].lane_priority))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Panic: cannot map low priority\n"),
                              1);
          lanes[1].static_threads = 1;
          lanes[1].dynamic_threads = 0;

          factory_holder =
            new TAO_EC_RTCORBA_Factory (&default_factory,
                                        lanes);
          factory = factory_holder.get ();
        }

      TAO_EC_Event_Channel* ec_impl;
      ACE_NEW_RETURN (ec_impl,
                      TAO_EC_Event_Channel (attr, factory, 0),
                      1);
      PortableServer::ServantBase_var ec_owner (ec_impl);

      ec_impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished EC configuration and activation\n"));

      ACE_Thread_Manager my_thread_manager;

      int thread_count = 1;
      if (disable_low_priority == 0)
        thread_count += nthreads;
      ACE_Barrier barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Supplier> > low_priority_suppliers (new Servant_var<ECCL_Supplier>[nthreads]);
      ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Consumer> > low_priority_consumers (new Servant_var<ECCL_Consumer>[nthreads]);
      ACE_Auto_Basic_Array_Ptr<ECCL_Send_Task> low_priority_tasks (new ECCL_Send_Task[nthreads]);
      for (int i = 0; i != nthreads; ++i)
        {
          low_priority_suppliers[i] =
            Servant_var<ECCL_Supplier> (new ECCL_Supplier);
          low_priority_suppliers[i]->connect (ec.in (),
                                              ACE_ES_EVENT_UNDEFINED + 1 + i
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          int low_priority_iterations = iterations;
          if (low_priority_period == 0)
            low_priority_iterations = INT_MAX;

          int per_consumer_workload =
            low_priority_workload / nthreads;
          if (per_consumer_workload == 0)
            per_consumer_workload = 1;
          low_priority_consumers[i] =
            Servant_var<ECCL_Consumer> (new ECCL_Consumer (iterations,
                                                           per_consumer_workload,
                                                           gsf));
          low_priority_consumers[i]->connect (ec.in (),
                                              ACE_ES_EVENT_UNDEFINED + 1 + i
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;


          low_priority_tasks[i].init (low_priority_iterations,
                                      low_priority_period,
                                      ACE_ES_EVENT_UNDEFINED + 1 + i,
                                      1,
                                      low_priority_suppliers[i],
                                      &barrier);

          if (disable_low_priority == 0)
            {
              low_priority_tasks[i].thr_mgr (&my_thread_manager);
              if (low_priority_tasks[i].activate (
                      thr_sched_class | THR_NEW_LWP | THR_JOINABLE,
                      1, 1,
                      low_priority) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "Panic - cannot activate "
                                     "low priority task\n"),
                                    1);
                }
            }
        }

      Servant_var<ECCL_Consumer> consumer_impl (new ECCL_Consumer (iterations,
                                                                   high_priority_workload,
                                                                   gsf));
      consumer_impl->connect (ec.in (), ACE_ES_EVENT_UNDEFINED
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Servant_var<ECCL_Supplier> supplier_impl (new ECCL_Supplier);
      supplier_impl->connect (ec.in (), ACE_ES_EVENT_UNDEFINED
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ECCL_Send_Task high_priority_task;
      high_priority_task.init (iterations,
                               high_priority_period,
                               ACE_ES_EVENT_UNDEFINED,
                               1,
                               supplier_impl,
                               &barrier);
      high_priority_task.thr_mgr (&my_thread_manager);
      if (high_priority_task.activate (
             thr_sched_class | THR_NEW_LWP | THR_JOINABLE,
             1, 1,
             high_priority) == -1)
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
        consumer_impl->sample_history ();
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
          low_priority_consumers[k]->sample_history ().collect_basic_stats (low_priority_stats);
        }
      low_priority_stats.dump_results ("Low Priority", gsf);

      supplier_impl->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer_impl->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int j = 0; j != nthreads; ++j)
        {
          low_priority_suppliers[j]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          low_priority_consumers[j]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ec_impl->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

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

template class Servant_var<ECCL_Supplier>;
template class Servant_var<ECCL_Consumer>;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Supplier> >;
template class ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Consumer> >;
template class ACE_Auto_Basic_Array_Ptr<ECCL_Send_Task>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<ECCL_Supplier>
#pragma instantiate Servant_var<ECCL_Consumer>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Supplier> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<Servant_var<ECCL_Consumer> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<ECCL_Send_Task>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
