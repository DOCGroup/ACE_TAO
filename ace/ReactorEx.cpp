// ReactorEx.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/ReactorEx.h"

#if defined (ACE_WIN32)

#if !defined (__ACE_INLINE__)
#include "ace/ReactorEx.i"
#endif /* __ACE_INLINE__ */

ACE_ReactorEx::ACE_ReactorEx (void)
  : active_handles_ (0),
    timer_skew_ (0, ACE_TIMER_SKEW),
    token_ (*this)
{
  if (this->register_handler (&this->notify_handler_) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "registering notify handler"));
}

ACE_ReactorEx::~ACE_ReactorEx (void)
{
}

int 
ACE_ReactorEx::notify (void)
{
  ACE_GUARD_RETURN (ACE_ReactorEx_Token, ace_mon, this->token_, -1);

  return notify_handler_.notify ();
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
  this->handlers_[this->active_handles_] = eh;
  this->active_handles_++;

  // Assign *this* <ReactorEx> to the <Event_Handler>.
  eh->reactorex (this);
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
	{
	  if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::DONT_CALL) == 0)
	    handlers_[index]->handle_close (handle,
					    ACE_Event_Handler::NULL_MASK);

	  // Reinitial the ReactorEx pointer since we no longer point
	  // to this one.
	  handlers_[index]->reactorex (0);
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
	      this->handlers_[index] = this->handlers_[this->active_handles_];
	    }
	}
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

  return this->timer_queue_.schedule 
    (handler, arg, ACE_OS::gettimeofday () + delta_time, interval);
}

int 
ACE_ReactorEx::handle_events (ACE_Time_Value *how_long,
			      int wait_all)
{
  // @@ Need to implement -wait_all-.

  DWORD wait_status;
  int handles_skipped = 0;
  // These relative_things are moved through the handles_ each time
  // handle_events is called.  Set relative index = -1 so we can check
  // in case of timeouts.
  int relative_index = -1;
  ACE_HANDLE *relative_handles = this->handles_;
  ACE_Event_Handler **relative_handlers = this->handlers_;

  // Stash the current time.
  ACE_Time_Value prev_time = ACE_OS::gettimeofday ();
  // Check for pending timeout events.
  how_long = timer_queue_.calculate_timeout (how_long);
  // Translate into Win32 time value.
  int timeout = how_long == 0 ? INFINITE : how_long->msec ();

  int active_handles = this->active_handles_;

  while (active_handles > 0)
    {
      wait_status = ::WaitForMultipleObjects (active_handles,
					      relative_handles,
					      wait_all,
					      timeout);
      if (!this->timer_queue_.is_empty ())
	// Fudge factor accounts for problems with Solaris timers...
	this->timer_queue_.expire (ACE_OS::gettimeofday () + this->timer_skew_);

      // Compute the time while the ReactorEx was processing.
      ACE_Time_Value elapsed_time = ACE_OS::gettimeofday () - prev_time;

      // Update -how_long- to reflect the amount of time since
      // handle_events was called.  This is computed in case we return
      // from the switch.
      if (how_long != 0)
	{
	  if (*how_long > elapsed_time)
	    *how_long = *how_long - elapsed_time;
	  else
	    *how_long = ACE_Time_Value::zero; // Used all of timeout.
	}

      switch (wait_status)
	{
	case WAIT_TIMEOUT:
	  errno = ETIME;
	  // If we timed out on the first call, return 0, otherwise
	  // an event must have occured; return 1.
	  return relative_index == -1 ? 0 : 1;
	case WAIT_FAILED:
	  errno = ::GetLastError ();
	  return -1;
	default:
	  {
	    // @@ Need to implement WAIT_ABANDONED_0 stuff.
	    relative_index = wait_status - WAIT_OBJECT_0;
	    // Assign the ``signaled'' HANDLE so that callers can get
	    // it.
	    siginfo_t sig (relative_handles[relative_index]);
	    
	    if (relative_handlers[relative_index]->handle_signal 
		(0, &sig) == -1)
	      // If we remove a handler, then the index should stay
	      // the same since it may have been replaced with the end
	      // handle by remove_handler.
	      this->remove_handler (relative_handlers[relative_index]);
	    else
	      // If we did not remove the handler, then move the index
	      // up one so that we skip this handler on the next
	      // iteration.
	      relative_index++;

	    // Update the relative pointers.
	    relative_handles = &relative_handles[relative_index];
	    relative_handlers = &relative_handlers[relative_index];

	    // The number of remaining active handles is
	    // active_handles_ less the number of handles we skipped
	    // over.
	    handles_skipped += relative_index;
	    active_handles = this->active_handles_ - handles_skipped;

	    // Make the timeout be zero so that we don't block on any
	    // subsequent calls to WaitForMultipleObjects.  Rather, we
	    // just poll through the rest looking for signaled handles.
	    timeout = 0; 
	  }
	}
    }

  // Compute the time while the ReactorEx was processing.  Note that
  // this is recomputed to reflect time spent dispatching handlers.
  ACE_Time_Value elapsed_time = ACE_OS::gettimeofday () - prev_time;

  // Update how_long to reflect the amount of time since handle_events
  // was called.
  if (how_long != 0)
    {
      if (*how_long > elapsed_time)
	*how_long = *how_long - elapsed_time;
      else
	*how_long = ACE_Time_Value::zero; // Used all of timeout.
    }

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

ACE_ReactorEx_Notify::ACE_ReactorEx_Notify (void)
{
  // Create an "auto-reset" event that is used to unblock the
  // ReactorEx.
  handle_ = ::CreateEvent (NULL, FALSE, FALSE, NULL);
}

ACE_ReactorEx_Notify::~ACE_ReactorEx_Notify (void)
{
  ACE_OS::close (handle_);
}

ACE_HANDLE
ACE_ReactorEx_Notify::get_handle (void) const
{
  return this->handle_;
}

int
ACE_ReactorEx_Notify::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  // Do nothing.
  return 0;
}

int 
ACE_ReactorEx_Notify::notify (void)
{
  return ::SetEvent (handle_) ? 0 : -1;
}

#endif /* ACE_WIN32 */
