// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_global_queue_length (void) const
{
  return this->max_global_queue_length_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_global_queue_length (void)
{
  return this->max_global_queue_length_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_consumers (void) const
{
  return this->max_consumers_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_consumers (void)
{
  return this->max_consumers_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_suppliers (void) const
{
  return this->max_suppliers_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_suppliers (void)
{
  return this->max_suppliers_;
}

ACE_INLINE const TAO_Notify_Property_Boolean&
TAO_Notify_AdminProperties::reject_new_events (void) const
{
  return this->reject_new_events_;
}

ACE_INLINE TAO_Notify_Property_Boolean&
TAO_Notify_AdminProperties::reject_new_events (void)
{
  return this->reject_new_events_;
}

ACE_INLINE CORBA::Long&
TAO_Notify_AdminProperties::global_queue_length (void)
{
  return this->global_queue_length_;
}

ACE_INLINE TAO_SYNCH_MUTEX&
TAO_Notify_AdminProperties::global_queue_lock (void)
{
  return this->global_queue_lock_;
}

ACE_INLINE TAO_SYNCH_CONDITION&
TAO_Notify_AdminProperties::global_queue_not_full(void)
{
  return this->global_queue_not_full_;
}

ACE_INLINE TAO_Notify_Atomic_Property_Long&
TAO_Notify_AdminProperties::consumers (void)
{
  return this->consumers_;
}

ACE_INLINE TAO_Notify_Atomic_Property_Long&
TAO_Notify_AdminProperties::suppliers (void)
{
  return this->suppliers_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
