// $Id$

ACE_INLINE int
TAO_NS_ProxyConsumer::is_connected (void)
{
  return supplier_ == 0 ? 0 : 1;
}

ACE_INLINE TAO_NS_Supplier*
TAO_NS_ProxyConsumer::supplier (void)
{
  return this->supplier_;
}
