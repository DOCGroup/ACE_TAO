// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Send_Task.h"
#include "Client_Pair.h"
#include "ORB_Task.h"
#include "ORB_Task_Activator.h"
#include "Auto_Disconnect.h"
#include "Low_Priority_Setup.h"
#include "EC_Destroyer.h"

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

  RT_Class rt_class;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      RTServer_Setup rtserver_setup (use_rt_corba,
                                     orb,
                                     rt_class
                                     ACE_ENV_ARG_PARAMETER);
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

      PortableServer::POA_var supplier_poa (root_poa);
      PortableServer::POA_var consumer_poa (root_poa);
      if (use_rt_corba != 0)
        {
          supplier_poa = rtserver_setup.poa ();
          consumer_poa = rtserver_setup.poa ();
        }

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      ACE_Thread_Manager my_thread_manager;

      ORB_Task orb_task (orb);
      orb_task.thr_mgr (&my_thread_manager);
      ORB_Task_Activator orb_task_activator (rt_class.priority_high (),
                                             rt_class.thr_sched_class (),
                                             &orb_task);

      ACE_DEBUG ((LM_DEBUG, "ORB is active\n"));

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      EC_Destroyer ec_destroyer (ec.in ());

      CORBA::PolicyList_var inconsistent_policies;
      (void) ec->_validate_connection (inconsistent_policies
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished EC configuration and activation\n"));

      int thread_count = 1;
      if (disable_low_priority == 0)
        thread_count += nthreads;
      ACE_Barrier barrier (thread_count);

      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      Low_Priority_Setup<Client_Pair> low_priority_setup (
          nthreads,
          iterations,
          0, // all clients get the same type
          experiment_id,
          ACE_ES_EVENT_UNDEFINED + 2,
          low_priority_workload,
          gsf,
          disable_low_priority ? 0 : 1,
          rt_class.priority_low (),
          rt_class.thr_sched_class (),
          low_priority_period,
          supplier_poa.in (),
          consumer_poa.in (),
          ec.in (),
          &barrier
          ACE_ENV_ARG_PARAMETER);

      Client_Pair high_priority_group;
      high_priority_group.init (experiment_id,
                                ACE_ES_EVENT_UNDEFINED,
                                iterations,
                                high_priority_workload,
                                gsf,
                                supplier_poa.in (),
                                consumer_poa.in ());
      high_priority_group.connect (ec.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Auto_Disconnect<Client_Pair> high_priority_disconnect (&high_priority_group);

      Send_Task high_priority_task;
      high_priority_task.init (iterations,
                               high_priority_period,
                               ACE_ES_EVENT_UNDEFINED,
                               1,
                               high_priority_group.supplier (),
                               &barrier);
      high_priority_task.thr_mgr (&my_thread_manager);
      {
        // Artificial scope to wait for the high priority task...
        Task_Activator<Send_Task> high_priority_act (rt_class.priority_high (),
                                                     rt_class.thr_sched_class (),
                                                     &high_priority_task);
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high priority task completed\n"));

      if (disable_low_priority == 0)
        {
          low_priority_setup.stop_all_threads ();
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - low priority task(s) stopped\n"));

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
      low_priority_setup.collect_basic_stats (low_priority_stats);
      low_priority_stats.dump_results ("Low Priority", gsf);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting cleanup\n"));
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
