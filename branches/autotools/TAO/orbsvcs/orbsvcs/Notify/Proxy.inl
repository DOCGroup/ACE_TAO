// $Id$

ACE_INLINE TAO_NS_EventTypeSeq&
TAO_NS_Proxy::subscribed_types (void)
{
  return this->subscribed_types_;
}

ACE_INLINE CORBA::Boolean
TAO_NS_Proxy::updates_off (void)
{
  return this->updates_off_;
}
