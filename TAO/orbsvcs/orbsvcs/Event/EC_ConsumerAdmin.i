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
