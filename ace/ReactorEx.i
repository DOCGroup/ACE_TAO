/* -*- C++ -*- */
// $Id$

#if defined (ACE_WIN32)

/************************************************************/

ACE_INLINE int
ACE_Wakeup_All_Threads_Handler::handle_signal (int signum, 
					       siginfo_t *siginfo, 
					       ucontext_t *)
{
  // This will get called when <ReactorEx->wakeup_all_threads_> event
  // is signaled. There is nothing to be done here.
  //  ACE_DEBUG ((LM_DEBUG, "(%t) waking up to get updated handle set info\n"));
  return 0;
}

/************************************************************/

ACE_INLINE 
ACE_ReactorEx_Handler_Repository::Common_Info::Common_Info (void)
  : event_handler_ (0),
    io_entry_ (0),
    io_handle_ (ACE_INVALID_HANDLE),    
    network_events_ (0),
    delete_event_ (0)
{
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Common_Info::reset (void)
{
  this->event_handler_ = 0;
  this->io_entry_ = 0;
  this->io_handle_ = ACE_INVALID_HANDLE;
  this->network_events_ = 0;
  this->delete_event_ = 0;
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Common_Info::set (int io_entry,
						    ACE_Event_Handler *event_handler,
						    ACE_HANDLE io_handle,
						    long network_events,
						    int delete_event)
{
  this->event_handler_ = event_handler;
  this->io_entry_ = io_entry;
  this->io_handle_ = io_handle;
  this->network_events_ = network_events;
  this->delete_event_ = delete_event;
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Common_Info::set (Common_Info &common_info)
{
  *this = common_info;
}

/************************************************************/

ACE_INLINE 
ACE_ReactorEx_Handler_Repository::Current_Info::Current_Info (void)
  : delete_entry_ (0),
    close_masks_ (0),
    suspend_entry_ (0)
{  
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::Current_Info::set (int io_entry,
						     ACE_Event_Handler *event_handler,
						     ACE_HANDLE io_handle,
						     long network_events,
						     int delete_event,
						     int delete_entry,
						     ACE_Reactor_Mask close_masks,
						     int suspend_entry)
{
  this->delete_entry_ = delete_entry;
  this->close_masks_ = close_masks;
  this->suspend_entry_ = suspend_entry;
  Common_Info::set (io_entry,
		    event_handler,
		    io_handle,
		    network_events,
		    delete_event);
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::Current_Info::set (Common_Info &common_info,
						     int delete_entry,
						     ACE_Reactor_Mask close_masks,
						     int suspend_entry)
{
  this->delete_entry_ = delete_entry;
  this->close_masks_ = close_masks;
  this->suspend_entry_ = suspend_entry;
  Common_Info::set (common_info);
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::Current_Info::reset (void)
{
  this->delete_entry_ = 0;
  this->close_masks_ = 0;
  this->suspend_entry_ = 0;
  Common_Info::reset ();
}

/************************************************************/

ACE_INLINE 
ACE_ReactorEx_Handler_Repository::To_Be_Added_Info::To_Be_Added_Info (void)
  : event_handle_ (ACE_INVALID_HANDLE)
{  
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::To_Be_Added_Info::set (ACE_HANDLE event_handle,
							 int io_entry,
							 ACE_Event_Handler *event_handler,
							 ACE_HANDLE io_handle,
							 long network_events,
							 int delete_event)
{
  this->event_handle_ = event_handle;
  Common_Info::set (io_entry,
		    event_handler,
		    io_handle,
		    network_events,
		    delete_event);
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::To_Be_Added_Info::set (ACE_HANDLE event_handle,
							 Common_Info &common_info)
{
  this->event_handle_ = event_handle;
  Common_Info::set (common_info);
}

ACE_INLINE void 
ACE_ReactorEx_Handler_Repository::To_Be_Added_Info::reset (void)
{
  this->event_handle_ = ACE_INVALID_HANDLE;
  Common_Info::reset ();
}

/************************************************************/

ACE_INLINE 
ACE_ReactorEx_Handler_Repository::Suspended_Info::Suspended_Info (void)
  : event_handle_ (ACE_INVALID_HANDLE),
    resume_entry_ (0),
    delete_entry_ (0),
    close_masks_ (0)
{
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Suspended_Info::reset (void)
{
  this->event_handle_ = ACE_INVALID_HANDLE;
  this->resume_entry_ = 0;
  this->delete_entry_ = 0;
  this->close_masks_ = 0;
  Common_Info::reset ();
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Suspended_Info::set (ACE_HANDLE event_handle,
						       int io_entry,
						       ACE_Event_Handler *event_handler,
						       ACE_HANDLE io_handle,
						       long network_events,
						       int delete_event,
						       int resume_entry,
						       int delete_entry,
						       ACE_Reactor_Mask close_masks)
{
  this->event_handle_ = event_handle;
  this->resume_entry_ = resume_entry;
  this->delete_entry_ = delete_entry;
  this->close_masks_ = close_masks;
  Common_Info::set (io_entry,
		    event_handler,
		    io_handle,
		    network_events,
		    delete_event);  
}

ACE_INLINE void
ACE_ReactorEx_Handler_Repository::Suspended_Info::set (ACE_HANDLE event_handle,
						       Common_Info &common_info,
						       int resume_entry,
						       int delete_entry,
						       ACE_Reactor_Mask close_masks)
{
  this->event_handle_ = event_handle;
  this->resume_entry_ = resume_entry;
  this->delete_entry_ = delete_entry;
  this->close_masks_ = close_masks;
  Common_Info::set (common_info);  
}

/************************************************************/

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::close (void)
{
  // Let all the handlers know that the <ReactorEx> is closing down
  this->unbind_all ();

  return 0;
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
  if (ACE_Thread::self () == this->reactorEx_.owner_i ())
    return this->current_handles_;
  else
    return this->current_handles_ + 1;
}

ACE_INLINE ACE_ReactorEx_Handler_Repository::Current_Info *
ACE_ReactorEx_Handler_Repository::current_info (void) const
{
  if (ACE_Thread::self () == this->reactorEx_.owner_i ())
    return this->current_info_;
  else
    return this->current_info_ + 1;
}

ACE_INLINE size_t
ACE_ReactorEx_Handler_Repository::max_handlep1 (void) const
{
  if (ACE_Thread::self () == this->reactorEx_.owner_i ())
    return this->max_handlep1_;
  else
    return this->max_handlep1_ - 1;
}

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::scheduled_for_deletion (size_t index) const
{
  if (ACE_Thread::self () == this->reactorEx_.owner_i ())
    return this->current_info_[index].delete_entry_ == 1;
  else
    return this->current_info_[index + 1].delete_entry_ == 1;
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

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::changes_required (void)
{
  // Check if handles have be scheduled for additions or removal
  return this->handles_to_be_added_ > 0     || 
         this->handles_to_be_deleted_ > 0   ||
	 this->handles_to_be_suspended_ > 0 ||
         this->handles_to_be_resumed_ > 0;
}

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::make_changes (void)
{
  // This method must ONLY be called by the
  // <ReactorEx->change_state_thread_>. We therefore assume that there
  // will be no contention for this method and hence no guards are
  // neccessary.

  // DELETIONS first
  this->handle_deletions ();

  // ADDITIONS here
  this->handle_additions ();

  return 0;
}

ACE_INLINE int
ACE_ReactorEx_Handler_Repository::unbind (ACE_HANDLE handle,
					  ACE_Reactor_Mask mask)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->reactorEx_.lock_, -1);
    
  int changes_required = 0;
  int result = this->unbind_i (handle, mask, changes_required);

  if (changes_required)
    // Wake up all threads in WaitForMultipleObjects so that they can
    // reconsult the handle set
    this->reactorEx_.wakeup_all_threads ();  
  
  return result;
}

/************************************************************/

ACE_INLINE int
ACE_ReactorEx::schedule_timer (ACE_Event_Handler *handler,
			       const void *arg,
			       const ACE_Time_Value &delta_time, 
			       const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_ReactorEx::schedule_timer");

  int result = this->timer_queue_->schedule 
    (handler, arg, timer_queue_->gettimeofday () + delta_time, interval);
  
  // Wakeup the owner thread so that it gets the latest timer values
  this->notify ();
  
  return result;
}

ACE_INLINE int
ACE_ReactorEx::cancel_timer (ACE_Event_Handler *handler,
			     int dont_call_handle_close)
{
  ACE_TRACE ("ACE_ReactorEx::cancel_timer");
  return this->timer_queue_->cancel (handler, dont_call_handle_close);
}

ACE_INLINE int
ACE_ReactorEx::cancel_timer (int timer_id, 
			     const void **arg,
			     int dont_call_handle_close)
{
  ACE_TRACE ("ACE_ReactorEx::cancel_timer");
  return this->timer_queue_->cancel (timer_id, arg, dont_call_handle_close);
}

ACE_INLINE int 
ACE_ReactorEx::register_handler (ACE_Event_Handler *event_handler,
				 ACE_HANDLE event_handle)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  return this->handler_rep_.bind_i (0, 
				    event_handler, 
				    0, 
				    ACE_INVALID_HANDLE, 
				    event_handle, 
				    0);
}

ACE_INLINE int
ACE_ReactorEx::register_handler (ACE_Event_Handler *eh,
				 ACE_Reactor_Mask mask)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  return this->register_handler_i (ACE_INVALID_HANDLE,
				   ACE_INVALID_HANDLE,
				   eh, 
				   mask);
}

ACE_INLINE int
ACE_ReactorEx::register_handler (ACE_HANDLE io_handle, 
				 ACE_Event_Handler *eh,
				 ACE_Reactor_Mask mask)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  return this->register_handler_i (ACE_INVALID_HANDLE,
				   io_handle,
				   eh, 
				   mask);
}

ACE_INLINE int 
ACE_ReactorEx::register_handler (ACE_HANDLE event_handle,
				 ACE_HANDLE io_handle, 
				 ACE_Event_Handler *event_handler,
				 ACE_Reactor_Mask mask)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  return this->register_handler_i (event_handle,
				   io_handle,
				   event_handler,
				   mask);

}

ACE_INLINE int
ACE_ReactorEx::register_handler (const ACE_Handle_Set &handles, 
				 ACE_Event_Handler *handler, 
				 ACE_Reactor_Mask mask)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;

  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->register_handler_i (h, ACE_INVALID_HANDLE, handler, mask) == -1)
      return -1;
  
  return 0;    
}

ACE_INLINE int
ACE_ReactorEx::remove_handler (const ACE_Handle_Set &handles,
			       ACE_Reactor_Mask mask)
{
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;
  int changes_required = 0;

  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
    
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->handler_rep_.unbind_i (h, mask, changes_required) == -1)
      return -1;
  
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wakeup_all_threads ();  
  
  return 0;
}

ACE_INLINE int 
ACE_ReactorEx::remove_handler (ACE_Event_Handler *eh,
			       ACE_Reactor_Mask mask)
{
  return this->handler_rep_.unbind (eh->get_handle (), mask);
}

ACE_INLINE int 
ACE_ReactorEx::remove_handler (ACE_HANDLE handle,
			       ACE_Reactor_Mask mask)
{
  return this->handler_rep_.unbind (handle, mask);
}

ACE_INLINE int
ACE_ReactorEx::suspend_handler (ACE_HANDLE handle)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
    
  int changes_required = 0;
  int result = this->handler_rep_.suspend_handler_i (handle, changes_required);

  if (changes_required)
    // Wake up all threads in WaitForMultipleObjects so that they can
    // reconsult the handle set
    this->wakeup_all_threads ();  
  
  return result;
}

ACE_INLINE int
ACE_ReactorEx::suspend_handler (ACE_Event_Handler *eh)
{
  return this->suspend_handler (eh->get_handle ());
}

ACE_INLINE int
ACE_ReactorEx::suspend_handler (const ACE_Handle_Set &handles)
{
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;
  int changes_required = 0;

  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
    
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->handler_rep_.suspend_handler_i (h, changes_required) == -1)
      return -1;
  
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wakeup_all_threads ();  
  
  return 0;
}

ACE_INLINE int
ACE_ReactorEx::suspend_all (void)
{
  int error = 0;
  int result = 0;
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  int changes_required = 0;
  int total_handles = this->handler_rep_.max_handlep1_ - 1;
  for (size_t i = 0; i < total_handles && error == 0; i++)
    {      
      result = this->handler_rep_.suspend_handler_i (this->handler_rep_.current_handles_[i], changes_required);
      if (result == -1)
	error = 1;
    }

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wakeup_all_threads ();  
  
  return error ? -1 : 0;
}

ACE_INLINE int
ACE_ReactorEx::resume_handler (ACE_HANDLE handle)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
    
  int changes_required = 0;
  int result = this->handler_rep_.resume_handler_i (handle, changes_required);

  if (changes_required)
    // Wake up all threads in WaitForMultipleObjects so that they can
    // reconsult the handle set
    this->wakeup_all_threads ();  
  
  return result;
}

ACE_INLINE int
ACE_ReactorEx::resume_handler (ACE_Event_Handler *eh)
{
  return this->resume_handler (eh->get_handle ());
}

ACE_INLINE int
ACE_ReactorEx::resume_handler (const ACE_Handle_Set &handles)
{
  ACE_Handle_Set_Iterator handle_iter (handles);
  ACE_HANDLE h;
  int changes_required = 0;

  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
    
  while ((h = handle_iter ()) != ACE_INVALID_HANDLE)
    if (this->handler_rep_.resume_handler_i (h, changes_required) == -1)
      return -1;
  
  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wakeup_all_threads ();  
  
  return 0;
}

ACE_INLINE int
ACE_ReactorEx::resume_all (void)
{
  int error = 0;
  int result = 0;
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  
  int changes_required = 0;
  for (size_t i = 0; i < this->handler_rep_.suspended_handles_ && error == 0; i++)
    {      
      result = this->handler_rep_.resume_handler_i (this->handler_rep_.current_suspended_info_[i].event_handle_, 
						    changes_required);
      if (result == -1)
	error = 1;
    }

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->wakeup_all_threads ();  
  
  return error ? -1 : 0;
}

ACE_INLINE int
ACE_ReactorEx::handle_events (ACE_Time_Value &how_long)
{
  return this->event_handling (&how_long, 0);
}

ACE_INLINE int
ACE_ReactorEx::alertable_handle_events (ACE_Time_Value &how_long)
{
  return this->event_handling (&how_long, 1);
}

ACE_INLINE int
ACE_ReactorEx::handle_events (ACE_Time_Value *how_long)
{
  return this->event_handling (how_long, 0);
}

ACE_INLINE int
ACE_ReactorEx::alertable_handle_events (ACE_Time_Value *how_long)
{
  return this->event_handling (how_long, 1);
}

ACE_INLINE int
ACE_ReactorEx::owner (ACE_thread_t *t)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);
  *t = this->owner_i ();
  return 0;
}

ACE_INLINE ACE_thread_t 
ACE_ReactorEx::owner_i (void)
{
  return this->owner_;
}

ACE_INLINE int
ACE_ReactorEx::owner (ACE_thread_t new_owner, ACE_thread_t *old_owner)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, monitor, this->lock_, -1);
  this->new_owner_ = new_owner;

  if (old_owner != 0)
    *old_owner = this->owner_i ();

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the new owner responsibilities
  this->wakeup_all_threads ();

  return 0;
}

ACE_INLINE int
ACE_ReactorEx::new_owner (void)
{
  return this->new_owner_ != ACE_thread_t (0);
}

ACE_INLINE int
ACE_ReactorEx::change_owner (void)
{
  this->owner_ = this->new_owner_;
  this->new_owner_ = ACE_thread_t (0);
  return 0;
}

ACE_INLINE int
ACE_ReactorEx::safe_dispatch (int wait_status)
{
  int result;
  ACE_SEH_TRY
    {
      result = this->dispatch (wait_status);  
    }
  ACE_SEH_FINALLY
    {
      this->update_state ();
    }

  return result;
}

ACE_INLINE void
ACE_ReactorEx::wakeup_all_threads (void)
{
  this->wakeup_all_threads_.signal ();
}

ACE_INLINE int 
ACE_ReactorEx::notify (ACE_Event_Handler *eh,
		       ACE_Reactor_Mask mask,
		       ACE_Time_Value *timeout)
{
  return this->notify_handler_.notify (eh, mask, timeout);
}

#endif /* ACE_WIN32 */
