// $Id$

#define ACE_BUILD_DLL
#include "ace/Proactor.h"

ACE_RCSID(ace, Proactor, "$Id$")

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
  // Constructor.

  ~ACE_Proactor_Timer_Handler (void);
  // Destructor.

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
  // Mark for closing down.
  this->shutting_down_ = 1;

  // Signal timer event.
  this->timer_event_.signal ();
}

int
ACE_Proactor_Timer_Handler::svc (void)
{
#if defined (ACE_HAS_AIO_CALLS)
  // @@ To be implemented.
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
        }

      // Wait for event upto <time> milli seconds.
      u_int result = ::WaitForSingleObject (this->timer_event_.handle (),
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

#if defined (ACE_HAS_AIO_CALLS)
class ACE_Export ACE_AIO_Accept_Handler : public ACE_Handler
{
  // = TITLE
  //     Helper class for doing Asynch_Accept in POSIX4 systems, in
  //     the case of doing AIO_CONTROL_BLOCKS strategy.
  //
  // = DESCRIPTION
  //     Doing Asynch_Accept in POSIX4 implementation is tricky. In
  //     the case of doing the things with AIO_CONTROL_BLOCKS and not
  //     with Real-Time Signals, this becomes even more trickier. We
  //     use a notifyn pipe here to implement Asynch_Accpet. This class
  //     will issue a <Asynch_Read> on the pipe. <Asynch_Accept> will
  //     send a result pointer containg all the information through
  //     this pipe.
  //     Handling the MessageBlock:
  //     We give this message block to read the result pointer through
  //     the notify pipe. We expect that to read 4 bytes from the
  //     notify pipe, for each <accept> call. Before giving this
  //     message block to another <accept>, we update <wr_ptr> and put
  //     it in its initial position. 
public:
  ACE_AIO_Accept_Handler (ACE_Proactor* proactor);
  // Constructor.

  ~ACE_AIO_Accept_Handler (void);
  // Destructor.
  
  int notify (ACE_Asynch_Accept::Result* result);
  // Send this Result to Proactor through the notification pipe.

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // Read from the pipe is complete.  We get the <Result> from
  // Asynch_Handler. We have to do the notification here.

private:
  ACE_Proactor* proactor_;
  // The proactor in use.

  ACE_Message_Block message_block_;
  // Message block to get ACE_Asynch_Accept::Result pointer from
  // ACE_Asych_Accept. 

  ACE_Pipe pipe_;
  // Pipe for the communication between Proactor and the
  // Asynch_Accept.

  ACE_Asynch_Read_Stream read_stream_;
  // To do asynch_read on the pipe.

  ACE_AIO_Accept_Handler (void);
  // Default constructor. Shouldnt be called.
};

ACE_AIO_Accept_Handler::ACE_AIO_Accept_Handler (ACE_Proactor *proactor)
  : proactor_ (proactor),
    message_block_ (sizeof (ACE_Asynch_Accept::Result *) + 64)
{
  // Open the pipe.
  this->pipe_.open ();

  // Open the read stream.
  if (this->read_stream_.open (*this,
                               this->pipe_.read_handle (),
                               0,
                               this->proactor_) == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Open on Read Stream failed"));

  // Issue an asynch_read on the read_stream of the notify pipe. 
  if (this->read_stream_.read (this->message_block_,
                               sizeof (ACE_Asynch_Accept::Result *)) == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Read from stream failed"));
}

ACE_AIO_Accept_Handler::~ACE_AIO_Accept_Handler (void)
{
}

int
ACE_AIO_Accept_Handler::notify (ACE_Asynch_Accept::Result* result)
{
  // Send the result pointer through the pipe.
  int return_val = ACE::send (this->pipe_.write_handle (),
                              (char *) &result,
                              sizeof (result));
  if (return_val != sizeof (result))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P %t):%p\n",
                       "Error:Writing on to pipe failed"),
                      -1);
  return 0;
}

void
ACE_AIO_Accept_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  // @@
  ACE_DEBUG ((LM_DEBUG, "ACE_AIO_Accept_Handler::handle_read_stream called\n"));
  
  // The message block actually contains the ACE_Asynch_Accept::Result.
  ACE_Asynch_Accept::Result *accept_result = 0;
  accept_result = *(ACE_Asynch_Accept::Result **) result.message_block ().rd_ptr ();
  
  // Do the upcall.
  this->proactor_->application_specific_code (accept_result,
                                              0,  // No Bytes transferred.
                                              1,  // Success.
                                              0,  // Completion token.
                                              0); // Error.

  // Set the message block properly. Put the <wr_ptr> back in the
  // initial position. 
  if (this->message_block_.length () > 0)
      this->message_block_.wr_ptr (this->message_block_.rd_ptr ());
  
  // One accept has completed. Issue a read to handle any <accept>s in
  // the future.
  if (this->read_stream_.read (this->message_block_,
                               sizeof (ACE_Asynch_Accept::Result)) == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Read from stream failed"));
}
#endif /* ACE_HAS_AIO_CALLS */

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
                       ASYS_TEXT ("(%t) No Proactor set in ACE_Proactor_Handle_Timeout_Upcall,")
                       ASYS_TEXT (" no completion port to post timeout to?!@\n")),
                      -1);

  // Create the Asynch_Timer.
  ACE_Proactor::Asynch_Timer *asynch_timer;
  ACE_NEW_RETURN (asynch_timer,
                  ACE_Proactor::Asynch_Timer (*handler,
                                              act,
                                              time),
                  -1);

  // Post a completion.
  if (this->proactor_->post_completion (asynch_timer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("Failure in dealing with timers: ")
                       ASYS_TEXT ("PostQueuedCompletionStatus failed\n")),
                      -1);
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
                       ASYS_TEXT ("ACE_Proactor_Handle_Timeout_Upcall is only suppose")
                       ASYS_TEXT (" to be used with ONE (and only one) Proactor\n")),
                      -1);
}

ACE_Proactor::ACE_Proactor (size_t number_of_threads,
                            Timer_Queue *tq,
                            int used_with_reactor_event_loop,
                            POSIX_COMPLETION_STRATEGY completion_strategy)
  :
#if defined (ACE_HAS_AIO_CALLS)
  posix_completion_strategy_ (completion_strategy),
  aio_accept_handler_ (0),
  aiocb_list_max_size_ (ACE_RTSIG_MAX),
  aiocb_list_cur_size_ (0),
#else /* ACE_HAS_AIO_CALLS */
  completion_port_ (0),
  // This *MUST* be 0, *NOT* ACE_INVALID_HANDLE !!!
  number_of_threads_ (number_of_threads),
#endif /* ACE_HAS_AIO_CALLS */
  timer_queue_ (0),
  delete_timer_queue_ (0),
  timer_handler_ (0),
  used_with_reactor_event_loop_ (used_with_reactor_event_loop)
{
#if defined (ACE_HAS_AIO_CALLS)
  ACE_UNUSED_ARG (number_of_threads);
  ACE_UNUSED_ARG (tq);

  // The following things are necessary only for the
  // AIO_CONTROL_BLOCKS strategy.
  if (this->posix_completion_strategy_ == AIO_CONTROL_BLOCKS)
    {
      // Initialize the array.
      for (size_t ai = 0;
           ai < this->aiocb_list_max_size_;
           ai++)
        aiocb_list_[ai] = 0;

      // Accept Handler for aio_accept. Remember! this issues a Asynch_Read
      // on the notify pipe for doing the Asynch_Accept.
      ACE_NEW (aio_accept_handler_,
               ACE_AIO_Accept_Handler (this));
    }


  // Mask the RT_compeltion signals if we are using the RT_SIGNALS
  // STRATEGY for completion querying.
  if (completion_strategy == RT_SIGNALS)
    {
      // Make the sigset_t consisting of the completion signals.
      if (sigemptyset (&this->RT_completion_signals_) < 0)
        ACE_ERROR ((LM_ERROR,
                    "Error:%p\n",
                    "Couldn't init the RT completion signal set"));

      if (sigaddset (&this->RT_completion_signals_,
                     ACE_SIG_AIO) < 0)
        ACE_ERROR ((LM_ERROR,
                    "Error:%p\n",
                    "Couldnt init the RT completion signal set"));

      // Mask them.
      if (sigprocmask (SIG_BLOCK, &RT_completion_signals_, 0) < 0)
        ACE_ERROR ((LM_ERROR,
                    "Error:%p\n",
                    "Couldnt mask the RT completion signals"));

      // Setting up the handler(!) for these signals.
      struct sigaction reaction;
      sigemptyset (&reaction.sa_mask);   // Nothing else to mask.
      reaction.sa_flags = SA_SIGINFO;    // Realtime flag.
#if defined (SA_SIGACTION)
      // Lynx says, it is better to set this bit to be portable.
      reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */
      reaction.sa_sigaction = 0;         // No handler.
      int sigaction_return = sigaction (ACE_SIG_AIO,
                                        &reaction,
                                        0);
      if (sigaction_return == -1)
        ACE_ERROR ((LM_ERROR,
                    "Error:%p\n",
                    "Proactor couldnt do sigaction for the RT SIGNAL"));
    }

#else /* ACE_HAS_AIO_CALLS */

  ACE_UNUSED_ARG (completion_strategy);

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
#if !defined  (ACE_HAS_AIO_CALLS)
  // Close the completion port
  if (this->completion_port_ != 0)
    {
      int result = ACE_OS::close (this->completion_port_);
      this->completion_port_ = 0;
      return result;
    }
#endif /* NOT ACE_HAS_AIO_CALLS */
  return 0;
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
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->timer_queue_->mutex (), -1);

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
  int result = 0;

  for (;;)
    {
      result = this->handle_events (timeout);

      if (result != 0 || errno == ETIME)
        break;
    }

  // If our handle_events failed, we'll report a failure to the
  // Reactor.
  if (result == -1)
    return -1;
  else
    return 0;
}

int
ACE_Proactor::handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (close_mask);
  ACE_UNUSED_ARG (handle);

  return this->close ();
}

// @@ get_handle () implementation. (Alex)
ACE_HANDLE
ACE_Proactor::get_handle (void) const
{
#if defined (ACE_HAS_AIO_CALLS)
  return ACE_INVALID_HANDLE;
#else /* Not ACE_HAS_AIO_CALLS */
  if (this->used_with_reactor_event_loop_)
    return this->event_.handle ();
  else
    return 0;
#endif /* ACE_HAS_AIO_CALLS */
}

#if defined (ACE_HAS_AIO_CALLS)
ACE_Proactor::POSIX_COMPLETION_STRATEGY
ACE_Proactor::posix_completion_strategy (void)
{
  return posix_completion_strategy_;
}

#if 0
void
ACE_Proactor::posix_completion_strategy (ACE_Proactor::POSIX_COMPLETION_STRATEGY strategy)
{
  this->posix_completion_strategy_ = strategy;
}
#endif /* 0 */

int
ACE_Proactor::notify_asynch_accept (ACE_Asynch_Accept::Result* result)
{
  this->aio_accept_handler_->notify (result);

  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

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
  if (posix_completion_strategy () == ACE_Proactor::RT_SIGNALS)
    {
      // Using RT Signals.

      // Wait for <milli_seconds> amount of time.
      // @@ Assigning <milli_seconds> to tv_sec.
      timespec timeout;
      timeout.tv_sec = milli_seconds;
      timeout.tv_nsec = 0;

      // To get back the signal info.
      siginfo_t sig_info;

      // Await the RT completion signal.
      int sig_return = sigtimedwait (&this->RT_completion_signals_,
                                     &sig_info,
                                     &timeout);

      // Error case.
      // If failure is coz of timeout, then return *0* but set
      // errno appropriately. This is what the WinNT proactor
      // does.
      if (sig_return == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p\n",
                           "Waiting for RT completion signals"),
                          0);

      // RT completion signals returned.
      if (sig_return != ACE_SIG_AIO)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unexpected signal (%d) has been received while waiting for RT Completion Signals\n",
                           sig_return),
                          -1);

      // @@ Debugging.
      ACE_DEBUG ((LM_DEBUG,
                  "Sig number found in the sig_info block : %d\n",
                  sig_info.si_signo));

      // Is the signo returned consistent?
      if (sig_info.si_signo != sig_return)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Inconsistent signal number (%d) in the signal info block\n",
                           sig_info.si_signo),
                          -1);

      // @@ Debugging.
      ACE_DEBUG ((LM_DEBUG,
                  "Signal code for this signal delivery : %d\n",
                  sig_info.si_code));

      // Retrive the result pointer.
      ACE_Asynch_Result *asynch_result =
        (ACE_Asynch_Result *) sig_info.si_value.sival_ptr;

      // Check the <signal code> and act according to that.
      if (sig_info.si_code == SI_ASYNCIO)
        {
          // Retrieve the aiocb from Result ptr.
          // @@ Some checking should be done to make sure this pointer
          // is valid. Otherwise <aio_error> will bomb.
          aiocb* aiocb_ptr =
            (aiocb *)asynch_result->aiocb_ptr ();

          // Analyze error and return values. Return values are
          // actually <errno>'s associated with the <aio_> call
          // corresponding to aiocb_ptr.
          int error_code = aio_error (aiocb_ptr);
          if (error_code == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Error:%p\n",
                               "Invalid control block was sent to <aio_error> for compleion querying"),
                              -1);

          if (error_code != 0)
            // Error occurred in the <aio_>call. Return the errno
            // corresponding to that <aio_> call.
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Error:%p\n",
                               "An AIO call has failed"),
                              error_code);

          // No error occured in the AIO operation.
          int nbytes = aio_return (aiocb_ptr);
          if (nbytes == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Error:%p\n",
                               "Invalid control block was send to <aio_return>"),
                              -1);

          // <nbytes> have been successfully transmitted.
          size_t bytes_transferred = nbytes;

          // Call the application code.
          this->application_specific_code (asynch_result,
                                           bytes_transferred,
                                           1,  // Result : True.
                                           0,  // No completion_signal.
                                           0); // Error.
        }
      else if (sig_info.si_code == SI_QUEUE)
        {
          // @@ Just debugging.
          ACE_DEBUG ((LM_DEBUG, "<sigqueue>'d signal received\n"));

          // Should be from the <Asynch_Accept> call.
          this->application_specific_code (asynch_result,
                                           0, // No bytes transferred.
                                           1, // Result : True.
                                           0, // No completion key.
                                           0); // No error.

        }
      else
        // Unknown signal code.
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Unexpected signal code (%d) returned on completion querying\n",
                           sig_info.si_code),
                          -1);
    }
  else
    {
      //  Not RT_SIGNALS approach. Using <aiocb> control blocks.

      // Is there any entries in the list.
      if (this->aiocb_list_cur_size_ == 0)
        // No aio is pending.
        return 0;

      // Wait for asynch operation to complete.
      timespec timeout;
      timeout.tv_sec = milli_seconds;
      timeout.tv_nsec = 0;

      if (aio_suspend (this->aiocb_list_,
                       this->aiocb_list_max_size_,
                       &timeout) == -1)
        // If failure is coz of timeout, then return *0* but set errno
        // appropriately. This is what the WinNT proactor does.
        if (errno ==  EAGAIN)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error:%p\n",
                             "aio_suspend"),
                            0);
        else
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error:%p\n",
                             "aio_suspend"),
                            -1);
      // Check which aio has finished.
      size_t ai;
      ssize_t nbytes = 0;
      for (ai = 0; ai < this->aiocb_list_max_size_; ai++)
        {
          if (aiocb_list_ [ai] == 0)
            continue;
          // Analyze error and return values.
          if (aio_error (aiocb_list_[ai]) != EINPROGRESS)
            {
              nbytes = aio_return (aiocb_list_[ai]);

              if (nbytes == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Error:%p\n",
                                   "An AIO has failed"),
                                  -1);
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "An aio has finished\n"));
                  // This AIO is done.
                  break;
                }
            }
        }

      if (ai == this->aiocb_list_max_size_)
        // Nothing completed.
        return 0;

      // Get the values for the completed aio.
      
      // Bytes transfered is what the aio_return gives back.
      size_t bytes_transferred = nbytes;

      // Retrive the result pointer.
      ACE_Asynch_Result *asynch_result = (ACE_Asynch_Result *)
        aiocb_list_[ai]->aio_sigevent.sigev_value.sival_ptr;

      // Invalidate entry in the aiocb list.
      delete this->aiocb_list_[ai];
      this->aiocb_list_[ai] = 0;
      this->aiocb_list_cur_size_--;

      // Call the application code.
      // @@ Pass <errno> instead of 0. Check out on LynxOS. It is set
      // to 77 somewhere.
      this->application_specific_code (asynch_result,
                                       bytes_transferred,
                                       1,
                                       0, // No completion key.
                                       0);
    }
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("GetQueuedCompletionStatus")),
                          -1);
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
                                    0,// completion key
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

#if !defined (ACE_HAS_AIO_CALLS)
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
#endif /* ACE_HAS_AIO_CALLS */

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
