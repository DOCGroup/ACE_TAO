// $Id$

#define ACE_BUILD_DLL
#include "ace/Proactor.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) \
    || (defined (ACE_HAS_AIO_CALLS))
// This only works on Win32 platforms and on Unix platforms with aio
// calls.

#include "ace/Task_T.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Proactor.i"
#endif /* __ACE_INLINE__ */

// Process-wide ACE_Proactor.
ACE_Proactor *ACE_Proactor::proactor_ = 0;

// Controls whether the Proactor is deleted when we shut down (we can
// only delete it safely if we created it!)
int ACE_Proactor::delete_proactor_ = 0;

// Terminate the eventloop.
sig_atomic_t ACE_Proactor::end_event_loop_ = 0;

class ACE_Export ACE_Proactor_Timer_Handler : public ACE_Task <ACE_NULL_SYNCH>
  // = TITLE
  //     A Handler for timer. It helps in the management of timers
  //     registered with the Proactor.
  //
  // = DESCRIPTION
  //     This object has a thread that will wait on the earliest time
  //     in a list of timers and an event. When a timer expires, the
  //     thread will post a completion event on the port and go back
  //     to waiting on the timer queue and event. If the event is
  //     signaled, the thread will refresh the time it is currently
  //     waiting on (in case the earliest time has changed)
{
  friend class ACE_Proactor;
  // Proactor has special privileges
  // Access needed to: timer_event_

public:
  ACE_Proactor_Timer_Handler (ACE_Proactor &proactor);
  // Constructor

  ~ACE_Proactor_Timer_Handler (void);
  // Destructor

protected:
  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing. In other
  // words, this method will do the waiting on the earliest timer and
  // event.

  ACE_Auto_Event timer_event_;
  // Event to wait on.

  ACE_Proactor &proactor_;
  // Proactor.

  int shutting_down_;
  // Flag used to indicate when we are shutting down.
};

ACE_Proactor_Timer_Handler::ACE_Proactor_Timer_Handler (ACE_Proactor &proactor)
  : ACE_Task <ACE_NULL_SYNCH> (&proactor.thr_mgr_),
    proactor_ (proactor),
    shutting_down_ (0)
{
}

ACE_Proactor_Timer_Handler::~ACE_Proactor_Timer_Handler (void)
{
  // Mark for closing down
  this->shutting_down_ = 1;

  // Signal timer event
  this->timer_event_.signal ();
}

int
ACE_Proactor_Timer_Handler::svc (void)
{
#if defined (ACE_HAS_AIO_CALLS)
  // @@ Alex, can you please document why this is a "no-op" for the
  // AIO calls?
  return 0;
#else /* ACE_HAS_AIO_CALLS */
  u_long time;
  ACE_Time_Value absolute_time;

  while (this->shutting_down_ == 0)
    {
      // default value
      time = ACE_INFINITE;

      // If the timer queue is not empty
      if (!this->proactor_.timer_queue ()->is_empty ())
	{
	  // Get the earliest absolute time.
	  absolute_time
	    = this->proactor_.timer_queue ()->earliest_time ()
	    - this->proactor_.timer_queue ()->gettimeofday ();

	  // Time to wait.
	  time = absolute_time.msec ();

	  // Make sure the time is positive.
	  if (time < 0)
	    time = 0;
	}

      // Wait for event upto <time> milli seconds
      int result = ::WaitForSingleObject (this->timer_event_.handle (),
					  time);
      switch (result)
	{
	case ACE_WAIT_TIMEOUT:
	  // timeout: expire timers
	  this->proactor_.timer_queue ()->expire ();
	  break;
	case ACE_WAIT_FAILED:
	  // error
	  ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("%p\n"),
                             ASYS_TEXT ("WaitForSingleObject")), -1);
	}
    }

  return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

ACE_Proactor_Handle_Timeout_Upcall::ACE_Proactor_Handle_Timeout_Upcall (void)
  : proactor_ (0)
{
}

int
ACE_Proactor_Handle_Timeout_Upcall::timeout (TIMER_QUEUE &timer_queue,
					     ACE_Handler *handler,
					     const void *act,
					     const ACE_Time_Value &time)
{
  ACE_UNUSED_ARG (timer_queue);

  if (this->proactor_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "(%t) No Proactor set in ACE_Proactor_Handle_Timeout_Upcall, no completion port to post timeout to?!@\n"),
		      -1);

  // Create the Asynch_Timer.
  ACE_Proactor::Asynch_Timer *asynch_timer
    = new ACE_Proactor::Asynch_Timer (*handler,
				      act,
				      time);
  // Post a completion.
  if (this->proactor_->post_completion (asynch_timer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failure in dealing with timers: PostQueuedCompletionStatus failed\n"), -1);

  return 0;
}

int
ACE_Proactor_Handle_Timeout_Upcall::cancellation (TIMER_QUEUE &timer_queue,
						  ACE_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);
  ACE_UNUSED_ARG (handler);

  // Do nothing
  return 0;
}

int
ACE_Proactor_Handle_Timeout_Upcall::deletion (TIMER_QUEUE &timer_queue,
                                              ACE_Handler *handler,
                                              const void *arg)
{
  ACE_UNUSED_ARG (timer_queue);
  ACE_UNUSED_ARG (handler);
  ACE_UNUSED_ARG (arg);

  // Do nothing
  return 0;
}

int
ACE_Proactor_Handle_Timeout_Upcall::proactor (ACE_Proactor &proactor)
{
  if (this->proactor_ == 0)
    {
      this->proactor_ = &proactor;
      return 0;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
		       "ACE_Proactor_Handle_Timeout_Upcall is only suppose to be used with ONE (and only one) Proactor\n"),
		      -1);
}

ACE_Proactor::ACE_Proactor (size_t number_of_threads,
			    Timer_Queue *tq,
			    int used_with_reactor_event_loop)
  :
#if defined (ACE_HAS_AIO_CALLS)
#if defined (AIO_LISTIO_MAX)
  aiocb_list_max_size_ (AIO_LISTIO_MAX),
#else /* AIO_LISTIO_MAX */
  aiocb_list_max_size_ (2),
#endif /* AIO_LISTIO_MAX */
  aiocb_list_cur_size_ (0),
#else /* ACE_HAS_AIO_CALLS */
  // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE!!!!
   completion_port_ (0),
#endif /* ACE_HAS_AIO_CALLS */
   number_of_threads_ (number_of_threads),
   timer_queue_ (0),
   delete_timer_queue_ (0),
   timer_handler_ (0),
   used_with_reactor_event_loop_ (used_with_reactor_event_loop)
{
#if defined (ACE_HAS_AIO_CALLS)
  // Init the array.
  for (size_t ai = 0;
       ai < this->aiocb_list_max_size_;
       ai++)
    {
      aiocb_list_[ai] = 0;
      result_list_[ai] = 0;
    }
  ACE_UNUSED_ARG (tq);
#else /* ACE_HAS_AIO_CALLS */
  // create the completion port
  this->completion_port_ = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE,
						     this->completion_port_,
						     0,
						     this->number_of_threads_);
  if (this->completion_port_ == 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("CreateIoCompletionPort")));

  // set the timer queue
  this->timer_queue (tq);

  // Create the timer handler
  ACE_NEW (this->timer_handler_,
           ACE_Proactor_Timer_Handler (*this));

  // activate <timer_handler>
  if (this->timer_handler_->activate (THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p Could not create thread\n"),
                ASYS_TEXT ("Task::activate")));
#endif /* ACE_HAS_AIO_CALLS */
}

ACE_Proactor *
ACE_Proactor::instance (size_t threads)
{
  ACE_TRACE ("ACE_Proactor::instance");

  if (ACE_Proactor::proactor_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
				*ACE_Static_Object_Lock::instance (),
                                0));

      if (ACE_Proactor::proactor_ == 0)
	{
	  ACE_NEW_RETURN (ACE_Proactor::proactor_,
                          ACE_Proactor (threads),
                          0);
	  ACE_Proactor::delete_proactor_ = 1;
	}
    }
  return ACE_Proactor::proactor_;
}

ACE_Proactor *
ACE_Proactor::instance (ACE_Proactor *r)
{
  ACE_TRACE ("ACE_Proactor::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
			    *ACE_Static_Object_Lock::instance (), 0));

  ACE_Proactor *t = ACE_Proactor::proactor_;

  // We can't safely delete it since we don't know who created it!
  ACE_Proactor::delete_proactor_ = 0;

  ACE_Proactor::proactor_ = r;
  return t;
}

void
ACE_Proactor::close_singleton (void)
{
  ACE_TRACE ("ACE_Proactor::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
		     *ACE_Static_Object_Lock::instance ()));

  if (ACE_Proactor::delete_proactor_)
    {
      delete ACE_Proactor::proactor_;
      ACE_Proactor::proactor_ = 0;
      ACE_Proactor::delete_proactor_ = 0;
    }
}

int
ACE_Proactor::run_event_loop (void)
{
  ACE_TRACE ("ACE_Proactor::run_event_loop");

  while (ACE_Proactor::end_event_loop_ == 0)
    {
      int result = ACE_Proactor::instance ()->handle_events ();

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();

      else if (result == -1)
	return -1;
    }

  /* NOTREACHED */
  return 0;
}

// Handle events for -tv- time.  handle_events updates -tv- to reflect
// time elapsed, so do not return until -tv- == 0, or an error occurs.
int
ACE_Proactor::run_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Proactor::run_event_loop");

  while (ACE_Proactor::end_event_loop_ == 0
         && tv != ACE_Time_Value::zero)
    {
      int result = ACE_Proactor::instance ()->handle_events (tv);

      if (ACE_Service_Config::reconfig_occurred ())
	ACE_Service_Config::reconfigure ();

      // An error has occurred.
      else if (result == -1)
	return result;
    }

  /* NOTREACHED */
  return 0;
}

int
ACE_Proactor::end_event_loop (void)
{
  ACE_TRACE ("ACE_Proactor::end_event_loop");
  ACE_Proactor::end_event_loop_ = 1;
  //  ACE_Proactor::instance()->notify ();
  return 0;
}

/* static */
int
ACE_Proactor::event_loop_done (void)
{
  ACE_TRACE ("ACE_Proactor::event_loop_done");
  return ACE_Proactor::end_event_loop_ != 0;
}

ACE_Proactor::~ACE_Proactor (void)
{
  this->close ();
}

int
ACE_Proactor::close (void)
{
#if defined (ACE_HAS_AIO_CALLS)
  // @@ Alex, shouldn't we be handling the cleanup of the timer queue
  // stuff for the POSIX version of the Proactor, as well?!
  return 0;
#else /* ACE_HAS_AIO_CALLS */
  // Take care of the timer handler
  if (this->timer_handler_)
    {
      delete this->timer_handler_;
      this->timer_handler_ = 0;
    }

  // Take care of the timer queue
  if (this->delete_timer_queue_)
    {
      delete this->timer_queue_;
      this->timer_queue_ = 0;
      this->delete_timer_queue_ = 0;
    }

  // Close the completion port
  if (this->completion_port_ != 0)
    {
      int result = ACE_OS::close (this->completion_port_);
      this->completion_port_ = 0;
      return result;
    }
  else
    return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

int
ACE_Proactor::register_handle (ACE_HANDLE handle,
			       const void *completion_key)
{
#if defined (ACE_HAS_AIO_CALLS)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (completion_key);
  return 0;
#else /* ACE_HAS_AIO_CALLS */
  // No locking is needed here as no state changes.
  ACE_HANDLE cp = ::CreateIoCompletionPort (handle,
					    this->completion_port_,
					    (u_long) completion_key,
					    this->number_of_threads_);
  if (cp == 0)
    {
      errno = ::GetLastError ();
      // If errno == ERROR_INVALID_PARAMETER, then this handle was
      // already registered.
      if (errno != ERROR_INVALID_PARAMETER)
	ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("CreateIoCompletionPort")), -1);
    }
  return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

long
ACE_Proactor::schedule_timer (ACE_Handler &handler,
			      const void *act,
			      const ACE_Time_Value &time)
{
  return this->schedule_timer (handler,
                               act,
                               time,
                               ACE_Time_Value::zero);
}

long
ACE_Proactor::schedule_repeating_timer (ACE_Handler &handler,
					const void *act,
					const ACE_Time_Value &interval)
{
  return this->schedule_timer (handler,
                               act,
                               interval,
                               interval);
}

long
ACE_Proactor::schedule_timer (ACE_Handler &handler,
			      const void *act,
			      const ACE_Time_Value &time,
			      const ACE_Time_Value &interval)
{
  // absolute time.
  ACE_Time_Value absolute_time =
    this->timer_queue_->gettimeofday () + time;

  // Only one guy goes in here at a time
  ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, this->timer_queue_->mutex (), -1);

  // Schedule the timer
  long result = this->timer_queue_->schedule (&handler,
					      act,
					      absolute_time,
					      interval);
  if (result != -1)
    {
      // no failures: check to see if we are the earliest time
      if (this->timer_queue_->earliest_time () == absolute_time)

	// wake up the timer thread
	if (this->timer_handler_->timer_event_.signal () == -1)
	  {
	    // Cancel timer
	    this->timer_queue_->cancel (result);
	    result = -1;
	  }
    }
  return result;
}

int
ACE_Proactor::cancel_timer (long timer_id,
			    const void **arg,
			    int dont_call_handle_close)
{
  // No need to singal timer event here. Even if the cancel timer was
  // the earliest, we will have an extra wakeup.
  return this->timer_queue_->cancel (timer_id,
                                     arg,
                                     dont_call_handle_close);
}

int
ACE_Proactor::cancel_timer (ACE_Handler &handler,
			    int dont_call_handle_close)
{
  // No need to signal timer event here. Even if the cancel timer was
  // the earliest, we will have an extra wakeup.
  return this->timer_queue_->cancel (&handler,
                                     dont_call_handle_close);
}

int
ACE_Proactor::handle_signal (int, siginfo_t *, ucontext_t *)
{
  // Perform a non-blocking "poll" for all the I/O events that have
  // completed in the I/O completion queue.

  ACE_Time_Value timeout (0, 0);
  int result;

  while ((result = this->handle_events (timeout)) == 1)
    continue;

  // If our handle_events failed, we'll report a failure to the
  // Reactor.
  return result == -1 ? -1 : 0;
}

int
ACE_Proactor::handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (close_mask);
  ACE_UNUSED_ARG (handle);

  return this->close ();
}

// @@ get_handle () implementation.
ACE_HANDLE
ACE_Proactor::get_handle (void) const
{
#if defined (ACE_HAS_AIO_CALLS)
  return ACE_INVALID_HANDLE;
#else /* ACE_HAS_AIO_CALLS */
  if (this->used_with_reactor_event_loop_)
    return this->event_.handle ();
  else
    return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

int
ACE_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_Proactor::handle_events (unsigned long milli_seconds)
{
#if defined (ACE_HAS_AIO_CALLS)
  // Is there any entries in the list.
  if (this->aiocb_list_cur_size_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "No AIO pending"),
                      0);

  // Wait for asynch operation to complete.
  timespec timeout;
  timeout.tv_sec = milli_seconds;
  timeout.tv_nsec = 0;

  // Alex, I think we want to revise this implementation so that it
  // DOESN'T need to use aio_suspend, which is going to be
  // non-scalable since we need to search the aiocb_list...  Instead,
  // we need to use the sigtimedwait(3R) in conjunction with the POSIX
  // real-time signal mechanism, which should be much more scalable.
  // Let's talk about how to make this work.
  if (aio_suspend (this->aiocb_list_,
                   this->aiocb_list_max_size_,
                   &timeout) < 0)
    // If failure is coz of timeout, then return *0* but set errno
    // appropriately. This is what the WinNT proactor does.
    if (errno ==  EAGAIN)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%p):aio_suspend"),
                        0);
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%p):aio_suspend"),
                        -1);

  // Check which aio has finished.
  size_t ai;

  for (ai = 0; ai < this->aiocb_list_max_size_; ai++)
    // Analyze error and return values.
    if (aio_error (aiocb_list_[ai]) != EINPROGRESS)
      {
        // @@ Alex, should this be == -1 or < 0?
        if (aio_return (aiocb_list_[ai]) < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%p):AIO failed"),
                            -1);
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "An aio has finished\n"));
            // This AIO is done.
            break;
          }
      }

  if (ai == this->aiocb_list_max_size_)
    // Nothing completed.
    return 0;

  // Get the values for the completed aio.
  size_t bytes_transferred = aiocb_list_[ai]->aio_nbytes;

  void *completion_key =
    (void *) aiocb_list_[ai]->aio_sigevent.sigev_value.sival_ptr;

  ACE_Asynch_Result *asynch_result =
    this->result_list_[ai];

  // Invalidate entry in the aiocb list.
  delete this->aiocb_list_[ai];
  this->aiocb_list_[ai] = 0;
  this->aiocb_list_cur_size_--;
  this->result_list_[ai] = 0;

  // Call the application code.
  this->application_specific_code (asynch_result,
                                   bytes_transferred,
                                   ACE_TRUE,
                                   completion_key,
                                   0);

  return 0;
#else /* ACE_HAS_AIO_CALLS */
  ACE_OVERLAPPED *overlapped = 0;
  u_long bytes_transferred = 0;
  u_long completion_key = 0;

  // Get the next asynchronous operation that completes
  BOOL result = ::GetQueuedCompletionStatus (this->completion_port_,
					     &bytes_transferred,
					     &completion_key,
					     &overlapped,
					     milli_seconds);
  if (result == FALSE && overlapped == 0)
    {
      errno = ::GetLastError ();

      if (errno == WAIT_TIMEOUT)
	{
	  errno = ETIME;
	  return 0;
	}
      else
	ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("GetQueuedCompletionStatus")), -1);
    }
  else
    {
      // Narrow the result.
      ACE_Asynch_Result *asynch_result = (ACE_Asynch_Result *) overlapped;

      // If errors happen, grab the error.
      if (result == FALSE)
	errno = ::GetLastError ();

      this->application_specific_code (asynch_result,
				       bytes_transferred,
				       result,
				       (void *) completion_key,
				       errno);
    }
  return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

void
ACE_Proactor::application_specific_code (ACE_Asynch_Result *asynch_result,
					 u_long bytes_transferred,
					 int success,
					 const void *completion_key,
					 u_long error)
{
  ACE_SEH_TRY
    {
      // Call completion hook
      asynch_result->complete (bytes_transferred,
			       success,
			       (void *) completion_key,
			       error);
    }
  ACE_SEH_FINALLY
    {
      // This is crucial to prevent memory leaks
      delete asynch_result;
    }
}

int
ACE_Proactor::post_completion (ACE_Asynch_Result *result)
{
#if defined (ACE_HAS_AIO_CALLS)
  ACE_UNUSED_ARG (result);
  return 0;
#else /* ACE_HAS_AIO_CALLS */
  // Grab the event associated with the Proactor
  HANDLE handle = this->get_handle ();

  // If Proactor event is valid, signal it
  if (handle != ACE_INVALID_HANDLE &&
      handle != 0)
    ACE_OS::event_signal (&handle);

  // Post a completion
  if (::PostQueuedCompletionStatus (this->completion_port_, // completion port
				    0, // number of bytes tranferred
				    0,	// completion key
				    result // overlapped
				    ) == FALSE)
    {
      delete result;
      ACE_ERROR_RETURN ((LM_ERROR, "Failure in dealing with timers: PostQueuedCompletionStatus failed\n"), -1);
    }

  return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

int
ACE_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_Proactor::number_of_threads (void) const
{
  return this->number_of_threads_;
}

void
ACE_Proactor::number_of_threads (size_t threads)
{
  this->number_of_threads_ = threads;
}

ACE_Proactor::Timer_Queue *
ACE_Proactor::timer_queue (void) const
{
  return this->timer_queue_;
}

void
ACE_Proactor::timer_queue (Timer_Queue *tq)
{
  // cleanup old timer queue
  if (this->delete_timer_queue_)
    {
      delete this->timer_queue_;
      this->delete_timer_queue_ = 0;
    }

  // new timer queue
  if (tq == 0)
    {
      this->timer_queue_ = new Timer_Heap;
      this->delete_timer_queue_ = 1;
    }
  else
    {
      this->timer_queue_ = tq;
      this->delete_timer_queue_ = 0;
    }

  // Set the proactor in the timer queue's functor
  this->timer_queue_->upcall_functor ().proactor (*this);
}

ACE_Proactor::Asynch_Timer::Asynch_Timer (ACE_Handler &handler,
					  const void *act,
					  const ACE_Time_Value &tv,
					  ACE_HANDLE event)
  : ACE_Asynch_Result (handler, act, event),
    time_ (tv)
{
}

void
ACE_Proactor::Asynch_Timer::complete (u_long bytes_transferred,
				      int success,
				      const void *completion_key,
				      u_long error)
{
  ACE_UNUSED_ARG (error);
  ACE_UNUSED_ARG (completion_key);
  ACE_UNUSED_ARG (success);
  ACE_UNUSED_ARG (bytes_transferred);

  this->handler_.handle_time_out (this->time_, this->act ());
}

#if defined (ACE_HAS_AIO_CALLS)
int
ACE_Proactor::insert_to_aiocb_list (aiocb *aiocb_ptr,
                                    ACE_Asynch_Result *result)
{
  // Is there any place?
  if (this->aiocb_list_cur_size_ >= this->aiocb_list_max_size_)
    return -1;

  // Find the first free slot.
  size_t ai;
  for (ai = 0;
       ai < this->aiocb_list_max_size_;
       ai++)
    if (this->aiocb_list_[ai] == 0)
      break;

  if (ai == this->aiocb_list_max_size_)
    return -1;

  // Store the pointers.
  this->aiocb_list_[ai] = aiocb_ptr;
  this->result_list_[ai] = result;
  this->aiocb_list_cur_size_ ++;
  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Timer_Queue_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_Iterator_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_Iterator_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Node_T<ACE_Handler *>;
template class ACE_Unbounded_Set<ACE_Timer_Node_T<ACE_Handler *> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Timer_Node_T<ACE_Handler *> *>;
template class ACE_Node <ACE_Timer_Node_T<ACE_Handler *> *>;
template class ACE_Free_List<ACE_Timer_Node_T<ACE_Handler *> >;
template class ACE_Locked_Free_List<ACE_Timer_Node_T<ACE_Handler *>, ACE_Null_Mutex>;
template class ACE_Timer_Heap_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Heap_Iterator_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_Iterator_T<ACE_Handler *,
  ACE_Proactor_Handle_Timeout_Upcall,
  ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Timer_Queue_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_Queue_Iterator_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_List_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_List_Iterator_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_Heap_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_Heap_Iterator_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_Wheel_T<ACE_Handler *,
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Timer_Wheel_Iterator_T<ACE_Handler *,\
        ACE_Proactor_Handle_Timeout_Upcall, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else /* ACE_WIN32 */

ACE_Proactor *
ACE_Proactor::instance (size_t threads)
{
  ACE_UNUSED_ARG (threads);
  return 0;
}

ACE_Proactor *
ACE_Proactor::instance (ACE_Proactor *)
{
  return 0;
}

void
ACE_Proactor::close_singleton (void)
{
}

int
ACE_Proactor::run_event_loop (void)
{
  // not implemented
  return -1;
}

int
ACE_Proactor::run_event_loop (ACE_Time_Value &tv)
{
  // not implemented
  ACE_UNUSED_ARG (tv);
  return -1;
}

int
ACE_Proactor::end_event_loop (void)
{
  // not implemented
  return -1;
}

sig_atomic_t
ACE_Proactor::event_loop_done (void)
{
  return sig_atomic_t (1);
}
#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS*/
