// $Id$

#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.h"
#include "orbsvcs/FtRtEvent/Utils/activate_with_id.h"
#include "orbsvcs/FtRtEvent/Utils/UUID.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "orbsvcs/FtRtecEventCommS.h"
#include "tao/ORB_Core.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/FTEC_Gateway.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRTEC {
class FTEC_Gateway_ConsumerAdmin
  : public POA_RtecEventChannelAdmin::ConsumerAdmin
{
public:
  FTEC_Gateway_ConsumerAdmin(FTEC_Gateway_Impl* impl);
  ~FTEC_Gateway_ConsumerAdmin();
  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (void);
  FTEC_Gateway_Impl* impl_;
};

class FTEC_Gateway_SupplierAdmin
  : public POA_RtecEventChannelAdmin::SupplierAdmin
{
public:
  FTEC_Gateway_SupplierAdmin(FTEC_Gateway_Impl* impl);
  ~FTEC_Gateway_SupplierAdmin();
  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (void);
  FTEC_Gateway_Impl* impl_;
};

class FTEC_Gateway_ProxyPushSupplier
  : public POA_RtecEventChannelAdmin::ProxyPushSupplier
{
public:
  FTEC_Gateway_ProxyPushSupplier(FTEC_Gateway_Impl* impl);
  ~FTEC_Gateway_ProxyPushSupplier();
  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  virtual void connect_push_consumer (
                RtecEventComm::PushConsumer_ptr push_consumer,
                const RtecEventChannelAdmin::ConsumerQOS &qos);
  virtual void disconnect_push_supplier (void);
  virtual void suspend_connection (void);
  virtual void resume_connection (void);
  FTEC_Gateway_Impl* impl_;
};

class FTEC_Gateway_ProxyPushConsumer
  : public POA_RtecEventChannelAdmin::ProxyPushConsumer
{
public:
  FTEC_Gateway_ProxyPushConsumer(FTEC_Gateway_Impl* impl);
  ~FTEC_Gateway_ProxyPushConsumer();

  virtual void push (const RtecEventComm::EventSet & data);
      // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  virtual void connect_push_supplier (
                RtecEventComm::PushSupplier_ptr push_supplier,
                const RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void disconnect_push_consumer (void);

  FTEC_Gateway_Impl* impl_;
};

class PushConsumerHandler : public POA_FtRtecEventComm::AMI_PushConsumerHandler
{
public:
  PushConsumerHandler();
  ~PushConsumerHandler();
  virtual void push (void);

  virtual void push_excep (::Messaging::ExceptionHolder * excep_holder);
};

/**
 * This is used for remove the interceptors registered in the ORB.
 */
class Interceptor_Destoryer : public TAO_ORB_Core
{
public:
  inline static void execute(CORBA::ORB_ptr orb) {
    static_cast<Interceptor_Destoryer*> (orb->orb_core())->do_it();
  }
private:
  Interceptor_Destoryer();
  inline void do_it(void) {
    destroy_interceptors();
  };
};

struct FTEC_Gateway_Impl
{
  CORBA::ORB_var orb;
  PortableServer::POA_var poa;
  FtRtecEventChannelAdmin::EventChannel_var ftec;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin;
  FTEC_Gateway_ConsumerAdmin consumer_admin_servant;
  FTEC_Gateway_SupplierAdmin supplier_admin_servant;
  FTEC_Gateway_ProxyPushSupplier proxy_supplier_servant;
  FTEC_Gateway_ProxyPushConsumer proxy_consumer_servant;
  PushConsumerHandler push_handler_servant;
  FtRtecEventComm::AMI_PushConsumerHandler_var push_handler;
  bool local_orb;
  FTEC_Gateway_Impl();
};

FTEC_Gateway_Impl::FTEC_Gateway_Impl()
: consumer_admin_servant(this),
  supplier_admin_servant(this),
  proxy_supplier_servant(this),
  proxy_consumer_servant(this)
{
}

FTEC_Gateway::FTEC_Gateway(CORBA::ORB_ptr orb, FtRtecEventChannelAdmin::EventChannel_ptr ftec)
: impl_(new FTEC_Gateway_Impl)
{
  impl_->local_orb = CORBA::is_nil(orb);
  if (!impl_->local_orb)
    impl_->orb = CORBA::ORB::_duplicate(orb);
  impl_->ftec = FtRtecEventChannelAdmin::EventChannel::_duplicate(ftec);
}

FTEC_Gateway::~FTEC_Gateway()
{
  if (impl_->local_orb)
    impl_->orb->shutdown();
  delete impl_;
}

RtecEventChannelAdmin::EventChannel_ptr
FTEC_Gateway::activate(PortableServer::POA_ptr root_poa)
{
  PortableServer::POA_var poa;
  PortableServer::POAManager_var mgr;

  if (impl_->local_orb) {
    int argc = 0;
    char** argv = 0;
    impl_->orb = CORBA::ORB_init(argc, argv, "FTEC_GatewayORB");

    Interceptor_Destoryer::execute(impl_->orb.in());

    poa = resolve_init<PortableServer::POA>(impl_->orb.in(), "RootPOA");

    mgr = poa->the_POAManager();

    mgr->activate();
  }
  else {
    poa = PortableServer::POA::_duplicate(root_poa);
    mgr = poa->the_POAManager();
  }

  PortableServer::IdUniquenessPolicy_var id_uniqueness_policy =
    poa->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);

  PortableServer::LifespanPolicy_var lifespan =
    poa->create_lifespan_policy(PortableServer::PERSISTENT);

  // create a USER_ID IdAssignmentPolicy object
  PortableServer::IdAssignmentPolicy_var assign =
    poa->create_id_assignment_policy(PortableServer::USER_ID);

  CORBA::PolicyList policy_list;
  policy_list.length(3);

  policy_list[0] = PortableServer::IdUniquenessPolicy::_duplicate(
    id_uniqueness_policy.in());
  policy_list[1]=
    PortableServer::LifespanPolicy::_duplicate(lifespan.in());
  policy_list[2]=
    PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  impl_->poa = poa->create_POA("gateway_poa", mgr.in(), policy_list);

  id_uniqueness_policy->destroy();
  lifespan->destroy();
  assign->destroy();

  FtRtecEventComm::ObjectId oid;
  oid.length(16);
  TAO_FtRt::UUID::create(oid.get_buffer());

  RtecEventChannelAdmin::EventChannel_var gateway;

  activate_object_with_id(gateway.out(), impl_->poa.in(), this, oid);
  ++oid[9];
  activate_object_with_id(impl_->consumer_admin.out(),
    impl_->poa.in(),
    &impl_->consumer_admin_servant,
    oid);
  ++oid[9];
  activate_object_with_id(impl_->supplier_admin.out(),
    impl_->poa.in(),
    &impl_->supplier_admin_servant,
    oid);

  return gateway._retn();
}

//= The RtecEventChannelAdmin::EventChannel methods
RtecEventChannelAdmin::ConsumerAdmin_ptr
FTEC_Gateway::for_consumers (void)
{
  return RtecEventChannelAdmin::ConsumerAdmin::_duplicate(impl_->consumer_admin.in());
}

RtecEventChannelAdmin::SupplierAdmin_ptr
FTEC_Gateway::for_suppliers (void)
{
  ACE_DEBUG((LM_DEBUG, "FTEC_Gateway::for_suppliers\n"));
  return RtecEventChannelAdmin::SupplierAdmin::_duplicate(impl_->supplier_admin.in());
}

void FTEC_Gateway::destroy (void)
{
  impl_->ftec->destroy();
}

RtecEventChannelAdmin::Observer_Handle
FTEC_Gateway::append_observer (RtecEventChannelAdmin::Observer_ptr observer)
{
  return impl_->ftec->append_observer(observer);
}

void FTEC_Gateway::remove_observer (RtecEventChannelAdmin::Observer_Handle handle)
{
  impl_->ftec->remove_observer(handle);
}

void FTEC_Gateway::push(RtecEventChannelAdmin::ProxyPushConsumer_ptr proxy_consumer,
                        const RtecEventComm::EventSet & data)
{
  PortableServer::ObjectId_var object_id =
    impl_->poa->reference_to_id(proxy_consumer);
  FtRtecEventComm::ObjectId** result;
  ACE_OS::memcpy(&result, &object_id[0], sizeof(FtRtecEventComm::ObjectId**));

  impl_->ftec->push(**result, data);
}


/// FTEC_Gateway_ConsumerAdmin

FTEC_Gateway_ConsumerAdmin::FTEC_Gateway_ConsumerAdmin(FTEC_Gateway_Impl* impl)
: impl_(impl)
{
}

FTEC_Gateway_ConsumerAdmin::~FTEC_Gateway_ConsumerAdmin()
{
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
FTEC_Gateway_ConsumerAdmin::obtain_push_supplier (void)
{

  FtRtecEventComm::ObjectId** remote_proxy_oid_ptr;
  ACE_NEW_THROW_EX(remote_proxy_oid_ptr, FtRtecEventComm::ObjectId*, CORBA::NO_MEMORY());

  FtRtecEventComm::ObjectId local_oid;
  local_oid.length(sizeof(remote_proxy_oid_ptr));
  ACE_OS::memcpy(&local_oid[0], &remote_proxy_oid_ptr, sizeof(remote_proxy_oid_ptr));

  RtecEventChannelAdmin::ProxyPushSupplier_ptr result;
  activate_object_with_id(result, impl_->poa.in(),
                         &impl_->proxy_supplier_servant,
                         local_oid);
  return result;
}

/// FTEC_Gateway_SupplierAdmin

FTEC_Gateway_SupplierAdmin::FTEC_Gateway_SupplierAdmin(FTEC_Gateway_Impl* impl)
: impl_(impl)
{
}

FTEC_Gateway_SupplierAdmin::~FTEC_Gateway_SupplierAdmin()
{
}

// = The RtecEventChannelAdmin::SupplierAdmin methods...
RtecEventChannelAdmin::ProxyPushConsumer_ptr
FTEC_Gateway_SupplierAdmin::obtain_push_consumer (void)
{
  FtRtecEventComm::ObjectId** remote_proxy_oid_ptr;
  ACE_NEW_THROW_EX(remote_proxy_oid_ptr, FtRtecEventComm::ObjectId*, CORBA::NO_MEMORY());

  FtRtecEventComm::ObjectId local_oid;
  local_oid.length(sizeof(remote_proxy_oid_ptr));
  ACE_OS::memcpy(&local_oid[0], &remote_proxy_oid_ptr, sizeof(remote_proxy_oid_ptr));

  RtecEventChannelAdmin::ProxyPushConsumer_ptr result;
  activate_object_with_id(result, impl_->poa.in(),
                          &impl_->proxy_consumer_servant,
                          local_oid);
  return result;
}


FtRtecEventComm::ObjectId**
get_remote_oid_ptr(CORBA::ORB_ptr orb)
{
  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(orb,
    "POACurrent");

  PortableServer::ObjectId_var object_id =
    current->get_object_id();

  FtRtecEventComm::ObjectId** result;
  ACE_OS::memcpy(&result, &object_id[0], sizeof(FtRtecEventComm::ObjectId**));
  return result;
}

/// FTEC_Gateway_ProxyPushSupplier

FTEC_Gateway_ProxyPushSupplier::FTEC_Gateway_ProxyPushSupplier(FTEC_Gateway_Impl* impl)
: impl_(impl)
{
}

FTEC_Gateway_ProxyPushSupplier::~FTEC_Gateway_ProxyPushSupplier()
{
}


// = The RtecEventChannelAdmin::ProxyPushSupplier methods...
void FTEC_Gateway_ProxyPushSupplier::connect_push_consumer (
  RtecEventComm::PushConsumer_ptr push_consumer,
  const RtecEventChannelAdmin::ConsumerQOS &qos)
{

  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());

  *oid_ptr = impl_->ftec->connect_push_consumer(push_consumer, qos);
}

void FTEC_Gateway_ProxyPushSupplier::disconnect_push_supplier (void)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());
  impl_->ftec->disconnect_push_supplier(**oid_ptr);
  delete *oid_ptr;
  delete oid_ptr;
}

void FTEC_Gateway_ProxyPushSupplier::suspend_connection (void)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());
  impl_->ftec->suspend_push_supplier(**oid_ptr);
}

void FTEC_Gateway_ProxyPushSupplier::resume_connection (void)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());
  impl_->ftec->resume_push_supplier(**oid_ptr);
}

/// FTEC_Gateway_ProxyPushConsumer

FTEC_Gateway_ProxyPushConsumer::FTEC_Gateway_ProxyPushConsumer(FTEC_Gateway_Impl* impl)
: impl_(impl)
{
}

FTEC_Gateway_ProxyPushConsumer::~FTEC_Gateway_ProxyPushConsumer()
{
}


// = The RtecEventChannelAdmin::ProxyPushConsumer methods...

void FTEC_Gateway_ProxyPushConsumer::push (const RtecEventComm::EventSet & data)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());

  /*
  if (CORBA::is_nil(impl_->push_handler.in())) {
    impl_->push_handler = impl_->push_handler_servant._this();
  }

  impl_->ftec->sendc_push (impl_->push_handler.in(),
                           **oid_ptr,
                           data);
 */
  impl_->ftec->push(**oid_ptr, data);
}

void FTEC_Gateway_ProxyPushConsumer::connect_push_supplier (
  RtecEventComm::PushSupplier_ptr push_supplier,
  const RtecEventChannelAdmin::SupplierQOS& qos)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());
  *oid_ptr = impl_->ftec->connect_push_supplier(push_supplier, qos);
}

void FTEC_Gateway_ProxyPushConsumer::disconnect_push_consumer (void)
{
  FtRtecEventComm::ObjectId** oid_ptr = get_remote_oid_ptr(impl_->orb.in());
  impl_->ftec->disconnect_push_consumer(**oid_ptr);
  delete *oid_ptr;
  delete oid_ptr;
}

PushConsumerHandler::PushConsumerHandler()
{
}

PushConsumerHandler::~PushConsumerHandler()
{
}

void PushConsumerHandler::push (void)
{
}

void PushConsumerHandler::push_excep (::Messaging::ExceptionHolder *)
{
}

}  // TAO_FTRTEC

TAO_END_VERSIONED_NAMESPACE_DECL
