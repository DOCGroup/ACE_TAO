// ReactorEx.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/ReactorEx.h"

#if defined (ACE_WIN32)

#if !defined (__ACE_INLINE__)
#include "ace/ReactorEx.i"
#endif /* __ACE_INLINE__ */

ACE_ReactorEx::ACE_ReactorEx (ACE_Timer_Queue *tq)
  : active_handles_ (0),
    token_ (*this)
{
  this->timer_queue_ = tq;

  if (this->timer_queue_ == 0)
    {
      ACE_NEW (this->timer_queue_, ACE_Timer_Queue);
      this->delete_timer_queue_ = 1;
    }

  if (this->register_handler (&this->notify_handler_) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "registering notify handler"));
}

ACE_ReactorEx::~ACE_ReactorEx (void)
{
  if (this->delete_timer_queue_)
    delete this->timer_queue_;
}

int 
ACE_ReactorEx::notify (ACE_Event_Handler *eh,
		       ACE_Reactor_Mask mask)
{
  return notify_handler_.notify (eh, mask);
}

int 
ACE_ReactorEx::register_handler (ACE_Event_Handler *eh,
				 ACE_HANDLE handle)
{
  ACE_GUARD_RETURN (ACE_ReactorEx_Token, ace_mon, this->token_, -1);

  if (this->active_handles_ >= ACE_ReactorEx::MAX_SIZE)
    return -1;

  if (handle == ACE_INVALID_HANDLE)
    handle = eh->get_handle ();
  this->handles_[this->active_handles_] = handle;
  this->event_handlers_[this->active_handles_] = eh;
  this->active_handles_++;

  // Assign *this* <ReactorEx> to the <Event_Handler>.
  eh->reactorEx (this);
  return 0;
}

// Removes <eh> from the <ReactorEx>.  Note that the <ReactorEx> will
// call eh->get_handle() to extract the underlying I/O handle.

int 
ACE_ReactorEx::remove_handler (ACE_Event_Handler *eh,
			       ACE_Reactor_Mask mask)
{
  ACE_GUARD_RETURN (ACE_ReactorEx_Token, ace_mon, this->token_, -1);

  ACE_HANDLE handle = eh->get_handle ();

  // Go through all the handles looking for <handle>.  Even if we find
  // it, we continue through the rest of the list.  <handle> could
  // appear multiple times.
  for (size_t index = 0; index < this->active_handles_; index++)
    {
      if (this->handles_[index] == handle)
	this->remove_handler (index, mask);
    }
  
  return 0;
}

int 
ACE_ReactorEx::remove_handler (int index,
			       ACE_Reactor_Mask mask)
{
  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
    event_handlers_[index]->handle_close (event_handlers_[index]->get_handle (),
					  ACE_Event_Handler::NULL_MASK);

  // If there was only one handle, reset the pointer to 0.
  if (this->active_handles_ == 1)
    {
      // This is logically correct, but probably should never
      // happen.  This means that ACE_ReactorEx_Notify is
      // being removed!  We'll do it anyway and print out a
      // warning.
      this->active_handles_ = 0;
      ACE_ERROR ((LM_ERROR, "ReactorEx: ReactorEx_Notify was"
		  "just removed!\n"));
    }
  // Otherwise, take the handle and handler from the back and
  // overwrite the ones being removed.
  else
    {
      this->handles_[index] = this->handles_[--this->active_handles_];
      this->event_handlers_[index] = this->event_handlers_[this->active_handles_];
    }

  return 0;
}

int
ACE_ReactorEx::schedule_timer (ACE_Event_Handler *handler,
			       const void *arg,
			       const ACE_Time_Value &delta_time, 
			       const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_ReactorEx::schedule_timer");

  ACE_GUARD_RETURN (ACE_ReactorEx_Token, ace_mon, this->token_, -1);

  return this->timer_queue_->schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
}

// Waits for and dispatches all events.  Returns -1 on error, 0 if
// max_wait_time expired, and 1 if events were dispatched.
int 
ACE_ReactorEx::handle_events (ACE_Time_Value *max_wait_time,
			      int wait_all,
			      ACE_Event_Handler *wait_all_callback,
			      int alertable)
{
  ACE_TRACE ("ACE_ReactorEx::handle_events");

  // Stash the current time -- the destructor of this object will
  // automatically compute how much time elapsed since this method was
  // called.
  ACE_Countdown_Time countdown (max_wait_time);

#if defined (ACE_MT_SAFE)
  ACE_GUARD_RETURN (ACE_ReactorEx_Token, ace_mon, this->token_, -1);

  // Update the countdown to reflect time waiting for the mutex.
  countdown.update ();
#endif /* ACE_MT_SAFE */

  if (active_handles_ < 1)
    {
      // @@ What should this be?
      errno = ENOENT;
      return -1;
    }

  // Check for pending timeout events.
  ACE_Time_Value *wait_time = timer_queue_->calculate_timeout (max_wait_time);
  // Translate into Win32 time value.
  int timeout = wait_time == 0 ? INFINITE : wait_time->msec ();

  DWORD wait_status;
  // Wait for any of handles_ to be active, or until timeout expires.
  // If wait_all is true, then wait for all handles_ to be active.

  // Allow asynchronous completion of ReadFileEx and WriteFileEx
  // operations.
  wait_status= ::WaitForMultipleObjects (active_handles_,
					   handles_,
					   wait_all,
					 timeout);//,
  //					   alertable);

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
      if (wait_all != 0)
	return this->dispatch (wait_all_callback);
      else
	return this->dispatch (wait_status - WAIT_OBJECT_0);
    }
}

int
ACE_ReactorEx::dispatch (ACE_Event_Handler *wait_all_callback)
{
  if (wait_all_callback != 0)
    {
      siginfo_t handles (this->handles_);
      if (wait_all_callback->handle_signal (0, &handles) == -1)
	{
	  // Loop through all handles removing each one.
	  while (active_handles_ > 0)
	    this->remove_handler (0, 0);
	  return -1;
	}
    }
  else
    {
      int result = 0;

      for (int i = 0; i < this->active_handles_; i++)
	if (this->dispatch_handler (i) == -1)
	  result--;

      // Return the number of bad handler dispatches (negated, of
      // course!).
      return result;
    }

  return 0;
}

// Dispatches any active handles from handles_[-index-] to
// handles_[active_handles_] using WaitForMultipleObjects to poll
// through our handle set looking for active handles.

int
ACE_ReactorEx::dispatch (size_t index)
{
  for (;;)
    {
      // If dispatch_handler returns -1 then a handler was removed and
      // index alread points to the correct place.
      if (this->dispatch_handler (index) == 0)
	index++;

      // We're done.
      if (index >= this->active_handles_)
	return 0;

      DWORD wait_status = 
	::WaitForMultipleObjects (active_handles_ - index,
				  &handles_[index],
				  FALSE, 0); // We're polling.

      switch (wait_status)
	{
	case WAIT_FAILED: // Failure.
	  errno = ::GetLastError ();
	  return -1;
	case WAIT_TIMEOUT:
	  // There are no more handles ready, we can return.
	  return 0;
	default:  // Dispatch.
	  // Check if a handle successfully became signaled.
	  if (wait_status >= WAIT_OBJECT_0 &&
	      wait_status < WAIT_OBJECT_0 + active_handles_)
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
  siginfo_t sig (handles_[index]);

  // Dispatch the handler.
  if (event_handlers_[index]->handle_signal (0, &sig) == -1)
    {
      this->remove_handler (event_handlers_[index]);
      return -1;
    }
  else
    return 0;
}

// ************************************************************

void
ACE_ReactorEx_Token::dump (void) const
{
  ACE_TRACE ("ACE_ReactorEx_Token::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

ACE_ReactorEx_Token::ACE_ReactorEx_Token (ACE_ReactorEx &r)
  : reactorEx_ (r)
#if defined (ACE_ReactorEx_HAS_DEADLOCK_DETECTION)
    , ACE_Local_Mutex (0) // Triggers unique name by stringifying "this"...
#endif /* ACE_ReactorEx_HAS_DEADLOCK_DETECTION */
{
  ACE_TRACE ("ACE_ReactorEx_Token::ACE_ReactorEx_Token");
}

// Used to wakeup the Reactor.

void
ACE_ReactorEx_Token::sleep_hook (void)
{
  ACE_TRACE ("ACE_ReactorEx_Token::sleep_hook");
  if (this->reactorEx_.notify () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "sleep_hook failed"));
}

// ************************************************************

ACE_HANDLE
ACE_ReactorEx_Notify::get_handle (void) const
{
  return this->notify_event_.handle ();
}

// Handle all pending notifications.

int
ACE_ReactorEx_Notify::handle_signal (int signum, 
				     siginfo_t *siginfo, 
				     ucontext_t *)
{
	ACE_UNUSED_ARG(signum);

	// Just check for sanity...
  if (siginfo->si_handle_ != this->notify_event_.handle ())
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

	  // If eh == 0 then we've got major problems!  Otherwise, we need
	  // to dispatch the appropriate handle_* method on the
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
	  delete mb;
	}
    }
}

// Notify the ReactorEx, potentially enqueueing the
// <ACE_Event_Handler> for subsequent processing in the ReactorEx
// thread of control.

int 
ACE_ReactorEx_Notify::notify (ACE_Event_Handler *eh, 
			      ACE_Reactor_Mask mask)
{
  if (eh != 0)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof ACE_Notification_Buffer), -1);

      ACE_Notification_Buffer *buffer = (ACE_Notification_Buffer *) mb->base ();
      buffer->eh_ = eh;
      buffer->mask_ = mask;

      if (this->message_queue_.enqueue_tail (mb) == -1)
	{
	  delete mb;
	  return -1;
	}
    }

  return this->notify_event_.signal ();
}

#endif /* ACE_WIN32 */
