// $Id$

#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
//#include "orbsvcs/Event/Module_Factory.h"
//#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "Consumer.h"
#include "Supplier.h"


#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#include "ace/OS_NS_strings.h"
#include "ace/Thread.h"



namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
}

inline RtecScheduler::Period_t time_val_to_period (const ACE_Time_Value &tv)
{
  //100s of nanoseconds
  return static_cast<RtecScheduler::Period_t> (tv.sec () * 1000000 + tv.usec ())*10;
}

int parse_args (int argc, ACE_TCHAR *argv[]);

typedef TAO_Reconfig_Scheduler<TAO_RMS_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_RMS_SCHED_TYPE;

typedef TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_MUF_SCHED_TYPE;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (config_run);

  //TAO_EC_Default_Factory::init_svcs ();

  TAO_EC_Kokyu_Factory::init_svcs ();


  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage: Service [-o IOR_file_name]\n"));
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // ****************************************************************

      // Create an scheduling service
      POA_RtecScheduler::Scheduler* sched_impl = 0;

      if (ACE_OS::strcasecmp(sched_type.c_str(), "rms") == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Creating RMS scheduler\n"));
          ACE_NEW_RETURN (sched_impl,
                          RECONFIG_RMS_SCHED_TYPE,
                          1);
        }
      else if (ACE_OS::strcasecmp(sched_type.c_str(), "muf") == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Creating MUF scheduler\n"));
          ACE_NEW_RETURN (sched_impl,
                          RECONFIG_MUF_SCHED_TYPE,
                          1);
        }

      RtecScheduler::Scheduler_var scheduler =
        sched_impl->_this ();

      // ****************************************************************
      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.scheduler = scheduler.in (); // no need to dup

      TAO_EC_Event_Channel ec_impl (attributes);
      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();
      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      Consumer consumer_impl1, consumer_impl2;

      RtecScheduler::handle_t consumer1_rt_info =
        scheduler->create ("consumer1");

      RtecScheduler::handle_t consumer2_rt_info =
        scheduler->create ("consumer2");

      //consumer's rate will get propagated from the supplier.
      //so no need to specify a period here. Specifying
      //criticality is crucial since it propagates from
      //consumer to supplier.
      ACE_Time_Value tv (0,0);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      scheduler->set (consumer1_rt_info,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      tmp, tmp, tmp,
                      time_val_to_period (tv),
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION);

      scheduler->set (consumer2_rt_info,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      tmp, tmp, tmp,
                      time_val_to_period (tv),
                      RtecScheduler::VERY_HIGH_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION);

      ACE_ConsumerQOS_Factory consumer_qos1, consumer_qos2;
      //consumer_qos.start_disjunction_group ();
      // The types int the range [0,ACE_ES_EVENT_UNDEFINED) are
      // reserved for the EC...
      consumer_qos1.insert_type (ACE_ES_EVENT_UNDEFINED,
                                consumer1_rt_info);

      RtecEventChannelAdmin::ConsumerQOS qos =
        consumer_qos1.get_ConsumerQOS ();
/*
      for (int i=0;i<qos.dependencies.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "consumer_qos1[%d] event.header.type = %d, "
                      "consumer_qos1[%d] rt_info = %d, "
                      "consumer_qos1[%d] event.header.source = %d\n",
                      i,qos.dependencies[i].event.header.type,
                      i,qos.dependencies[i].rt_info,
                      i,qos.dependencies[i].event.header.source));
        }
*/

      consumer_qos2.insert_type (ACE_ES_EVENT_UNDEFINED + 1,
                                consumer2_rt_info);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy1 =
        consumer_admin->obtain_push_supplier ();

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy2 =
        consumer_admin->obtain_push_supplier ();

      RtecEventComm::PushConsumer_var consumer1 =
        consumer_impl1._this ();

      RtecEventComm::PushConsumer_var consumer2 =
        consumer_impl2._this ();

      ACE_DEBUG ((LM_DEBUG, "connecting consumers\n"));
      ACE_DEBUG ((LM_DEBUG, "connecting consumer1\n"));
      supplier_proxy1->connect_push_consumer (consumer1.in (),
                                             consumer_qos1.get_ConsumerQOS ());

      ACE_DEBUG ((LM_DEBUG, "connecting consumer2\n"));
      supplier_proxy2->connect_push_consumer (consumer2.in (),
                                             consumer_qos2.get_ConsumerQOS ());

      ACE_DEBUG ((LM_DEBUG, "consumers connected\n"));

      // ****************************************************************

      RtecScheduler::handle_t supplier1_rt_info =
        scheduler->create ("supplier1");

      RtecScheduler::handle_t supplier2_rt_info =
        scheduler->create ("supplier2");

      RtecEventComm::EventSourceID supplier_id1 = 1, supplier_id2 = 2;
      ACE_SupplierQOS_Factory supplier_qos1, supplier_qos2;
      supplier_qos1.insert (supplier_id1,
                           ACE_ES_EVENT_UNDEFINED,
                           supplier1_rt_info,
                           1 /* number of calls, but what does that mean? */);
      supplier_qos2.insert (supplier_id2,
                           ACE_ES_EVENT_UNDEFINED + 1,
                           supplier2_rt_info,
                           1 /* number of calls, but what does that mean? */);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy1 =
        supplier_admin->obtain_push_consumer ();

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy2 =
        supplier_admin->obtain_push_consumer ();

      Supplier supplier_impl1(supplier_id1, consumer_proxy1.in ());
      Supplier supplier_impl2(supplier_id2, consumer_proxy2.in ());

      RtecEventComm::PushSupplier_var supplier1 =
        supplier_impl1._this ();

      RtecEventComm::PushSupplier_var supplier2 =
        supplier_impl2._this ();

      ACE_DEBUG ((LM_DEBUG, "connecting suppliers\n"));
      ACE_DEBUG ((LM_DEBUG, "connecting supplier1\n"));
      consumer_proxy1->connect_push_supplier (supplier1.in (),
                                             supplier_qos1.get_SupplierQOS ());

      ACE_DEBUG ((LM_DEBUG, "connecting supplier2\n"));
      consumer_proxy2->connect_push_supplier (supplier2.in (),
                                             supplier_qos2.get_SupplierQOS ());
      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));

      // ****************************************************************

      //Timer Registration part

      //Timeout consumers for the two suppliers.
      Timeout_Consumer timeout_consumer_impl1(&supplier_impl1);
      Timeout_Consumer timeout_consumer_impl2(&supplier_impl2);

      RtecScheduler::handle_t supplier1_timeout_consumer_rt_info =
        scheduler->create ("supplier1_timeout_consumer");

      //Period = 1sec
      tv.set (1,0);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      scheduler->set (supplier1_timeout_consumer_rt_info,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      tmp, tmp, tmp,
                      time_val_to_period (tv),
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION);

      RtecScheduler::handle_t supplier2_timeout_consumer_rt_info =
        scheduler->create ("supplier2_timeout_consumer");

      //Period = 3sec
      tv.set (3, 0);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      scheduler->set (supplier2_timeout_consumer_rt_info,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      tmp, tmp, tmp,
                      time_val_to_period (tv),
                      RtecScheduler::VERY_HIGH_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION);

      ACE_ConsumerQOS_Factory timer_qos1, timer_qos2;
      timer_qos1.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                              10000000, //in 100s of nanosec
                              supplier1_timeout_consumer_rt_info);
      timer_qos2.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                              30000000, //in 100s of nanosec
                              supplier2_timeout_consumer_rt_info);

      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy1 =
        consumer_admin->obtain_push_supplier ();

      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy2 =
        consumer_admin->obtain_push_supplier ();

      RtecEventComm::PushConsumer_var safe_timeout_consumer1 =
        timeout_consumer_impl1._this ();

      RtecEventComm::PushConsumer_var safe_timeout_consumer2 =
        timeout_consumer_impl2._this ();

      ACE_DEBUG ((LM_DEBUG, "connecting timeout consumers\n"));
      timeout_supplier_proxy1->
        connect_push_consumer (safe_timeout_consumer1.in (),
                               timer_qos1.get_ConsumerQOS ());

      timeout_supplier_proxy2->
        connect_push_consumer (safe_timeout_consumer2.in (),
                               timer_qos2.get_ConsumerQOS ());

      ACE_DEBUG ((LM_DEBUG, "timeout consumers connected\n"));

      // ****************************************************************
      //Registering dependency between timeout consumers and our suppliers
      //with the scheduler

      scheduler->add_dependency (supplier1_timeout_consumer_rt_info,
                                 supplier1_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL);

      scheduler->add_dependency (supplier2_timeout_consumer_rt_info,
                                 supplier2_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL);

      // ****************************************************************

      // At this point the consumer and supplier are connected to the
      // EC, they have provided their QoS info to the Scheduling
      // Service and the EC has informed the Scheduler about the
      // dependencies between them.
      // We can now compute the schedule for this configuration...

      // The schedule is returned in this variables....

      ACE_DEBUG ((LM_DEBUG, "Computing schedule\n"));
      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Dependency_Set_var dependencies;
      RtecScheduler::Scheduling_Anomaly_Set unsafe_anomalies;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

      scheduler->get_rt_info_set (infos.out() );
      scheduler->get_dependency_set (dependencies.out() );
      scheduler->get_config_info_set (configs.out() );

      ACE_DEBUG ((LM_DEBUG, "Printing intermediate results\n"));
      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            dependencies.in (),
                                            configs.in (),
                                            unsafe_anomalies,
                                            ACE_TEXT("schedule.out"));

      // Obtain the range of valid priorities in the current
      // platform, the scheduler hard-code this values in the
      // generated file, but in the future we may just use the
      // "logical" priorities and define the mapping to OS
      // priorities at run-time.
      int min_os_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      int max_os_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      scheduler->compute_scheduling (min_os_priority,
                                     max_os_priority,
                                     infos.out (),
                                     dependencies.out (),
                                     configs.out (),
                                     anomalies.out ());

      // Dump the schedule to a file..
      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            dependencies.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            ACE_TEXT("schedule.out"));

      // ****************************************************************
      ACE_DEBUG ((LM_DEBUG, "Pushing events\n"));

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

//     // Generate a few events....
//       RtecEventComm::EventSet event1 (1);
//       event1.length (1);
//       event1[0].header.type   = ACE_ES_EVENT_UNDEFINED;
//       event1[0].header.source = supplier_id1;
//       event1[0].header.ttl    = 1;

//       RtecEventComm::EventSet event2 (1);
//       event2.length (1);
//       event2[0].header.type   = ACE_ES_EVENT_UNDEFINED + 1;
//       event2[0].header.source = supplier_id2;
//       event2[0].header.ttl    = 1;

//       for (int i = 0; i != 200; ++i)
//         {
//           if (i % 2 == 0)
//             {
//               consumer_proxy1->push (event1);
//             }
//           else
//             {
//               consumer_proxy2->push (event2);
//             }

//           ACE_Time_Value rate (0, 10000);
//           ACE_OS::sleep (rate);
//         }

      ACE_DEBUG ((LM_DEBUG, "(%t) activating EC\n"));
      ec_impl.activate ();
      ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

      orb->run ();

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
  return 0;
}

// ****************************************************************

int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("cs:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s %s"
                           "\n",
                           argv [0],
                           "-s <rms|muf>"),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

