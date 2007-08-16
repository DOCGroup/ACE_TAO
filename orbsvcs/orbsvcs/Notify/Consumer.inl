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

TAO_END_VERSIONED_NAMESPACE_DECL
