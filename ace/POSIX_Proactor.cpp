// $Id$

#define ACE_BUILD_DLL
#include "ace/POSIX_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS)

#include "ace/Task_T.h"
#include "ace/Log_Msg.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/POSIX_Proactor.i"
#endif /* __ACE_INLINE__ */

ACE_POSIX_Proactor::~ACE_POSIX_Proactor (void)
{
}

int
ACE_POSIX_Proactor::close (void)
{
#if 0
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
#endif /* 0 */
  return 0;
}

int
ACE_POSIX_Proactor::register_handle (ACE_HANDLE handle,
                                     const void *completion_key)
{
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (completion_key);
  return 0;
}

long
ACE_POSIX_Proactor::schedule_timer (ACE_Handler &handler,
                                    const void *act,
                                    const ACE_Time_Value &time)
{
  return this->schedule_timer (handler,
                               act,
                               time,
                               ACE_Time_Value::zero);
}

long
ACE_POSIX_Proactor::schedule_repeating_timer (ACE_Handler &handler,
                                              const void *act,
                                              const ACE_Time_Value &interval)
{
  return this->schedule_timer (handler,
                               act,
                               interval,
                               interval);
}

long
ACE_POSIX_Proactor::schedule_timer (ACE_Handler &handler,
                                    const void *act,
                                    const ACE_Time_Value &time,
                                    const ACE_Time_Value &interval)
{
  ACE_UNUSED_ARG (handler);
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (time);
  ACE_UNUSED_ARG (interval);
  ACE_NOTSUP_RETURN ((long) -1);

#if 0
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
#endif /* 0 */
}

int
ACE_POSIX_Proactor::cancel_timer (long timer_id,
                                  const void **arg,
                                  int dont_call_handle_close)
{
  ACE_NOTSUP_RETURN (-1);
#if 0
  // No need to singal timer event here. Even if the cancel timer was
  // the earliest, we will have an extra wakeup.
  return this->timer_queue_->cancel (timer_id,
                                     arg,
                                     dont_call_handle_close);
#endif /* 0 */
}

int
ACE_POSIX_Proactor::cancel_timer (ACE_Handler &handler,
                                  int dont_call_handle_close)
{
  ACE_NOTSUP_RETURN (-1);
#if 0
  // No need to signal timer event here. Even if the cancel timer was
  // the earliest, we will have an extra wakeup.
  return this->timer_queue_->cancel (&handler,
                                     dont_call_handle_close);
#endif /* 0 */
}

int
ACE_POSIX_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_POSIX_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_POSIX_Proactor::number_of_threads (void) const
{
  // @@ Implement it. 
  ACE_NOTSUP_RETURN (0);
}

void
ACE_POSIX_Proactor::number_of_threads (size_t threads)
{
  // @@ Implement it.
  ACE_UNUSED_ARG (threads);
}

#if 0
ACE_Proactor::Timer_Queue *
ACE_POSIX_Proactor::timer_queue (void) const
{
  return this->timer_queue_;
}

void
ACE_POSIX_Proactor::timer_queue (Timer_Queue *tq)
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
#endif /* 0 */

ACE_HANDLE
ACE_POSIX_Proactor::get_handle (void) const
{
  return ACE_INVALID_HANDLE;
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_POSIX_Proactor::create_asynch_read_stream_result (ACE_Handler &handler,
                                                      ACE_HANDLE handle,
                                                      ACE_Message_Block &message_block,
                                                      u_long bytes_to_read,
                                                      const void* act,
                                                      ACE_HANDLE event,
                                                      int priority)
{
  ACE_Asynch_Read_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Read_Stream_Result (handler,
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
ACE_POSIX_Proactor::create_asynch_write_stream_result (ACE_Handler &handler,
                                                       ACE_HANDLE handle,
                                                       ACE_Message_Block &message_block,
                                                       u_long bytes_to_write,
                                                       const void* act,
                                                       ACE_HANDLE event,
                                                       int priority)
{
  ACE_Asynch_Write_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Write_Stream_Result (handler,
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
ACE_POSIX_Proactor::create_asynch_read_file_result (ACE_Handler &handler,
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
                  ACE_POSIX_Asynch_Read_File_Result (handler,
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
ACE_POSIX_Proactor::create_asynch_write_file_result (ACE_Handler &handler,
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
                  ACE_POSIX_Asynch_Write_File_Result (handler,
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
ACE_POSIX_Proactor::create_asynch_accept_result (ACE_Handler &handler,
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
                  ACE_POSIX_Asynch_Accept_Result (handler,
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
ACE_POSIX_Proactor::create_asynch_transmit_file_result (ACE_Handler &handler,
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
                  ACE_POSIX_Asynch_Transmit_File_Result (handler,
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

ACE_POSIX_Proactor::ACE_POSIX_Proactor (void)
{
}

#if 0
int
ACE_POSIX_Proactor::handle_signal (int, siginfo_t *, ucontext_t *)
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
ACE_POSIX_Proactor::handle_close (ACE_HANDLE handle,
                                  ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG (close_mask);
  ACE_UNUSED_ARG (handle);

  return this->close ();
}
#endif /* 0 */

void
ACE_POSIX_Proactor::application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
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

// *********************************************************************

class ACE_Export ACE_AIOCB_Notify_Pipe_Manager : public ACE_Handler
{
  // = TITLE
  //     This class manages the notify pipe of the AIOCB
  //     Proactor. This class acts as the Handler for the
  //     <Asynch_Read> operations issued on the notify pipe. This
  //     class is very useful in implementing <Asynch_Accept> operation
  //     class for the <AIOCB_Proactor>. This is also useful for
  //     implementing <post_completion> for <AIOCB_Proactor>.
  //
  // = DESCRIPTION
  //     <AIOCB_Proactor> class issues a <Asynch_Read> on
  //     the pipe, using this class as the
  //     Handler. <POSIX_Asynch_Result *>'s are sent through the
  //     notify pipe. When <POSIX_Asynch_Result *>'s show up on the
  //     notify pipe, the <POSIX_AIOCB_Proactor> dispatches the
  //     completion of the <Asynch_Read_Stream> and calls the
  //     <handle_read_stream> of this class. This class calls complete
  //     on the <POSIX_Asynch_Result *> and thus calls the application
  //     handler. 
  //     Handling the MessageBlock:
  //     We give this message block to read the result pointer through
  //     the notify pipe. We expect that to read 4 bytes from the
  //     notify pipe, for each <accept> call. Before giving this
  //     message block to another <accept>, we update <wr_ptr> and put
  //     it in its initial position.
public:
  ACE_AIOCB_Notify_Pipe_Manager (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Constructor. You need the posix proactor because you need to call
  // <application_specific_code>
  
  virtual ~ACE_AIOCB_Notify_Pipe_Manager (void);
  // Destructor.
  
  int notify (ACE_POSIX_Asynch_Result *result);
  // Send the result pointer through the notification pipe.

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is the call back method when <Asynch_Read> from the pipe is
  // complete.

private:
  ACE_POSIX_AIOCB_Proactor  *posix_aiocb_proactor_;
  // The implementation proactor class.
  
  ACE_Message_Block message_block_;
  // Message block to get ACE_POSIX_Asynch_Result pointer from the
  // pipe. 

  ACE_Pipe pipe_;
  // Pipe for the communication between Proactor and the
  // Asynch_Accept.

  ACE_POSIX_AIOCB_Asynch_Read_Stream read_stream_;
  // To do asynch_read on the pipe.

  ACE_AIOCB_Notify_Pipe_Manager (void);
  // Default constructor. Shouldnt be called.
};

ACE_AIOCB_Notify_Pipe_Manager::ACE_AIOCB_Notify_Pipe_Manager (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : posix_aiocb_proactor_ (posix_aiocb_proactor),
    message_block_ (sizeof (ACE_POSIX_Asynch_Accept_Result *)),
    read_stream_ (posix_aiocb_proactor)
{
  // Open the pipe.
  this->pipe_.open ();

  // Open the read stream.
  if (this->read_stream_.open (*this,
                               this->pipe_.read_handle (),
                               0, // Completion Key
                               0) // Proactor
      == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Open on Read Stream failed"));

  // Issue an asynch_read on the read_stream of the notify pipe.
  if (this->read_stream_.read (this->message_block_,
                               sizeof (ACE_POSIX_Asynch_Result *),
                               0, // ACT
                               0) // Priority
      == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Read from pipe failed"));
}

ACE_AIOCB_Notify_Pipe_Manager::~ACE_AIOCB_Notify_Pipe_Manager (void)
{
}

int
ACE_AIOCB_Notify_Pipe_Manager::notify (ACE_POSIX_Asynch_Result *result)
{
  // Send the result pointer through the pipe.
  int return_val = ACE::send (this->pipe_.write_handle (),
                              (char *) &result,
                              sizeof (result));
  if (return_val != sizeof (result))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P %t):%p\n",
                       "Error:Writing on to notify pipe failed"),
                      -1);
  return 0;
}

void
ACE_AIOCB_Notify_Pipe_Manager::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  // The message block actually contains the ACE_POSIX_Asynch_Result
  // pointer. 
  ACE_POSIX_Asynch_Result *asynch_result = 0;
  asynch_result = *(ACE_POSIX_Asynch_Result **) result.message_block ().rd_ptr ();

  // Do the upcall.
  this->posix_aiocb_proactor_->application_specific_code (asynch_result,
                                                          0,  // No Bytes transferred.
                                                          1,  // Success.
                                                          0,  // Completion token.
                                                          0); // Error.

  // Set the message block properly. Put the <wr_ptr> back in the
  // initial position.
  if (this->message_block_.length () > 0)
      this->message_block_.wr_ptr (this->message_block_.rd_ptr ());

  // One accept has completed. Issue a read to handle any
  // <post_completion>s in the future.
  if (this->read_stream_.read (this->message_block_,
                               sizeof (ACE_POSIX_Asynch_Result *),
                               0,  // ACT
                               0)  // Priority
      == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "Read from pipe failed"));
}

// *********************************************************************

ACE_POSIX_AIOCB_Proactor::ACE_POSIX_AIOCB_Proactor (void)
  : aiocb_notify_pipe_manager_ (0),
    aiocb_list_max_size_ (ACE_RTSIG_MAX),
    aiocb_list_cur_size_ (0)
{
  // Initialize the array.
  for (size_t ai = 0; ai < this->aiocb_list_max_size_; ai++)
    aiocb_list_[ai] = 0;

  // Accept Handler for aio_accept. Remember! this issues a Asynch_Read
  // on the notify pipe for doing the Asynch_Accept.
  ACE_NEW (aiocb_notify_pipe_manager_,
           ACE_AIOCB_Notify_Pipe_Manager (this));
}

// Destructor.
ACE_POSIX_AIOCB_Proactor::~ACE_POSIX_AIOCB_Proactor (void)
{
}

int
ACE_POSIX_AIOCB_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_POSIX_AIOCB_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_POSIX_AIOCB_Proactor::post_completion (ACE_POSIX_Asynch_Result *result)
{
  return this->aiocb_notify_pipe_manager_->notify (result);
}

ACE_Asynch_Read_Stream_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_read_stream (void)
{
  ACE_Asynch_Read_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Read_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Write_Stream_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_write_stream (void)
{
  ACE_Asynch_Write_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Write_Stream (this),
                  0);
  return implementation;
}

ACE_Asynch_Read_File_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_read_file (void)
{
  ACE_Asynch_Read_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Read_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Write_File_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_write_file (void)
{
  ACE_Asynch_Write_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Write_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Accept_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_accept (void)
{
  ACE_Asynch_Accept_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Accept (this),
                  0);
  return implementation;
}

ACE_Asynch_Transmit_File_Impl *
ACE_POSIX_AIOCB_Proactor::create_asynch_transmit_file (void)
{
  ACE_Asynch_Transmit_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_AIOCB_Asynch_Transmit_File (this),
                  0);
  return  implementation;
}

int
ACE_POSIX_AIOCB_Proactor::handle_events (unsigned long milli_seconds)
{
  if (this->aiocb_list_cur_size_ == 0)
    // No aio is pending.
    return 0;

  // Wait for asynch operation to complete.
  // @@ Assing milli seconds correctly.
  timespec timeout;
  timeout.tv_sec = milli_seconds;
  timeout.tv_nsec = 0;

  if (aio_suspend (this->aiocb_list_,
                   this->aiocb_list_max_size_,
                   &timeout) == -1)
    {
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
    }

  // Check which aio has finished.
  size_t ai;
  int error_status = 0;
  int return_status = 0;
  for (ai = 0; ai < this->aiocb_list_max_size_; ai++)
    {
      if (aiocb_list_ [ai] == 0)
        continue;

      // Analyze error and return values.

      // Get the error status of the aio_ operation.
      error_status = aio_error (aiocb_list_[ai]);
      if (error_status == -1)
        // <aio_error> itself has failed.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p\n",
                           "<aio_error> has failed"),
                          -1);

      if (error_status == EINPROGRESS)
        // <aio_> operation is still in progress.
        continue;

      // Error_status is not -1 and not EINPROGRESS. So, an <aio_>
      // operation has finished (successfully or unsuccessfully!!!)

      ACE_ERROR ((LM_ERROR,
                  "%N:%l:error_status = %d\n",
                  error_status));

      // Get the return_status of the <aio_> operation.
      return_status = aio_return (aiocb_list_[ai]);
      if (return_status == -1)
        {
          // <aio_return> itself has failed.
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error:%p\n",
                             "<aio_return> failed"),
                            -1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "An aio has finished\n"));
          break;
        }
    }

  if (ai == this->aiocb_list_max_size_)
    // Nothing completed.
    return 0;

  // Retrive the result pointer.
  ACE_POSIX_Asynch_Result *asynch_result =
    // dynamic_cast <ACE_POSIX_Asynch_Result *> (this->aiocb_list_[ai]);
    (ACE_POSIX_Asynch_Result *) (this->aiocb_list_[ai]);
  if (asynch_result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%N:%l:%p\n",
                       "Dynamic cast failed"),
                      -1);

  // Invalidate entry in the aiocb list.
  this->aiocb_list_[ai] = 0;
  this->aiocb_list_cur_size_--;

  // Call the application code.
  // @@ Pass <errno> instead of 0. Check out on LynxOS. It is set
  // to 77 somewhere.
  this->application_specific_code (asynch_result,
                                   return_status, // Bytes transferred.
                                   1,             // Success
                                   0,             // No completion key.
                                   error_status); // Error

  return 0;
}

void
ACE_POSIX_AIOCB_Proactor::application_specific_code (ACE_POSIX_Asynch_Result *asynch_result,
                                                     u_long bytes_transferred,
                                                     int success,
                                                     const void *completion_key,
                                                     u_long error)
{
  ACE_POSIX_Proactor::application_specific_code (asynch_result,
                                                 bytes_transferred,
                                                 success,
                                                 completion_key,
                                                 error);
}

int
ACE_POSIX_AIOCB_Proactor::register_aio_with_proactor (aiocb *aiocb_ptr)
{
  if (aiocb_ptr == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Status check max %d cur %d\n",
                  this->aiocb_list_max_size_,
                  this->aiocb_list_cur_size_));

      // Just check the status of the list.
      if (this->aiocb_list_cur_size_ >=
          this->aiocb_list_max_size_)
        return -1;
      else
        return 0;
    }

  // Non-zero ptr. Find a free slot and store.

  // Make sure again.
  if (this->aiocb_list_cur_size_ >=
      this->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Slot(s) available. Find a free one.
  size_t ai;
  for (ai = 0;
       ai < this->aiocb_list_max_size_;
       ai++)
    if (this->aiocb_list_[ai] == 0)
      break;

   // Sanity check.
  if (ai == this->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Store the pointers.
  this->aiocb_list_[ai] = aiocb_ptr;
  this->aiocb_list_cur_size_ ++;

  return 0;
}

// *********************************************************************

ACE_POSIX_SIG_Proactor::ACE_POSIX_SIG_Proactor (void)
{
  // Make the sigset_t consisting of the completion signals.
  if (sigemptyset (&this->RT_completion_signals_) < 0)
    ACE_ERROR ((LM_ERROR,
                "Error:%p\n",
                "Couldn't init the RT completion signal set"));

  if (sigaddset (&this->RT_completion_signals_, ACE_SIG_AIO) < 0)
    ACE_ERROR ((LM_ERROR,
                "Error:%p\n",
                "Couldnt init the RT completion signal set"));

  // Mask them.
  if (sigprocmask (SIG_BLOCK, &RT_completion_signals_, 0) < 0)
    ACE_ERROR ((LM_ERROR,
                "Error:%p\n",
                "Couldnt mask the RT completion signals"));

  // Setting up the handler(actually Null handler) for these signals.
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


ACE_POSIX_SIG_Proactor::~ACE_POSIX_SIG_Proactor (void)
{
  // @@ Enable the masked signals again.
}

int
ACE_POSIX_SIG_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  // Decrement <wait_time> with the amount of time spent in the method
  ACE_Countdown_Time countdown (&wait_time);
  return this->handle_events (wait_time.msec ());
}

int
ACE_POSIX_SIG_Proactor::handle_events (void)
{
  return this->handle_events (ACE_INFINITE);
}

int
ACE_POSIX_SIG_Proactor::post_completion (ACE_POSIX_Asynch_Result *result)
{
  // Get this process id.
  pid_t pid = ACE_OS::getpid ();
  if (pid == (pid_t) -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p",
                       "<getpid> failed\n"),
                      -1);
  
  // Set the signal information.
  sigval value;
  value.sival_ptr = (void *) result;
  
  // Queue the signal.
  if (sigqueue (pid, ACE_SIG_AIO, value) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p",
                       "<sigqueue> failed\n"),
                      -1);
  return 0;
}

ACE_Asynch_Read_Stream_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_read_stream (void)
{
  ACE_Asynch_Read_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation, ACE_POSIX_SIG_Asynch_Read_Stream (this), 0);
  return  implementation;
}

ACE_Asynch_Write_Stream_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_write_stream (void)
{
  ACE_Asynch_Write_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation, ACE_POSIX_SIG_Asynch_Write_Stream (this), 0);
  return  implementation;
}

ACE_Asynch_Read_File_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_read_file (void)
{
  ACE_Asynch_Read_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation, ACE_POSIX_SIG_Asynch_Read_File (this), 0);
  return  implementation;
}

ACE_Asynch_Write_File_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_write_file (void)
{
  ACE_Asynch_Write_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation, ACE_POSIX_SIG_Asynch_Write_File (this), 0);
  return  implementation;
}

ACE_Asynch_Accept_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_accept (void)
{
  ACE_Asynch_Accept_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation, ACE_POSIX_SIG_Asynch_Accept (this), 0);
  return implementation;
}

ACE_Asynch_Transmit_File_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_transmit_file (void)
{
  ACE_Asynch_Transmit_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Transmit_File (this),
                  0);
  return  implementation;
}

int
ACE_POSIX_SIG_Proactor::handle_events (unsigned long milli_seconds)
{
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
  ACE_POSIX_Asynch_Result *asynch_result =
    (ACE_POSIX_Asynch_Result *) sig_info.si_value.sival_ptr;

  // Check the <signal code> and act according to that.
  if (sig_info.si_code == SI_ASYNCIO)
    {
      // Retrieve the aiocb from Result ptr.
      // @@ Some checking should be done to make sure this pointer
      // is valid. Otherwise <aio_error> will bomb.
      aiocb* aiocb_ptr =
        (aiocb *)asynch_result;

      // Analyze error and return values. Return values are
      // actually <errno>'s associated with the <aio_> call
      // corresponding to aiocb_ptr.
      int error_status = aio_error (aiocb_ptr);
      if (error_status == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p\n",
                           "Invalid control block was sent to <aio_error> for compleion querying"),
                          -1);

      // Completion signal has been received, so it can't be in
      // progress.
      // ACE_ASSERT (error_status != EINPROGRESS)

      // No error occured in the AIO operation.
      int return_status = aio_return (aiocb_ptr);
      if (return_status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error:%p\n",
                             "Invalid control block was send to <aio_return>"),
                            -1);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "An aio has finished\n"));

          this->application_specific_code (asynch_result,
                                           return_status,
                                           1,  // Result : True.
                                           0,  // No completion_signal.
                                           error_status); // Error.
        }
    }
  else if (sig_info.si_code == SI_QUEUE)
    {
      // @@ Just debugging.
      ACE_DEBUG ((LM_DEBUG, "<sigqueue>'d signal received\n"));

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

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if 0
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
#endif /* 0 */
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

#endif /* ACE_HAS_AIO_CALLS */
