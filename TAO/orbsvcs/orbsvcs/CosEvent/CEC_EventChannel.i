// $Id$

ACE_INLINE
TAO_CEC_EventChannel_Attributes::
TAO_CEC_EventChannel_Attributes (PortableServer::POA_ptr s_poa,
                                  PortableServer::POA_ptr c_poa)
  :  consumer_reconnect (TAO_CEC_DEFAULT_CONSUMER_RECONNECT),
     supplier_reconnect (TAO_CEC_DEFAULT_SUPPLIER_RECONNECT),
     disconnect_callbacks (TAO_CEC_DEFAULT_DISCONNECT_CALLBACKS),
     busy_hwm (TAO_CEC_DEFAULT_BUSY_HWM),
     max_write_delay (TAO_CEC_DEFAULT_MAX_WRITE_DELAY),
     supplier_poa (s_poa),
     consumer_poa (c_poa)
{
}

ACE_INLINE TAO_CEC_Dispatching*
TAO_CEC_EventChannel::dispatching (void) const
{
  return this->dispatching_;
}

ACE_INLINE TAO_CEC_ConsumerAdmin*
TAO_CEC_EventChannel::consumer_admin (void) const
{
  return this->consumer_admin_;
}

ACE_INLINE TAO_CEC_SupplierAdmin*
TAO_CEC_EventChannel::supplier_admin (void) const
{
  return this->supplier_admin_;
}

ACE_INLINE TAO_CEC_ProxyPushSupplier*
TAO_CEC_EventChannel::create_proxy_push_supplier (void)
{
  return this->factory_->create_proxy_push_supplier (this);
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_proxy_push_supplier (TAO_CEC_ProxyPushSupplier* supplier)
{
  this->factory_->destroy_proxy_push_supplier (supplier);
}

ACE_INLINE TAO_CEC_ProxyPushConsumer*
TAO_CEC_EventChannel::create_proxy_push_consumer (void)
{
  return this->factory_->create_proxy_push_consumer (this);
}

ACE_INLINE TAO_CEC_ProxyPushConsumer_Collection*
TAO_CEC_EventChannel::create_proxy_push_consumer_collection (void)
{
  return this->factory_->create_proxy_push_consumer_collection (this);
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_proxy_push_consumer_collection (TAO_CEC_ProxyPushConsumer_Collection* x)
{
  this->factory_->destroy_proxy_push_consumer_collection (x);
}

ACE_INLINE TAO_CEC_ProxyPushSupplier_Collection*
TAO_CEC_EventChannel::create_proxy_push_supplier_collection (void)
{
  return this->factory_->create_proxy_push_supplier_collection (this);
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_proxy_push_supplier_collection (TAO_CEC_ProxyPushSupplier_Collection* x)
{
  this->factory_->destroy_proxy_push_supplier_collection (x);
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_proxy_push_consumer (TAO_CEC_ProxyPushConsumer* consumer)
{
  this->factory_->destroy_proxy_push_consumer (consumer);
}

ACE_INLINE PortableServer::POA_ptr
TAO_CEC_EventChannel::supplier_poa (void)
{
  return PortableServer::POA::_duplicate (this->supplier_poa_.in ());
}

ACE_INLINE PortableServer::POA_ptr
TAO_CEC_EventChannel::consumer_poa (void)
{
  return PortableServer::POA::_duplicate (this->consumer_poa_.in ());
}

ACE_INLINE ACE_Lock*
TAO_CEC_EventChannel::create_consumer_lock (void)
{
  return this->factory_->create_consumer_lock ();
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_consumer_lock (ACE_Lock* x)
{
  this->factory_->destroy_consumer_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_CEC_EventChannel::create_supplier_lock (void)
{
  return this->factory_->create_supplier_lock ();
}

ACE_INLINE void
TAO_CEC_EventChannel::destroy_supplier_lock (ACE_Lock* x)
{
  this->factory_->destroy_supplier_lock (x);
}

ACE_INLINE int
TAO_CEC_EventChannel::consumer_reconnect (void) const
{
  return this->consumer_reconnect_;
}

ACE_INLINE int
TAO_CEC_EventChannel::supplier_reconnect (void) const
{
  return this->supplier_reconnect_;
}

ACE_INLINE int
TAO_CEC_EventChannel::disconnect_callbacks (void) const
{
  return this->disconnect_callbacks_;
}

ACE_INLINE int
TAO_CEC_EventChannel::busy_hwm (void) const
{
  return this->busy_hwm_;
}

ACE_INLINE int
TAO_CEC_EventChannel::max_write_delay (void) const
{
  return this->max_write_delay_;
}
