// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Send_Task.h"
#include "Client_Group.h"
#include "ORB_Task.h"
#include "ORB_Task_Activator.h"
#include "Low_Priority_Setup.h"
#include "EC_Destroyer.h"
#include "Client_Options.h"

#include "orbsvcs/Event_Service_Constants.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Continuous_Priority_Mapping.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Sched_Params.h"
#include "ace/Barrier.h"



int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const CORBA::Long experiment_id = 1;

  RT_Class rt_class;

  try
    {
      ORB_Holder orb (argc, argv, "");

      Client_Options options (argc, argv);
      if (argc != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage:  %s "
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
                            1);
        }

      RTServer_Setup rtserver_setup (options.use_rt_corba,
                                     orb,
                                     rt_class,
                                     1 // options.nthreads
                                     );

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA");

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      PortableServer::POA_var the_poa (rtserver_setup.poa ());

      ACE_Thread_Manager my_thread_manager;

      ORB_Task orb_task (orb);
      orb_task.thr_mgr (&my_thread_manager);
      ORB_Task_Activator orb_task_activator (rt_class.priority_high (),
                                             rt_class.thr_sched_class (),
                                             1,
                                             &orb_task);

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ());

      EC_Destroyer ec_destroyer (ec.in ());

      CORBA::PolicyList_var inconsistent_policies;
      (void) ec->_validate_connection (inconsistent_policies);

      ACE_DEBUG ((LM_DEBUG, "Found EC, validated connection\n"));

      int thread_count = 1 + options.nthreads;

      ACE_Barrier the_barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      CORBA::Long event_range = 1;
      if (options.funky_supplier_publication)
        {
          if (options.unique_low_priority_event)
            event_range = 1 + options.low_priority_consumers;
          else
            event_range = 2;
        }

      Client_Group high_priority_group;
      high_priority_group.init (experiment_id,
                                ACE_ES_EVENT_UNDEFINED,
                                event_range,
                                options.iterations,
                                options.high_priority_workload,
                                gsf,
                                the_poa.in (),
                                the_poa.in ());

      Auto_Disconnect<Client_Group> high_priority_disconnect;

      if (!options.high_priority_is_last)
        {
          high_priority_group.connect (ec.in ());
          high_priority_disconnect = &high_priority_group;
        }

      int per_thread_period = options.low_priority_period;
      if (options.global_low_priority_rate)
        per_thread_period = options.low_priority_period * options.nthreads;

      Low_Priority_Setup<Client_Group> low_priority_setup (
          options.low_priority_consumers,
          0, // no limit on the number of iterations
          options.unique_low_priority_event,
          experiment_id,
          ACE_ES_EVENT_UNDEFINED + 2,
          options.low_priority_workload,
          gsf,
          options.nthreads,
          rt_class.priority_low (),
          rt_class.thr_sched_class (),
          per_thread_period,
          the_poa.in (),
          the_poa.in (),
          ec.in (),
          &the_barrier);

      if (options.high_priority_is_last)
        {
          high_priority_group.connect (ec.in ());
          high_priority_disconnect = &high_priority_group;
        }
      Send_Task high_priority_task;
      high_priority_task.init (options.iterations,
                               options.high_priority_period,
                               0,
                               ACE_ES_EVENT_UNDEFINED,
                               experiment_id,
                               high_priority_group.supplier (),
                               &the_barrier);
      high_priority_task.thr_mgr (&my_thread_manager);
      {
        // Artificial scope to wait for the high priority task...
        Task_Activator<Send_Task> high_priority_act (rt_class.priority_high (),
                                                     rt_class.thr_sched_class (),
                                                     1,
                                                     &high_priority_task);
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high priority task completed\n"));

      low_priority_setup.stop_all_threads ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - low priority task(s) stopped\n"));

      ACE_Sample_History &history =
        high_priority_group.consumer ()->sample_history ();
      if (options.dump_history)
        {
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats high_priority_stats;
      history.collect_basic_stats (high_priority_stats);
      high_priority_stats.dump_results (ACE_TEXT("High Priority"), gsf);

      ACE_Basic_Stats low_priority_stats;
      low_priority_setup.collect_basic_stats (low_priority_stats);
      low_priority_stats.dump_results (ACE_TEXT("Low Priority"), gsf);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting cleanup\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
