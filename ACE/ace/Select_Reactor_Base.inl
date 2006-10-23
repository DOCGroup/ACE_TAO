// -*- C++ -*-
//
// $Id$

#include "ace/Reactor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
  
ACE_INLINE ACE_Select_Reactor_Handler_Repository::size_type
ACE_Select_Reactor_Handler_Repository::size (void) const
{
#ifdef ACE_WIN32
  return this->event_handlers_.total_size ();
#else
  return this->event_handlers_.size ();
#endif  /* ACE_WIN32 */
}
  
ACE_INLINE size_t
ACE_Select_Reactor_Handler_Repository::max_handlep1 (void) const
{
#ifdef ACE_WIN32
  return this->event_handlers_.current_size ();
#else
  return this->max_handlep1_;
#endif  /* ACE_WIN32 */
}
  
ACE_INLINE int
ACE_Select_Reactor_Handler_Repository::unbind (ACE_HANDLE handle,
                                               ACE_Reactor_Mask mask)
{
  map_type::iterator const pos = this->find_eh (handle);

  return (pos == this->event_handlers_.end ()
          ? -1
          : this->unbind (handle, pos, mask));
}
  
ACE_INLINE ACE_Event_Handler *
ACE_Select_Reactor_Handler_Repository::find (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_Select_Reactor_Handler_Repository::find");

  ACE_Event_Handler * eh = 0;

  map_type::iterator const pos = this->find_eh (handle);

  if (pos != this->event_handlers_.end ())
    {
#ifdef ACE_WIN32
      eh = (*pos).item ();
#else
      eh = *pos;
#endif  /* ACE_WIN32 */
    }
  // Don't bother setting errno.  It isn't used in the select()-based
  // reactors and incurs a TSS access.
  //   else
  //     {
  //       errno = ENOENT;
  //     }

  return eh;
}
  
// ------------------------------------------------------------------

ACE_INLINE bool
ACE_Select_Reactor_Handler_Repository_Iterator::done (void) const
{
#ifdef ACE_WIN32
  return this->current_ != this->rep_->event_handlers_.end ();
#else
  return this->current_ != (this->rep_->event_handlers_.begin ()
                            + this->rep_->max_handlep1 ());
#endif /* ACE_WIN32 */
}

// ------------------------------------------------------------------

ACE_INLINE
ACE_Event_Tuple::ACE_Event_Tuple (void)
  : handle_ (ACE_INVALID_HANDLE),
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

ACE_INLINE bool
ACE_Event_Tuple::operator== (const ACE_Event_Tuple &rhs) const
{
  return this->handle_ == rhs.handle_;
}

ACE_INLINE bool
ACE_Event_Tuple::operator!= (const ACE_Event_Tuple &rhs) const
{
  return !(*this == rhs);
}

ACE_INLINE
ACE_Select_Reactor_Impl::ACE_Select_Reactor_Impl (bool ms)
  : handler_rep_ (*this)
  , timer_queue_ (0)
  , signal_handler_ (0)
  , notify_handler_ (0)
  , delete_timer_queue_ (false)
  , delete_signal_handler_ (false)
  , delete_notify_handler_ (false)
  , initialized_ (false)
  , restart_ (0)
  , requeue_position_ (-1) // Requeue at end of waiters by default.
  , state_changed_ (0)
  , mask_signals_ (ms)
  , supress_renew_ (0)
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

ACE_END_VERSIONED_NAMESPACE_DECL
