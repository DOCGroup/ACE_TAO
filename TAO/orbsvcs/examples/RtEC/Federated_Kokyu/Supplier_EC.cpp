// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Limit.h"
#include <ace/OS_NS_sys_time.h>

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "Supplier.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
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

class Supplier_EC : public Kokyu_EC
{
  Supplier supplier_impl1;
  Supplier supplier_impl2_1;
  Supplier supplier_impl2_2;
  Timeout_Consumer timeout_consumer_impl1;
  Timeout_Consumer timeout_consumer_impl2_1;
  Timeout_Consumer timeout_consumer_impl2_2;
  Consumer consumer_impl1;
  Consumer consumer_impl2_1;
  TAO_EC_Gateway_Sched gateway;
public:
  Supplier_EC()
    :
    supplier_impl1(1)
    , supplier_impl2_1(2)
    , supplier_impl2_2(3)
    , timeout_consumer_impl1(&supplier_impl1)
    , timeout_consumer_impl2_1(&supplier_impl2_1)
    , timeout_consumer_impl2_2(&supplier_impl2_2)
    , consumer_impl1() //worktime will be set later
    , consumer_impl2_1(&supplier_impl2_2) //worktime will be set later
  {
  }

  void init_gateway(CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const char* consumer_ec_ior)
  {
      CORBA::Object_var obj;
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec, consumer_ec;

      ACE_CHECK;
      obj = orb->string_to_object(consumer_ec_ior ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      consumer_ec = RtEventChannelAdmin::RtSchedEventChannel::_narrow(obj.in()
                                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var supplier_event_channel =
        this->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var consumer_event_channel =
        consumer_ec->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var supplier_scheduler =
        this->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var consumer_scheduler =
        consumer_ec->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      gateway.init(supplier_event_channel.in(),
                   consumer_event_channel.in(),
                   supplier_scheduler.in(),
                   consumer_scheduler.in(),
                   "gateway1", "gateway2"
                   ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;
            PortableServer::ObjectId_var gateway_oid =
         poa->activate_object(&gateway ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var gateway_obj =
         poa->id_to_reference(gateway_oid.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

       RtecEventChannelAdmin::Observer_var obs =
         RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
         consumer_event_channel->append_observer (obs.in () ACE_ENV_ARG_PARAMETER);
      ACE_UNUSED_ARG(local_ec_obs_handle);
      ACE_CHECK;

      consumer_ec->start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
  }

  virtual void start (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ))
  {
      RtecEventComm::EventSourceID supplier_id1 = supplier_impl1.get_id();
      RtecEventComm::EventSourceID supplier_id2_1 = supplier_impl2_1.get_id();
      RtecEventComm::EventSourceID supplier_id2_2 = supplier_impl2_2.get_id();

      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy1;
      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy2_1;
      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy2_2;
      RtecEventComm::PushSupplier_var supplier1;
      RtecEventComm::PushSupplier_var supplier2_1, supplier2_2;

      supplier1 = supplier_impl1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      supplier2_1 = supplier_impl2_1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      supplier2_2 = supplier_impl2_2._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::handle_t supplier1_rt_info =
        this->register_supplier("supplier1",
                                supplier_id1,
                                ACE_ES_EVENT_UNDEFINED,
                                supplier1.in(),
                                consumer_proxy1.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::handle_t supplier2_1_rt_info =
        this->register_supplier("supplier2_1",
                                supplier_id2_1,
                                ACE_ES_EVENT_UNDEFINED+1,
                                supplier2_1.in(),
                                consumer_proxy2_1.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::handle_t supplier2_2_rt_info =
        this->register_supplier("supplier2_2",
                                supplier_id2_2,
                                ACE_ES_EVENT_UNDEFINED+2,
                                supplier2_2.in(),
                                consumer_proxy2_2.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      //ACE_UNUSED_ARG(supplier2_2_rt_info);

      supplier_impl1.set_consumer_proxy(consumer_proxy1.in());
      supplier_impl2_1.set_consumer_proxy(consumer_proxy2_1.in());
      supplier_impl2_2.set_consumer_proxy(consumer_proxy2_2.in());

      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));

      //Timer Registration part
      //Timeout consumers for the suppliers.

      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy1;
      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy2_1;
      RtecEventComm::PushConsumer_var safe_timeout_consumer1;
      RtecEventComm::PushConsumer_var safe_timeout_consumer2_1;

      safe_timeout_consumer1= timeout_consumer_impl1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      safe_timeout_consumer2_1= timeout_consumer_impl2_1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      //TODO: Setup timeouts to test EDF
      ACE_Time_Value tv(4,0); //Period

      RtEventChannelAdmin::SchedInfo info;
      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecScheduler::handle_t supplier1_timeout_consumer_rt_info =
        this->register_consumer("supplier1_timeout_consumer",
                                   info,
                                   ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                   safe_timeout_consumer1.in(),
                                   timeout_supplier_proxy1.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      tv.set (6, 0); //Period
      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;

      RtecScheduler::handle_t supplier2_1_timeout_consumer_rt_info =
        this->register_consumer("supplier2_1_timeout_consumer",
                                info,
                                ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                safe_timeout_consumer2_1.in(),
                                timeout_supplier_proxy2_1.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG, "timeout consumers connected\n"));

      // *************************************************************
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      tv.set(2,0);
      consumer_impl1.setWorkTime(tv);
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier1;

      tv.set(6,0);
      consumer_impl2_1.setWorkTime(tv);
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier2_1;

      //consumer's rate will get propagated from the supplier.
      //so no need to specify a period here. Specifying
      //criticality is crucial since it propagates from
      //consumer to supplier.
      tv.set(0,0); //Period
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecEventComm::PushConsumer_var consumer1 =
        consumer_impl1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer1_rt_info =
        this->register_consumer("consumer1",
                                info,
                                ACE_ES_EVENT_UNDEFINED,
                                consumer1.in(),
                                proxy_supplier1.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_UNUSED_ARG(consumer1_rt_info);

      tv.set(0,0); //Period
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecEventComm::PushConsumer_var consumer2_1 =
        consumer_impl2_1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer2_1_rt_info =
        this->register_consumer("consumer2_1",
                                info,
                                ACE_ES_EVENT_UNDEFINED+1,
                                consumer2_1.in(),
                                proxy_supplier2_1.out()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //ACE_UNUSED_ARG(consumer2_1_rt_info);

      ACE_DEBUG ((LM_DEBUG, "consumers connected\n"));

      // ****************************************************************
      //Registering dependency between timeout consumers and our suppliers
      //with the scheduler

      this->add_dependency (supplier1_timeout_consumer_rt_info,
                                 supplier1_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->add_dependency (supplier2_1_timeout_consumer_rt_info,
                                 supplier2_1_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      //TODO: Why does supplier2_2 need this to have dependencies fulfilled?
      this->add_dependency (consumer2_1_rt_info,
                                 supplier2_2_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      Kokyu_EC::start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
  }
};


int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
#ifdef ACE_HAS_DSUI
  ds_control* ds_cntl = new ds_control ("Federated_Test_Supplier","supplier_enabled.dsui");
#endif //ACE_HAS_DSUI

  TAO_EC_Kokyu_Factory::init_svcs ();
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  //@BT
  //DSUI_EVENT_LOG(MAIN_GROUP_FAM, START,1,0,NULL);
  ACE_Time_Value tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t START at %u\n",tv.msec()));
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, START,0,0,NULL);

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
                      "Usage:  %s -s <rms|muf|edf>"
                      "\n",
                      argv [0]));
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

      Supplier_EC supplier_ec;
      if (supplier_ec.init(sched_type.c_str(), poa.in()) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);

      supplier_ec.init_gateway(orb.in(),
                               poa.in(),
                               "file://consumer_ec.ior" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec_ior =
        supplier_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(supplier_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);


      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      supplier_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_ACTIVATED at %u\n",tv.msec()));
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);
      //DSUI_EVENT_LOG (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_STARTED at %u\n",tv.msec()));
      DSUI_EVENT_LOG (WORKER_GROUP_FAM, WORKER_STARTED, 0, 0, NULL);

#ifdef ACE_HAS_DSUI
      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance(), ds_cntl);
#else
      EC_Event_Limit* e_limit = new EC_Event_Limit (TAO_ORB_Core_instance());
#endif //ACE_HAS_DSUI
      ACE_Time_Value ticker (300);
      orb->orb_core()->reactor()->schedule_timer(e_limit,0, ticker);


      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      //@BT: ORB shutting down; currently, this isn't expected to happen
      //DSUI_EVENT_LOG (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 1, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t CALL_SERVER_SHUTDOWN at %u\n",tv.msec()));
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 0, 0, NULL);

      //@BT: Scheduler shuts down with the EC and ORB
      //DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 1, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t SCHEDULER_SHUTDOWN at %u\n",tv.msec()));
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 0, 0, NULL);

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

      //@BT: Done clean up
      //DSUI_EVENT_LOG (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 1, 0, NULL);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t AFTER_SERVER_SHUTDOWN at %u\n",tv.msec()));
      DSUI_EVENT_LOG (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 0, 0, NULL);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;

  //@BT
  //DSUI_EVENT_LOG(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
  tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t STOP at %u\n",tv.msec()));
  DSUI_EVENT_LOG(MAIN_GROUP_FAM, STOP, 1, 0, NULL);

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
                           "Usage:  %s -s <rms|muf|edf>"
                           "\n",
                           argv [0]),
                          -1);
      }
  if (ior_output_file == 0)
    ior_output_file = ACE_OS::fopen ("supplier_ec.ior", "w");
  // Indicates sucessful parsing of the command line
  return 0;
}
