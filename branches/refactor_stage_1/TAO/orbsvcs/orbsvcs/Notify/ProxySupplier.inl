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

ACE_INLINE TAO_NS_ConsumerAdmin*
TAO_NS_ProxySupplier::consumer_admin (void)
{
  return this->consumer_admin_;
}
