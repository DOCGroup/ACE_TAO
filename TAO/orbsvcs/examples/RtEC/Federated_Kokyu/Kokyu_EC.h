// $Id$

#ifndef KOKYU_EC_H
#define KOKYU_EC_H

#include "RtSchedEventChannelS.h"
#include "tao/Utils/Servant_Var.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

class Kokyu_EC : public POA_RtEventChannelAdmin::RtSchedEventChannel
{
public:
    Kokyu_EC();

    int init(const char* schedule_discipline, PortableServer::POA_ptr poa);

    virtual RtEventChannelAdmin::handle_t register_consumer (
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
      ));

    virtual RtEventChannelAdmin::handle_t register_supplier (
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
      ));


    virtual void add_dependency (
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
      ));

    virtual void start (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ));

    virtual ::RtecEventChannelAdmin::EventChannel_ptr event_channel (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::RtecScheduler::Scheduler_ptr scheduler (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:
  TAO::Utils::Servant_Var<POA_RtecScheduler::Scheduler> scheduler_impl_;
  TAO::Utils::Servant_Var<TAO_EC_Event_Channel> ec_impl_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecScheduler::Scheduler_var scheduler_;
};

#endif
