// $Id$

ACE_INLINE
TAO_EC_Event_Channel_Attributes::
TAO_EC_Event_Channel_Attributes (PortableServer::POA_ptr s_poa,
                                 PortableServer::POA_ptr c_poa)
  :  consumer_reconnect (TAO_EC_DEFAULT_CONSUMER_RECONNECT),
     supplier_reconnect (TAO_EC_DEFAULT_SUPPLIER_RECONNECT),
     disconnect_callbacks (TAO_EC_DEFAULT_DISCONNECT_CALLBACKS),
     busy_hwm (TAO_EC_DEFAULT_BUSY_HWM),
     max_write_delay (TAO_EC_DEFAULT_MAX_WRITE_DELAY),
     scheduler (RtecScheduler::Scheduler::_nil ()),
     supplier_poa (s_poa),
     consumer_poa (c_poa)
{
}

ACE_INLINE TAO_EC_Dispatching*
TAO_EC_Event_Channel::dispatching (void) const
{
  return this->dispatching_;
}

ACE_INLINE TAO_EC_Filter_Builder*
TAO_EC_Event_Channel::filter_builder (void) const
{
  return this->filter_builder_;
}

ACE_INLINE TAO_EC_Supplier_Filter_Builder*
TAO_EC_Event_Channel::supplier_filter_builder (void) const
{
  return this->supplier_filter_builder_;
}

ACE_INLINE TAO_EC_ConsumerAdmin*
TAO_EC_Event_Channel::consumer_admin (void) const
{
  return this->consumer_admin_;
}

ACE_INLINE TAO_EC_SupplierAdmin*
TAO_EC_Event_Channel::supplier_admin (void) const
{
  return this->supplier_admin_;
}

ACE_INLINE TAO_EC_Timeout_Generator*
TAO_EC_Event_Channel::timeout_generator (void) const
{
  return this->timeout_generator_;
}

ACE_INLINE TAO_EC_Scheduling_Strategy*
TAO_EC_Event_Channel::scheduling_strategy (void) const
{
  return this->scheduling_strategy_;
}

ACE_INLINE TAO_EC_ProxyPushSupplier*
TAO_EC_Event_Channel::create_proxy_push_supplier (void)
{
  return this->factory_->create_proxy_push_supplier (this);
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier* supplier)
{
  this->factory_->destroy_proxy_push_supplier (supplier);
}

ACE_INLINE TAO_EC_ProxyPushConsumer*
TAO_EC_Event_Channel::create_proxy_push_consumer (void)
{
  return this->factory_->create_proxy_push_consumer (this);
}

ACE_INLINE TAO_EC_ProxyPushSupplier_Set*
TAO_EC_Event_Channel::create_proxy_push_supplier_set (void)
{
  return this->factory_->create_proxy_push_supplier_set (this);
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_proxy_push_supplier_set (TAO_EC_ProxyPushSupplier_Set* x)
{
  this->factory_->destroy_proxy_push_supplier_set (x);
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer* consumer)
{
  this->factory_->destroy_proxy_push_consumer (consumer);
}

ACE_INLINE PortableServer::POA_ptr
TAO_EC_Event_Channel::supplier_poa (void)
{
  return PortableServer::POA::_duplicate (this->supplier_poa_.in ());
}

ACE_INLINE PortableServer::POA_ptr
TAO_EC_Event_Channel::consumer_poa (void)
{
  return PortableServer::POA::_duplicate (this->consumer_poa_.in ());
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel::create_consumer_lock (void)
{
  return this->factory_->create_consumer_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_consumer_lock (ACE_Lock* x)
{
  this->factory_->destroy_consumer_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel::create_supplier_lock (void)
{
  return this->factory_->create_supplier_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_supplier_lock (ACE_Lock* x)
{
  this->factory_->destroy_supplier_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel::create_consumer_admin_lock (void)
{
  return this->factory_->create_consumer_admin_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_consumer_admin_lock (ACE_Lock* x)
{
  this->factory_->destroy_consumer_admin_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel::create_supplier_admin_lock (void)
{
  return this->factory_->create_supplier_admin_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel::destroy_supplier_admin_lock (ACE_Lock* x)
{
  this->factory_->destroy_supplier_admin_lock (x);
}

ACE_INLINE int
TAO_EC_Event_Channel::consumer_reconnect (void) const
{
  return this->consumer_reconnect_;
}

ACE_INLINE int
TAO_EC_Event_Channel::supplier_reconnect (void) const
{
  return this->supplier_reconnect_;
}

ACE_INLINE int
TAO_EC_Event_Channel::disconnect_callbacks (void) const
{
  return this->disconnect_callbacks_;
}

ACE_INLINE RtecScheduler::Scheduler_ptr
TAO_EC_Event_Channel::scheduler (void)
{
  return RtecScheduler::Scheduler::_duplicate (this->scheduler_.in ());
}

ACE_INLINE int
TAO_EC_Event_Channel::busy_hwm (void) const
{
  return this->busy_hwm_;
}

ACE_INLINE int
TAO_EC_Event_Channel::max_write_delay (void) const
{
  return this->max_write_delay_;
}
