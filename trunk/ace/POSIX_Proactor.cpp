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

class ACE_Export ACE_POSIX_Wakeup_Completion : public ACE_POSIX_Asynch_Result
{
  // = TITLE
  //     This is result object is used by the <end_event_loop> of the 
  //     ACE_Proactor interface to wake up all the threads blocking
  //     for completions.
public:
  ACE_POSIX_Wakeup_Completion (ACE_Handler &handler,
                               const void *act = 0,
                               ACE_HANDLE event = ACE_INVALID_HANDLE,
                               int priority = 0,
                               int signal_number = ACE_SIGRTMIN);
  // Constructor.
  
  virtual ~ACE_POSIX_Wakeup_Completion (void);
  // Destructor.
  
  
  virtual void complete (u_long bytes_transferred = 0,
                         int success = 1,
                         const void *completion_key = 0,
                         u_long error = 0);
  // This method calls the <handler>'s <handle_wakeup> method.
};

// *********************************************************************

ACE_POSIX_Proactor::~ACE_POSIX_Proactor (void)
{
  this->close ();
}

int
ACE_POSIX_Proactor::close (void)
{
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
                                                      int priority,
                                                      int signal_number)
{
  ACE_Asynch_Read_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Read_Stream_Result (handler,
                                                       handle,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       event,
                                                       priority,
                                                       signal_number),
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
                                                       int priority,
                                                       int signal_number)
{
  ACE_Asynch_Write_Stream_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Write_Stream_Result (handler,
                                                        handle,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        event,
                                                        priority,
                                                        signal_number),
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
                                                    int priority,
                                                    int signal_number)
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
                                                     priority,
                                                     signal_number),
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
                                                     int priority,
                                                     int signal_number)
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
                                                      priority,
                                                      signal_number),
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
                                                 int priority,
                                                 int signal_number)
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
                                                  priority,
                                                  signal_number),
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
                                                        int priority,
                                                        int signal_number)
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
                                                         priority,
                                                         signal_number),
                  0);
  return implementation;
}

ACE_Asynch_Result_Impl *
ACE_POSIX_Proactor::create_asynch_timer (ACE_Handler &handler,
                                         const void *act,
                                         const ACE_Time_Value &tv,
                                         ACE_HANDLE event,
                                         int priority,
                                         int signal_number)
{
  ACE_Asynch_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Timer (handler,
                                          act,
                                          tv,
                                          event,
                                          priority,
                                          signal_number),
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

  for (;;)
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
                                               const void */* completion_key*/,
                                               u_long error)
{
  ACE_SEH_TRY
    {
      // Call completion hook
      asynch_result->complete (bytes_transferred,
                               success,
                               0, // No completion key.
                               error);
    }
  ACE_SEH_FINALLY
    {
      // This is crucial to prevent memory leaks
      delete asynch_result;
    }
}

int
ACE_POSIX_Proactor::post_wakeup_completions (int how_many)
{
  ACE_POSIX_Wakeup_Completion *wakeup_completion = 0;

  for (ssize_t ci = 0; ci < how_many; ci++)
    {
      ACE_NEW_RETURN (wakeup_completion,
                      ACE_POSIX_Wakeup_Completion (this->wakeup_handler_),
                      -1);
      
      if (wakeup_completion->post_completion (this) == -1)
        return -1;
    }
  
  return 0;
}  

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
  //     <handle_read_stream> of this class. This class calls
  //     <complete> on the <POSIX_Asynch_Result *> and thus calls the
  //     application handler.
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
                "ACE_AIOCB_Notify_Pipe_Manager::ACE_AIOCB_Notify_Pipe_Manager:"
                "Open on Read Stream failed"));

  // Issue an asynch_read on the read_stream of the notify pipe.
  if (this->read_stream_.read (this->message_block_,
                               sizeof (ACE_POSIX_Asynch_Result *),
                               0, // ACT
                               0) // Priority
      == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:%p\n",
                "ACE_AIOCB_Notify_Pipe_Manager::ACE_AIOCB_Notify_Pipe_Manager:"
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
                              ACE_reinterpret_cast (char *,
                                                    &result),
                              sizeof (result));
  if (return_val != sizeof (result))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P %t):%p\n",
                       "ACE_AIOCB_Notify_Pipe_Manager::notify"
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
                "%N:%l:(%P | %t):%p\n",
                "ACE_AIOCB_Notify_Pipe_Manager::handle_read_stream:"
                "Read from pipe failed"));
}

// Public constructor for common use.
ACE_POSIX_AIOCB_Proactor::ACE_POSIX_AIOCB_Proactor (size_t max_aio_operations)
  : aiocb_notify_pipe_manager_ (0),
    aiocb_list_ (0),
    result_list_ (0),
    aiocb_list_max_size_ (max_aio_operations),
    aiocb_list_cur_size_ (0)
{
  if (aiocb_list_max_size_ > 8192)  
    // @@ Alex, this shouldn't be a magic number -- it should be a
    // constant, e.g., ACE_AIO_MAX_SIZE or something.
    aiocb_list_max_size_ = 8192;

  ACE_NEW (aiocb_list_, 
           aiocb *[aiocb_list_max_size_]);
  ACE_NEW (result_list_,
           ACE_POSIX_Asynch_Result *[aiocb_list_max_size_]);

  // Initialize the array.
  for (size_t ai = 0; ai < this->aiocb_list_max_size_; ai++)
    {
      aiocb_list_[ai] = 0;
      result_list_[ai] = 0;
    }

  create_notify_manager ();
}

// Special protected constructor for ACE_SUN_Proactor
ACE_POSIX_AIOCB_Proactor::ACE_POSIX_AIOCB_Proactor (size_t max_aio_operations,int Flg)
  : aiocb_notify_pipe_manager_ (0),
    aiocb_list_ (0),
    result_list_ (0),
    aiocb_list_max_size_ (max_aio_operations),
    aiocb_list_cur_size_ (0)
{
  ACE_UNUSED_ARG (Flg);

  if (aiocb_list_max_size_ > 8192)  
    // @@ Alex, this shouldn't be a magic number -- it should be a
    // constant, e.g., ACE_AIO_MAX_SIZE or something.
    aiocb_list_max_size_ = 8192;

  ACE_NEW (aiocb_list_, 
           aiocb *[aiocb_list_max_size_]);
  ACE_NEW (result_list_,
           ACE_POSIX_Asynch_Result *[aiocb_list_max_size_]);

  // Initialize the array.
  for (size_t ai = 0; ai < this->aiocb_list_max_size_; ai++)
    {
      aiocb_list_[ai] = 0;
      result_list_[ai] = 0;
    }

  // @@ We should create Notify_Pipe_Manager in the derived class to
  // provide correct calls for virtual functions !!!
}

// Destructor.
ACE_POSIX_AIOCB_Proactor::~ACE_POSIX_AIOCB_Proactor (void)
{
  delete_notify_manager ();

  delete [] aiocb_list_;
  aiocb_list_ = 0;
	
  delete [] result_list_;
  result_list_ = 0;
}

void  
ACE_POSIX_AIOCB_Proactor::create_notify_manager (void)
{
  // Accept Handler for aio_accept. Remember! this issues a Asynch_Read
  // on the notify pipe for doing the Asynch_Accept.

  if (aiocb_notify_pipe_manager_ == 0)
    ACE_NEW (aiocb_notify_pipe_manager_,
             ACE_AIOCB_Notify_Pipe_Manager (this));
}

void  
ACE_POSIX_AIOCB_Proactor::delete_notify_manager (void)
{
  // We are responsible for delete as all pointers set to 0 after
  // delete, it is save to delete twice

  delete aiocb_notify_pipe_manager_;
  aiocb_notify_pipe_manager_ = 0;
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
  // Notify to the completion queue.
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
  int result_suspend = 0;

  if (milli_seconds == ACE_INFINITE)
    // Indefinite blocking.
    result_suspend = aio_suspend (aiocb_list_,
                                  aiocb_list_max_size_,
                                  0);
  else
    {
      // Block on <aio_suspend> for <milli_seconds>
      timespec timeout;
      timeout.tv_sec = milli_seconds / 1000;
      timeout.tv_nsec = (milli_seconds - (timeout.tv_sec * 1000)) * 1000;
      result_suspend = aio_suspend (aiocb_list_,
                                    aiocb_list_max_size_,
                                    &timeout);
    }

  // Check for errors
  if (result_suspend == -1)
    {
      if (errno == EAGAIN)  // Timeout
        return 0;

      ACE_ERROR_RETURN ((LM_ERROR,
                         "%N:%l:(%P | %t)::%p\n",
                         "ACE_POSIX_AIOCB_Proactor::handle_events:"
                         "aio_suspend failed\n"),
                        0);  // let continue work
    }

  int error_status = 0;
  int return_status = 0;

  ACE_POSIX_Asynch_Result *asynch_result =
    find_completed_aio (error_status, return_status);

  if (asynch_result == 0)
    return 0;

  // Call the application code.
  this->application_specific_code (asynch_result,
                                   return_status, // Bytes transferred.
                                   1,             // Success
                                   0,             // No completion key.
                                   error_status); // Error
  return 1;    
}

ACE_POSIX_Asynch_Result *
ACE_POSIX_AIOCB_Proactor::find_completed_aio (int &error_status,
                                               int &return_status)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, 0);

  size_t ai;
  ACE_POSIX_Asynch_Result *asynch_result = 0;

  error_status = 0;
  return_status= 0;
	
  for (ai = 0; ai < aiocb_list_max_size_; ai++)
    {
      if (aiocb_list_[ai] == 0) // Dont process null blocks.
        continue;

      // Get the error status of the aio_ operation.
      error_status = aio_error (aiocb_list_[ai]);

      if (error_status == -1)   // <aio_error> itself has failed.
        {
          ACE_ERROR ((LM_ERROR,
                      "%N:%l:(%P | %t)::%p\n",
                      "ACE_POSIX_AIOCB_Proactor::find_completed_aio:"
                      "<aio_error> has failed\n"));

          // skip this operation  
          aiocb_list_[ai] = 0;
          result_list_[ai] = 0;
          aiocb_list_cur_size_--;
		
          continue;
        }

      // Continue the loop if <aio_> operation is still in progress.
      if (error_status != EINPROGRESS)
        break;

    } // end for

  if (ai >= this->aiocb_list_max_size_) // all processed
    return asynch_result;
  else if (error_status == ECANCELED)
    return_status = 0;
  else
    return_status = aio_return (aiocb_list_[ai]);

  if (return_status == -1) 
    {
      // was ACE_ERROR_RETURN
      ACE_ERROR ((LM_ERROR,   
                  "%N:%l:(%P | %t)::%p\n",
                  "ACE_POSIX_AIOCB_Proactor::find_completed_aio:"
                  "<aio_return> failed\n"));
      return_status = 0; // zero bytes transferred
    }

  asynch_result = result_list_[ai];

  aiocb_list_[ai] = 0;
  result_list_[ai] = 0;
  aiocb_list_cur_size_--;

  return asynch_result;
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
ACE_POSIX_AIOCB_Proactor::register_and_start_aio 
  (ACE_POSIX_Asynch_Result *result, int op)
{
  ACE_TRACE ("ACE_POSIX_AIOCB_Proactor::register_and_start_aio");

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, mutex_, -1);

  int ret_val = (aiocb_list_cur_size_ >= aiocb_list_max_size_) ? -1 : 0;

  if (result == 0) // Just check the status of the list
    return ret_val;

  // Non-zero ptr.  Find a free slot and store.
  if (ret_val == 0)
    {
      for (size_t i= 0; i < this->aiocb_list_max_size_; i++)
        if (aiocb_list_[i] == 0)  
          {
            ret_val = start_aio (result, op);

            if (ret_val == 0)   // Store the pointers.
              {
                aiocb_list_[i] = result; 
                result_list_[i] = result;

                aiocb_list_cur_size_++;
              }
            return ret_val;
          }

      errno = EAGAIN;
      ret_val = -1;
    }
				 
  ACE_ERROR ((LM_ERROR,
              "%N:%l:(%P | %t)::\n"
              "register_and_start_aio: No space to store the <aio>info\n"));
  return ret_val;
}

int
ACE_POSIX_AIOCB_Proactor::start_aio (ACE_POSIX_Asynch_Result *result, int op)
{
  ACE_TRACE ("ACE_POSIX_AIOCB_Proactor::start_aio");

  int ret_val;
  const ACE_TCHAR *ptype;

  // Start IO

  switch (op)
    {
    case 0:
      ptype = "read ";
      ret_val = aio_read (result);
      break;
    case 1:
      ptype = "write";
      ret_val = aio_write (result);
      break;
    default:
      ptype = "?????";
      ret_val = -1;
      break;
    }
	
  if (ret_val == -1)
    ACE_ERROR ((LM_ERROR,
                "%N:%l:(%P | %t)::start_aio: aio_%s %p\n",
                ptype,
                "queueing failed\n"));

  return ret_val;
}

// *********************************************************************

ACE_POSIX_SIG_Proactor::ACE_POSIX_SIG_Proactor (void)
{
  // = Mask all the signals, keep a mask set with ACE_SIGRTMIN and set
  //   up signal handler for SIGRTMIN.

  // Mask all the signals.
  if (this->mask_all () != 0)
    return;

  // = Keep a mask set with ACE_SIGRTMIN.

  // Clear the signal set.
  if (sigemptyset (&this->RT_completion_signals_) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error:%p\n",
                "Couldn't init the RT completion signal set"));

  // Add the signal number to the signal set.
  if (sigaddset (&this->RT_completion_signals_, ACE_SIGRTMIN) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error:%p\n",
                "Couldnt init the RT completion signal set"));

  // Set up the signal handler for SIGRTMIN.
  setup_signal_handler (ACE_SIGRTMIN);
}

ACE_POSIX_SIG_Proactor::ACE_POSIX_SIG_Proactor (const sigset_t signal_set)
{
  // = Keep <Signal_set> with the Proactor, mask all the signals and
  //   setup signal handlers for the signals in the <signal_set>.

  // = Keep <signal_set> with the Proactor.

  // Empty the signal set first.
  if (sigemptyset (&this->RT_completion_signals_) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "sigemptyset failed"));

  //  Put the <signal_set>.
  if (ACE_OS::pthread_sigmask (SIG_SETMASK, &signal_set, 0) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "pthread_sigmask failed"));

  // Get the <signal_set> back from the OS.
  if (ACE_OS::pthread_sigmask (SIG_SETMASK, 0, &this->RT_completion_signals_) != 0)
    ACE_ERROR ((LM_ERROR,
                "Error:(%P | %t):%p\n",
                "ACE_OS::pthread_sigmask failed"));


  // Mask all the signals.
  if (this->mask_all () != 0)
    return;

  // For each signal number present in the <signal_set>, set up the
  // signal handler.
  int member = 0;
  for (int si = ACE_SIGRTMIN; si <= ACE_SIGRTMAX; si++)
    {
      member = sigismember (&signal_set,
                            si);
      if (member == -1)
        ACE_ERROR ((LM_ERROR,
                    "%N:%l:(%P | %t)::%p\n",
                    "ACE_POSIX_SIG_Proactor::ACE_POSIX_SIG_Proactor:"
                    "sigismember failed"));
      else if (member == 1)
        {
          if (this->setup_signal_handler (si) == -1)
            return;
        }
    }
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
                       "Error:%N:%l(%P | %t):%p",
                       "<getpid> failed"),
                      -1);

  // Set the signal information.
  sigval value;
  value.sival_ptr = ACE_reinterpret_cast (void *,
                                          result);

  // Queue the signal.
  if (sigqueue (pid, result->signal_number (), value) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%N:%l:(%P | %t):%p\n",
                       "<sigqueue> failed"),
                      -1);
  return 0;
}

ACE_Asynch_Read_Stream_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_read_stream (void)
{
  ACE_Asynch_Read_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Read_Stream (this),
                  0);
  return  implementation;
}

ACE_Asynch_Write_Stream_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_write_stream (void)
{
  ACE_Asynch_Write_Stream_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Write_Stream (this),
                  0);
  return  implementation;
}

ACE_Asynch_Read_File_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_read_file (void)
{
  ACE_Asynch_Read_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Read_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Write_File_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_write_file (void)
{
  ACE_Asynch_Write_File_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Write_File (this),
                  0);
  return  implementation;
}

ACE_Asynch_Accept_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_accept (void)
{
  ACE_Asynch_Accept_Impl *implementation = 0;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_SIG_Asynch_Accept (this),
                  0);
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

ACE_Asynch_Result_Impl *
ACE_POSIX_SIG_Proactor::create_asynch_timer (ACE_Handler &handler,
                                             const void *act,
                                             const ACE_Time_Value &tv,
                                             ACE_HANDLE event,
                                             int priority,
                                             int signal_number)
{
  int is_member = 0;

  // Fix the signal number.
  if (signal_number == -1)
    {
      int si;
      for (si = ACE_SIGRTMAX;
           (is_member == 0) && (si >= ACE_SIGRTMIN);
           si--)
        {
          is_member = sigismember (&this->RT_completion_signals_,
                                   si);
          if (is_member == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%N:%l:(%P | %t)::%s\n",
                               "ACE_POSIX_SIG_Proactor::create_asynch_timer:"
                               "sigismember failed"),
                              0);
        }

      if (is_member == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%N:%l:(%P | %t)::%s\n",
                           "ACE_POSIX_SIG_Proactor::ACE_POSIX_SIG_Proactor:"
                           "Signal mask set empty"),
                          0);
      else
        // + 1 to nullify loop increment.
        signal_number = si + 1;
    }

  ACE_Asynch_Result_Impl *implementation;
  ACE_NEW_RETURN (implementation,
                  ACE_POSIX_Asynch_Timer (handler,
                                          act,
                                          tv,
                                          event,
                                          priority,
                                          signal_number),
                  0);
  return implementation;
}

int
ACE_POSIX_SIG_Proactor::setup_signal_handler (int signal_number) const
{
  // Set up the handler(actually Null handler) for this real-time
  // signal.
  struct sigaction reaction;
  sigemptyset (&reaction.sa_mask);   // Nothing else to mask.
  reaction.sa_flags = SA_SIGINFO;    // Realtime flag.
#if defined (SA_SIGACTION)
  // Lynx says, it is better to set this bit, to be portable.
  reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */
  // Null handler function.
  reaction.sa_sigaction =
    ACE_SIGNAL_C_FUNC (&ACE_POSIX_SIG_Proactor::null_handler);
  int sigaction_return = ACE_OS::sigaction (signal_number,
                                            &reaction,
                                            0);
  if (sigaction_return == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Proactor couldnt do sigaction for the RT SIGNAL"),
                      -1);
  return 0;
}

void
ACE_POSIX_SIG_Proactor::null_handler (int signal_number,
                                      siginfo_t * /* info */,
                                      void *      /* context */)
{
  ACE_ERROR ((LM_ERROR,
              "Error:(%P | %t):ACE_POSIX_SIG_Proactor::null_handler called,"
              "Signal number %d,"
              "Mask all the RT signals for this thread\n",
              signal_number));
}

int
ACE_POSIX_SIG_Proactor::mask_all (void) const
{
  sigset_t full_set;

  // Get full set.
  if (sigfillset (&full_set) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "sigfillset failed"),
                      -1);

  // Mask them.
  if (ACE_OS::pthread_sigmask (SIG_SETMASK, &full_set, 0) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "pthread_sigmask failed"),
                      -1);

  return 0;
}

int
ACE_POSIX_SIG_Proactor::handle_events (unsigned long milli_seconds)
{
  int result_sigwait = 0;
  siginfo_t sig_info;

  // Mask all the signals.
  if (this->mask_all () != 0)
    return -1;

  // Wait for the signals.
  if (milli_seconds == ACE_INFINITE)
    {
      result_sigwait = sigwaitinfo (&this->RT_completion_signals_,
                                    &sig_info);
    }
  else
    {
      // Wait for <milli_seconds> amount of time.
      timespec timeout;
      timeout.tv_sec = milli_seconds / 1000;
      timeout.tv_nsec = (milli_seconds - (timeout.tv_sec * 1000)) * 1000;
      result_sigwait = sigtimedwait (&this->RT_completion_signals_,
                                     &sig_info,
                                     &timeout);
    }

  // Check for errors
  if (result_sigwait == -1)
    {
      // If failure is coz of timeout, then return *0* but set errno
      // appropriately.
      if (errno == EAGAIN)
        return 0;
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:(%P | %t)::%p\n",
                           "ACE_POSIX_SIG_Proactor::handle_events:"
                           "sigtimedwait/sigwaitinfo failed"),
                          -1);
    }

  // No errors, RT compleion signal is received.

  // Is the signo returned consistent with the sig info?
  if (sig_info.si_signo != result_sigwait)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%N:%l:(%P | %t):"
                       "ACE_POSIX_SIG_Proactor::handle_events:"
                       "Inconsistent signal number (%d) in the signal info block",
                       sig_info.si_signo),
                      -1);

  // Retrive the result pointer.
  ACE_POSIX_Asynch_Result *asynch_result = ACE_reinterpret_cast (ACE_POSIX_Asynch_Result *,
                                                                 sig_info.si_value.sival_ptr);

  // Check the <signal code> and act according to that.
  if (sig_info.si_code == SI_ASYNCIO)
    {
      // Analyze error and return values.
      
      int error_status = 0;
      int return_status = 0;
      
      // Check the error status
      error_status = aio_error (asynch_result);

      // <aio_error> itself has failed.
      if (error_status == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:(%P | %t)::%p\n",
                           "ACE_POSIX_SIG_Proactor::handle_events:"
                           "<aio_error> has failed"),
                          -1);

      // Completion signal has been received, so it can't be in
      // progress.
      if (error_status == EINPROGRESS)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:(%P | %t)::%p\n",
                           "ACE_POSIX_SIG_Proactor::handle_events:"
                           "Internal error: AIO in progress. "
                           "But completion signal was received"),
                          -1);

      // Handle cancel'ed asynchronous operation. We dont have to call
      // <aio_return> in this case, since return_status is going to be
      // -1. We will pass 0 for the <bytes_transferred> in this case
      if (error_status == ECANCELED)
        {
          return_status = 0;
        }
      else 
        {
          // Get the return_status of the <aio_> operation.
          return_status = aio_return (asynch_result);

          // Failure.
          if (return_status == -1)
            {						
              ACE_ERROR ((LM_ERROR,
                          "%N:%l:(%P | %t)::%p\n",
                          "ACE_POSIX_SIG_Proactor::handle_events:"
                          "<aio_return> failed"));
              return_status = 0; // zero bytes transferred
            }

        }

      // Error status and return status are obtained. Dispatch the
      // completion.
      this->application_specific_code (asynch_result,
                                       return_status,
                                       1,             // Result : True.
                                       0,             // No completion key.
                                       error_status); // Error.
    }
  else if (sig_info.si_code == SI_QUEUE)
    {
      this->application_specific_code (asynch_result,
                                       0,  // No bytes transferred.
                                       1,  // Result : True.
                                       0,  // No completion key.
                                       0); // No error.
    }
  else
    // Unknown signal code.
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%N:%l:(%P | %t):",
                       "ACE_POSIX_SIG_Proactor::handle_events:\n"
                       "Unexpected signal code (%d) returned on completion querying\n",
                       sig_info.si_code),
                      -1);

  // Success
  return 1;
}

// *********************************************************************

ACE_POSIX_Asynch_Timer::ACE_POSIX_Asynch_Timer (ACE_Handler &handler,
                                                const void *act,
                                                const ACE_Time_Value &tv,
                                                ACE_HANDLE event,
                                                int priority,
                                                int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    time_ (tv)
{
}

void
ACE_POSIX_Asynch_Timer::complete (u_long       /* bytes_transferred */,
                                  int          /* success */,
                                  const void * /* completion_key */,
                                  u_long       /* error */)
{
  this->handler_.handle_time_out (this->time_, this->act ());
}

// *********************************************************************

ACE_POSIX_Wakeup_Completion::ACE_POSIX_Wakeup_Completion (ACE_Handler &handler,
                                                          const void *act,
                                                          ACE_HANDLE event,
                                                          int priority,
                                                          int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, 0, 0, priority, signal_number)
{
}

ACE_POSIX_Wakeup_Completion::~ACE_POSIX_Wakeup_Completion (void)
{
}

void
ACE_POSIX_Wakeup_Completion::complete (u_long       /* bytes_transferred */,
                                       int          /* success */,
                                       const void * /* completion_key */,
                                       u_long       /*  error */)
{
  this->handler_.handle_wakeup ();
}

#endif /* ACE_HAS_AIO_CALLS */
