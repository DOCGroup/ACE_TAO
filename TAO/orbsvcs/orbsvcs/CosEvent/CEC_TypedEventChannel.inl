// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_CEC_TypedEventChannel_Attributes::
TAO_CEC_TypedEventChannel_Attributes (PortableServer::POA_ptr s_poa,
                                      PortableServer::POA_ptr c_poa,
                                      CORBA::ORB_ptr _orb,
                                      CORBA::Repository_ptr _interface_repository)
  :  consumer_reconnect (TAO_CEC_DEFAULT_CONSUMER_RECONNECT),
     supplier_reconnect (TAO_CEC_DEFAULT_SUPPLIER_RECONNECT),
     disconnect_callbacks (TAO_CEC_DEFAULT_DISCONNECT_CALLBACKS),
     destroy_on_shutdown (0),
     //busy_hwm (TAO_CEC_DEFAULT_BUSY_HWM),
     //max_write_delay (TAO_CEC_DEFAULT_MAX_WRITE_DELAY),
     typed_supplier_poa (s_poa),
     typed_consumer_poa (c_poa),
     orb (_orb),
     interface_repository (_interface_repository)
{
}

ACE_INLINE TAO_CEC_Dispatching*
TAO_CEC_TypedEventChannel::dispatching (void) const
{
  return this->dispatching_;
}

ACE_INLINE TAO_CEC_TypedConsumerAdmin*
TAO_CEC_TypedEventChannel::typed_consumer_admin (void) const
{
  return this->typed_consumer_admin_;
}

ACE_INLINE TAO_CEC_TypedSupplierAdmin*
TAO_CEC_TypedEventChannel::typed_supplier_admin (void) const
{
  return this->typed_supplier_admin_;
}

ACE_INLINE TAO_CEC_ConsumerControl*
TAO_CEC_TypedEventChannel::consumer_control (void) const
{
  return this->consumer_control_;
}

ACE_INLINE TAO_CEC_SupplierControl*
TAO_CEC_TypedEventChannel::supplier_control (void) const
{
  return this->supplier_control_;
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::create_proxy (TAO_CEC_ProxyPushSupplier* &x)
{
  x = this->factory_->create_proxy_push_supplier (this);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_proxy (TAO_CEC_ProxyPushSupplier* supplier)
{
  this->factory_->destroy_proxy_push_supplier (supplier);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::create_proxy (TAO_CEC_TypedProxyPushConsumer* &x)
{
  x = this->factory_->create_proxy_push_consumer (this);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_proxy (TAO_CEC_TypedProxyPushConsumer* consumer)
{
  this->factory_->destroy_proxy_push_consumer (consumer);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::create_proxy_collection (TAO_CEC_ProxyPushSupplier_Collection* &x)
{
  x = this->factory_->create_proxy_push_supplier_collection (this);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_proxy_collection (TAO_CEC_ProxyPushSupplier_Collection* x)
{
  this->factory_->destroy_proxy_push_supplier_collection (x);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::create_proxy_collection (TAO_CEC_TypedProxyPushConsumer_Collection* &x)
{
  x = this->factory_->create_proxy_push_consumer_collection (this);
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_proxy_collection (TAO_CEC_TypedProxyPushConsumer_Collection* x)
{
  this->factory_->destroy_proxy_push_consumer_collection (x);
}

ACE_INLINE PortableServer::POA_ptr
TAO_CEC_TypedEventChannel::typed_supplier_poa (void)
{
  return PortableServer::POA::_duplicate (this->typed_supplier_poa_.in ());
}

ACE_INLINE PortableServer::POA_ptr
TAO_CEC_TypedEventChannel::typed_consumer_poa (void)
{
  return PortableServer::POA::_duplicate (this->typed_consumer_poa_.in ());
}

ACE_INLINE ACE_Lock*
TAO_CEC_TypedEventChannel::create_consumer_lock (void)
{
  return this->factory_->create_consumer_lock ();
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_consumer_lock (ACE_Lock* x)
{
  this->factory_->destroy_consumer_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_CEC_TypedEventChannel::create_supplier_lock (void)
{
  return this->factory_->create_supplier_lock ();
}

ACE_INLINE void
TAO_CEC_TypedEventChannel::destroy_supplier_lock (ACE_Lock* x)
{
  this->factory_->destroy_supplier_lock (x);
}

ACE_INLINE int
TAO_CEC_TypedEventChannel::consumer_reconnect (void) const
{
  return this->consumer_reconnect_;
}

ACE_INLINE int
TAO_CEC_TypedEventChannel::supplier_reconnect (void) const
{
  return this->supplier_reconnect_;
}

ACE_INLINE int
TAO_CEC_TypedEventChannel::disconnect_callbacks (void) const
{
  return this->disconnect_callbacks_;
}

ACE_INLINE const char *
TAO_CEC_TypedEventChannel::supported_interface (void) const
{
  return this->supported_interface_.c_str ();
}

ACE_INLINE CORBA::RepositoryId
TAO_CEC_TypedEventChannel::base_interfaces (CORBA::ULong index) const
{
  return const_cast <CORBA::RepositoryId>(this->base_interfaces_[index].in ());
}

ACE_INLINE CORBA::ULong
TAO_CEC_TypedEventChannel::number_of_base_interfaces (void) const
{
  return this->base_interfaces_.length ();
}

ACE_INLINE TAO_CEC_TypedEventChannel::ServantRetryMap&
TAO_CEC_TypedEventChannel::get_servant_retry_map (void)
{
  return this->retry_map_;
}

ACE_INLINE
TAO_CEC_Param::TAO_CEC_Param (void)
{
}

ACE_INLINE
TAO_CEC_Param::~TAO_CEC_Param (void)
{
}

ACE_INLINE
TAO_CEC_Operation_Params::TAO_CEC_Operation_Params (CORBA::ULong num_params)
  :  num_params_ (num_params)
{
  parameters_ = new TAO_CEC_Param[num_params];
}

ACE_INLINE
TAO_CEC_Operation_Params::~TAO_CEC_Operation_Params (void)
{
  delete [] parameters_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
