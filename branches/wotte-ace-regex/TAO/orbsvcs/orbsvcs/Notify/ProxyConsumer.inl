// $Id$

ACE_INLINE int
TAO_Notify_ProxyConsumer::is_connected (void)
{
  return supplier_ == 0 ? 0 : 1;
}

ACE_INLINE TAO_Notify_Supplier*
TAO_Notify_ProxyConsumer::supplier (void)
{
  return this->supplier_;
}

ACE_INLINE TAO_Notify_SupplierAdmin*
TAO_Notify_ProxyConsumer::supplier_admin (void)
{
  return this->supplier_admin_;
}
