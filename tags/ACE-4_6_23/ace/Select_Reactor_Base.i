/* -*- C++ -*- */
// $Id$

#include "ace/Reactor.h"

ACE_INLINE
ACE_Event_Tuple::~ACE_Event_Tuple (void)
{
}

ACE_INLINE
ACE_Select_Reactor_Notify::~ACE_Select_Reactor_Notify (void)
{
}

ACE_INLINE
ACE_Select_Reactor_Handler_Repository::~ACE_Select_Reactor_Handler_Repository (void)
{
}

ACE_INLINE
ACE_Select_Reactor_Handler_Repository_Iterator::~ACE_Select_Reactor_Handler_Repository_Iterator (void)
{
}

ACE_INLINE size_t
ACE_Select_Reactor_Handler_Repository::size (void)
{
  return this->max_size_;
}

ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (void)
:  handle_ (ACE_INVALID_HANDLE),
   event_handler_ (0)
{
}

ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (ACE_Event_Handler* eh,
				  ACE_HANDLE h)
: handle_ (h),
  event_handler_ (eh)
{
}

ACE_INLINE int
ACE_Event_Tuple::operator== (const ACE_Event_Tuple &rhs) const
{
  return this->handle_ == rhs.handle_;
}

ACE_INLINE int
ACE_Event_Tuple::operator!= (const ACE_Event_Tuple &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE
ACE_Select_Reactor_Impl::ACE_Select_Reactor_Impl ()
    : handler_rep_ (*this),
      timer_queue_ (0),
      delete_timer_queue_ (0),
      delete_signal_handler_ (0),
      delete_notify_handler_ (0),
      requeue_position_ (-1), // Requeue at end of waiters by default.
      initialized_ (0),
      state_changed_ (0),
      supress_renew_ (0)
{
}

ACE_INLINE int
ACE_Select_Reactor_Impl::supress_notify_renew (void)
{
  return this->supress_renew_;
}

ACE_INLINE void
ACE_Select_Reactor_Impl::supress_notify_renew (int sr)
{
  this->supress_renew_ = sr;
}
