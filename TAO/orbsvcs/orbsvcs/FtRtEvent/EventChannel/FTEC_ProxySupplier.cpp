// $Id$

#include "FTEC_ProxySupplier.h"
#include "../Utils/activate_with_id.h"
#include "Request_Context_Repository.h"
#include "Replication_Service.h"
#include "../Utils/ScopeGuard.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_ProxyPushSupplier,
           "$Id$")

const TAO_FTEC_ProxyPushSupplier::RollbackOperation
  TAO_FTEC_ProxyPushSupplier::rollback_obtain =
    &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_supplier;

TAO_FTEC_ProxyPushSupplier::TAO_FTEC_ProxyPushSupplier(TAO_EC_Event_Channel_Base* event_channel
                             ,int validate_connection)
: Inherited(event_channel, validate_connection)
{
}

const PortableServer::ObjectId&
TAO_FTEC_ProxyPushSupplier::id() const
{
  return object_id_.in();
}

    /// Activate in the POA
void
TAO_FTEC_ProxyPushSupplier::activate (
       RtecEventChannelAdmin::ProxyPushSupplier_ptr &result
       ACE_ENV_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{

  result =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil();
  ACE_TRY {
    object_id_ = Request_Context_Repository().get_object_id();
    PortableServer::POA_var poa = _default_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    activate_object_with_id(result, poa.in(), this, id() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    // ignore exceptions
  }
  ACE_ENDTRY;
}

    // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
void TAO_FTEC_ProxyPushSupplier::connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos
                ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       RtecEventChannelAdmin::AlreadyConnected,
                       RtecEventChannelAdmin::TypeError))
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  FtRtecEventChannelAdmin::Connect_push_consumer_param param;
  update.object_id = this->id();
  param.push_consumer = RtecEventComm::PushConsumer::_duplicate(push_consumer);
  param.qos = qos;
  update.param.connect_consumer_param(param);

  Inherited::connect_push_consumer(push_consumer, qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ScopeGuard guard = MakeObjGuard(*this, &Inherited::disconnect_push_supplier);

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  {
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannelFacade::disconnect_push_supplier
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  guard.Dismiss();
}


void TAO_FTEC_ProxyPushSupplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::DISCONNECT_PUSH_SUPPLIER);

  Inherited::disconnect_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  FTRTEC::Replication_Service *svc = FTRTEC::Replication_Service::instance();

  ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

  svc->replicate_request(update, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void TAO_FTEC_ProxyPushSupplier::suspend_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (Request_Context_Repository().is_executed_request())
    return;

  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::SUSPEND_CONNECTION);

  Inherited::suspend_connection(ACE_ENV_SINGLE_ARG_PARAMETER);

  ScopeGuard guard = MakeObjGuard(*this, &Inherited::resume_connection);
  ACE_CHECK;
  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  {
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannel::resume_push_supplier
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
  guard.Dismiss();
}

void TAO_FTEC_ProxyPushSupplier::resume_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (Request_Context_Repository().is_executed_request())
    return;

  Request_Context_Repository().set_object_id(id());
  FtRtecEventChannelAdmin::Operation update;
  update.object_id = id();
  update.param._d(FtRtecEventChannelAdmin::RESUME_CONNECTION);

  Inherited::resume_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ScopeGuard guard = MakeObjGuard(*this, &Inherited::suspend_connection);

  FTRTEC::Replication_Service* svc = FTRTEC::Replication_Service::instance();
  {
    ACE_Read_Guard<FTRTEC::Replication_Service> locker(*svc);

    svc->replicate_request(update,
      &FtRtecEventChannelAdmin::EventChannel::suspend_push_supplier
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  guard.Dismiss();
}


void TAO_FTEC_ProxyPushSupplier::get_state(FtRtecEventChannelAdmin::ProxyPushSupplierStat& state)
{
  state.object_id = this->object_id_;
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
      state.parameter.info().qos
      ACE_ENV_ARG_PARAMETER);
    if (state.suspended)
      Inherited::suspend_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  }
}
