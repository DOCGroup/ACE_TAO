// $Id$

ACE_INLINE TAO_EC_ConsumerAdmin::SupplierSetIterator
TAO_EC_ConsumerAdmin::begin (void)
{
  return this->supplier_set_->begin ();
}

ACE_INLINE TAO_EC_ConsumerAdmin::SupplierSetIterator
TAO_EC_ConsumerAdmin::end (void)
{
  return this->supplier_set_->end ();
}

ACE_INLINE TAO_EC_ConsumerAdmin::Busy_Lock&
TAO_EC_ConsumerAdmin::busy_lock (void)
{
  return this->supplier_set_->busy_lock ();
}

ACE_INLINE void
TAO_EC_ConsumerAdmin::busy_hwm (CORBA::ULong hwm)
{
  this->supplier_set_->busy_hwm (hwm);
}

ACE_INLINE CORBA::ULong
TAO_EC_ConsumerAdmin::busy_hwm (void) const
{
  return this->supplier_set_->busy_hwm ();
}

ACE_INLINE void
TAO_EC_ConsumerAdmin::max_write_delay (CORBA::ULong hwm)
{
  this->supplier_set_->max_write_delay (hwm);
}

ACE_INLINE CORBA::ULong
TAO_EC_ConsumerAdmin::max_write_delay (void) const
{
  return this->supplier_set_->max_write_delay ();
}
