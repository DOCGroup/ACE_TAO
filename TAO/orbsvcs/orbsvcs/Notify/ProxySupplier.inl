// $Id$

ACE_INLINE int
TAO_NS_ProxySupplier::is_connected (void)
{
  return consumer_ == 0 ? 0 : 1;
}

ACE_INLINE TAO_NS_Consumer*
TAO_NS_ProxySupplier::consumer (void)
{
  return this->consumer_;
}
