/* -*- C++ -*- */
//$Id$
ACE_INLINE long
TAO_ACE_Reactive_Handler::incr_ref_count (void)
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::incr_ref_count");

  return ++this->ref_count_;
}


ACE_INLINE long
TAO_ACE_Reactive_Handler::decr_ref_count (void)
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::decr_ref_count");
  --this->ref_count_;

  // Delete the object if the reference count goes yo 0.
  if (this->ref_count_ == 0)
    {
      delete this;
      return  0;
    }

  return this->ref_count_.value ();
}

ACE_INLINE long
TAO_ACE_Reactive_Handler::refcount (void) const
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::refcount");
  return this->ref_count_.value ();
}

ACE_INLINE long
TAO_ACE_Reactive_Handler::priority (void) const
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::priority");
  return this->priority_;
}

ACE_INLINE void
TAO_ACE_Reactive_Handler::priority (long priority)
{
  ACE_TRACE ("TAO_ACE_Reactive_Handler::priority");

  if (priority < TAO_ACE_Reactive_Handler::MAX_PRIORITY &&
      priority > TAO_ACE_Reactive_Handler::MIN_PRIORITY)
    this->priority_ = priority;
}
