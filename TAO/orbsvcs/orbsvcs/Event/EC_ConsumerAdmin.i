// $Id$

ACE_INLINE TAO_EC_ConsumerAdmin::SupplierSetIterator
TAO_EC_ConsumerAdmin::begin (void)
{
  return this->all_suppliers_.begin ();
}

ACE_INLINE TAO_EC_ConsumerAdmin::SupplierSetIterator
TAO_EC_ConsumerAdmin::end (void)
{
  return this->all_suppliers_.end ();
}

ACE_INLINE void
TAO_EC_ConsumerAdmin::busy_hwm (CORBA::ULong hwm)
{
  this->busy_hwm_ = hwm;
}

ACE_INLINE CORBA::ULong
TAO_EC_ConsumerAdmin::busy_hwm (void) const
{
  return this->busy_hwm_;
}

ACE_INLINE TAO_EC_ConsumerAdmin::Busy_Lock&
TAO_EC_ConsumerAdmin::busy_lock (void)
{
  return this->busy_lock_;
}
