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
			      int wait_all,
			      ACE_Event_Handler *wait_all_callback,
			      int alertable)
{
  return this->handle_events (&how_long, wait_all, 
			      wait_all_callback, alertable);
}

ACE_INLINE ACE_HANDLE *
ACE_ReactorEx_Handler_Repository::handles (void) const
{
  return this->handles_;
}

ACE_INLINE size_t
ACE_ReactorEx_Handler_Repository::max_handlep1 (void) const
{
  return this->max_handlep1_;
}

ACE_INLINE ACE_Event_Handler *
ACE_ReactorEx_Handler_Repository::find (size_t index) const
{
  if (this->handle_in_range (index))
    return this->event_handlers_[index];
  else
    {
      errno = ENOENT;
      return 0;
    }
}

#endif /* ACE_WIN32 */
