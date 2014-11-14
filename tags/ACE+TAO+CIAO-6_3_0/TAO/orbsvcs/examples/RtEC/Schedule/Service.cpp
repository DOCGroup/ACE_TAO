// $Id$

#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "Consumer.h"
#include "Supplier.h"

#include "Schedule.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_unistd.h"



int config_run = 0;

int parse_args (int argc, ACE_TCHAR *argv[]);

typedef TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_SCHED_TYPE;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

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

#if 0
      // Obtain a reference to the naming service...
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
#endif /* 0 */

      // ****************************************************************

      // Create an scheduling service
      POA_RtecScheduler::Scheduler* sched_impl = 0;
      if (config_run)
        {
          ACE_NEW_RETURN (sched_impl,
                          RECONFIG_SCHED_TYPE,
                          1);
        }
      else
        {
          ACE_NEW_RETURN (sched_impl,
                          RECONFIG_SCHED_TYPE (configs_size,
                                               configs,
                                               infos_size,
                                               infos,
                                               0, 0,
                                               0),
                          1);
        }

      RtecScheduler::Scheduler_var scheduler =
        sched_impl->_this ();

#if 0
      // Bind the scheduler with the naming service so clients
      // (consumers and suppliers) can resolve it, some (old)
      // implementations of the EC will try to do the same thing
      // (yikes!)
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");
      // Register the servant with the Naming Context....
      naming_context->rebind (schedule_name, scheduler.in ());
#endif /* 0 */

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.scheduler = scheduler.in (); // no need to dup

      TAO_EC_Event_Channel ec_impl (attributes);
      ACE_DEBUG ((LM_DEBUG, "activating EC\n"));
      ec_impl.activate ();
      ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();

      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      Consumer consumer_impl;

      RtecScheduler::handle_t consumer_rt_info1 =
        scheduler->create ("consumer_event_1");

      // Let's say that the execution time for event 1 is 2
      // milliseconds...
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      scheduler->set (consumer_rt_info1,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      time, time, time,
                      0,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      0,
                      RtecScheduler::OPERATION);

      RtecScheduler::handle_t consumer_rt_info2 =
        scheduler->create ("consumer_event_2");

      // Let's say that the execution time for event 2 is 1
      // milliseconds...
      tv.set (0, 1000);
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      scheduler->set (consumer_rt_info2,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      time, time, time,
                      0,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      0,
                      RtecScheduler::OPERATION);

      ACE_ConsumerQOS_Factory consumer_qos;
      consumer_qos.start_disjunction_group ();
      // The types int the range [0,ACE_ES_EVENT_UNDEFINED) are
      // reserved for the EC...
      consumer_qos.insert_type (ACE_ES_EVENT_UNDEFINED,
                                consumer_rt_info1);
      consumer_qos.insert_type (ACE_ES_EVENT_UNDEFINED + 1,
                                consumer_rt_info2);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy =
        consumer_admin->obtain_push_supplier ();

      RtecEventComm::PushConsumer_var consumer =
        consumer_impl._this ();

      ACE_DEBUG ((LM_DEBUG, "connecting consumer\n"));
      supplier_proxy->connect_push_consumer (consumer.in (),
                                             consumer_qos.get_ConsumerQOS ());
      ACE_DEBUG ((LM_DEBUG, "consumer connected\n"));

      // ****************************************************************

      Supplier supplier_impl;

      RtecScheduler::handle_t supplier_rt_info1 =
        scheduler->create ("supplier_event_1");

      // The execution times are set to reasonable values, but
      // actually they are changed on the real execution, i.e. we
      // lie to the scheduler to obtain right priorities; but we
      // don't care if the set is schedulable.
      tv.set (0, 10000);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      RtecScheduler::Period_t rate = ACE_U64_TO_U32(tmp);

      scheduler->set (supplier_rt_info1,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      0, 0, 0,
                      rate,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      0,
                      1,
                      RtecScheduler::OPERATION);

      RtecScheduler::handle_t supplier_rt_info2 =
        scheduler->create ("supplier_event_2");

      // The execution times are set to reasonable values, but
      // actually they are changed on the real execution, i.e. we
      // lie to the scheduler to obtain right priorities; but we
      // don't care if the set is schedulable.
      tv.set (0, 20000);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      rate = ACE_U64_TO_U32(tmp);

      scheduler->set (supplier_rt_info2,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      0, 0, 0,
                      rate,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      0,
                      1,
                      RtecScheduler::OPERATION);

      RtecEventComm::EventSourceID supplier_id = 1;
      ACE_SupplierQOS_Factory supplier_qos;
      supplier_qos.insert (supplier_id,
                           ACE_ES_EVENT_UNDEFINED,
                           supplier_rt_info1,
                           1 /* number of calls, but what does that mean? */);
      supplier_qos.insert (supplier_id,
                           ACE_ES_EVENT_UNDEFINED + 1,
                           supplier_rt_info2,
                           1 /* number of calls, but what does that mean? */);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy =
        supplier_admin->obtain_push_consumer ();

      RtecEventComm::PushSupplier_var supplier =
        supplier_impl._this ();

      ACE_DEBUG ((LM_DEBUG, "connecting supplier\n"));
      consumer_proxy->connect_push_supplier (supplier.in (),
                                             supplier_qos.get_SupplierQOS ());
      ACE_DEBUG ((LM_DEBUG, "supplier connected\n"));

      // ****************************************************************

      // At this point the consumer and supplier are connected to the
      // EC, they have provided their QoS info to the Scheduling
      // Service and the EC has informed the Scheduler about the
      // dependencies between them.
      // We can now compute the schedule for this configuration...

      // The schedule is returned in this variables....

      if (config_run)
        {
          ACE_DEBUG ((LM_DEBUG, "Computing schedule\n"));
          RtecScheduler::RT_Info_Set_var infos;
          RtecScheduler::Dependency_Set_var deps;
          RtecScheduler::Config_Info_Set_var configs;
          RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

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
                                         deps.out (),
                                         configs.out (),
                                         anomalies.out ());

          // Dump the schedule to a file..
          ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                                deps.in (),
                                                configs.in (),
                                                anomalies.in (),
                                                ACE_TEXT("schedule.out"));
        }

      // ****************************************************************

      ACE_DEBUG ((LM_DEBUG, "Pushing events\n"));

      // Generate a few events....

      RtecEventComm::EventSet event1 (1);
      event1.length (1);
      event1[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event1[0].header.source = supplier_id;
      event1[0].header.ttl    = 1;

      RtecEventComm::EventSet event2 (1);
      event2.length (1);
      event2[0].header.type   = ACE_ES_EVENT_UNDEFINED + 1;
      event2[0].header.source = supplier_id;
      event2[0].header.ttl    = 1;

      for (int i = 0; i != 200; ++i)
        {
          if (i % 2 == 0)
            {
              consumer_proxy->push (event1);
            }
          else
            {
              consumer_proxy->push (event2);
            }

          ACE_Time_Value rate (0, 10000);
          ACE_OS::sleep (rate);
        }

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        config_run = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c (config run)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

