// $Id$

ACE_INLINE const TAO_NS_Property_Long&
TAO_NS_AdminProperties::max_queue_length (void) const
{
  return this->max_queue_length_;
}

ACE_INLINE const TAO_NS_Property_Long&
TAO_NS_AdminProperties::max_consumers (void) const
{
  return this->max_consumers_;
}

ACE_INLINE const TAO_NS_Property_Long&
TAO_NS_AdminProperties::max_suppliers (void) const
{
  return this->max_suppliers_;
}

ACE_INLINE const TAO_NS_Property_Boolean&
TAO_NS_AdminProperties::reject_new_events (void) const
{
  return this->reject_new_events_;
}

ACE_INLINE TAO_NS_Signal_Property_Long&
TAO_NS_AdminProperties::queue_length (void)
{
  return this->queue_length_;
}

ACE_INLINE TAO_NS_Atomic_Property_Long&
TAO_NS_AdminProperties::consumers (void)
{
  return this->consumers_;
}

ACE_INLINE TAO_NS_Atomic_Property_Long&
TAO_NS_AdminProperties::suppliers (void)
{
  return this->suppliers_;
}
