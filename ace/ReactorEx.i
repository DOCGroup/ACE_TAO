/* -*- C++ -*- */
// $Id$

#if defined (ACE_WIN32)

ACE_INLINE int
ACE_ReactorEx::cancel_timer (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_ReactorEx::cancel_timer");
  return this->timer_queue_->cancel (handler);
}

ACE_INLINE int
ACE_ReactorEx::cancel_timer (int timer_id, 
			     const void **arg)
{
  ACE_TRACE ("ACE_ReactorEx::cancel_timer");
  return this->timer_queue_->cancel (timer_id, arg);
}

ACE_INLINE int
ACE_ReactorEx::handle_events (ACE_Time_Value &how_long,
			      int alertable)
{
  return this->handle_events (&how_long, alertable);
}

ACE_INLINE ACE_HANDLE *
ACE_ReactorEx_Handler_Repository::handles (void) const
{
  // This code is probably too subtle to be useful in the long run...
  // The basic idea is that all threads wait on all user handles plus
  // the <wakeup_all_threads_> handle. The owner thread additional
  // waits on the <notify_> handle. This is to ensure that only the
  // <owner_> thread get to expire timers and handle event on the
  // notify pipe.
  if (ACE_Thread::self () == this->reactorEx_.owner ())
    return this->current_handles_;
  else
    return this->current_handles_ + 1;
}

ACE_INLINE ACE_Event_Handler **
ACE_ReactorEx_Handler_Repository::event_handlers (void) const
{
  if (ACE_Thread::self () == this->reactorEx_.owner ())
    return this->current_event_handlers_;
  else
    return this->current_event_handlers_ + 1;
}

ACE_INLINE size_t
ACE_ReactorEx_Handler_Repository::max_handlep1 (void) const
{
  if (ACE_Thread::self () == this->reactorEx_.owner ())
    return this->max_handlep1_;
  else
    return this->max_handlep1_ - 1;
}

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::invalid_handle (ACE_HANDLE handle) const
{
  ACE_TRACE ("ACE_ReactorEx_Handler_Repository::invalid_handle");
  // It's too expensive to perform more exhaustive validity checks on
  // Win32 due to the way that they implement SOCKET HANDLEs.
  if (handle == ACE_INVALID_HANDLE)
    {
      errno = EINVAL;
      return 1;
    }
  else
    return 0;
}

ACE_INLINE ACE_thread_t 
ACE_ReactorEx::owner (void)
{
  return this->owner_;
}

ACE_INLINE void
ACE_ReactorEx::owner (ACE_thread_t new_owner)
{
  this->owner_ = new_owner;
}

#endif /* ACE_WIN32 */
