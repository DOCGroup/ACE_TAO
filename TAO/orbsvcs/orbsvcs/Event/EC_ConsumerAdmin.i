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
