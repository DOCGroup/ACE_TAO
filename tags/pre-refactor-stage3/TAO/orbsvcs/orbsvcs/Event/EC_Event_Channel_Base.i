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
     scheduler (CORBA::Object::_nil ()),
     supplier_poa (s_poa),
     consumer_poa (c_poa)
{
}

ACE_INLINE TAO_EC_Factory*
TAO_EC_Event_Channel_Base::factory (void) const
{
  return this->factory_;
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::factory (TAO_EC_Factory* factory,
                                    int own_factory)
{
  if (this->own_factory_)
    delete this->factory_;

  this->factory_ = factory;
  this->own_factory_ = own_factory;
}

ACE_INLINE TAO_EC_Dispatching*
TAO_EC_Event_Channel_Base::dispatching (void) const
{
  return this->dispatching_;
}

ACE_INLINE TAO_EC_Filter_Builder*
TAO_EC_Event_Channel_Base::filter_builder (void) const
{
  return this->filter_builder_;
}

ACE_INLINE TAO_EC_Supplier_Filter_Builder*
TAO_EC_Event_Channel_Base::supplier_filter_builder (void) const
{
  return this->supplier_filter_builder_;
}

ACE_INLINE TAO_EC_ConsumerAdmin*
TAO_EC_Event_Channel_Base::consumer_admin (void) const
{
  return this->consumer_admin_;
}

ACE_INLINE TAO_EC_SupplierAdmin*
TAO_EC_Event_Channel_Base::supplier_admin (void) const
{
  return this->supplier_admin_;
}

ACE_INLINE TAO_EC_Timeout_Generator*
TAO_EC_Event_Channel_Base::timeout_generator (void) const
{
  return this->timeout_generator_;
}

ACE_INLINE TAO_EC_Scheduling_Strategy*
TAO_EC_Event_Channel_Base::scheduling_strategy (void) const
{
  return this->scheduling_strategy_;
}

ACE_INLINE TAO_EC_ConsumerControl*
TAO_EC_Event_Channel_Base::consumer_control (void) const
{
  return this->consumer_control_;
}

ACE_INLINE TAO_EC_SupplierControl*
TAO_EC_Event_Channel_Base::supplier_control (void) const
{
  return this->supplier_control_;
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::create_proxy (TAO_EC_ProxyPushSupplier*& x)
{
  x = this->factory_->create_proxy_push_supplier (this);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_proxy (TAO_EC_ProxyPushSupplier* supplier)
{
  this->factory_->destroy_proxy_push_supplier (supplier);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::create_proxy (TAO_EC_ProxyPushConsumer*& x)
{
  x = this->factory_->create_proxy_push_consumer (this);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_proxy (TAO_EC_ProxyPushConsumer* consumer)
{
  this->factory_->destroy_proxy_push_consumer (consumer);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::create_proxy_collection (TAO_EC_ProxyPushConsumer_Collection*& x)
{
  x = this->factory_->create_proxy_push_consumer_collection (this);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_proxy_collection (TAO_EC_ProxyPushConsumer_Collection* x)
{
  this->factory_->destroy_proxy_push_consumer_collection (x);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::create_proxy_collection (TAO_EC_ProxyPushSupplier_Collection*& x)
{
  x = this->factory_->create_proxy_push_supplier_collection (this);
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_proxy_collection (TAO_EC_ProxyPushSupplier_Collection* x)
{
  this->factory_->destroy_proxy_push_supplier_collection (x);
}

ACE_INLINE PortableServer::POA_ptr
TAO_EC_Event_Channel_Base::supplier_poa (void)
{
  return PortableServer::POA::_duplicate (this->supplier_poa_.in ());
}

ACE_INLINE PortableServer::POA_ptr
TAO_EC_Event_Channel_Base::consumer_poa (void)
{
  return PortableServer::POA::_duplicate (this->consumer_poa_.in ());
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel_Base::create_consumer_lock (void)
{
  return this->factory_->create_consumer_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_consumer_lock (ACE_Lock* x)
{
  this->factory_->destroy_consumer_lock (x);
}

ACE_INLINE ACE_Lock*
TAO_EC_Event_Channel_Base::create_supplier_lock (void)
{
  return this->factory_->create_supplier_lock ();
}

ACE_INLINE void
TAO_EC_Event_Channel_Base::destroy_supplier_lock (ACE_Lock* x)
{
  this->factory_->destroy_supplier_lock (x);
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::consumer_reconnect (void) const
{
  return this->consumer_reconnect_;
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::supplier_reconnect (void) const
{
  return this->supplier_reconnect_;
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::disconnect_callbacks (void) const
{
  return this->disconnect_callbacks_;
}

ACE_INLINE CORBA::Object_ptr
TAO_EC_Event_Channel_Base::scheduler (void)
{
  return CORBA::Object::_duplicate (this->scheduler_.in ());
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::busy_hwm (void) const
{
  return this->busy_hwm_;
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::max_write_delay (void) const
{
  return this->max_write_delay_;
}

ACE_INLINE int
TAO_EC_Event_Channel_Base::destroyed (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return (this->status_ == EC_S_DESTROYED);
}
