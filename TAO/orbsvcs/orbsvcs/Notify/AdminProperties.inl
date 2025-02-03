// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_global_queue_length () const
{
  return this->max_global_queue_length_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_global_queue_length ()
{
  return this->max_global_queue_length_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_consumers () const
{
  return this->max_consumers_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_consumers ()
{
  return this->max_consumers_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_suppliers () const
{
  return this->max_suppliers_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_AdminProperties::max_suppliers ()
{
  return this->max_suppliers_;
}

ACE_INLINE const TAO_Notify_Property_Boolean&
TAO_Notify_AdminProperties::reject_new_events () const
{
  return this->reject_new_events_;
}

ACE_INLINE TAO_Notify_Property_Boolean&
TAO_Notify_AdminProperties::reject_new_events ()
{
  return this->reject_new_events_;
}

ACE_INLINE CORBA::Long&
TAO_Notify_AdminProperties::global_queue_length ()
{
  return this->global_queue_length_;
}

ACE_INLINE TAO_SYNCH_MUTEX&
TAO_Notify_AdminProperties::global_queue_lock ()
{
  return this->global_queue_lock_;
}

ACE_INLINE TAO_SYNCH_CONDITION&
TAO_Notify_AdminProperties::global_queue_not_full()
{
  return this->global_queue_not_full_;
}

ACE_INLINE TAO_Notify_Atomic_Property_Long&
TAO_Notify_AdminProperties::consumers ()
{
  return this->consumers_;
}

ACE_INLINE TAO_Notify_Atomic_Property_Long&
TAO_Notify_AdminProperties::suppliers ()
{
  return this->suppliers_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
