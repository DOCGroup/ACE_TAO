// $Id$

#define ACE_BUILD_DLL

// ACE_RCSID(ace, Proactor, "$Id$")

#include "ace/WIN32_Proactor.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
// WIN implemenatation of the Proactor.

#include "ace/Task_T.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/WIN32_Proactor.i"
#endif /* __ACE_INLINE__ */

class ACE_Export ACE_WIN32_Proactor_Timer_Handler : public ACE_Task <ACE_NULL_SYNCH>
{
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

  friend class ACE_WIN32_Proactor;
  // Proactor has special privileges
  // Access needed to: timer_event_

public:
  ACE_WIN32_Proactor_Timer_Handler (ACE_WIN32_Proactor &win32_proactor);
  // Constructor.

  ~ACE_WIN32_Proactor_Timer_Handler (void);
  // Destructor.

protected:
  virtual int svc (void);
  // Run by a daemon thread to handle deferred processing. In other
  // words, this method will do the waiting on the earliest timer and
  // event.

  ACE_Auto_Event timer_event_;
  // Event to wait on.

  ACE_WIN32_Proactor &win32_proactor_;
  // Proactor.

  int shutting_down_;
  // Flag used to indicate when we are shutting down.
};

ACE_WIN32_Proactor_Timer_Handler::ACE_WIN32_Proactor_Timer_Handler (ACE_WIN32_Proactor &win32_proactor)
  : ACE_Task <ACE_NULL_SYNCH> (&win32_proactor.thr_mgr_),
    win32_proactor_ (win32_proactor),
    shutting_down_ (0)
{
}

ACE_WIN32_Proactor_Timer_Handler::~ACE_WIN32_Proactor_Timer_Handler (void)
{
  // Mark for closing down.
  this->shutting_down_ = 1;

  // Signal timer event.
  this->timer_event_.signal ();
}

int
ACE_WIN32_Proactor_Timer_Handler::svc (void)
{
  u_long time;
  ACE_Time_Value absolute_time;

  while (this->shutting_down_ == 0)
    {
      // default value
      time = ACE_INFINITE;

      // If the timer queue is not empty
      if (!this->win32_proactor_.timer_queue ()->is_empty ())
	{
	  // Get the earliest absolute time.
	  absolute_time =
            this->win32_proactor_.timer_queue ()->earliest_time () -
            this->win32_proactor_.timer_queue ()->gettimeofday ();

	  // Time to wait.
	  time = absolute_time.msec ();

	  // Make sure the time is positive.
	  if (time < 0)
	    time = 0;
	}

      // Wait for event upto <time> milli seconds.
      int result = ::WaitForSingleObject (this->timer_event_.handle (),
					  time);
      switch (result)
	{
	case ACE_WAIT_TIMEOUT:
	  // timeout: expire timers
	  this->win32_proactor_.timer_queue ()->expire ();
	  break;
	case ACE_WAIT_FAILED:
	  // error
	  ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("%p\n"),
                             ASYS_TEXT ("WaitForSingleObject")), -1);
	}
    }

  return 0;
}

// *********************************************************************

ACE_WIN32_Proactor_Handle_Timeout_Upcall::ACE_WIN32_Proactor_Handle_Timeout_Upcall (void)
  : win32_proactor_ (0)
{
}

int
ACE_WIN32_Proactor_Handle_Timeout_Upcall::timeout (TIMER_QUEUE &timer_queue,
                                                   ACE_Handler *handler,
                                                   const void *act,
                                                   const ACE_Time_Value &time)
{
  ACE_UNUSED_ARG (timer_queue);

  if (this->win32_proactor_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%t) No Proactor set in ACE_Proactor_Handle_Timeout_Upcall,")
                       ASYS_TEXT (" no completion port to post timeout to?!@\n")),
                      -1);

  // Create the Asynch_Timer.
  ACE_WIN32_Asynch_Timer *asynch_timer;
  ACE_NEW_RETURN (asynch_timer,
                  ACE_WIN32_Asynch_Timer (*handler,
                                          act,
                                          time),
                  -1);

  // Post a completion.
  if (this->win32_proactor_->post_completion (asynch_timer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("Failure in dealing with timers: ")
                       ASYS_TEXT ("PostQueuedCompletionStatus failed\n")),
                      -1);
  return 0;
}

int
ACE_WIN32_Proactor_Handle_Timeout_Upcall::cancellation (TIMER_QUEUE &timer_queue,
                                                        ACE_Handler *handler)
{
  ACE_UNUSED_ARG (timer_queue);
  ACE_UNUSED_ARG (handler);

  // Do nothing
  return 0;
}

int
ACE_WIN32_Proactor_Handle_Timeout_Upcall::deletion (TIMER_QUEUE &timer_queue,
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
ACE_WIN32_Proactor_Handle_Timeout_Upcall::win32_proactor (ACE_WIN32_Proactor &win32_proactor)
{
  if (this->win32_proactor_ == 0)
    {
      this->win32_proactor_ = &win32_proactor;
      return 0;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("ACE_Proactor_Handle_Timeout_Upcall is only suppose")
                       ASYS_TEXT (" to be used with ONE (and only one) Proactor\n")),
                      -1);
}

// *********************************************************************

ACE_WIN32_Proactor::ACE_WIN32_Proactor (size_t number_of_threads,
                                        TIMER_QUEUE *tq,
                                        int used_with_reactor_event_loop)
  : completion_port_ (0),
    // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE !!!
    number_of_threads_ (number_of_threads),
    timer_queue_ (0),
    delete_timer_queue_ (0),
    timer_handler_ (0),
    used_with_reactor_event_loop_ (used_with_reactor_event_loop)
{
  // Create the completion port.
  this->completion_port_ = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE,
                                                     this->completion_port_,
                                                     0,
                                                     this->number_of_threads_);
  if (this->completion_port_ == 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("CreateIoCompletionPort")));

  // Set the timer queue.
  this->timer_queue (tq);

  // Create the timer handler
  ACE_NEW (this->timer_handler_,
           ACE_WIN32_Proactor_Timer_Handler (*this));

  // activate <timer_handler>
  if (this->timer_handler_->activate (THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p Could not create thread\n"),
                ASYS_TEXT ("Task::activate")));
}

ACE_WIN32_Proactor::~ACE_WIN32_Proactor (void)
{
  this->close ();
}

int
ACE_WIN32_Proactor::close (void)
{
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

  return 0;
}

int
ACE_WIN32_Proactor::register_handle (ACE_HANDLE handle,
                                     const void *completion_key)
{
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
}

long
ACE_WIN32_Proactor::schedule_timer (ACE_Handler &handler,
                                    const void *act,
                                    const ACE_Time_Value &time)
{
  return this->schedule_timer (handler,
                               act,
                               time,
                               ACE_Time_Value::zero);
}

long
ACE_WIN32_Proactor::schedule_repeating_timer (ACE_Handler &handler,
                                              const void *act,
                                              const ACE_Time_Value &interval)
{
  return this->schedule_timer (handler,
                               act,
                               interval,
                               interval);
}

long
ACE_WIN32_Proactor::schedule_timer (ACE_Handler &handler,
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
ACE_WIN32_Proactor::cancel_timer (long timer_id,
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
ACE_WIN32_Proactor::cancel_timer (ACE_Handler &handler,
                                  int dont_call_handle_close)
{
  // No need to signal timer event here. Even if the cancel timer was
  // the earliest, we will have an extra wakeup.
  return this->timer_queue_->cancel (&handler,
                                     dont_call_handle_close);
}

ACE_Asynch_Read_Stream_Impl *
ACE_WIN32_Proactor::create_asynch_read_stream (void)
{
  ACE_Asynch_Read_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Write_Stream_Impl *
ACE_WIN32_Proactor::create_asynch_write_stream (void)
{
  ACE_Asynch_Write_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Read_File_Impl *
ACE_WIN32_Proactor::create_asynch_read_file (void)
{
  ACE_Asynch_Read_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Write_File_Impl *
ACE_WIN32_Proactor::create_asynch_write_file (void)
{
  ACE_Asynch_Write_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Accept_Impl *
ACE_WIN32_Proactor::create_asynch_accept (void)
{
  ACE_Asynch_Accept_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Accept (this),
                  0);
  return implementation;
}

ACE_Asynch_Transmit_File_Impl *
ACE_WIN32_Proactor::create_asynch_transmit_file (void)
{
  ACE_Asynch_Transmit_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Transmit_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_WIN32_Proactor::create_asynch_read_stream_result (ACE_Handler &handler,
                                                      ACE_HANDLE handle,
                                                      ACE_Message_Block &message_block,
                                                      u_long bytes_to_read,
                                                      const void* act,
                                                      ACE_HANDLE event,
                                                      int priority)
{
  ACE_Asynch_Read_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_Stream_Result (handler,
                                                       handle,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       event,
                                                       priority),
                  0);
  return implementation;
}

ACE_Asynch_Write_Stream_Result_Impl *
ACE_WIN32_Proactor::create_asynch_write_stream_result (ACE_Handler &handler,
                                                       ACE_HANDLE handle,
                                                       ACE_Message_Block &message_block,
                                                       u_long bytes_to_write,
                                                       const void* act,
                                                       ACE_HANDLE event,
                                                       int priority)
{
  ACE_Asynch_Write_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_Stream_Result (handler,
                                                        handle,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        event,
                                                        priority),
                  0);
  return implementation;
}

ACE_Asynch_Read_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_read_file_result (ACE_Handler &handler,
                                                    ACE_HANDLE handle,
                                                    ACE_Message_Block &message_block,
                                                    u_long bytes_to_read,
                                                    const void* act,
                                                    u_long offset,
                                                    u_long offset_high,
                                                    ACE_HANDLE event,
                                                    int priority)
{
  ACE_Asynch_Read_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Read_File_Result (handler,
                                                     handle,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     event,
                                                     priority),
                  0);
  return implementation;
}

ACE_Asynch_Write_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_write_file_result (ACE_Handler &handler,
                                                     ACE_HANDLE handle,
                                                     ACE_Message_Block &message_block,
                                                     u_long bytes_to_write,
                                                     const void* act,
                                                     u_long offset,
                                                     u_long offset_high,
                                                     ACE_HANDLE event,
                                                     int priority)
{
  ACE_Asynch_Write_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Write_File_Result (handler,
                                                      handle,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      event,
                                                      priority),
                  0);
  return implementation;
}

ACE_Asynch_Accept_Result_Impl *
ACE_WIN32_Proactor::create_asynch_accept_result (ACE_Handler &handler,
                                                 ACE_HANDLE listen_handle,
                                                 ACE_HANDLE accept_handle,
                                                 ACE_Message_Block &message_block,
                                                 u_long bytes_to_read,
                                                 const void* act,
                                                 ACE_HANDLE event,
                                                 int priority)
{
  ACE_Asynch_Accept_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Accept_Result (handler,
                                                  listen_handle,
                                                  accept_handle,
                                                  message_block,
                                                  bytes_to_read,
                                                  act,
                                                  event,
                                                  priority),
                  0);
  return implementation;
}

ACE_Asynch_Transmit_File_Result_Impl *
ACE_WIN32_Proactor::create_asynch_transmit_file_result (ACE_Handler &handler,
                                                        ACE_HANDLE socket,
                                                        ACE_HANDLE file,
                                                        ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                        u_long bytes_to_write,
                                                        u_long offset,
                                                        u_long offset_high,
                                                        u_long bytes_per_send,
                                                        u_long flags,
                                                        const void *act,
                                                        ACE_HANDLE event,
                                                        int priority)
{
  ACE_Asynch_Transmit_File_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_WIN32_Asynch_Transmit_File_Result (handler,
                                                         socket,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         event,
                                                         priority),
                  0);
  return implementation;
}

int
ACE_WIN32_Proactor::handle_signal (int, siginfo_t *, ucontext_t *)
{
  // Perform a non-blocking "poll" for all the I/O events that have
  // completed in the I/O completion queue.

  ACE_Time_Value timeout (0, 0);
  int result = 0;

  while (1)
    {
      result = this->handle_events (timeout);
      if (result != 0 || errno == ETIME)
        break;
    }

  // If our handle_events failed, we'll report a failure to the
  // Reactor.
  return result == -1 ? -1 : 0;
}

int
ACE_WIN32_Proactor::handle_close (ACE_HANDLE handle,
                                  ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (close_mask);
  ACE_UNUSED_ARG (handle);

  return this->close ();
}

ACE_HANDLE
ACE_WIN32_Proactor::get_handle (void) const
{
  if (this->used_with_reactor_event_loop_)
    return this->event_.handle ();
  else
    return 0;
}

int
ACE_WIN32_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_WIN32_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_WIN32_Proactor::handle_events (unsigned long milli_seconds)
{
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("GetQueuedCompletionStatus")),
                          -1);
    }
  else
    {
      // Narrow the result.
      ACE_WIN32_Asynch_Result *asynch_result = (ACE_WIN32_Asynch_Result *) overlapped;

      // If errors happen, grab the error.
      if (result == FALSE)
        errno = ::GetLastError ();
      else
        errno = 0;

      this->application_specific_code (asynch_result,
                                       bytes_transferred,
                                       result,
                                       (void *) completion_key,
                                       errno);
    }
  return 0;
}

void
ACE_WIN32_Proactor::application_specific_code (ACE_WIN32_Asynch_Result *asynch_result,
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
ACE_WIN32_Proactor::post_completion (ACE_WIN32_Asynch_Result *result)
{
  // Grab the event associated with the Proactor
  HANDLE handle = this->get_handle ();

  // If Proactor event is valid, signal it
  if (handle != ACE_INVALID_HANDLE &&
      handle != 0)
    ACE_OS::event_signal (&handle);

  // Post a completion
  if (::PostQueuedCompletionStatus (this->completion_port_, // completion port
                                    0, // number of bytes tranferred
                                    0, // completion key
                                    result // overlapped
                                    ) == FALSE)
    {
      delete result;
      ACE_ERROR_RETURN ((LM_ERROR, "Failure in dealing with timers: PostQueuedCompletionStatus failed\n"), -1);
    }

  return 0;
}

int
ACE_WIN32_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_WIN32_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_WIN32_Proactor::number_of_threads (void) const
{
  return this->number_of_threads_;
}

void
ACE_WIN32_Proactor::number_of_threads (size_t threads)
{
  this->number_of_threads_ = threads;
}

ACE_WIN32_Proactor::TIMER_QUEUE *
ACE_WIN32_Proactor::timer_queue (void) const
{
  return this->timer_queue_;
}

void
ACE_WIN32_Proactor::timer_queue (TIMER_QUEUE *tq)
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
      this->timer_queue_ = new TIMER_HEAP;
      this->delete_timer_queue_ = 1;
    }
  else
    {
      this->timer_queue_ = tq;
      this->delete_timer_queue_ = 0;
    }

  // Set the proactor in the timer queue's functor
  this->timer_queue_->upcall_functor ().win32_proactor (*this);
}

// *********************************************************************

ACE_WIN32_Asynch_Timer::ACE_WIN32_Asynch_Timer (ACE_Handler &handler,
                                                const void *act,
                                                const ACE_Time_Value &tv,
                                                ACE_HANDLE event)
  : ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, 0),
    time_ (tv)
{
}

void
ACE_WIN32_Asynch_Timer::complete (u_long bytes_transferred,
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
#endif /* ACE_WIN32 */
