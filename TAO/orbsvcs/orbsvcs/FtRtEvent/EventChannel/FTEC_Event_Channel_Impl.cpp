// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel_Impl.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Factory.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_SupplierAdmin.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ConsumerAdmin.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxyConsumer.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxySupplier.h"
#include "orbsvcs/FtRtEvent/EventChannel/FtEventServiceInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/FT_ProxyAdmin_Base.h"
#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Service.h"
#include "../Utils/Safe_InputCDR.h"
#include "orbsvcs/FtRtecEventCommC.h"


ACE_RCSID (EventChannel,
           FTEC_Event_Channel_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void obtain_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                          FtRtecEventChannelAdmin::Operation& op
                          ACE_ENV_ARG_DECL)
{
  ec->consumer_admin()->obtain_proxy(op ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void obtain_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                          FtRtecEventChannelAdmin::Operation& op
                          ACE_ENV_ARG_DECL)
{
  ec->supplier_admin()->obtain_proxy(op ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void obtain_push_consumer_and_connect(TAO_FTEC_Event_Channel_Impl* ec,
                           const FtRtecEventChannelAdmin::ObjectId& oid,
                           RtecEventComm::PushSupplier_ptr push_supplier,
                           const RtecEventChannelAdmin::SupplierQOS & qos
                           ACE_ENV_ARG_DECL)
{
  Request_Context_Repository().set_object_id(oid ACE_ENV_ARG_PARAMETER);

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
    ec->supplier_admin()->obtain(ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_CHECK;

  ACE_TRY {
    consumer->connect_push_supplier(push_supplier, qos
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL {
    ec->supplier_admin()->disconnect(consumer.in());
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK;
}


void connect_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                           FtRtecEventChannelAdmin::Operation& op
                           ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa= ec->supplier_poa();
  ACE_CHECK;
  FtRtecEventChannelAdmin::Connect_push_supplier_param& param
    = op.param.connect_supplier_param();

  TAO_FTEC_ProxyPushConsumer* proxy
    = ec->find_proxy_push_consumer(op.object_id);

  if (proxy == NULL)  {
    obtain_push_consumer_and_connect(ec,
                                     op.object_id,
                                     param.push_supplier.in(),
                                     param.qos
                                     ACE_ENV_ARG_PARAMETER);
  }
  else {
    proxy->connect_push_supplier(param.push_supplier.in(),
      param.qos
      ACE_ENV_ARG_PARAMETER);
  }
  ACE_CHECK;
}

void obtain_push_supplier_and_connect(TAO_FTEC_Event_Channel_Impl* ec,
                           const FtRtecEventChannelAdmin::ObjectId& oid,
                           RtecEventComm::PushConsumer_ptr push_consumer,
                           const RtecEventChannelAdmin::ConsumerQOS & qos
                           ACE_ENV_ARG_DECL)
{
  Request_Context_Repository().set_object_id(oid ACE_ENV_ARG_PARAMETER);

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
    ec->consumer_admin()->obtain(ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_CHECK;

  ACE_TRY {
    supplier->connect_push_consumer(push_consumer, qos
                                    ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL {
    ec->consumer_admin()->disconnect(supplier.in());
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK;
}


void connect_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                           FtRtecEventChannelAdmin::Operation& op
                           ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa= ec->consumer_poa();
  ACE_CHECK;
  FtRtecEventChannelAdmin::Connect_push_consumer_param& param
    = op.param.connect_consumer_param();

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);

  if (proxy == NULL){
    obtain_push_supplier_and_connect(ec,
                                     op.object_id,
                                     param.push_consumer.in(),
                                     param.qos
                                     ACE_ENV_ARG_PARAMETER);
  }
  else {
    proxy->connect_push_consumer(param.push_consumer.in(),
      param.qos
      ACE_ENV_ARG_PARAMETER);
  }
  ACE_CHECK;
}

void disconnect_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                              FtRtecEventChannelAdmin::Operation& op
                              ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa= ec->consumer_poa();
  ACE_CHECK;

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == NULL) // proxy not found
    ACE_THROW(FTRT::InvalidUpdate());

  ACE_CHECK;
  proxy->disconnect_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void disconnect_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                              FtRtecEventChannelAdmin::Operation& op
                              ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa= ec->supplier_poa();
  ACE_CHECK;

  TAO_FTEC_ProxyPushConsumer* proxy = ec->find_proxy_push_consumer(op.object_id);

  if (proxy == NULL) // proxy not found
    ACE_THROW(FTRT::InvalidUpdate());

  ACE_CHECK;
  proxy->disconnect_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void suspend_connection (TAO_FTEC_Event_Channel_Impl* ec,
                         FtRtecEventChannelAdmin::Operation& op
                         ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa= ec->consumer_poa();
  ACE_CHECK;

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == NULL) // proxy not found
    ACE_THROW(FTRT::InvalidUpdate());

  ACE_CHECK;
  proxy->suspend_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void resume_connection(TAO_FTEC_Event_Channel_Impl* ec,
                       FtRtecEventChannelAdmin::Operation& op
                       ACE_ENV_ARG_DECL)
{
  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == NULL) // proxy not found
    ACE_THROW(FTRT::InvalidUpdate());

  ACE_CHECK;
  proxy->resume_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


typedef void (*Set_update_fun)(TAO_FTEC_Event_Channel_Impl* ec,
                               FtRtecEventChannelAdmin::Operation& op
                               ACE_ENV_ARG_DECL);

Set_update_fun const update_table[] = {
    &obtain_push_supplier,
    &obtain_push_consumer,
    &disconnect_push_supplier,
    &disconnect_push_consumer,
    &suspend_connection,
    &resume_connection,
    &connect_push_supplier,
    &connect_push_consumer
};

TAO_FTEC_Event_Channel_Impl::TAO_FTEC_Event_Channel_Impl(
  const TAO_EC_Event_Channel_Attributes& attributes)
  : TAO_EC_Event_Channel_Base(attributes, new TAO_FTEC_Basic_Factory, false)
{
  this->scheduler_ =
    CORBA::Object::_duplicate (attributes.scheduler);

  this->create_strategies ();

}

TAO_FTEC_Event_Channel_Impl::~TAO_FTEC_Event_Channel_Impl()
{
}


TAO_FTEC_Basic_Factory*
TAO_FTEC_Event_Channel_Impl::factory()
{
  return static_cast<TAO_FTEC_Basic_Factory*> (TAO_EC_Event_Channel_Base::factory());
}


/// Start the internal threads (if any), etc.
/// After this call the EC can be used.
void
TAO_FTEC_Event_Channel_Impl::activate_object (
  CORBA::ORB_var orb,
  const FtRtecEventComm::ObjectId& supplier_admin_oid,
  const FtRtecEventComm::ObjectId& consumer_admin_oid
  ACE_ENV_ARG_DECL)
{

  iogr_maker_.init(orb.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_EC_Event_Channel_Base::activate(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  supplier_admin()->activate(supplier_admin_oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_admin()->activate(consumer_admin_oid ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}



// = The RtecEventChannelAdmin::EventChannel methods...
/// The default implementation is:
///    this->consumer_admin ()->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_FTEC_Event_Channel_Impl::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj = consumer_admin()->reference(ACE_ENV_SINGLE_ARG_PARAMETER);
  obj = IOGR_Maker::instance()->forge_iogr(obj.in()
    ACE_ENV_ARG_PARAMETER);
  return RtecEventChannelAdmin::ConsumerAdmin::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
}


/// The default implementation is:
///    this->supplier_admin ()->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_FTEC_Event_Channel_Impl::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj = supplier_admin()->reference(ACE_ENV_SINGLE_ARG_PARAMETER);
  obj = IOGR_Maker::instance()->forge_iogr(obj.in()
    ACE_ENV_ARG_PARAMETER);
  return RtecEventChannelAdmin::SupplierAdmin::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
}


::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel_Impl::connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        const RtecEventChannelAdmin::ConsumerQOS & qos
        ACE_ENV_ARG_DECL
      )
{
  CORBA::Any_var any
    = Request_Context_Repository().get_cached_result(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  FtRtecEventChannelAdmin::ObjectId *oid;

  if (any.in() >>= oid) {
    FtRtecEventChannelAdmin::ObjectId* result;
    ACE_NEW_THROW_EX(result,
                     FtRtecEventChannelAdmin::ObjectId(*oid),
                     CORBA::NO_MEMORY());
    return result;
  }


  ACE_NEW_THROW_EX(oid, FtRtecEventChannelAdmin::ObjectId, CORBA::NO_MEMORY());

  FtRtecEventChannelAdmin::ObjectId_var  object_id = oid;

  Request_Context_Repository().generate_object_id(*oid ACE_ENV_ARG_PARAMETER);

  obtain_push_supplier_and_connect(this,
                                   object_id.in(),
                                   push_consumer,
                                   qos
                                   ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN(0);

  return object_id._retn();
}


::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel_Impl::connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        const RtecEventChannelAdmin::SupplierQOS & qos
        ACE_ENV_ARG_DECL
      )
{
  CORBA::Any_var any
    = Request_Context_Repository().get_cached_result(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  FtRtecEventChannelAdmin::ObjectId *oid;

  if (any.in() >>= oid) {
    FtRtecEventChannelAdmin::ObjectId* result;
    ACE_NEW_THROW_EX(result,
                     FtRtecEventChannelAdmin::ObjectId(*oid),
                     CORBA::NO_MEMORY());
    return result;
  }


  ACE_NEW_THROW_EX(oid, FtRtecEventChannelAdmin::ObjectId, CORBA::NO_MEMORY());
  FtRtecEventChannelAdmin::ObjectId_var object_id = oid;

  Request_Context_Repository().generate_object_id(*oid ACE_ENV_ARG_PARAMETER);

  obtain_push_consumer_and_connect(this,
                                   object_id.in(),
                                   push_supplier,
                                   qos
                                   ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN(0);

  return object_id._retn();

}

void TAO_FTEC_Event_Channel_Impl::disconnect_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == NULL) // proxy not found
    return;

  ACE_CHECK;
  proxy->disconnect_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void TAO_FTEC_Event_Channel_Impl::disconnect_push_consumer (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushConsumer* proxy = this->find_proxy_push_consumer(oid);

  if (proxy == NULL) // proxy not found
    return;

  ACE_CHECK;
  proxy->disconnect_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void TAO_FTEC_Event_Channel_Impl::suspend_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == NULL) // proxy not found
    ACE_THROW(FtRtecEventComm::InvalidObjectID());

  ACE_CHECK;
  proxy->suspend_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void TAO_FTEC_Event_Channel_Impl::resume_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == NULL) // proxy not found
    ACE_THROW(FtRtecEventComm::InvalidObjectID());

  ACE_CHECK;
  proxy->resume_connection(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void TAO_FTEC_Event_Channel_Impl::push (
        const FtRtecEventChannelAdmin::ObjectId & oid,
        const RtecEventComm::EventSet & data
        ACE_ENV_ARG_DECL
      )
{
  TAO_FTEC_ProxyPushConsumer* proxy = this->find_proxy_push_consumer(oid);

  if (proxy == NULL) // proxy not found
    ACE_THROW(FtRtecEventComm::InvalidObjectID());

  proxy->push(data ACE_ENV_ARG_PARAMETER);
}




void TAO_FTEC_Event_Channel_Impl::get_state (
                                        FtRtecEventChannelAdmin::EventChannelState & state
                                        ACE_ENV_ARG_DECL
                                        )
{
  FtEventServiceInterceptor::instance()->get_state(state.cached_operation_results);
  this->supplier_admin()->get_state(state.supplier_admin_state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_admin()->get_state(state.consumer_admin_state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}



void TAO_FTEC_Event_Channel_Impl::set_state (const FTRT::State & stat
                                             ACE_ENV_ARG_DECL)
{
  FtRtecEventChannelAdmin::EventChannelState state;

  Safe_InputCDR cdr((const char*)stat.get_buffer(), stat.length());
  cdr >> state;

  FtEventServiceInterceptor::instance()->set_state(state.cached_operation_results);
  this->supplier_admin()->set_state(state.supplier_admin_state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_admin()->set_state(state.consumer_admin_state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void TAO_FTEC_Event_Channel_Impl::set_update (const FTRT::State & s
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, FTRT::InvalidUpdate))
{
  FTRTEC::Replication_Service::instance()->check_validity(ACE_ENV_SINGLE_ARG_PARAMETER);

  if (!Request_Context_Repository().is_executed_request()) {
    Safe_InputCDR cdr((const char*)s.get_buffer(), s.length());

    FtRtecEventChannelAdmin::Operation_var op(new FtRtecEventChannelAdmin::Operation);
    if (!(cdr >> *op)) {
      ACE_THROW(FTRT::InvalidUpdate() );
    }

    (update_table[op->param._d()])(this, *op ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  }
}


TAO_FTEC_ConsumerAdmin* TAO_FTEC_Event_Channel_Impl::consumer_admin (void) const
{
  return static_cast<TAO_FTEC_ConsumerAdmin*> (TAO_EC_Event_Channel_Base::consumer_admin());
}

/// Access the supplier admin implementation, useful for controlling
/// the activation...
TAO_FTEC_SupplierAdmin* TAO_FTEC_Event_Channel_Impl::supplier_admin (void) const
{
  return static_cast<TAO_FTEC_SupplierAdmin*> (TAO_EC_Event_Channel_Base::supplier_admin());
}


TAO_FTEC_ProxyPushSupplier*
TAO_FTEC_Event_Channel_Impl::find_proxy_push_supplier(const FtRtecEventChannelAdmin::ObjectId& id)
{
  ACE_TRY_NEW_ENV {
    PortableServer::POA_var poa = consumer_poa();

    const PortableServer::Servant servant = poa->id_to_servant(
      reinterpret_cast<const PortableServer::ObjectId&> (id)
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    POA_RtecEventChannelAdmin::ProxyPushSupplier_ptr obj =
      dynamic_cast<POA_RtecEventChannelAdmin::ProxyPushSupplier_ptr> (servant);

    return static_cast<TAO_FTEC_ProxyPushSupplier*> (obj);
  }
  ACE_CATCHALL {
  }
  ACE_ENDTRY;
  return 0;
}

TAO_FTEC_ProxyPushConsumer*
TAO_FTEC_Event_Channel_Impl::find_proxy_push_consumer(const FtRtecEventChannelAdmin::ObjectId& id)
{
  ACE_TRY_NEW_ENV {
    PortableServer::POA_var poa= supplier_poa();

    const PortableServer::Servant servant = poa->id_to_servant(
      reinterpret_cast<const PortableServer::ObjectId&> (id)
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr obj =
      dynamic_cast<POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr> (servant);

    return static_cast<TAO_FTEC_ProxyPushConsumer*> (obj);
  }
  ACE_CATCHALL {
  }
  ACE_ENDTRY;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
