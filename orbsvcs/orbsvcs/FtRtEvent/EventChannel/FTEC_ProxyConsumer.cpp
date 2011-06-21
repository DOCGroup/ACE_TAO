// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxyConsumer.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "../Utils/activate_with_id.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const TAO_FTEC_ProxyPushConsumer::RollbackOperation
  TAO_FTEC_ProxyPushConsumer::rollback_obtain =
    &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_consumer;

TAO_FTEC_ProxyPushConsumer::TAO_FTEC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* event_channel)
: Inherited(event_channel)
{
}

const FtRtecEventChannelAdmin::ObjectId&
TAO_FTEC_ProxyPushConsumer::id() const
{
  return object_id_.in();
}

/// Activate in the POA
void
TAO_FTEC_ProxyPushConsumer::activate (
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &result)
{
  result = RtecEventChannelAdmin::ProxyPushConsumer::_nil();
  try{
    object_id_ = Request_Context_Repository().get_object_id();
    PortableServer::POA_var poa = _default_POA();
    activate_object_with_id(result, poa.in(), this, id());
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("");
    // ignore exceptions
  }
}

// = The RtecEventChannelAdmin::ProxyPushConsumer methods...
void TAO_FTEC_ProxyPushConsumer::connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos)
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = this->id();
  FtRtecEventChannelAdmin::Connect_push_supplier_param param;
  param.push_supplier = RtecEventComm::PushSupplier::_duplicate(push_supplier);
  param.qos = qos;
  update.param.connect_supplier_param(param);

  Inherited::connect_push_supplier(push_supplier, qos);

  try{
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

    svc->replicate_request(update,
                           &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_consumer);
  }
  catch (...){
    this->disconnect_push_consumer();
    throw;
  }
}

void TAO_FTEC_ProxyPushConsumer::disconnect_push_consumer (void)
{
  if (Request_Context_Repository().is_executed_request())
    return;
  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::DISCONNECT_PUSH_CONSUMER);

  Inherited::disconnect_push_consumer();

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  ACE_READ_GUARD (FTRTEC::Replication_Service, locker, *svc);

  svc->replicate_request(update, 0);
}

void TAO_FTEC_ProxyPushConsumer::get_state(FtRtecEventChannelAdmin::ProxyPushConsumerStat& state)
{
  state.object_id = this->object_id_.in ();
  if (this->is_connected()) {
    FtRtecEventChannelAdmin::ProxyPushConsumerConnectionInfo info;
    info.push_supplier = this->supplier();
    info.qos = this->publications();
    state.parameter.info(info);
  }
}

void TAO_FTEC_ProxyPushConsumer::set_state(const FtRtecEventChannelAdmin::ProxyPushConsumerStat& state)
{
  if (!CORBA::is_nil(state.parameter.info().push_supplier.in()) )
  {
    Inherited::connect_push_supplier(state.parameter.info().push_supplier.in(),
                     state.parameter.info().qos);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
