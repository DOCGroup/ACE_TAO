// $Id$

ACE_INLINE u_long
TAO_NS_EventType::hash (void) const
{
  return this->hash_value_;
}

ACE_INLINE const CosNotification::EventType&
TAO_NS_EventType::native (void) const
{
  return event_type_;
}
