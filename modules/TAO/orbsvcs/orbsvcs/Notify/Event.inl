// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const TAO_Notify_Property_Short&
TAO_Notify_Event::priority (void) const
{
  return this->priority_;
}

ACE_INLINE const TAO_Notify_Property_Time&
TAO_Notify_Event::timeout (void) const
{
  return this->timeout_;
}

ACE_INLINE const TAO_Notify_Property_Boolean&
TAO_Notify_Event::reliable (void) const
{
  return this->reliable_;
}

ACE_INLINE const ACE_Time_Value&
TAO_Notify_Event::creation_time (void) const
{
  return this->time_;
}

ACE_INLINE
TAO_Notify_Event*
TAO_Notify_Event::queueable_copy (void) const
{
  if ( is_on_heap_ )
  {
    return const_cast< TAO_Notify_Event* >( this );
  }
  else if (this->clone_.get() == 0)
  {
    TAO_Notify_Event* copied = this->copy ();
    copied->is_on_heap_ = true;
    this->clone_.reset( copied );
  }
  return this->clone_.get();
}

TAO_END_VERSIONED_NAMESPACE_DECL
