/* -*- C++ -*- */
// $Id$


ACE_INLINE off_t
ACE_Overlapped_File::offset (void) const
{
  ACE_TRACE ("ACE_Overlapped_File::offset");
  return this->offset_;
}

ACE_INLINE off_t
ACE_Overlapped_File::size (void) const
{
  ACE_TRACE ("ACE_Overlapped_File::size");
  return ACE_OS::filesize (this->handle_);
}

ACE_INLINE ACE_HANDLE
ACE_Overlapped_File::get_handle (void) const
{
  ACE_TRACE ("ACE_Overlapped_File::get_handle");
  return this->handle_;
}

ACE_INLINE int
ACE_Proactor::cancel_timer (ACE_Event_Handler *handler)
{
  ACE_TRACE ("ACE_Proactor::cancel_timer");
  return this->timer_queue_->cancel (handler);
}

ACE_INLINE int
ACE_Proactor::cancel_timer (int timer_id, 
			    const void **arg)
{
  ACE_TRACE ("ACE_Proactor::cancel_timer");
  return this->timer_queue_->cancel (timer_id, arg);
}

ACE_INLINE ACE_HANDLE
ACE_Proactor::get_handle (void) const
{
  ACE_TRACE ("ACE_Proactor::get_handle");

  return this->shared_event_.handle ();
}

ACE_INLINE int
ACE_Proactor::handle_events (ACE_Time_Value &how_long)
{
  return this->handle_events (&how_long);
}

