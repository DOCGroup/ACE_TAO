// ReactorEx.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/ReactorEx.h"
#include "ace/Timer_List.h"
#include "ace/Thread.h"

#if defined (ACE_WIN32)

#if !defined (__ACE_INLINE__)
#include "ace/ReactorEx.i"
#endif /* __ACE_INLINE__ */

int
ACE_Wakeup_All_Threads_Handler::handle_signal (int signum, 
					       siginfo_t *siginfo, 
					       ucontext_t *)
{
  // This will get called when <ReactorEx->wakeup_all_threads_> event
  // is signaled. There is nothing to be done here.
  ACE_DEBUG ((LM_DEBUG, "(%t) waking up to get updated handle set info\n"));
  return 0;
}

ACE_ReactorEx_Handler_Repository::ACE_ReactorEx_Handler_Repository (ACE_ReactorEx &reactorEx)
  : reactorEx_ (reactorEx)
{
}

int
ACE_ReactorEx_Handler_Repository::open (size_t size)
{
  // Dynamic allocation
  ACE_NEW_RETURN (this->current_handles_, ACE_HANDLE[size], -1);
  ACE_NEW_RETURN (this->current_event_handlers_, ACE_Event_Handler *[size], -1);
  ACE_NEW_RETURN (this->to_be_added_handles_, ACE_HANDLE[size], -1);
  ACE_NEW_RETURN (this->to_be_added_event_handlers_, ACE_Event_Handler *[size], -1);
  ACE_NEW_RETURN (this->to_be_deleted_set_, int[size], -1);

  // Initialization
  this->max_size_ = size;
  this->max_handlep1_ = 0;
  this->handles_to_be_added_ = 0;
  this->handles_to_be_deleted_ = 0;
  for (size_t i = 0; i < size; i++)
    {
      this->current_handles_[i] = ACE_INVALID_HANDLE;
      this->current_event_handlers_[i] = 0;
      this->to_be_added_handles_[i] = ACE_INVALID_HANDLE;
      this->to_be_added_event_handlers_[i] = 0;
      this->to_be_deleted_set_[i] = 0;
    }

  return 0;
}

int
ACE_ReactorEx_Handler_Repository::close (void)
{
  // Let all the handlers know that the <ReactorEx> is closing down
  this->unbind_all ();

  return 0;
}


ACE_ReactorEx_Handler_Repository::~ACE_ReactorEx_Handler_Repository (void)
{
  // Free up dynamically allocated space
  delete[] this->current_handles_;
  delete[] this->current_event_handlers_;
  delete[] this->to_be_added_handles_;
  delete[] this->to_be_added_event_handlers_;
  delete[] this->to_be_deleted_set_;
}

int 
ACE_ReactorEx_Handler_Repository::remove_handler_i (size_t index,
						    ACE_Reactor_Mask mask)
{
  // Make sure that the DONT_CALL mask is not set and that we haven't
  // already called handle_close() on this handle.
  if (this->to_be_deleted_set_[index] == 0)
    {
      // Add to <to_be_deleted_set_>
      this->to_be_deleted_set_[index] = 1;
      this->handles_to_be_deleted_++;

      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
	this->current_event_handlers_[index]->handle_close 
	  (this->current_handles_[index], mask);
      return 0;
    }
  else 
    return -1;
}

int
ACE_ReactorEx_Handler_Repository::unbind (ACE_HANDLE handle,
					  ACE_Reactor_Mask mask)
{
  int error = 0;
  {
    ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->reactorEx_.lock_, -1);
    
    // Go through all the handles looking for <handle>.  Even if we find
    // it, we continue through the rest of the list since <handle> could
    // appear multiple times. All handles are checked except the 0th one
    // and the 1st one (i.e., the "wakeup all" event and the notify
    // event).
    int result = 0;
    for (size_t i = 2; i < this->max_handlep1_ && error == 0; i++)
      if (this->current_handles_[i] == handle)
	{
	  result = this->remove_handler_i (i, mask);
	  if (result == -1)
	    error = 1;
	}
  }
  // The guard is released here

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->reactorEx_.wakeup_all_threads ();

  return error == 0 ? 0 : -1;
}

void
ACE_ReactorEx_Handler_Repository::unbind_all (void)
{
  {
    ACE_GUARD (ACE_Process_Mutex, ace_mon, this->reactorEx_.lock_);
    
    // Remove all the handlers except the 0th one and the 1st one (i.e.,
    // the "wakeup all" event and the notify event).
    for (size_t i = 2; i < this->max_handlep1_; i++)
      this->remove_handler_i (i, ACE_Event_Handler::NULL_MASK);
  }
  // The guard is released here

  // Wake up all threads in WaitForMultipleObjects so that they can
  // reconsult the handle set
  this->reactorEx_.wakeup_all_threads ();
}

int
ACE_ReactorEx_Handler_Repository::bind (ACE_HANDLE handle,
					ACE_Event_Handler *event_handler)
{
  // Make sure that the <handle> is valid
  if (handle == ACE_INVALID_HANDLE)
    handle = event_handler->get_handle ();

  if (this->invalid_handle (handle))
    return -1;
  
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->reactorEx_.lock_, -1);
  
  int current_size = this->max_handlep1_ + 
                     this->handles_to_be_added_ - 
                     this->handles_to_be_deleted_;

  // Make sure that there's room in the table.
  if (current_size < this->max_size_)
    {
      // Cache this set into the <to_be_added_*> arrays, till we come
      // around to actually add to the <current_*> arrays
      this->to_be_added_handles_[this->handles_to_be_added_] = handle;
      this->to_be_added_event_handlers_[this->handles_to_be_added_] = event_handler;
      this->handles_to_be_added_++;

      // Assign *this* <ReactorEx> to the <Event_Handler>.
      event_handler->reactorEx (&this->reactorEx_);      

      // Wake up all threads in WaitForMultipleObjects so that they can
      // reconsult the handle set
      this->reactorEx_.wakeup_all_threads ();
      return 0;
    }
  else
    return -1;
}

int
ACE_ReactorEx_Handler_Repository::changes_required (void)
{
  // Check if handles have be scheduled for additions or removal
  return this->handles_to_be_added_ > 0 
    || this->handles_to_be_deleted_ > 0;
}

int
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

int
ACE_ReactorEx_Handler_Repository::handle_deletions (void)
{
  // This will help us in keeping track of the last valid index in the
  // handle arrays
  int last_valid_index = this->max_handlep1_ - 1;

  // Go through the entire valid array and check for all handles that
  // have been schedule for deletion
  for (size_t i = last_valid_index; i > 0; i--)
    if (this->to_be_deleted_set_[i] == 1)
      {
	if (i == last_valid_index)
	  // If this is the last handle in the set, no need to swap
	  // places. Simply remove it.
	  {
	    this->current_handles_[i] = ACE_INVALID_HANDLE;
	    this->current_event_handlers_[i] = 0;
	  }
	else
	  // Swap this handle with the last valid handle
	  {	
	    this->current_handles_[i] = this->current_handles_[last_valid_index];
	    this->current_event_handlers_[i] = this->current_event_handlers_[last_valid_index];
	  }
	// Reset the last valid index and clean up the entry in the
	// <to_be_deleted_set_>
	last_valid_index--;
	this->to_be_deleted_set_[i] = 0;	
      }
  // Since all to be deleted handles have been taken care of, reset
  // the flag
  this->handles_to_be_deleted_ = 0;
  // Reset <this->max_handlep1_>
  this->max_handlep1_ = last_valid_index + 1;

  return 0;
}

int
ACE_ReactorEx_Handler_Repository::handle_additions (void)
{
  // Go through the <to_be_added_*> arrays
  for (int i = 0; i < this->handles_to_be_added_; i++)
    {
      // Add to the end of the current handles set
      this->current_handles_[this->max_handlep1_] = this->to_be_added_handles_[i];
      this->current_event_handlers_[this->max_handlep1_] = this->to_be_added_event_handlers_[i];
      this->max_handlep1_++;

      // Reset the <to_be_added_*> arrays entries
      this->to_be_added_handles_[i] = ACE_INVALID_HANDLE;
      this->to_be_added_event_handlers_[i] = 0;      
    }
  // Since all to be added handles have been taken care of, reset the
  // counter
  this->handles_to_be_added_ = 0;

  return 0;
}


ACE_ReactorEx::ACE_ReactorEx (ACE_Sig_Handler *sh,
			      ACE_Timer_Queue *tq)
  : timer_queue_ (tq),
    delete_timer_queue_ (0),
    handler_rep_ (*this),
    // this event is initially signaled
    ok_to_wait_ (1), 
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    change_state_thread_ (0)
{
  if (this->open (ACE_ReactorEx::DEFAULT_SIZE, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ReactorEx"));
}

ACE_ReactorEx::ACE_ReactorEx (size_t size,
			      int unused,
			      ACE_Sig_Handler *sh,
			      ACE_Timer_Queue *tq)
  : timer_queue_ (tq),
    delete_timer_queue_ (0),
    handler_rep_ (*this),
    // this event is initially signaled
    ok_to_wait_ (1), 
    // this event is initially unsignaled
    wakeup_all_threads_ (0),
    // this event is initially unsignaled
    waiting_to_change_state_ (0),
    active_threads_ (0),
    owner_ (ACE_Thread::self ()),
    change_state_thread_ (0)
{
  if (this->open (size, 0, sh, tq) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ReactorEx"));
}

int
ACE_ReactorEx::open (size_t size,
		     int unused,
		     ACE_Sig_Handler *sh,
		     ACE_Timer_Queue *tq)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);

  // Setup the atomic wait array (used later in <handle_events>)
  this->atomic_wait_array_[0] = this->lock_.lock ().proc_mutex_;
  this->atomic_wait_array_[1] = this->ok_to_wait_.handle ();
  
  // Open the handle repository
  // Two additional handles for internal purposes
  if (this->handler_rep_.open (size + 2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "opening handler repository"), 
		      -1);

  // Open the notification handler
  if (this->notify_handler_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "opening notify handler "), 
		      -1);

  // Register for <wakeup_all_threads> event
  if (this->register_handler (&this->wakeup_all_threads_handler_, 
			      this->wakeup_all_threads_.handle ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		       "registering thread wakeup handler"), 
		      -1);

  // Since we have added two handles into the handler repository,
  // update the <handler_repository_>
  if (this->handler_rep_.changes_required ())    
    {
      // Make necessary changes to the handler repository
      this->handler_rep_.make_changes ();
      // Turn off <wakeup_all_threads_> since all necessary changes
      // have completed
      this->wakeup_all_threads_.reset ();
    }

  // Timer Queue 
  if (this->timer_queue_ == 0)
    {
      ACE_NEW_RETURN (this->timer_queue_, ACE_Timer_List, -1);
      this->delete_timer_queue_ = 1;
    }

  return 0;
}

int
ACE_ReactorEx::close (void)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, this->lock_, -1);

  if (this->delete_timer_queue_ == 1)
    {
      delete this->timer_queue_;
      this->timer_queue_ = 0;
      this->delete_timer_queue_ = 0;      
    }

  return this->handler_rep_.close ();
}

ACE_ReactorEx::~ACE_ReactorEx (void)
{
  this->close ();
}

void
ACE_ReactorEx::wakeup_all_threads (void)
{
  this->wakeup_all_threads_.signal ();
}

int 
ACE_ReactorEx::notify (ACE_Event_Handler *eh,
		       ACE_Reactor_Mask mask,
		       ACE_Time_Value *timeout)
{
  return this->notify_handler_.notify (eh, mask, timeout);
}

int 
ACE_ReactorEx::register_handler (ACE_Event_Handler *eh,
				 ACE_HANDLE handle)
{
  return this->handler_rep_.bind (handle, eh);
}

int 
ACE_ReactorEx::remove_handler (ACE_Event_Handler *eh,
			       ACE_Reactor_Mask mask)
{
  return this->handler_rep_.unbind (eh->get_handle (), mask);
}

int
ACE_ReactorEx::schedule_timer (ACE_Event_Handler *handler,
			       const void *arg,
			       const ACE_Time_Value &delta_time, 
			       const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_ReactorEx::schedule_timer");

  int result = this->timer_queue_->schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
  
  // Wakeup the owner thread so that it gets the latest timer values
  this->notify ();
  
  return result;
}

// Waits for and dispatches all events.  Returns -1 on error, 0 if
// max_wait_time expired, or the number of events that were dispatched.
int 
ACE_ReactorEx::handle_events (ACE_Time_Value *max_wait_time,
			      int alertable)
{
  ACE_TRACE ("ACE_ReactorEx::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

  // Check to see if it is ok to enter ::WaitForMultipleObjects
  // This will acquire <this->lock_> on success
  int result = this->ok_to_wait (max_wait_time, alertable);  
  if (result != 1)
    return result;

  // Increment the number of active threads
  this->active_threads_++;

  // Release the <lock_>
  this->lock_.release ();

  // Update the countdown to reflect time waiting to play with the
  // mutex and event.
  countdown.update ();

  int wait_status = this->wait_for_multiple_events (max_wait_time,
						    alertable);

  result = this->dispatch (wait_status);  
  
  this->update_state ();

  return result;
}

int
ACE_ReactorEx::ok_to_wait (ACE_Time_Value *max_wait_time,
			   int alertable)
{
  // Calculate the max time we should spend here 
  //
  // Note: There is really no need to involve the <timer_queue_> here
  // because even if a timeout in the <timer_queue_> does expire we
  // will not be able to dispatch it
  int timeout = max_wait_time == 0 ? INFINITE : max_wait_time->msec ();

  // Atomically wait for both the <lock_> and <ok_to_wait_> event
  int result = ::WaitForMultipleObjectsEx (sizeof this->atomic_wait_array_ / sizeof (ACE_HANDLE),
					   this->atomic_wait_array_,
					   TRUE,
					   timeout,
					   alertable);
  switch (result)
    {
    case WAIT_TIMEOUT: 
      errno = ETIME;
      return 0;
    case WAIT_FAILED: 
    case WAIT_ABANDONED_0:
      errno = ::GetLastError ();
      return -1;      
    default:
      break;
    }  
  
  // It is ok to enter ::WaitForMultipleObjects
  return 1;
}

int 
ACE_ReactorEx::update_state (void)
{
  // This GUARD is necessary since we are updating shared state.
  ACE_GUARD_RETURN (ACE_Process_Mutex, monitor, this->lock_, -1);

  // Decrement active threads
  this->active_threads_--;

  // Check if the state of the handler repository has changed
  if (this->handler_rep_.changes_required ())    
    {
      if (this->change_state_thread_ == 0)
	// Try to become the thread which will be responsible for the
	// changes
	{
	  this->change_state_thread_ = ACE_Thread::self ();
	  // Make sure no new threads are allowed to enter 
	  this->ok_to_wait_.reset ();

	  if (this->active_threads_ > 0)
	    // Check for other active threads
	    {
	      // Wake up all other threads
	      this->wakeup_all_threads_.signal ();
	      // Release <lock_>
	      monitor.release ();
	      // Go to sleep waiting for all other threads to get done
	      this->waiting_to_change_state_.wait ();
	      // Re-acquire <lock_> again
	      monitor.acquire ();
	    }
	  
	  // Make necessary changes to the handler repository
	  this->handler_rep_.make_changes ();
	  // Turn off <wakeup_all_threads_>
	  this->wakeup_all_threads_.reset ();
	  // Let everyone know that it is ok to go ahead
	  this->ok_to_wait_.signal ();	  
	  // Reset this flag
	  this->change_state_thread_ = 0;
	}
      else if (this->active_threads_ == 0)
	// This thread did not get a chance to become the change
	// thread. If it is the last one out, it will wakeup the
	// change thread
	this->waiting_to_change_state_.signal ();
    }

  return 0;
}

int
ACE_ReactorEx::wait_for_multiple_events (ACE_Time_Value *max_wait_time,
					 int alertable)
{
  int timeout = this->calculate_timeout (max_wait_time);

  // Wait for any of handles_ to be active, or until timeout expires.
  // If <alertable> is enabled allow asynchronous completion of
  // ReadFileEx and WriteFileEx operations.
  return ::WaitForMultipleObjectsEx (this->handler_rep_.max_handlep1 (),
				     this->handler_rep_.handles (),
				     FALSE,
				     timeout,
				     alertable);
}

int
ACE_ReactorEx::calculate_timeout (ACE_Time_Value *max_wait_time)
{
  ACE_Time_Value *time = 0;
  if (this->owner_ == ACE_Thread::self ())
    time = this->timer_queue_->calculate_timeout (max_wait_time);	  
  else
    time = max_wait_time;
  
  if (time == 0)
    return INFINITE;
  else
    return time->msec ();
}


int
ACE_ReactorEx::dispatch (int wait_status)
{
  // If "owner" thread
  if (ACE_Thread::self () == this->owner_)
    // Expire all pending timers.
    this->timer_queue_->expire ();

  switch (wait_status)
    {
    case WAIT_FAILED: // Failure.
      errno = ::GetLastError ();
      return -1;
    case WAIT_TIMEOUT: // Timeout.
      errno = ETIME;
      return 0;
    case WAIT_ABANDONED_0:
      // We'll let dispatch worry about abandoned mutexes.
    default:  // Dispatch.
      return this->dispatch_handles (wait_status - WAIT_OBJECT_0);
    }
}

// Dispatches any active handles from <handles_[index]> to
// <handles_[max_handlep1_]> using <WaitForMultipleObjects> to poll
// through our handle set looking for active handles.

int
ACE_ReactorEx::dispatch_handles (size_t index)
{
  for (int number_of_handlers_dispatched = 1;
       ; 
       number_of_handlers_dispatched++)
    {
      this->dispatch_handler (index++);

      // We're done.
      if (index >= this->handler_rep_.max_handlep1 ())
	return number_of_handlers_dispatched;
      
      DWORD wait_status = 
	::WaitForMultipleObjects (this->handler_rep_.max_handlep1 () - index,
				  this->handler_rep_.handles () + index,
				  FALSE, 0); // We're polling.
      switch (wait_status)
	{
	case WAIT_FAILED: // Failure.
	  errno = ::GetLastError ();
	  /* FALLTHRU */
	case WAIT_TIMEOUT:
	  // There are no more handles ready, we can return.
	  return number_of_handlers_dispatched;
	default: // Dispatch.
	  // Check if a handle successfully became signaled.
	  if (wait_status >= WAIT_OBJECT_0 && 
	      wait_status < WAIT_OBJECT_0 + this->handler_rep_.max_handlep1 ())
	    index += wait_status - WAIT_OBJECT_0;
	  else
	    // Otherwise, a handle was abandoned.
	    index += wait_status - WAIT_ABANDONED_0;
	}
    }
}

// Dispatches a single handler.  Returns 0 on success, -1 if the
// handler was removed.

int
ACE_ReactorEx::dispatch_handler (int index)
{
  // Assign the ``signaled'' HANDLE so that callers can get
  // it.
  ACE_HANDLE handle = *(this->handler_rep_.handles () + index);
  siginfo_t sig (handle);

  // Dispatch the handler.
  if (this->handler_rep_.event_handlers ()[index]->handle_signal (0, &sig) == -1)
    this->handler_rep_.unbind (handle, ACE_Event_Handler::NULL_MASK);
  return 0;
}

// ************************************************************

ACE_ReactorEx_Notify::ACE_ReactorEx_Notify (void) 
{
}

int 
ACE_ReactorEx_Notify::open (ACE_ReactorEx &reactorEx)
{
  return reactorEx.register_handler (this);
}

ACE_HANDLE
ACE_ReactorEx_Notify::get_handle (void) const
{
  return this->wakeup_one_thread_.handle ();
}

// Handle all pending notifications.

int
ACE_ReactorEx_Notify::handle_signal (int signum, 
				     siginfo_t *siginfo, 
				     ucontext_t *)
{
  ACE_UNUSED_ARG (signum);

  // Just check for sanity...
  if (siginfo->si_handle_ != this->wakeup_one_thread_.handle ())
    return -1;

  for (;;)
    {
      ACE_Message_Block *mb = 0;
  
      if (this->message_queue_.dequeue_head 
	  (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
	{
	  if (errno == EWOULDBLOCK)
	    // We've reached the end of the processing, return
	    // normally.
	    return 0;
	  else
	    return -1; // Something weird happened...
	}
      else
	{
	  ACE_Notification_Buffer *buffer = 
	    (ACE_Notification_Buffer *) mb->base ();

	  // If eh == 0 then we've got major problems!  Otherwise, we
	  // need to dispatch the appropriate handle_* method on the
	  // ACE_Event_Handler pointer we've been passed.

	  if (buffer->eh_ != 0)
	    {
	      int result = 0;

	      switch (buffer->mask_)
		{
		case ACE_Event_Handler::READ_MASK:
		  result = buffer->eh_->handle_input (ACE_INVALID_HANDLE);
		  break;
		case ACE_Event_Handler::WRITE_MASK:
		  result = buffer->eh_->handle_output (ACE_INVALID_HANDLE);
		  break;
		case ACE_Event_Handler::EXCEPT_MASK:
		  result = buffer->eh_->handle_exception (ACE_INVALID_HANDLE);
		  break;
		default:
		  ACE_ERROR ((LM_ERROR, "invalid mask = %d\n", buffer->mask_));
		  break;
		}
	      if (result == -1)
		buffer->eh_->handle_close (ACE_INVALID_HANDLE, 
					   ACE_Event_Handler::EXCEPT_MASK);
	    }
	  // Make sure to delete the memory regardless of success or
	  // failure!
	  mb->release ();
	}
    }
}

// Notify the ReactorEx, potentially enqueueing the
// <ACE_Event_Handler> for subsequent processing in the ReactorEx
// thread of control.

int 
ACE_ReactorEx_Notify::notify (ACE_Event_Handler *eh, 
			      ACE_Reactor_Mask mask,
			      ACE_Time_Value *timeout)
{
  if (eh != 0)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof ACE_Notification_Buffer), -1);

      ACE_Notification_Buffer *buffer = 
	(ACE_Notification_Buffer *) mb->base ();
      buffer->eh_ = eh;
      buffer->mask_ = mask;

      // Convert from relative time to absolute time by adding the
      // current time of day.  This is what <ACE_Message_Queue>
      // expects.
      if (timeout != 0)
	*timeout += ACE_OS::gettimeofday ();

      if (this->message_queue_.enqueue_tail 
	  (mb, timeout) == -1)
	{
	  mb->release ();
	  return -1;
	}
    }

  return this->wakeup_one_thread_.signal ();
}

#endif /* ACE_WIN32 */
