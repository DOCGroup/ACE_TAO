// $Id$

#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "ace/OS_NS_strings.h" //for ACE_OS::strcasecmp
#include "ace/OS_NS_sys_time.h" // for ACE_OS::gettimeofday

#include "Kokyu/Dispatch_Deferrer.h"

#include "Kokyu_EC.h"

namespace {

  typedef TAO_Reconfig_Scheduler<TAO_RMS_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_RMS_SCHED_TYPE;
  typedef TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_MUF_SCHED_TYPE;
  typedef TAO_Reconfig_Scheduler<TAO_EDF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RECONFIG_EDF_SCHED_TYPE;

  POA_RtecScheduler::Scheduler*
    create_scheduler(const char* schedule_discipline)
  {
    POA_RtecScheduler::Scheduler* sched_impl=0;
    if (ACE_OS::strcasecmp(schedule_discipline, "rms") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Creating RMS scheduler\n"));
      ACE_NEW_RETURN (sched_impl,
        RECONFIG_RMS_SCHED_TYPE,
        0);
    }
    else if (ACE_OS::strcasecmp(schedule_discipline, "muf") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Creating MUF scheduler\n"));
      ACE_NEW_RETURN (sched_impl,
        RECONFIG_MUF_SCHED_TYPE,
        0);
    }
    else if (ACE_OS::strcasecmp(schedule_discipline, "edf") == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Creating EDF scheduler\n"));
      ACE_NEW_RETURN (sched_impl,
        RECONFIG_EDF_SCHED_TYPE,
        0);
    }

    //@BT: Decided on which kind of scheduling to do, same as DT SCHEDULE SETUP
    //DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULE_SETUP, 1, strlen (policy), policy);
    ACE_DEBUG((LM_DEBUG,"Kokyu_EC thread %t SCHEDULE_SETUP at %u\n",ACE_OS::gettimeofday().msec()));
    return sched_impl;
  }
} //anonymous namespace

Kokyu_EC::Kokyu_EC(void)
{
}

Kokyu_EC::~Kokyu_EC(void)
{
  //Need ACE_ENV_ARG_PARAMETER for these?
  for(size_t i=0; i<suppliers_.size(); ++i) {
    suppliers_[i]->disconnect_push_supplier();
    delete suppliers_[i];
  }
  for(size_t i=0; i<timeout_consumers_.size(); ++i) {
    timeout_consumers_[i]->disconnect_push_consumer();
    delete timeout_consumers_[i];
  }
  for(size_t i=0; i<consumers_.size(); ++i) {
    consumers_[i]->disconnect_push_consumer();
    delete consumers_[i];
  }

  this->ec_impl_->destroy();

}

int
Kokyu_EC::init(const char* schedule_discipline, PortableServer::POA_ptr poa)
{
  ACE_TRY_NEW_ENV {

    scheduler_impl_ = create_scheduler(schedule_discipline);
    if (scheduler_impl_.in() == 0) return -1;

    scheduler_ = scheduler_impl_->_this();

    TAO_EC_Event_Channel_Attributes attrib(poa, poa);
    attrib.scheduler = scheduler_.in();

    TAO_EC_Event_Channel* ec;
    ACE_NEW_RETURN(ec, TAO_EC_Event_Channel(attrib), -1);
    ec_impl_ = ec;

    consumer_admin_ = ec_impl_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    supplier_admin_ = ec_impl_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
  ACE_CATCHALL {
    return -1;
  }
  ACE_ENDTRY;
  return 0;
}


RtEventChannelAdmin::handle_t
Kokyu_EC::register_consumer (
        const char * entry_point,
        const RtEventChannelAdmin::SchedInfo & info,
        RtecEventComm::EventType cons_type,
        RtecEventComm::PushConsumer_ptr consumer,
        RtecEventChannelAdmin::ProxyPushSupplier_out proxy_supplier
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ))
{
  RtecScheduler::handle_t consumer1_rt_info =
    scheduler_impl_->create (entry_point ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  scheduler_impl_->set (consumer1_rt_info,
    info.criticality,
    info.wc_time,
    info.typical_time,
    info.cached_time,
    info.period,
    info.importance,
    info.quantum,
    info.threads,
    info.info_type
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ConsumerQOS_Factory consumer_qos1;
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC register_consumer() registering event type %d\n",cons_type));
  if (cons_type != ACE_ES_EVENT_INTERVAL_TIMEOUT)
    {
      ACE_DEBUG((LM_DEBUG,"\t(Generic type)\n"));
      consumer_qos1.insert_type (cons_type,  consumer1_rt_info);
    }
  else
    {
      ACE_DEBUG((LM_DEBUG,"\t(Periodic timeout)\n"));
      consumer_qos1.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                 info.period, //in 100s of nanosec
                                 consumer1_rt_info);
    }

  proxy_supplier =
    consumer_admin_->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy_supplier->connect_push_consumer (consumer,
    consumer_qos1.get_ConsumerQOS ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"Kokyu_EC register_consumer() returning %d\n",consumer1_rt_info));
  return consumer1_rt_info;
} //register_consumer()

RtEventChannelAdmin::handle_t
Kokyu_EC::register_supplier (
        const char * entry_point,
        RtecEventComm::EventSourceID source,
        const EventType_Vector& supp_types,
        RtecEventComm::PushSupplier_ptr supplier,
        RtecEventChannelAdmin::ProxyPushConsumer_out proxy_consumer
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ))
{
  RtecScheduler::handle_t supplier1_rt_info =
    scheduler_impl_->create (entry_point ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_SupplierQOS_Factory supplier_qos1;
  for(size_t i=0; i<supp_types.size(); ++i)
    {
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC::register_supplier() inserting type %d into RT_Info %d\n",supp_types[i],supplier1_rt_info));
      supplier_qos1.insert (source,
                            supp_types[i],
                            supplier1_rt_info,
                            1 /* number of calls, but what does that mean? */);
    }

  proxy_consumer =
    supplier_admin_->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy_consumer->connect_push_supplier (supplier,
    supplier_qos1.get_SupplierQOS ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return supplier1_rt_info;
} //register_suppliers()

void
Kokyu_EC::add_dependency (
        RtEventChannelAdmin::handle_t handle,
        RtEventChannelAdmin::handle_t dependency,
        CORBA::Long number_of_calls,
        RtecScheduler::Dependency_Type_t dependency_type
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::SYNCHRONIZATION_FAILURE
        , RtecScheduler::UNKNOWN_TASK
      ))
{
  scheduler_impl_->add_dependency (handle,
                                   dependency,
                                   number_of_calls,
                                   dependency_type
                                   ACE_ENV_ARG_PARAMETER);
}

void
Kokyu_EC::start (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ))
{
      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Dependency_Set_var dependencies;
      RtecScheduler::Scheduling_Anomaly_Set unsafe_anomalies;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

      int min_os_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      int max_os_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      scheduler_impl_->compute_scheduling (min_os_priority,
                                     max_os_priority,
                                     infos.out (),
                                     dependencies.out (),
                                     configs.out (),
                                     anomalies.out ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      ec_impl_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      //@BT: EC activated is roughly equivalent to having the DT scheduler ready to run
      //DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_STARTED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC thread %t SCHEDULER_STARTED at %u\n",ACE_OS::gettimeofday().msec()));
}

RtecEventChannelAdmin::EventChannel_ptr
Kokyu_EC::event_channel (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ec_impl_->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecScheduler::Scheduler_ptr
Kokyu_EC::scheduler (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RtecScheduler::Scheduler::_duplicate(scheduler_.in());
}

///Takes ownership of Supplier and Timeout_Consumer
void
Kokyu_EC::add_supplier_with_timeout(
                                    Supplier * supplier_impl,
                                    const char * supp_entry_point,
                                    const EventType_Vector& supp_types,
                                    Timeout_Consumer * timeout_consumer_impl,
                                    const char * timeout_entry_point,
                                    ACE_Time_Value period,
                                    RtecScheduler::Criticality_t crit,
                                    RtecScheduler::Importance_t imp
                                    ACE_ENV_ARG_DECL
                                    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  add_supplier(supplier_impl,supp_entry_point,supp_types ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  add_timeout_consumer(supplier_impl,timeout_consumer_impl,timeout_entry_point,period,crit,imp ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

///Takes ownership of Timeout_Consumer
void
Kokyu_EC::add_timeout_consumer(
                               Supplier * supplier_impl,
                               Timeout_Consumer * timeout_consumer_impl,
                               const char * timeout_entry_point,
                               ACE_Time_Value period,
                               RtecScheduler::Criticality_t crit,
                               RtecScheduler::Importance_t imp
                               ACE_ENV_ARG_DECL
                               )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy;
  RtecEventComm::PushConsumer_var safe_timeout_consumer;

  safe_timeout_consumer= timeout_consumer_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtEventChannelAdmin::SchedInfo info;
  info.criticality = crit;
  info.period = time_val_to_period (period);
  info.importance = imp;
  info.threads = 0;
  info.info_type = RtecScheduler::OPERATION;

  RtecScheduler::handle_t supplier_timeout_consumer_rt_info =
    this->register_consumer(timeout_entry_point,
                            info,
                            ACE_ES_EVENT_INTERVAL_TIMEOUT,
                            safe_timeout_consumer.in(),
                            timeout_supplier_proxy.out()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  //don't need to save supplier_timeout_consumer_rt_info because only used to set dependency here:

  this->add_dependency (supplier_timeout_consumer_rt_info,
                        supplier_impl->rt_info(),
                        1,
                        RtecBase::TWO_WAY_CALL
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->timeout_consumers_.push_back(timeout_consumer_impl);
} //add_supplier_with_timeout()

///Takes ownership of Supplier
void
Kokyu_EC::add_supplier(
                       Supplier * supplier_impl,
                       const char * entry_point,
                       const EventType_Vector& supp_types
                       ACE_ENV_ARG_DECL
                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  Supplier::SourceID supplier_id = supplier_impl->get_id();

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy;
  RtecEventComm::PushSupplier_var supplier;

  supplier = supplier_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecScheduler::handle_t supplier_rt_info =
    this->register_supplier(entry_point,
                            supplier_id,
                            supp_types, //type,
                            supplier.in(),
                            consumer_proxy.out()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_impl->set_consumer_proxy(consumer_proxy);
  supplier_impl->rt_info(supplier_rt_info);

  this->suppliers_.push_back(supplier_impl);
} //add_supplier()

///Takes ownership of Consumer and Supplier
void
Kokyu_EC::add_consumer_with_supplier(
                                     Consumer * consumer_impl,
                                     const char * cons_entry_point,
                                     ACE_Time_Value cons_period,
                                     const EventType_Vector& cons_types,
                                     RtecScheduler::Criticality_t cons_crit,
                                     RtecScheduler::Importance_t cons_imp,
                                     Supplier * supplier_impl,
                                     const char * supp_entry_point,
                                     const EventType_Vector& supp_types
                                     ACE_ENV_ARG_DECL
                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  add_consumer(consumer_impl,cons_entry_point,cons_period,cons_types,cons_crit,cons_imp ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  add_supplier(supplier_impl,supp_entry_point,supp_types ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //This is kind of redundant since we have the DUMMY supplier, but we should at least
  //make a nod to a dependency map which somewhat reflects reality. To go all the way
  //would mean adding dependencies for each of the consumer's RT_Infos.

  //ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_consumer_with_supplier() adding dependency %d\n",*cons_info));
  this->add_dependency (supplier_impl->rt_info(),
                        consumer_impl->rt_info(),
                        1,
                        RtecBase::ONE_WAY_CALL
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
} //add_consumer_with_supplier()

///Takes ownership of Consumer
void
Kokyu_EC::add_consumer(
                       Consumer * consumer_impl,
                       const char * entry_point,
                       ACE_Time_Value period,
                       const EventType_Vector& cons_types,
                       RtecScheduler::Criticality_t crit,
                       RtecScheduler::Importance_t imp
                       ACE_ENV_ARG_DECL
                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier;

  //Specifying criticality is crucial since it propagates from
  //consumer to supplier.
  RtEventChannelAdmin::SchedInfo info;
  info.criticality = crit;
  info.period = time_val_to_period (period);
  info.importance = imp;
  info.threads = 0;
  info.info_type = RtecScheduler::OPERATION;

  RtecEventComm::PushConsumer_var consumer =
    consumer_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecScheduler::handle_t consumer_rt_info;
  for(size_t i=0; i<cons_types.size(); ++i)
    {
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC add_consumer() registering event type %d\n",cons_types[i]));

      std::stringstream entry_pt;
      entry_pt << entry_point << "_" << cons_types[i]; //need a different entry point per type

      consumer_rt_info = this->register_consumer(entry_pt.str().c_str(), //entry_point,
                                                 info,
                                                 cons_types[i],
                                                 consumer.in(),
                                                 proxy_supplier.out()
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  consumer_impl->rt_info(consumer_rt_info);

  this->consumers_.push_back(consumer_impl);
} //add_consumer()

void
Kokyu_EC::add_dummy_supplier(const EventType_Vector& supp_types
                             ACE_ENV_ARG_DECL
                             )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  //Gateways register as consumers of ALL the consumers in the
  //remote EC, so we need a generalized RT_Info to cover all the
  //dependencies not actually provided by this node. That is, we
  //need an RT_Info for a fictional supplier which publishes all
  //known event types.
  RtecScheduler::handle_t general_rt_info =
    this->scheduler_->create ("Generalized Supplier (DUMMY)" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_Time_Value exec(0,500); //some arbitrary execution time
  RtecScheduler::Time exec_time;
  ORBSVCS_Time::Time_Value_to_TimeT(exec_time,exec);
  ACE_Time_Value period(0,500); //some arbitrary period
  RtecScheduler::Time period_time;
  ORBSVCS_Time::Time_Value_to_TimeT(period_time,period);
  this->scheduler_->set (general_rt_info,
                         RtecScheduler::LOW_CRITICALITY, //doesn't matter for EDF anyway
                         exec_time,
                         exec_time,
                         exec_time,
                         period_time,
                         RtecScheduler::LOW_IMPORTANCE, //doesn't matter for EDF anyway
                         0, //quantum
                         0, //threads
                         RtecScheduler::OPERATION
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Supplier::SourceID general_id = 3000;  //we assume 3000 is big enough to not overlap w/ actual suppliers
  ACE_SupplierQOS_Factory general_qos;
  //NOTE that this is kind of hard-cody since it assumes types between UNDEF+6 and UNDEF+9
  for(size_t i=0; i<supp_types.size(); ++i)
    {
      general_qos.insert (general_id,
                          supp_types[i],
                          general_rt_info,
                          1 //number of calls
                          );
    }

  //we need to actually connect to the EC so the scheduler will set up the dependencies right
  Supplier::EventType supp_type = general_id; //some arbitrary type, never pushed
  Supplier * general_impl;
  ACE_NEW(general_impl,
          Supplier(general_id,supp_type,supp_type));
          //dummy supplier since nothing will trigger it to push!

  RtecEventComm::PushSupplier_var general;
  general = general_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;
  proxy_consumer = this->supplier_admin_->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy_consumer->connect_push_supplier (general.in(),
                                         general_qos.get_SupplierQOS()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  general_impl->set_consumer_proxy(proxy_consumer);
  general_impl->rt_info(general_rt_info);

  this->suppliers_.push_back(general_impl);
} //add_dummy_supplier

//*************************************************************

Reactor_Task::Reactor_Task (void)
  : initialized_(0)
    , react_(0)
{
}

Reactor_Task::~Reactor_Task (void)
{
}

int
Reactor_Task::initialize(void)
{
  /*
  //We need to set the ACE_Reactor::instance() to be the ORB
  //reactor so Kokyu's RG implementation can use it w/o creating
  //an extra thread to run the reactor event loop. I hope this
  //doesn't screw something else up!
  //use Select_Reactor explicitly?
  ACE_Reactor *reactor; //TODO: how clean up reactor and stop thread?
  ACE_NEW_RETURN(reactor,
                 ACE_Reactor,
                 -1);
  reactor->open(ACE_Select_Reactor_Impl::DEFAULT_SIZE);
  ACE_Reactor::instance(reactor);

  this->react_ = reactor;
  */
  //this->react_ = ACE_Reactor::instance();
  this->react_ = Kokyu::Dispatch_Deferrer::Singleton_Reactor::instance();
  //assume reactor is already opened!

  this->initialized_ = 1;

  return 0;
} //initialize()

ACE_Reactor *
Reactor_Task::reactor(void)
{
  return this->react_;
}

/// Process the events in the queue.
int
Reactor_Task::svc (void)
{
  ACE_DEBUG((LM_DEBUG,"Reactor_Task (%P|%t) svc(): ENTER\n"));

  if (!this->initialized_)
    {
      this->initialize();
    }

  this->react_->owner(ACE_Thread::self()); //set this thread as owner

  int err = this->react_->run_reactor_event_loop();
  if (err < 0)
    {
      ACE_DEBUG((LM_ERROR,"Reactor_Task (%t) error running Reactor event loop\n"));
    }

  ACE_DEBUG((LM_DEBUG,"Reactor_Task (%P|%t) svc(): LEAVE\n"));
  return 0;
} //svc()
