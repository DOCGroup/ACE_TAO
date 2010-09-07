// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxySupplier.h"
#include "../Utils/activate_with_id.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_ProxyPushSupplier,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const TAO_FTEC_ProxyPushSupplier::RollbackOperation
  TAO_FTEC_ProxyPushSupplier::rollback_obtain =
    &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_supplier;

TAO_FTEC_ProxyPushSupplier::TAO_FTEC_ProxyPushSupplier(TAO_EC_Event_Channel_Base* event_channel
                             ,int validate_connection)
: Inherited(event_channel, validate_connection)
{
}

const FtRtecEventChannelAdmin::ObjectId&
TAO_FTEC_ProxyPushSupplier::id() const
{
  return object_id_.in();
}

    /// Activate in the POA
void
TAO_FTEC_ProxyPushSupplier::activate (
       RtecEventChannelAdmin::ProxyPushSupplier_ptr &result)
{

  result =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil();
  try{
    object_id_ = Request_Context_Repository().get_object_id();
    PortableServer::POA_var poa = _default_POA();
    activate_object_with_id(result, poa.in(), this, id());
  }
  catch (const CORBA::Exception&)
  {
    // ignore exceptions
  }
}

    // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
void TAO_FTEC_ProxyPushSupplier::connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos)
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  FtRtecEventChannelAdmin::Connect_push_consumer_param param;
  update.object_id = this->id();
  param.push_consumer = RtecEventComm::PushConsumer::_duplicate(push_consumer);
  param.qos = qos;
  update.param.connect_consumer_param(param);

  Inherited::connect_push_consumer(push_consumer, qos);

  try{
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();

    ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_supplier);
  }
  catch (...){
  }
}


void TAO_FTEC_ProxyPushSupplier::disconnect_push_supplier (void)
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::DISCONNECT_PUSH_SUPPLIER);

  Inherited::disconnect_push_supplier();
  FTRTEC::Replication_Service *svc = FTRTEC::Replication_Service::instance();

  ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

  svc->replicate_request(update, 0);
}


void TAO_FTEC_ProxyPushSupplier::suspend_connection (void)
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::SUSPEND_CONNECTION);

  Inherited::suspend_connection();

  try
  {
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannelFacade::resume_push_supplier);
  }
  catch (...){
    this->resume_connection();
    throw;
  }
}

void TAO_FTEC_ProxyPushSupplier::resume_connection (void)
{
  if (Request_Context_Repository().is_executed_request())
    return;

  Request_Context_Repository().set_object_id(id());
  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::RESUME_CONNECTION);

  Inherited::resume_connection();

  try{
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();

    ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannelFacade::suspend_push_supplier);
  }
  catch (...){
    this->suspend_connection();
    throw;
  }
}


void TAO_FTEC_ProxyPushSupplier::get_state(FtRtecEventChannelAdmin::ProxyPushSupplierStat& state)
{
  state.object_id = this->object_id_.in ();
  state.suspended = this->is_suspended();
  if (this->is_connected()) {
    FtRtecEventChannelAdmin::ProxyPushSupplierConnectionInfo info;
    info.push_consumer = this->consumer();
    info.qos = this->subscriptions();
    state.parameter.info(info);
  }
}


void TAO_FTEC_ProxyPushSupplier::set_state(const FtRtecEventChannelAdmin::ProxyPushSupplierStat& state)
{
  if (!CORBA::is_nil(state.parameter.info().push_consumer.in()))
  {
    Inherited::connect_push_consumer(state.parameter.info().push_consumer.in(),
      state.parameter.info().qos);
    if (state.suspended)
      Inherited::suspend_connection();
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
