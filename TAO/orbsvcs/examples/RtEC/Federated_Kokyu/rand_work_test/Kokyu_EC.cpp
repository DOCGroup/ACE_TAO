// $Id$

#include "ace/OS_NS_strings.h" //for ACE_OS::strcasecmp
#include "ace/OS_NS_sys_time.h" // for ACE_OS::gettimeofday
#include "ace/Reactor.h"

#include <orbsvcs/Sched/Reconfig_Scheduler.h>
#include <orbsvcs/Runtime_Scheduler.h>
#include <orbsvcs/Event_Service_Constants.h>
#include <orbsvcs/Event_Utilities.h>
#include <orbsvcs/Scheduler_Factory.h>
#include <orbsvcs/Event/EC_Event_Channel.h>
#include <orbsvcs/Event/EC_Default_Factory.h>
#include <orbsvcs/Event/EC_Kokyu_Factory.h>

#include "Kokyu_EC.h"
#include "RtSchedEventChannelC.h"

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
  this->gateways.open();
}

Kokyu_EC::~Kokyu_EC(void)
{
  //TODO: close down gateways

  //now delete them and their IOR strings
  Gateway_Map::iterator iter = this->gateways.begin();
  Gateway_Map::iterator done = this->gateways.end();
  while(iter != done)
    {
      Gateway_Map::ENTRY entry = *iter;

      Gateway_Map::KEY key = entry.ext_id_;
      Gateway_Map::VALUE val = entry.int_id_;
      //we can delete as long as we don't unbind
      //don't delete keys since we didn't allocate them
      delete key;
      delete val;

      ++iter;
    }
  this->gateways.unbind_all();

  this->gateways.close();

  for(size_t i=0; i<suppliers_.size(); ++i) {
    delete suppliers_[i];
  }
  for(size_t i=0; i<consumers_.size(); ++i) {
    delete consumers_[i];
  }
  if (this->reactor_ != 0)
    {
      for(size_t i=0; i<timer_handles_.size(); ++i) {
        this->reactor_->cancel_timer(timer_handles_[i]);
      }
      this->timer_handles_.clear();
      for(size_t i=0; i<task_triggers_.size(); ++i) {
        delete this->task_triggers_[i].handler;
      }
      this->task_triggers_.clear();
      /*
      for(size_t i=0; i<timeout_handlers_.size(); ++i) {
        delete this->timeout_handlers_[i];
      }
      this->timeout_handlers_.clear();
      */
    }
}

int
Kokyu_EC::init(bool time_master, const char* schedule_discipline, PortableServer::POA_ptr poa, ACE_Reactor * reactor)
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
    ACE_TRY_CHECK;
    supplier_admin_ = ec_impl_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->reactor_ = reactor;
    this->time_master_ = time_master;
    this->remote_gateways_connected_ = 0;

    this->started_ = false;
    this->start_time_ = ACE_Time_Value::zero;
    this->remote_ecs_.clear();
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
        RtecEventComm::EventType type,
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

  if (type != ACE_ES_EVENT_INTERVAL_TIMEOUT)
    {
      consumer_qos1.insert_type (type,  consumer1_rt_info);
    }
  else
    {
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
  return consumer1_rt_info;
}

RtEventChannelAdmin::handle_t
Kokyu_EC::register_supplier (
        const char * entry_point,
        RtecEventComm::EventSourceID source,
        RtecEventComm::EventType type,
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
  supplier_qos1.insert (source,
    type,
    supplier1_rt_info,
    1 /* number of calls, but what does that mean? */);

  proxy_consumer =
    supplier_admin_->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy_consumer->connect_push_supplier (supplier,
    supplier_qos1.get_SupplierQOS ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return supplier1_rt_info;
}

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
      ACE_CHECK;
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
  if (this->started_)
    {
      return;
    }
  //else
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
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) SCHEDULER_STARTED at %u\n",ACE_OS::gettimeofday().msec()));

  //now we go through all the timeout_handlers and schedule them
  for(size_t i=0; i<this->task_triggers_.size(); ++i)
    {
      if (this->reactor_ != 0)
        {
          long timer_handle = this->reactor_->schedule_timer(this->task_triggers_[i].handler,
                                                             0, //arg
                                                             this->task_triggers_[i].phase, //delay
                                                             this->task_triggers_[i].period //period
                                                             );
          this->timer_handles_.push_back(timer_handle);
          ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) scheduled timeout %d with delay %isec %iusec\n",timer_handle,this->task_triggers_[i].phase.sec(),this->task_triggers_[i].phase.usec()));
        }
    }

  this->started_ = true;
}

void
Kokyu_EC::notify_gateway_connection (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remote_gateways_connected_++;
}

void
Kokyu_EC::set_start_time (RtEventChannelAdmin::Time start_time ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ORBSVCS_Time::TimeT_to_Time_Value(this->start_time_,start_time);
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) set_start_time() to %isec %iusec\n",this->start_time_.sec(),this->start_time_.usec()));
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

int
Kokyu_EC::remote_gateways_connected(void)
{
  return this->remote_gateways_connected_;
}

///Takes ownership of Supplier and Timeout_Consumer
void
Kokyu_EC::add_supplier_with_timeout(
                                    Supplier * supplier_impl,
                                    const char * supp_entry_point,
                                    RtecEventComm::EventType supp_type,
                                    Supplier_Timeout_Handler * timeout_handler_impl,
                                    ACE_Time_Value phase,
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
  add_supplier(supplier_impl,supp_entry_point,supp_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //need to set supplier's criticalities and period
  RtecScheduler::Time period_timeT;
  ORBSVCS_Time::Time_Value_to_TimeT (period_timeT,period);
  scheduler_impl_->set (supplier_impl->rt_info(),
                        crit,
                        0,
                        0,
                        0,
                        period_timeT,
                        imp,
                        0,
                        0,
                        RtecScheduler::OPERATION
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //We can't schedule the timer here because we want the phase to be from when the reactor loop starts, not now!
  task_trigger_t trigger;
  trigger.handler = timeout_handler_impl;
  trigger.period = period;
  trigger.phase = phase;
  this->task_triggers_.push_back(trigger);

} //add_supplier_with_timeout()

///Takes ownership of Supplier
void
Kokyu_EC::add_supplier(
                       Supplier * supplier_impl,
                       const char * entry_point,
                       RtecEventComm::EventType type
                       ACE_ENV_ARG_DECL
                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_supplier() with id %d, entry point \"%s\", and for event type %d\n",supplier_impl->get_id(),entry_point,type));

  RtecEventComm::EventSourceID supplier_id = supplier_impl->get_id();

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy;
  RtecEventComm::PushSupplier_var supplier;

  supplier = supplier_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecScheduler::handle_t supplier_rt_info =
    this->register_supplier(entry_point,
                            supplier_id,
                            type,
                            supplier.in(),
                            consumer_proxy.out()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_impl->set_consumer_proxy(consumer_proxy.in());
  supplier_impl->rt_info(supplier_rt_info);

  this->suppliers_.push_back(supplier_impl);
  this->supplier_proxies_.push_back(supplier);
} //add_supplier()

///Takes ownership of Consumer and Supplier
void
Kokyu_EC::add_consumer_with_supplier(
                                     Consumer * consumer_impl,
                                     const char * cons_entry_point,
                                     ACE_Time_Value cons_period,
                                     RtecEventComm::EventType cons_type,
                                     RtecScheduler::Criticality_t cons_crit,
                                     RtecScheduler::Importance_t cons_imp,
                                     Supplier * supplier_impl,
                                     const char * supp_entry_point,
                                     RtecEventComm::EventType supp_type
                                     ACE_ENV_ARG_DECL
                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  add_consumer(consumer_impl,cons_entry_point,cons_period,cons_type,cons_crit,cons_imp ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  add_supplier(supplier_impl,supp_entry_point,supp_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->add_dependency (consumer_impl->rt_info(),
                        supplier_impl->rt_info(),
                        1,
                        RtecBase::TWO_WAY_CALL
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
} //add_consumer_with_supplier()

///Takes ownership of Consumer
void
Kokyu_EC::add_consumer(
                       Consumer * consumer_impl,
                       const char * entry_point,
                       ACE_Time_Value period,
                       RtecEventComm::EventType cons_type,
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
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_consumer() with entry point \"%s\" and for event type %d\n",entry_point,cons_type));

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

  RtecScheduler::handle_t consumer_rt_info =
    this->register_consumer(entry_point,
                            info,
                            cons_type,
                            consumer.in(),
                            proxy_supplier.out()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_impl->rt_info(consumer_rt_info);

  this->consumers_.push_back(consumer_impl);
  this->consumer_proxies_.push_back(consumer);
  this->push_suppliers_.push_back(proxy_supplier);
} //add_consumer()

void
Kokyu_EC::set_up_last_subtask (subtask_t subtask,
                               RtecEventComm::EventType in_type ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  Consumer * consumer_impl;
  ACE_NEW(consumer_impl,
          Consumer());

  consumer_impl->setWorkTime(subtask.exec);

  std::stringstream cons_entry;
  cons_entry << "consumer" << subtask.task_index << "_" << subtask.subtask_index;

  add_consumer(consumer_impl, //stores impl inside Kokyu_EC
               cons_entry.str().c_str(),
               subtask.period,
               in_type,
               RtecScheduler::VERY_LOW_CRITICALITY,
               RtecScheduler::VERY_LOW_IMPORTANCE
               ACE_ENV_ARG_PARAMETER
               );
  ACE_CHECK;

} //set_up_last_subtask()

void
Kokyu_EC::set_up_first_subtask (subtask_t subtask,
                                RtecEventComm::EventSourceID supp_id1, RtecEventComm::EventSourceID supp_id2,
                                RtecEventComm::EventType in_type, RtecEventComm::EventType out_type ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  Supplier *supplier_impl;
  Supplier_Timeout_Handler *timeout_handler_impl;
  ACE_NEW(supplier_impl,
          Supplier(supp_id1));
  ACE_NEW(timeout_handler_impl,
          Supplier_Timeout_Handler(supplier_impl));

  std::stringstream supp_entry;
  supp_entry << "supplier" << subtask.task_index << "_" << subtask.subtask_index;

  add_supplier_with_timeout(supplier_impl, //stores impl inside Kokyu_EC
                            supp_entry.str().c_str(),
                            in_type,
                            timeout_handler_impl, //stores impl inside Kokyu_EC
                            subtask.phase,
                            subtask.period,
                            RtecScheduler::LOW_CRITICALITY, //these shouldn't really matter for EDF
                            RtecScheduler::LOW_IMPORTANCE
                            ACE_ENV_ARG_PARAMETER
                            );
  ACE_CHECK;

  set_up_middle_subtask(subtask,supp_id1,supp_id2,in_type,out_type ACE_ENV_ARG_PARAMETER);
} //set_up_first_subtask

void
Kokyu_EC::set_up_middle_subtask (subtask_t subtask,
                                 RtecEventComm::EventSourceID supp_id, RtecEventComm::EventSourceID next_supp_id,
                                 RtecEventComm::EventType in_type, RtecEventComm::EventType out_type ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  Supplier *next_supplier_impl;
  ACE_NEW(next_supplier_impl,
          Supplier(next_supp_id));
  Consumer * consumer_impl;
  ACE_NEW(consumer_impl,
          Consumer(next_supplier_impl));

  consumer_impl->setWorkTime(subtask.exec);

  std::stringstream cons_entry;
  cons_entry << "consumer" << subtask.task_index << "_" << subtask.subtask_index;

  std::stringstream next_entry;
  next_entry << "supplier" << subtask.task_index << "_" << (subtask.subtask_index+1);

  add_consumer_with_supplier(consumer_impl, //stores impl inside Kokyu_EC
                             cons_entry.str().c_str(),
                             subtask.period,
                             in_type,
                             RtecScheduler::VERY_LOW_CRITICALITY,
                             RtecScheduler::VERY_LOW_IMPORTANCE,
                             next_supplier_impl, //stores impl inside Kokyu_EC
                             next_entry.str().c_str(),
                             out_type
                             ACE_ENV_ARG_PARAMETER
                             );
  ACE_CHECK;

} //set_up_middle_subtask()

void
Kokyu_EC::init_gateway(CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr poa,
                       const char* consumer_ec_ior,
                       const char* ior_output_filename
                       ACE_ENV_ARG_DECL)
{
  if (this->gateways.find(consumer_ec_ior) == 0)
    {
      //Already a gateway for that EC
      ACE_DEBUG((LM_DEBUG,"Node1_EC (%P|%t) init_gateway(): Tried to create already-existing gateway for %s\n",
                 consumer_ec_ior));
      return;
    }

  CORBA::Object_var obj;
  RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec, consumer_ec;
  ACE_CHECK;

  ACE_Time_Value sleeptime(0,500000);
  while(ACE_OS::filesize(consumer_ec_ior) <= 0)
    {
      //while file doesn't exist
      ACE_DEBUG((LM_DEBUG,"Node1_EC (%P|%t) init_gateway(): waiting for IOR file %s\n",consumer_ec_ior));
      ACE_OS::sleep(sleeptime);
    }

  std::stringstream ior_file;
  ior_file << "file://" << consumer_ec_ior;

  obj = orb->string_to_object(ior_file.str().c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil(obj.in()))
    {
      ACE_DEBUG((LM_DEBUG,"Unable to read IOR file for remote EC; Object_var is nil.\n"));
      return;
    }

  //WARNING: if consumer_ec_ior is not a readable file, obj is null!
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

  TAO_EC_Gateway_Sched *gateway;
  ACE_NEW(gateway,
          TAO_EC_Gateway_Sched() );

  std::stringstream remote_entry_point;
  remote_entry_point << ior_output_filename << " -> " << consumer_ec_ior;

  gateway->init(supplier_event_channel.in(),
                consumer_event_channel.in(),
                supplier_scheduler.in(),
                consumer_scheduler.in(),
                ior_output_filename, //consumer entry point goes first for some reason
                remote_entry_point.str().c_str() //supplier entry point
                ACE_ENV_ARG_PARAMETER);

  this->gateways.bind(consumer_ec_ior,gateway);

  ACE_CHECK;
  PortableServer::ObjectId_var gateway_oid =
    poa->activate_object(gateway ACE_ENV_ARG_PARAMETER);
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

  consumer_ec->notify_gateway_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if(!CORBA::is_nil(consumer_ec.in()))
    {
      this->remote_ecs_.push_back(consumer_ec);
    }

  //consumer_ec->start(ACE_ENV_SINGLE_ARG_PARAMETER);
  //ACE_CHECK;
} //init_gateway()

bool
Kokyu_EC::time_master(void)
{
  return this->time_master_;
}

ACE_Time_Value
Kokyu_EC::start_time(void)
{
  return this->start_time_;
}

ACE_Vector<RtEventChannelAdmin::RtSchedEventChannel_var>*
Kokyu_EC::remote_ecs(void)
{
  return &(this->remote_ecs_);
}

//***********************************************************
/// Constructor
Reactor_Task::Reactor_Task (void)
  : initialized_(0)
  , react_(0)
{}


Reactor_Task::~Reactor_Task (void)
{
  delete react_;
}

int
Reactor_Task::initialize(void)
{
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

//***********************************************************

Gateway_Initializer::Gateway_Initializer(void)
{}

Gateway_Initializer::~Gateway_Initializer(void)
{}

int
Gateway_Initializer::init(CORBA::ORB_var orb, PortableServer::POA_var poa, Kokyu_EC *ec,
                          const ACE_CString& ior_output_filename,const Filename_Array& ior_input_files)
{
  this->orb_ = orb;
  this->poa_ = poa;
  this->ec_ = ec;
  this->ior_output_filename_ = ior_output_filename;
  this->ior_input_files_ = ior_input_files;

  return 0;
}

int
Gateway_Initializer::handle_timeout (const ACE_Time_Value &,
                                     const void *)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for(size_t i=0; i<this->ior_input_files_.size(); ++i)
        {
          Filename_Array::TYPE filename = this->ior_input_files_[i];
          this->ec_->init_gateway(this->orb_.in(),
                                  this->poa_.in(),
                                  filename,
                                  this->ior_output_filename_.c_str()
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      RtecScheduler::Scheduler_var scheduler = this->ec_->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      RtecEventChannelAdmin::EventChannel_var event_channel = this->ec_->event_channel(ACE_ENV_SINGLE_ARG_DECL);
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);

      //Gateways register as consumers of ALL the consumers in the
      //remote EC, so we need a generalized RT_Info to cover all the
      //dependencies not actually provided by this node. That is, we
      //need an RT_Info for a fictional supplier which publishes all
      //known event types.
      RtecScheduler::handle_t general_rt_info =
        scheduler->create ("Generalized Supplier (DUMMY)" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value exec(0,500); //some arbitrary execution time
      RtecScheduler::Time exec_time;
      ORBSVCS_Time::Time_Value_to_TimeT(exec_time,exec);
      ACE_Time_Value period(0,500); //some arbitrary period
      RtecScheduler::Time period_time;
      ORBSVCS_Time::Time_Value_to_TimeT(period_time,period);
      scheduler->set (general_rt_info,
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
      ACE_TRY_CHECK;


      RtecEventComm::EventSourceID general_id = 3000;  //we assume 3000 is big enough to not overlap w/ actual suppliers
      ACE_SupplierQOS_Factory general_qos;
      //NOTE that this is kind of hard-cody since it assumes 12 tasks, each with 4 subtasks!
      for(int task = 1; task <= 12; task++)
        {
          //for each task
          for(int subtask = 1; subtask <= 4; subtask++)
            {
              //for each subtask
              RtecEventComm::EventType type = 100 * task + subtask; //because these are how the event types are calculated!
              general_qos.insert (general_id,
                                  type,
                                  general_rt_info,
                                  1 //number of calls
                                  );
            }
        }

      //we need to actually connect to the EC so the scheduler will set up the dependencies right
      Supplier * general_impl;
      ACE_NEW_RETURN(general_impl,
                     Supplier(general_id),-1); //dummy supplier since nothing will trigger it to push!

      RtecEventComm::PushSupplier_var general;
      general = general_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;
      proxy_consumer = supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy_consumer->connect_push_supplier (general.in(),
                                             general_qos.get_SupplierQOS()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      general_impl->set_consumer_proxy(proxy_consumer.in());
      general_impl->rt_info(general_rt_info);

      //NOTE: memory leak; we lose track of dynamically allocated general_impl!
      //DONE connecting dummy supplier

      //TODO: How ensure all gateways TO us are connected?
      ACE_Time_Value sleeptime(0,500000);
      for (size_t remote_gateways_connected = 0;
           remote_gateways_connected < this->ec_->remote_ecs()->size();
           remote_gateways_connected = this->ec_->remote_gateways_connected())
        {
          //while file doesn't exist
          ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): waiting for remote gateway connections (%d left)\n",(this->ec_->remote_ecs()->size()-remote_gateways_connected)));
          ACE_OS::sleep(sleeptime);
        }

      if (this->ec_->time_master())
        {
          //set start time for all other ECs
          RtEventChannelAdmin::Time st;
          ACE_Time_Value start_time(30,0);
          start_time += ACE_OS::gettimeofday(); //now + 30 sec
          ORBSVCS_Time::Time_Value_to_TimeT(st,start_time);
          this->ec_->set_start_time(st); //set start time for this EC
          ACE_Vector<RtEventChannelAdmin::RtSchedEventChannel_var>* remote_ecs(this->ec_->remote_ecs());
          for(size_t i=0; i< remote_ecs->size(); ++i) //set start time for remote ECs
            {
              (*remote_ecs)[i]->set_start_time(st);
              ACE_TRY_CHECK;
            }
        }

      //now wait for start time
      sleeptime.set(0,5); //we'll be within 5usec of start time when we call ec_->start()
      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): waiting for start time to be set or to pass; now is %isec %iusec, start time is %isec %iusec\n",now.sec(),now.usec(),this->ec_->start_time().sec(),this->ec_->start_time().usec()));
      while (this->ec_->start_time() == ACE_Time_Value::zero
             || now < this->ec_->start_time())
        {
          //while not time to start
          //ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): waiting for start time to be set or to pass; now is %isec %iusec, start time is %isec %iusec\n",now.sec(),now.usec(),this->ec_->start_time().sec(),this->ec_->start_time().usec()));
          ACE_OS::sleep(sleeptime);
          now = ACE_OS::gettimeofday();
        }

      ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): Reached start time at %isec %iusec\n",now.sec(),now.usec()));

      //start self once we know that all the gateways are connected
      this->ec_->start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //DEBUG: print out schedule
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

      scheduler->compute_scheduling (min_os_priority,
                                     max_os_priority,
                                     infos.out (),
                                     dependencies.out (),
                                     configs.out (),
                                     anomalies.out ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      std::stringstream sched_out;
      ACE_CString ior_prefix(ior_output_filename_.c_str(),ior_output_filename_.length()-4); //cut off '.ior'
      sched_out << "schedule_" << ior_prefix.c_str() << ".out";

      ACE_DEBUG((LM_DEBUG,"Gateway_Initializer writing schedule to %s\n",sched_out.str().c_str()));

      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            dependencies.in (),
                                            configs.in (),
                                            anomalies.in (),
                                            sched_out.str().c_str());

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Gateway_Initializer");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
} //handle_timeout()

