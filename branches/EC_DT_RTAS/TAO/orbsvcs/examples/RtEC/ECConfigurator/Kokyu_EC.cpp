// $Id$

#include "ace/OS_NS_strings.h" //for ACE_OS::strcasecmp
#include "ace/OS_NS_sys_time.h" // for ACE_OS::gettimeofday
#include "ace/Reactor.h"

#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#include "Kokyu/Dispatch_Deferrer.h"

#include "Kokyu_EC.h"
#include "RtSchedEventChannelC.h"
#include "ECDriver.h"

#include <algorithm>

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

    return sched_impl;
  }

class Print_Consumer_Subscriptions :
    public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  // Default constructor

  virtual void work (TAO_EC_ProxyPushSupplier *consumer
                     ACE_ENV_ARG_DECL)
  {
    ACE_DEBUG ((LM_DEBUG, "Consumer subscriptions:"));

    const RtecEventChannelAdmin::ConsumerQOS& sub =
      consumer->subscriptions ();
    if (sub.is_gateway)
      ACE_DEBUG ((LM_DEBUG, " (Gateway)"));

    ACE_DEBUG((LM_DEBUG,"\n"));

    for (CORBA::ULong j = 0; j < sub.dependencies.length (); ++j)
      {
        const RtecEventComm::Event& event =
          sub.dependencies[j].event;
        RtecEventComm::EventType type = event.header.type;

        ACE_DEBUG ((LM_DEBUG, "  %d\n",type));
      }
  }
}; // class Print_Consumer_Subscriptions

class Print_Supplier_Subscriptions :
    public TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
{
public:
  // Default constructor

  virtual void work (TAO_EC_ProxyPushConsumer *supplier
                     ACE_ENV_ARG_DECL)
  {
    ACE_DEBUG ((LM_DEBUG, "Supplier publications:"));

    const RtecEventChannelAdmin::SupplierQOS& pub =
      supplier->publications ();
    if (pub.is_gateway)
      ACE_DEBUG ((LM_DEBUG, " (Gateway)"));

    ACE_DEBUG((LM_DEBUG,"\n"));

    for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
      {
        const RtecEventComm::Event& event =
          pub.publications[j].event;
        RtecEventComm::EventType type = event.header.type;

        ACE_DEBUG ((LM_DEBUG, "  %d\n",type));
      }
  }
}; // class Print_Supplier_Subscriptions

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
      delete key.name;
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
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) register_supplier(id=%d,rt_info=%d) inserting type %d into supplier QoS\n",source,supplier1_rt_info,supp_types[i]));;
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
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_dependency() with handle %d, dependency %d, and type %s\n",handle,dependency,(dependency_type==RtecBase::ONE_WAY_CALL) ? "ONE_WAY" : "TWO_WAY"));

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

  //DEBUG: print out schedule
  std::stringstream sched_out;
  sched_out << "schedule_" << this->name_ << ".out";

  ACE_DEBUG((LM_DEBUG,"Kokyu_EC::start() writing schedule to %s\n",sched_out.str().c_str()));

  ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                        dependencies.in (),
                                        configs.in (),
                                        anomalies.in (),
                                        sched_out.str().c_str());
  // END DEBUG
  // DEBUG: print subscriptions
  Print_Consumer_Subscriptions worker;
  ec_impl_->for_each_consumer (&worker
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // END DEBUG

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
          ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) scheduled timeout %d with delay %isec %iusec and period %isec %iusec\n",timer_handle,this->task_triggers_[i].phase.sec(),this->task_triggers_[i].phase.usec(),this->task_triggers_[i].period.sec(),this->task_triggers_[i].period.usec()));
        }
    }

  this->started_ = true;
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) EC started\n"));
}

void
Kokyu_EC::notify_gateway_connection (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remote_gateways_connected_++;
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) notify_gateway_connection(); now %d gateways connected\n",this->remote_gateways_connected_ ));
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

///Takes ownership of ECSupplier and ECTimeout_Consumer
void
Kokyu_EC::add_supplier_with_timeout(
                                    ECSupplier * supplier_impl,
                                    const char * supp_entry_point,
                                    const EventType_Vector& supp_types,
                                    ECSupplier_Timeout_Handler * timeout_handler_impl,
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
  add_supplier(supplier_impl,supp_entry_point,supp_types ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //need to set supplier's criticalities and period
  RtecScheduler::Time period_timeT;
  ORBSVCS_Time::Time_Value_to_TimeT (period_timeT,period);
  scheduler_impl_->set (supplier_impl->rt_info(),
                        crit,
                        0, // time
                        0, // typical_time
                        0, // cached_time
                        period_timeT,
                        imp,
                        0, // quantum
                        1, // threads
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

///Takes ownership of ECSupplier
void
Kokyu_EC::add_supplier(
                       ECSupplier * supplier_impl,
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
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_supplier() with id %d, entry point \"%s\"\n",supplier_impl->get_id(),entry_point));

  if (std::find(this->suppliers_.begin(),
                this->suppliers_.end(),supplier_impl) != this->suppliers_.end())
    {
      // supplier already registered
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_supplier() tried to add ECSupplier which was already registerd; ignoring\n"));
      return;
    }

  RtecEventComm::EventSourceID supplier_id = supplier_impl->get_id();

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy;
  RtecEventComm::PushSupplier_var supplier;

  supplier = supplier_impl->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecScheduler::handle_t supplier_rt_info =
    this->register_supplier(entry_point,
                            supplier_id,
                            supp_types,
                            supplier.in(),
                            consumer_proxy.out()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_supplier() (%d) has handle %d\n",supplier_impl->get_id(),supplier_rt_info));

  supplier_impl->set_consumer_proxy(consumer_proxy.in());
  supplier_impl->rt_info(supplier_rt_info);

  // DEBUG: print subscriptions
  Print_Supplier_Subscriptions worker;
  ec_impl_->for_each_supplier (&worker
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // END DEBUG

  this->suppliers_.push_back(supplier_impl);
  this->supplier_proxies_.push_back(supplier);
} //add_supplier()

///Takes ownership of ECConsumer and ECSupplier
void
Kokyu_EC::add_consumer_with_supplier(
                                     ECConsumer * consumer_impl,
                                     const char * cons_entry_point,
                                     const QoSVector& qos_infos,
                                     /*
                                     ACE_Time_Value cons_period,
                                     const EventType_Vector& cons_types,
                                     RtecScheduler::Criticality_t cons_crit,
                                     RtecScheduler::Importance_t cons_imp,
                                     */
                                     ECSupplier * supplier_impl,
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
  add_supplier(supplier_impl,supp_entry_point,supp_types ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  add_consumer(consumer_impl,cons_entry_point,qos_infos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->add_dependency (consumer_impl->rt_info(),
                        supplier_impl->rt_info(),
                        1,
                        RtecBase::TWO_WAY_CALL
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
} //add_consumer_with_supplier()

///Takes ownership of ECConsumer
void
Kokyu_EC::add_consumer(
                       ECConsumer * consumer_impl,
                       const char * entry_point,
                       const QoSVector& qos_infos
                       /*
                       ACE_Time_Value period,
                       const EventType_Vector& cons_types,
                       RtecScheduler::Criticality_t crit,
                       RtecScheduler::Importance_t imp
                       */
                       ACE_ENV_ARG_DECL
                       )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , RtecScheduler::UNKNOWN_TASK
                   , RtecScheduler::INTERNAL
                   , RtecScheduler::SYNCHRONIZATION_FAILURE
                   ))
{
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_consumer() with entry point \"%s\"\n",entry_point));

  if (std::find(this->consumers_.begin(),
                this->consumers_.end(),consumer_impl) != this->consumers_.end())
    {
      // consumer already registered
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_consumer() tried to add ECConsumer which was already registerd; ignoring\n"));
      return;
    }

  RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier;

  //Specifying criticality is crucial since it propagates from
  //consumer to supplier.
  // TODO: for now, QoS from first consumer SchedInfo only (except type)

  RtecEventComm::PushConsumer_var consumer =
    consumer_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecScheduler::handle_t consumer_rt_info;
  for(size_t i=0; i<qos_infos.size(); ++i)
    {
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC add_consumer() registering event type %d\n",qos_infos[i].type));

      std::stringstream entry_pt;
      entry_pt << entry_point << "_" << qos_infos[i].type; //need a different entry point per type

      consumer_rt_info = this->register_consumer(entry_pt.str().c_str(),
                                                 qos_infos[i],
                                                 qos_infos[i].type,
                                                 consumer.in(),
                                                 proxy_supplier.out()
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) add_consumer() (%s) has handle %d\n",entry_pt.str().c_str(),consumer_rt_info));
    }
  // TODO: why only set the one rt_info?
  consumer_impl->rt_info(consumer_rt_info);

  this->consumers_.push_back(consumer_impl);
  this->consumer_proxies_.push_back(consumer);
  this->push_suppliers_.push_back(proxy_supplier);
} //add_consumer()

void
Kokyu_EC::init_gateway(CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr poa,
                       const char* consumer_ec_ior,
                       const char* ior_output_filename
                       ACE_ENV_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) init_gateway() for remote EC \"%s\"\n",consumer_ec_ior));

  // we now own consumer_ec_ior; it will be deleted in the destructor
  name_t cior = { consumer_ec_ior };
  TAO_EC_Gateway_Sched* csched = NULL;
  if (this->gateways.find(cior,csched) == 0) //0 means found
    {
      //Already a gateway for that EC
      ACE_OS::fprintf(stderr,"%s init_gateway(): Tried to create already-existing gateway for %s (sched=%p)\n",
                      this->get_name(),
                      consumer_ec_ior,csched);
      //DEBUG print all known gateways
//       ACE_OS::fprintf(stderr,"All known gateways:");
//       Gateway_Map::iterator giter = this->gateways.begin();
//       for (; giter != this->gateways.end(); ++giter)
//         {
//           ACE_OS::fprintf(stderr,"\t%s\n",(*giter).ext_id_.name);
//           ACE_OS::fprintf(stderr,"\tequal? %d\n",(*giter).ext_id_==cior);
//         }
      //END DEBUG
      delete consumer_ec_ior; // not bound, so need delete here
      return;
    }

  RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec, consumer_ec;
  ACE_CHECK;

  this->init_remote_ec(consumer_ec_ior,orb,consumer_ec.out());
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

  //ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) binding gateway <%s,%p>\n",cior.name,gateway));
  this->gateways.bind(cior,gateway);

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

  ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) init_gateway() notifying consumer_ec %s\n",consumer_ec_ior));
  consumer_ec->notify_gateway_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if(!CORBA::is_nil(consumer_ec.in()))
    {
      this->remote_ecs_.push_back(consumer_ec);
    }
} //init_gateway()

void
Kokyu_EC::init_remote_ec (const char *consumer_ec_ior,
                          CORBA::ORB_ptr orb,
                          RtEventChannelAdmin::RtSchedEventChannel_out consumer_ec)
{
  CORBA::Object_var obj;
  ACE_Time_Value sleeptime(0,500000);
  while(ACE_OS::filesize(consumer_ec_ior) <= 0)
    {
      //while file doesn't exist
      ACE_DEBUG((LM_DEBUG,"Kokyu_EC (%P|%t) init_remote_ec(): waiting for IOR file %s\n",consumer_ec_ior));
      ACE_OS::sleep(sleeptime);
    }

  std::stringstream ior_file;
  ior_file << "file://" << consumer_ec_ior;

  obj = orb->string_to_object(ior_file.str().c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil(obj.in()))
    {
      ACE_OS::fprintf(stderr,"Unable to read IOR file for remote EC; Object_var is nil.\n");
      return;
    }

  //WARNING: if consumer_ec_ior is not a readable file, obj is null!
  consumer_ec = RtEventChannelAdmin::RtSchedEventChannel::_narrow(obj.in()
                                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

} //init_remote_ec

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

Kokyu_EC::ECVector *
Kokyu_EC::remote_ecs(void)
{
  return &(this->remote_ecs_);
}

void
Kokyu_EC::setEventTypes(EventType_Vector *ev)
{
  this->etypes_ = ev;
}

const Kokyu_EC::EventType_Vector*
Kokyu_EC::getEventTypes()
{
  return this->etypes_;
}

void
Kokyu_EC::set_name(const char *name)
{
  this->name_ = name;
}

const char *
Kokyu_EC::get_name (void)
{
  return this->name_.c_str();
}

//***********************************************************
/// Constructor
Reactor_Task::Reactor_Task (void)
  : initialized_(0)
  , react_(0)
{}


Reactor_Task::~Reactor_Task (void)
{
  //ACE_DEBUG((LM_DEBUG,"Deleting Reactor_Task, so deleting its reactor!\n"));
  //delete react_;
}

int
Reactor_Task::initialize(void)
{
  this->react_ = ACE_Reactor::instance();
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
      ACE_DEBUG((LM_DEBUG,"Reactor_Task (%P|%t) svc(): initializing\n"));
      this->initialize();
    }

  this->react_->owner(ACE_Thread::self()); //set this thread as owner

  int err = this->react_->run_reactor_event_loop();
  if (err < 0)
    {
      ACE_OS::fprintf(stderr,"Reactor_Task error running Reactor event loop\n");
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
Gateway_Initializer::init(CORBA::ORB_var orb,
                          PortableServer::POA_var poa,
                          Kokyu_EC *ec,
                          ECDriver *drv,
                          const ACE_CString& ior_filename,
                          const FileNameVector& ior_output_files,
                          int numInputECs)
{
  this->orb_ = orb;
  this->poa_ = poa;
  this->ec_ = ec;
  this->driver_ = drv;
  this->ior_filename_ = ior_filename;
  this->ior_output_files_ = ior_output_files;
  this->numInputECs_ = numInputECs;

  return 0;
}

int
Gateway_Initializer::handle_timeout (const ACE_Time_Value &,
                                     const void *arg)
{
  ACE_UNUSED_ARG(arg);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      FileNameVector::iterator fniter = this->ior_output_files_.begin();
      for (; fniter != this->ior_output_files_.end(); ++fniter)
        {
          FileNameVector::value_type filename = *fniter;
          this->ec_->init_gateway(this->orb_.in(),
                                  this->poa_.in(),
                                  filename,
                                  this->ior_filename_.c_str()
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_OS::printf("Gateway_Initializer: initialized all gateways\n");

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
        scheduler->create ("Generalized ECSupplier (DUMMY)" ACE_ENV_ARG_PARAMETER);
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
      const Kokyu_EC::EventType_Vector *pub_types = this->ec_->getEventTypes();
      ACE_ASSERT(pub_types);
      Kokyu_EC::EventType_Vector::const_iterator etiter = pub_types->begin();
      for(; etiter != pub_types->end(); etiter++)
        {
          //for each task
          Kokyu_EC::EventType_Vector::value_type type = *etiter;
          general_qos.insert (general_id,
                              type,
                              general_rt_info,
                              1 //number of calls
                              );
        }

      //we need to actually connect to the EC so the scheduler will set up the dependencies right
      ECSupplier * general_impl;
      ACE_NEW_RETURN(general_impl,
                     ECSupplier(general_id,*pub_types),-1); //dummy supplier since nothing will trigger it to push!

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

      //TODO: memory leak; we lose track of dynamically allocated general_impl!
      //DONE connecting dummy supplier

      ACE_Time_Value sleeptime(0,500000);
      for (int remote_gateways_connected = 0;
           remote_gateways_connected < this->numInputECs_;
           remote_gateways_connected = this->ec_->remote_gateways_connected())
        {
          //while not all other gateways connected
          ACE_DEBUG((LM_DEBUG,"Gateway_Initializer (%P|%t): waiting for remote gateway connections (%d left)\n",(this->numInputECs_-remote_gateways_connected)));
          ACE_OS::sleep(sleeptime);
        }

      ACE_OS::printf("Gateway_Initializer: all remote gateways connected\n");

      this->driver_->wait_for_start(this->ec_);

      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_OS::printf("Gateway_Initializer: Reached start time at %ldsec %ldusec\n",now.sec(),now.usec());

      //start self once we know that all the gateways are connected
      this->ec_->start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Gateway_Initializer");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
} //handle_timeout()

