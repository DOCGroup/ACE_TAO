// $Id$

#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "Consumer.h"
#include "Supplier.h"


#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(EC_Examples, Service, "$Id$")

int config_run = 0;

int parse_args (int argc, char *argv[]);

typedef TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_SCHED_TYPE;

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();

  TAO_EC_Kokyu_Factory::init_svcs ();


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

#if 0
      // Obtain a reference to the naming service...
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif /* 0 */

      // ****************************************************************

      // Create an scheduling service
      POA_RtecScheduler::Scheduler* sched_impl = 0;

      ACE_NEW_RETURN (sched_impl,
		      RECONFIG_SCHED_TYPE,
		      1);

      RtecScheduler::Scheduler_var scheduler =
        sched_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

#if 0
      // Bind the scheduler with the naming service so clients
      // (consumers and suppliers) can resolve it, some (old)
      // implementations of the EC will try to do the same thing
      // (yikes!)
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");
      // Register the servant with the Naming Context....
      naming_context->rebind (schedule_name, scheduler.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif /* 0 */

      // ****************************************************************

#if 0
      // Create an event channel implementation...
      TAO_Default_Module_Factory module_factory;
      ACE_EventChannel event_channel_impl (scheduler.in (),
                                           1,
                                           ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                           &module_factory);

      RtecEventChannelAdmin::EventChannel_var event_channel =
        event_channel_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.scheduler = scheduler.in (); // no need to dup

      TAO_EC_Event_Channel ec_impl (attributes);
      //ACE_DEBUG ((LM_DEBUG, "activating EC\n"));
      //ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;
      //ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif /* 0 */

      // ****************************************************************

      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      Consumer consumer_impl1, consumer_impl2;

      RtecScheduler::handle_t consumer_rt_info1 =
        scheduler->create ("consumer_event_1" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Let's say that the execution time for event 1 is 2
      // milliseconds...
      ACE_Time_Value tv (0, 2000);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      scheduler->set (consumer_rt_info1,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      time, time, time,
                      time,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      0,
                      RtecScheduler::OPERATION
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer_rt_info2 =
        scheduler->create ("consumer_event_2" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Let's say that the execution time for event 2 is 1
      // milliseconds...
      tv.set (0, 1000);
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      scheduler->set (consumer_rt_info2,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      time, time, time,
                      time,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      0,
                      RtecScheduler::OPERATION
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_ConsumerQOS_Factory consumer_qos1, consumer_qos2;
      //consumer_qos.start_disjunction_group ();
      // The types int the range [0,ACE_ES_EVENT_UNDEFINED) are
      // reserved for the EC...
      consumer_qos1.insert_type (ACE_ES_EVENT_UNDEFINED,
                                consumer_rt_info1);
      consumer_qos2.insert_type (ACE_ES_EVENT_UNDEFINED + 1,
                                consumer_rt_info2);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy1 =
        consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy2 =
        consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushConsumer_var consumer1 =
        consumer_impl1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushConsumer_var consumer2 =
        consumer_impl2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connecting consumers\n"));
      supplier_proxy1->connect_push_consumer (consumer1.in (),
                                             consumer_qos1.get_ConsumerQOS ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_proxy2->connect_push_consumer (consumer2.in (),
                                             consumer_qos2.get_ConsumerQOS ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "consumers connected\n"));

      // ****************************************************************

      Supplier supplier_impl1, supplier_impl2;

      RtecScheduler::handle_t supplier_rt_info1 =
        scheduler->create ("supplier_event_1" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                      tmp, tmp, tmp,
                      tmp,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t supplier_rt_info2 =
        scheduler->create ("supplier_event_2" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The execution times are set to reasonable values, but
      // actually they are changed on the real execution, i.e. we
      // lie to the scheduler to obtain right priorities; but we
      // don't care if the set is schedulable.
      tv.set (0, 20000);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      rate = ACE_U64_TO_U32(tmp);

      scheduler->set (supplier_rt_info2,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      tmp, tmp, tmp,
                      tmp,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      tmp,
                      0,
                      RtecScheduler::OPERATION
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::EventSourceID supplier_id1 = 1, supplier_id2 = 2;
      ACE_SupplierQOS_Factory supplier_qos1, supplier_qos2;
      supplier_qos1.insert (supplier_id1,
                           ACE_ES_EVENT_UNDEFINED,
                           supplier_rt_info1,
                           1 /* number of calls, but what does that mean? */);
      supplier_qos2.insert (supplier_id2,
                           ACE_ES_EVENT_UNDEFINED + 1,
                           supplier_rt_info2,
                           1 /* number of calls, but what does that mean? */);

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy1 =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy2 =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushSupplier_var supplier1 =
        supplier_impl1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushSupplier_var supplier2 =
        supplier_impl2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connecting suppliers\n"));
      consumer_proxy1->connect_push_supplier (supplier1.in (),
                                             supplier_qos1.get_SupplierQOS ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      consumer_proxy2->connect_push_supplier (supplier2.in (),
                                             supplier_qos2.get_SupplierQOS ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));

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
				     configs.out (),
				     anomalies.out ()
				     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
	  
      // Dump the schedule to a file..
      ACE_Scheduler_Factory::dump_schedule (infos.in (),
					    configs.in (),
					    anomalies.in (),
					    "schedule.out");

      // ****************************************************************

      ACE_DEBUG ((LM_DEBUG, "activating EC\n"));
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

      ACE_DEBUG ((LM_DEBUG, "Pushing events\n"));

      // Generate a few events....

      RtecEventComm::EventSet event1 (1);
      event1.length (1);
      event1[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event1[0].header.source = supplier_id1;
      event1[0].header.ttl    = 1;

      RtecEventComm::EventSet event2 (1);
      event2.length (1);
      event2[0].header.type   = ACE_ES_EVENT_UNDEFINED + 1;
      event2[0].header.source = supplier_id2;
      event2[0].header.ttl    = 1;

      for (int i = 0; i != 200; ++i)
        {
          if (i % 2 == 0)
            {
              consumer_proxy1->push (event1 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              consumer_proxy2->push (event2 ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          ACE_Time_Value rate (0, 10000);
          ACE_OS::sleep (rate);
        }

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
  ACE_Get_Opt get_opts (argc, argv, "c");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

// ****************************************************************

// Instantiate the templates used by the Reconfig scheduler above

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<RtecScheduler::Config_Info>;
template class auto_ptr<RtecScheduler::RT_Info>;
template class auto_ptr<TAO_Reconfig_Scheduler_Entry>;
template class ACE_Auto_Basic_Ptr<RtecScheduler::Config_Info>;
template class ACE_Auto_Basic_Ptr<RtecScheduler::RT_Info>;
template class ACE_Auto_Basic_Ptr<TAO_Reconfig_Scheduler_Entry>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::Config_Info *>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::Dependency_Set *>;
template class ACE_Hash_Map_Entry<int, RtecScheduler::RT_Info *>;
template class ACE_RB_Tree<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Node<const char *, RtecScheduler::RT_Info *>;
template class ACE_RB_Tree_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Iterator_Base<char const *, RtecScheduler::RT_Info *, ACE_Less_Than<char const *>, TAO_SYNCH_MUTEX>;
template class ACE_RB_Tree_Reverse_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>;
template class TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Dependency_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_DFS_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Priority_Visitor<TAO_MUF_Reconfig_Sched_Strategy>;
template class TAO_RSE_Propagation_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_SCC_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>;
template class TAO_RSE_Utilization_Visitor<TAO_MUF_Reconfig_Sched_Strategy>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<RtecScheduler::Config_Info>
#pragma instantiate auto_ptr<RtecScheduler::RT_Info>
#pragma instantiate auto_ptr<TAO_Reconfig_Scheduler_Entry>
#pragma instantiate ACE_Auto_Basic_Ptr<RtecScheduler::Config_Info>
#pragma instantiate ACE_Auto_Basic_Ptr<RtecScheduler::RT_Info>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Reconfig_Scheduler_Entry>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Config_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::Dependency_Set *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int, RtecScheduler::RT_Info *, ACE_Hash<int>, ACE_Equal_To<int>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Config_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::Dependency_Set*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<int,RtecScheduler::RT_Info*,ACE_Hash<int>,ACE_Equal_To<int>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::Config_Info *>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::Dependency_Set *>
#pragma instantiate ACE_Hash_Map_Entry<int, RtecScheduler::RT_Info *>
#pragma instantiate ACE_RB_Tree<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Node<const char *, RtecScheduler::RT_Info *>
#pragma instantiate ACE_RB_Tree_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Iterator_Base<char const *, RtecScheduler::RT_Info *, ACE_Less_Than<char const *>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<const char *, RtecScheduler::RT_Info *, ACE_Less_Than<const char *>, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Dependency_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_DFS_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Priority_Visitor<TAO_MUF_Reconfig_Sched_Strategy>
#pragma instantiate TAO_RSE_Propagation_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_SCC_Visitor<TAO_MUF_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX>
#pragma instantiate TAO_RSE_Utilization_Visitor<TAO_MUF_Reconfig_Sched_Strategy>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
