// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include <ace/Counter.h>
#include "orbsvcs/Event/EC_Event_Limit.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#include <stdlib.h>
#endif //ACE_HAS_DSUI

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
#ifdef ACE_HAS_DSUI
  ds_control* ds_cntl = new ds_control ("Federated_Test_Consumer","consumer_enabled.dsui");
  int result = system("dstream_daemon -y d 300 -r 305 -v /tmp/Federated_Test_DSKI.binary -f 2 -e 3 -f 2 -e 4");
  if (WIFSIGNALED(result) && (WTERMSIG(result) == SIGINT || WTERMSIG(result) == SIGQUIT))
    {
      ACE_DEBUG((LM_ERROR, "Unable to access DSKI daemon. Please double check the kernel was configured with DSKI.\n"));
    }

#endif // ACE_HAS_DSUI

  TAO_EC_Kokyu_Factory::init_svcs ();

  //@BT
  //DSUI_EVENT_LOG(MAIN_GROUP_FAM, START,1,0,NULL);
  ACE_Time_Value tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Consumer_EC thread %t START at %u\n",tv.msec()));
#ifdef ACE_HAS_DSUI
  //  ACE_Object_Counter::object_id oid = ACE_OBJECT_COUNTER->increment();
  //  DSUI_EVENT_LOG(MAIN_GROUP_FAM, START, 1, sizeof(EC_Event_Counter::event_id), (char*)&eid);
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, START, 0, 0, NULL);
#endif //ACE_HAS_DSUI

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

      //// CONSUMER 2_2 ////
      ACE_Time_Value tv(6,0);
      Consumer consumer_impl2_2(tv);
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier2_2;

      //consumer's rate will get propagated from the supplier.
      //so no need to specify a period here. Specifying
      //criticality is crucial since it propagates from
      //consumer to supplier.
      tv.set(6,0);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      RtEventChannelAdmin::SchedInfo info;
      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecEventComm::PushConsumer_var consumer2_2 =
        consumer_impl2_2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer2_2_rt_info =
        kokyu_ec.register_consumer("consumer2_2",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED+2,
                                   consumer2_2.in(),
                                   proxy_supplier2_2.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_UNUSED_ARG(consumer2_2_rt_info);

      //// CONSUMER 3 ////
      tv.set(12,0);
      Consumer consumer_impl3(tv);
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier3;

      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;

      RtecEventComm::PushConsumer_var consumer3 =
        consumer_impl3._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer3_rt_info =
        kokyu_ec.register_consumer("consumer3",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED+3,
                                   consumer3.in(),
                                   proxy_supplier3.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_UNUSED_ARG(consumer3_rt_info);

      // *************************************************************
      // Create Supplier, intialize its RT_Info structures, and
      // connnect to the event channel....

      //// SUPPLIER 3 ////
      Supplier supplier_impl3(4);
      Timeout_Consumer timeout_consumer_impl3(&supplier_impl3);

      RtecEventComm::EventSourceID supplier_id3 = 4;

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy3;
      RtecEventComm::PushSupplier_var supplier3;

      supplier3 = supplier_impl3._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::handle_t supplier3_rt_info =
        kokyu_ec.register_supplier("supplier3",
                                   supplier_id3,
                                   ACE_ES_EVENT_UNDEFINED+3,
                                   supplier3.in(),
                                   consumer_proxy3.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      supplier_impl3.set_consumer_proxy(consumer_proxy3.in());

      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));
      //Timer Registration part
      //Timeout consumers for the suppliers.

      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy3;
      RtecEventComm::PushConsumer_var safe_timeout_consumer3;

      safe_timeout_consumer3= timeout_consumer_impl3._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      tv.set(12,0); //Period

      info.criticality = RtecScheduler::LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecScheduler::handle_t supplier3_timeout_consumer_rt_info =
        kokyu_ec.register_consumer("supplier3_timeout_consumer",
                                   info,
                                   ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                   safe_timeout_consumer3.in(),
                                   timeout_supplier_proxy3.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG, "timeout consumers connected\n"));

      kokyu_ec.add_dependency (supplier3_timeout_consumer_rt_info,
                               supplier3_rt_info,
                               1,
                               RtecBase::TWO_WAY_CALL
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

      kokyu_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Consumer_EC thread %t WORKER_ACTIVATED at %u\n",tv.msec()));
#ifdef ACE_HAS_DSUI
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);
#endif //ACE_HAS_DSUI

      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance(), ds_cntl);
      ACE_Time_Value ticker (305);
      orb->orb_core()->reactor()->schedule_timer(e_limit,0, ticker);

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Consumer_EC - Service");
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
