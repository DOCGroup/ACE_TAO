// $Id$

ACE_INLINE TAO_EC_ProxyPushSupplier_Set::SupplierSetIterator
TAO_EC_ProxyPushSupplier_Set::begin (void)
{
  return this->all_suppliers_.begin ();
}

ACE_INLINE TAO_EC_ProxyPushSupplier_Set::SupplierSetIterator
TAO_EC_ProxyPushSupplier_Set::end (void)
{
  return this->all_suppliers_.end ();
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier_Set::busy_hwm (CORBA::ULong hwm)
{
  this->busy_hwm_ = hwm;
}

ACE_INLINE CORBA::ULong
TAO_EC_ProxyPushSupplier_Set::busy_hwm (void) const
{
  return this->busy_hwm_;
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier_Set::max_write_delay (CORBA::ULong mwd)
{
  this->max_write_delay_ = mwd;
}

ACE_INLINE CORBA::ULong
TAO_EC_ProxyPushSupplier_Set::max_write_delay (void) const
{
  return this->max_write_delay_;
}

ACE_INLINE TAO_EC_ProxyPushSupplier_Set::Busy_Lock&
TAO_EC_ProxyPushSupplier_Set::busy_lock (void)
{
  return this->busy_lock_;
}
