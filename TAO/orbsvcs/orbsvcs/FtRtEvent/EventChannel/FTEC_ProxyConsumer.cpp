// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxyConsumer.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "../Utils/activate_with_id.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_ProxyPushConsumer,
           "$Id$")

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
     RtecEventChannelAdmin::ProxyPushConsumer_ptr &result
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  result = RtecEventChannelAdmin::ProxyPushConsumer::_nil();
  ACE_TRY {
    object_id_ = Request_Context_Repository().get_object_id();
    PortableServer::POA_var poa = _default_POA();
    activate_object_with_id(result, poa.in(), this, id() ACE_ENV_ARG_PARAMETER);
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "");
    // ignore exceptions
  }
  ACE_ENDTRY;
}

    // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
void TAO_FTEC_ProxyPushConsumer::connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos
                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected))
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = this->id();
  FtRtecEventChannelAdmin::Connect_push_supplier_param param;
  param.push_supplier = RtecEventComm::PushSupplier::_duplicate(push_supplier);
  param.qos = qos;
  update.param.connect_supplier_param(param);

  Inherited::connect_push_supplier(push_supplier, qos ACE_ENV_ARG_PARAMETER);

  ACE_TRY {
    FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

    svc->replicate_request(update,
                           &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_consumer
                           ACE_ENV_ARG_PARAMETER);
  }
  ACE_CATCHALL {
    this->disconnect_push_consumer();
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
}

void TAO_FTEC_ProxyPushConsumer::disconnect_push_consumer (void)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (Request_Context_Repository().is_executed_request())
    return;
  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::DISCONNECT_PUSH_CONSUMER);

  Inherited::disconnect_push_consumer();

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

  svc->replicate_request(update, 0 ACE_ENV_ARG_PARAMETER);
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

void TAO_FTEC_ProxyPushConsumer::set_state(const FtRtecEventChannelAdmin::ProxyPushConsumerStat& state
                                           ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil(state.parameter.info().push_supplier.in()) )
  {
    Inherited::connect_push_supplier(state.parameter.info().push_supplier.in(),
                     state.parameter.info().qos
                     ACE_ENV_ARG_PARAMETER);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
