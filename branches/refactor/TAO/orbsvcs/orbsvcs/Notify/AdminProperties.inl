// $Id$

ACE_INLINE const TAO_NS_Property_Long&
TAO_NS_AdminProperties::max_global_queue_length (void) const
{
  return this->max_global_queue_length_;
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

ACE_INLINE CORBA::Long&
TAO_NS_AdminProperties::global_queue_length (void)
{
  return this->global_queue_length_;
}

ACE_INLINE TAO_SYNCH_MUTEX&
TAO_NS_AdminProperties::global_queue_lock (void)
{
  return this->global_queue_lock_;
}

ACE_INLINE TAO_SYNCH_CONDITION&
TAO_NS_AdminProperties::global_queue_not_full_condition (void)
{
  return this->global_queue_not_full_condition_;
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
