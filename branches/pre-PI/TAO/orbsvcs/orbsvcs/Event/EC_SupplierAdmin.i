// $Id$

ACE_INLINE TAO_EC_SupplierAdmin::ConsumerSetIterator
TAO_EC_SupplierAdmin::begin (void)
{
  return this->all_consumers_.begin ();
}

ACE_INLINE TAO_EC_SupplierAdmin::ConsumerSetIterator
TAO_EC_SupplierAdmin::end (void)
{
  return this->all_consumers_.end ();
}
