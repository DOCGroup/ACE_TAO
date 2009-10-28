// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Notify_Consumer::Request_Queue&
TAO_Notify_Consumer::pending_events()
{
  ACE_ASSERT( pending_events_.get() != 0 );
  return *pending_events_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Consumer::is_suspended (void)
{
  return this->is_suspended_;
}

ACE_INLINE void
TAO_Notify_Consumer::suspend (void)
{
  this->is_suspended_ = 1;
}

ACE_INLINE size_t
TAO_Notify_Consumer::pending_count (void)
{
  return this->pending_events_->size();
}


TAO_END_VERSIONED_NAMESPACE_DECL
