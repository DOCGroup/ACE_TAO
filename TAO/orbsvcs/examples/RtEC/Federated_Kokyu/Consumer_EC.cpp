// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"

#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "orbsvcs/Event/EC_Event_Counter.h"
#include "orbsvcs/Event/EC_Event_Limit.h"

#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;
}

inline RtecScheduler::Period_t time_val_to_period (const ACE_Time_Value &tv)
{
  //100s of nanoseconds
  return (tv.sec () * 1000000 + tv.usec ())*10;
}

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
  ds_control* ds_cntl = new ds_control ("Federated_Test_Consumer","federated_enabled.dsui");

  TAO_EC_Kokyu_Factory::init_svcs ();

  //@BT
  EC_Event_Counter::event_id eid = EC_EVENT_COUNTER->increment();
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, START,1, sizeof(EC_Event_Counter::event_id), (char*)&eid);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Service [-o IOR_file_name]\n"));
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      Kokyu_EC kokyu_ec;
      if (kokyu_ec.init(sched_type.c_str(), poa.in()) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);

      // *************************************************************
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      Consumer consumer_impl1, consumer_impl2;
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier1,
                                                    proxy_supplier2;

      //consumer's rate will get propagated from the supplier.
      //so no need to specify a period here. Specifying
      //criticality is crucial since it propagates from
      //consumer to supplier.
      ACE_Time_Value tv (0,0);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      RtEventChannelAdmin::SchedInfo info;
      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecEventComm::PushConsumer_var consumer1 =
        consumer_impl1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      RtecScheduler::handle_t consumer1_rt_info =
        kokyu_ec.register_consumer("consumer1",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED,
                                   consumer1.in(),
                                   proxy_supplier1.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_UNUSED_ARG(consumer1_rt_info);
      ACE_TRY_CHECK;

      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;

      RtecEventComm::PushConsumer_var consumer2 =
        consumer_impl2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer2_rt_info =
        kokyu_ec.register_consumer("consumer2",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED + 1,
                                   consumer2.in(),
                                   proxy_supplier2.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_UNUSED_ARG(consumer2_rt_info);

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var kokyu_ec_ior =
        kokyu_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(kokyu_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);


      // ****************************************************************

      // At this point the consumer and supplier are connected to the
      // EC, they have provided their QoS info to the Scheduling
      // Service and the EC has informed the Scheduler about the
      // dependencies between them.
      // We can now compute the schedule for this configuration...

      // The schedule is returned in this variables....


      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);

      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance(), ds_cntl);
      ACE_Time_Value ticker (25);
      orb->orb_core()->reactor()->schedule_timer(e_limit,0, ticker);

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "cs:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (ior_output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case 's':
        sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -s <rms|muf|edf>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  if (ior_output_file == 0)
    ior_output_file = ACE_OS::fopen ("consumer_ec.ior", "w");
  return 0;
}
