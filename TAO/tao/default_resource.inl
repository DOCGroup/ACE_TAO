// -*- C++ -*-
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_RSF_Timer_Queue_Ptr::TAO_RSF_Timer_Queue_Ptr (TAO_Default_Resource_Factory const & rsf, ACE_Timer_Queue* tmq)
 : resource_factory_ (rsf),
   timer_queue_ (tmq)
{
}

ACE_INLINE
TAO_RSF_Timer_Queue_Ptr::~TAO_RSF_Timer_Queue_Ptr ()
{
  if (this->timer_queue_ != 0)
    this->resource_factory_.destroy_timer_queue (this->timer_queue_);
}

ACE_INLINE ACE_Timer_Queue*
TAO_RSF_Timer_Queue_Ptr::get ()
{
  return this->timer_queue_;
}

ACE_INLINE void
TAO_RSF_Timer_Queue_Ptr::release ()
{
  this->timer_queue_ = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
