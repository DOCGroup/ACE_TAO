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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void obtain_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                          FtRtecEventChannelAdmin::Operation& op)
{
  ec->consumer_admin()->obtain_proxy(op);
}

void obtain_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                          FtRtecEventChannelAdmin::Operation& op)
{
  ec->supplier_admin()->obtain_proxy(op);
}

void obtain_push_consumer_and_connect(TAO_FTEC_Event_Channel_Impl* ec,
                           const FtRtecEventChannelAdmin::ObjectId& oid,
                           RtecEventComm::PushSupplier_ptr push_supplier,
                           const RtecEventChannelAdmin::SupplierQOS & qos)
{
  Request_Context_Repository().set_object_id(oid);

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer =
    ec->supplier_admin()->obtain();


  try{
    consumer->connect_push_supplier(push_supplier, qos);
  }
  catch (...){
    ec->supplier_admin()->disconnect(consumer.in());
    throw;
  }
}


void connect_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                           FtRtecEventChannelAdmin::Operation& op)
{
  PortableServer::POA_var poa= ec->supplier_poa();
  FtRtecEventChannelAdmin::Connect_push_supplier_param& param
    = op.param.connect_supplier_param();

  TAO_FTEC_ProxyPushConsumer* proxy
    = ec->find_proxy_push_consumer(op.object_id);

  if (proxy == 0)  {
    obtain_push_consumer_and_connect(ec,
                                     op.object_id,
                                     param.push_supplier.in(),
                                     param.qos);
  }
  else {
    proxy->connect_push_supplier(param.push_supplier.in(),
      param.qos);
  }
}

void obtain_push_supplier_and_connect(TAO_FTEC_Event_Channel_Impl* ec,
                           const FtRtecEventChannelAdmin::ObjectId& oid,
                           RtecEventComm::PushConsumer_ptr push_consumer,
                           const RtecEventChannelAdmin::ConsumerQOS & qos)
{
  Request_Context_Repository().set_object_id(oid);

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier =
    ec->consumer_admin()->obtain();


  try{
    supplier->connect_push_consumer(push_consumer, qos);
  }
  catch (...){
    ec->consumer_admin()->disconnect(supplier.in());
    throw;
  }
}


void connect_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                           FtRtecEventChannelAdmin::Operation& op)
{
  PortableServer::POA_var poa= ec->consumer_poa();
  FtRtecEventChannelAdmin::Connect_push_consumer_param& param
    = op.param.connect_consumer_param();

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);

  if (proxy == 0){
    obtain_push_supplier_and_connect(ec,
                                     op.object_id,
                                     param.push_consumer.in(),
                                     param.qos);
  }
  else {
    proxy->connect_push_consumer(param.push_consumer.in(),
      param.qos);
  }
}

void disconnect_push_supplier(TAO_FTEC_Event_Channel_Impl* ec,
                              FtRtecEventChannelAdmin::Operation& op)
{
  PortableServer::POA_var poa= ec->consumer_poa();

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == 0) // proxy not found
    throw FTRT::InvalidUpdate();

  proxy->disconnect_push_supplier();
}

void disconnect_push_consumer(TAO_FTEC_Event_Channel_Impl* ec,
                              FtRtecEventChannelAdmin::Operation& op)
{
  PortableServer::POA_var poa= ec->supplier_poa();

  TAO_FTEC_ProxyPushConsumer* proxy = ec->find_proxy_push_consumer(op.object_id);

  if (proxy == 0) // proxy not found
    throw FTRT::InvalidUpdate();

  proxy->disconnect_push_consumer();
}


void suspend_connection (TAO_FTEC_Event_Channel_Impl* ec,
                         FtRtecEventChannelAdmin::Operation& op)
{
  PortableServer::POA_var poa= ec->consumer_poa();

  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == 0) // proxy not found
    throw FTRT::InvalidUpdate();

  proxy->suspend_connection();
}

void resume_connection(TAO_FTEC_Event_Channel_Impl* ec,
                       FtRtecEventChannelAdmin::Operation& op)
{
  TAO_FTEC_ProxyPushSupplier* proxy = ec->find_proxy_push_supplier(op.object_id);


  if (proxy == 0) // proxy not found
    throw FTRT::InvalidUpdate();

  proxy->resume_connection();
}


typedef void (*Set_update_fun)(TAO_FTEC_Event_Channel_Impl* ec,
                               FtRtecEventChannelAdmin::Operation& op);

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
  const FtRtecEventComm::ObjectId& consumer_admin_oid)
{

  iogr_maker_.init(orb.in());

  TAO_EC_Event_Channel_Base::activate();

  supplier_admin()->activate(supplier_admin_oid);

  consumer_admin()->activate(consumer_admin_oid);
}



// = The RtecEventChannelAdmin::EventChannel methods...
/// The default implementation is:
///    this->consumer_admin ()->_this ();
RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_FTEC_Event_Channel_Impl::for_consumers (void)
{
  CORBA::Object_var obj = consumer_admin()->reference();
  obj = IOGR_Maker::instance()->forge_iogr(obj.in());
  return RtecEventChannelAdmin::ConsumerAdmin::_narrow(obj.in());
}


/// The default implementation is:
///    this->supplier_admin ()->_this ();
RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_FTEC_Event_Channel_Impl::for_suppliers (void)
{
  CORBA::Object_var obj = supplier_admin()->reference();
  obj = IOGR_Maker::instance()->forge_iogr(obj.in());
  return RtecEventChannelAdmin::SupplierAdmin::_narrow(obj.in());
}


::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel_Impl::connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        const RtecEventChannelAdmin::ConsumerQOS & qos
      )
{
  CORBA::Any_var any
    = Request_Context_Repository().get_cached_result();

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

  Request_Context_Repository().generate_object_id(*oid);

  obtain_push_supplier_and_connect(this,
                                   object_id.in(),
                                   push_consumer,
                                   qos);


  return object_id._retn();
}


::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel_Impl::connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        const RtecEventChannelAdmin::SupplierQOS & qos
      )
{
  CORBA::Any_var any
    = Request_Context_Repository().get_cached_result();

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

  Request_Context_Repository().generate_object_id(*oid);

  obtain_push_consumer_and_connect(this,
                                   object_id.in(),
                                   push_supplier,
                                   qos);


  return object_id._retn();

}

void TAO_FTEC_Event_Channel_Impl::disconnect_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == 0) // proxy not found
    return;

  proxy->disconnect_push_supplier();
}

void TAO_FTEC_Event_Channel_Impl::disconnect_push_consumer (
        const FtRtecEventChannelAdmin::ObjectId & oid
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushConsumer* proxy = this->find_proxy_push_consumer(oid);

  if (proxy == 0) // proxy not found
    return;

  proxy->disconnect_push_consumer();
}

void TAO_FTEC_Event_Channel_Impl::suspend_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == 0) // proxy not found
    throw FtRtecEventComm::InvalidObjectID();

  proxy->suspend_connection();
}


void TAO_FTEC_Event_Channel_Impl::resume_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
      )
{
  if (Request_Context_Repository().is_executed_request())
    return;

  TAO_FTEC_ProxyPushSupplier* proxy = this->find_proxy_push_supplier(oid);

  if (proxy == 0) // proxy not found
    throw FtRtecEventComm::InvalidObjectID();

  proxy->resume_connection();
}

void TAO_FTEC_Event_Channel_Impl::push (
        const FtRtecEventChannelAdmin::ObjectId & oid,
        const RtecEventComm::EventSet & data
      )
{
  TAO_FTEC_ProxyPushConsumer* proxy = this->find_proxy_push_consumer(oid);

  if (proxy == 0) // proxy not found
    throw FtRtecEventComm::InvalidObjectID();

  proxy->push(data);
}




void TAO_FTEC_Event_Channel_Impl::get_state (
                                        FtRtecEventChannelAdmin::EventChannelState & state
                                        )
{
  FtEventServiceInterceptor::instance()->get_state(state.cached_operation_results);
  this->supplier_admin()->get_state(state.supplier_admin_state);
  this->consumer_admin()->get_state(state.consumer_admin_state);
}



void TAO_FTEC_Event_Channel_Impl::set_state (const FTRT::State & stat)
{
  FtRtecEventChannelAdmin::EventChannelState state;

  Safe_InputCDR cdr((const char*)stat.get_buffer(), stat.length());
  cdr >> state;

  FtEventServiceInterceptor::instance()->set_state(state.cached_operation_results);
  this->supplier_admin()->set_state(state.supplier_admin_state);
  this->consumer_admin()->set_state(state.consumer_admin_state);
}


void TAO_FTEC_Event_Channel_Impl::set_update (const FTRT::State & s)
{
  FTRTEC::Replication_Service::instance()->check_validity();

  if (!Request_Context_Repository().is_executed_request()) {
    Safe_InputCDR cdr((const char*)s.get_buffer(), s.length());

    FtRtecEventChannelAdmin::Operation_var op(new FtRtecEventChannelAdmin::Operation);
    if (!(cdr >> *op)) {
      throw FTRT::InvalidUpdate();
    }

    (update_table[op->param._d()])(this, *op);

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
  try{
    PortableServer::POA_var poa = consumer_poa();

    const PortableServer::Servant servant = poa->id_to_servant(
      reinterpret_cast<const PortableServer::ObjectId&> (id));
    POA_RtecEventChannelAdmin::ProxyPushSupplier_ptr obj =
      dynamic_cast<POA_RtecEventChannelAdmin::ProxyPushSupplier_ptr> (servant);

    return static_cast<TAO_FTEC_ProxyPushSupplier*> (obj);
  }
  catch (...){
  }
  return 0;
}

TAO_FTEC_ProxyPushConsumer*
TAO_FTEC_Event_Channel_Impl::find_proxy_push_consumer(const FtRtecEventChannelAdmin::ObjectId& id)
{
  try{
    PortableServer::POA_var poa= supplier_poa();

    const PortableServer::Servant servant = poa->id_to_servant(
      reinterpret_cast<const PortableServer::ObjectId&> (id));

    POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr obj =
      dynamic_cast<POA_RtecEventChannelAdmin::ProxyPushConsumer_ptr> (servant);

    return static_cast<TAO_FTEC_ProxyPushConsumer*> (obj);
  }
  catch (...){
  }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
