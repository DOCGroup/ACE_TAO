// $Id$

ACE_INLINE int
TAO_Notify_ProxySupplier::is_connected (void)
{
  return consumer_ == 0 ? 0 : 1;
}

ACE_INLINE TAO_Notify_Consumer*
TAO_Notify_ProxySupplier::consumer (void)
{
  return this->consumer_;
}

ACE_INLINE TAO_Notify_ConsumerAdmin*
TAO_Notify_ProxySupplier::consumer_admin (void)
{
  return this->consumer_admin_;
}
