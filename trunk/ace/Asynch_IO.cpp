// $Id$

#define ACE_BUILD_DLL
#include "ace/Asynch_IO.h"

ACE_RCSID(ace, Asynch_IO, "$Id$")

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))
// This only works on Win32 platforms

#include "ace/Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/Task_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_IO.i"
#endif /* __ACE_INLINE__ */

ACE_Asynch_Result::ACE_Asynch_Result (ACE_Handler &handler,
                                      const void* act,
                                      ACE_HANDLE event,
                                      u_long offset,
                                      u_long offset_high)
  : handler_ (handler),
    act_ (act),
    bytes_transferred_ (0),
    success_ (0),
    completion_key_ (0),
    error_ (0)
{
  // Set the ACE_OVERLAPPED structure
  this->Internal = 0;
  this->InternalHigh = 0;
  this->Offset = offset;
  this->OffsetHigh = offset_high;
  this->hEvent = event;

#if defined (ACE_HAS_AIO_CALLS)
  ACE_NEW (this->aiocb_ptr_, aiocb);
#endif /* ACE_HAS_AIO_CALLS */
}

ACE_Asynch_Result::~ACE_Asynch_Result (void)
{
}

u_long
ACE_Asynch_Result::bytes_transferred (void) const
{
  return this->bytes_transferred_;
}

const void *
ACE_Asynch_Result::act (void) const
{
  return this->act_;
}

int
ACE_Asynch_Result::success (void) const
{
  return this->success_;
}

const void *
ACE_Asynch_Result::completion_key (void) const
{
  return this->completion_key_;
}

u_long
ACE_Asynch_Result::error (void) const
{
  return this->error_;
}

u_long
ACE_Asynch_Result::offset (void) const
{
  return this->Offset;
}

u_long
ACE_Asynch_Result::offset_high (void) const
{
  return this->OffsetHigh;
}

ACE_HANDLE
ACE_Asynch_Result::event (void) const
{
  return this->hEvent;
}

#if defined (ACE_HAS_AIO_CALLS)
aiocb *
ACE_Asynch_Result::aiocb_ptr (void)
{
  return aiocb_ptr_;
}
#endif /* ACE_HAS_AIO_CALLS */

ACE_Asynch_Operation::ACE_Asynch_Operation (void)
  : handler_ (0),
    handle_ (ACE_INVALID_HANDLE)
{
}

int
ACE_Asynch_Operation::open (ACE_Handler &handler,
                            ACE_HANDLE handle,
                            const void *completion_key,
                            ACE_Proactor *proactor)
{
  this->proactor_ = proactor;
  this->handler_ = &handler;
  this->handle_ = handle;

  // Grab the handle from the <handler> if <handle> is invalid
  if (this->handle_ == ACE_INVALID_HANDLE)
    this->handle_ = this->handler_->handle ();
  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  // If no proactor was passed
  if (this->proactor_ == 0)
    {
      // Grab the proactor from the <Service_Config> if
      // <handler->proactor> is zero
      this->proactor_ = this->handler_->proactor ();
      if (this->proactor_ == 0)
        this->proactor_ = ACE_Proactor::instance();
    }

#if defined (ACE_HAS_AIO_CALLS)

  // AIO stuff is present. So no registering.
  // @@ But something has to be done to associate completion key with
  // the handle provided. How about a HashTable of size "the number of
  // file descriptors that are possible in the system".
  ACE_UNUSED_ARG (completion_key);
  return 0;

#else /* ACE_HAS_AIO_CALLS */

  // Register with the <proactor>.
  return this->proactor_->register_handle (this->handle_,
                                           completion_key);
#endif /* ACE_HAS_AIO_CALLS */
}

int
ACE_Asynch_Operation::cancel (void)
{
#if defined (ACE_HAS_AIO_CALLS)

  // @@ <aio_cancel> will come here soon.
  return 0;

#elif (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && (defined (_MSC_VER) && (_MSC_VER > 1020))
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.

  return (int) ::CancelIo (this->handle_);

#else /* Not ACE_HAS_WINNT4 && ACE_HAS_WINNT4!=0 && _MSC... */
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_AIO_CALLS */
}

#if defined (ACE_HAS_AIO_CALLS)
// If the ptr is o, just check whether there is any slot free and
// return 0 if yes, else return -1. If a valid ptr is passed, keep it
// in a free slot.
int
ACE_Asynch_Operation::register_aio_with_proactor (aiocb *aiocb_ptr)
{
  if (aiocb_ptr == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Status check max %d cur %d\n",
                  this->proactor_->aiocb_list_max_size_,
                  this->proactor_->aiocb_list_cur_size_));

      // Just check the status of the list.
      if (this->proactor_->aiocb_list_cur_size_ >=
          this->proactor_->aiocb_list_max_size_)
        return -1;
      else
        return 0;
    }

  // Non-zero ptr. Find a free slot and store.

  // Make sure again.
  if (this->proactor_->aiocb_list_cur_size_ >=
      this->proactor_->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Slot(s) available. Find a free one.
  size_t ai;
  for (ai = 0;
       ai < this->proactor_->aiocb_list_max_size_;
       ai++)
    if (this->proactor_->aiocb_list_[ai] == 0)
      break;

  // Sanity check.
  if (ai == this->proactor_->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Store the pointers.
  this->proactor_->aiocb_list_[ai] = aiocb_ptr;
  this->proactor_->aiocb_list_cur_size_ ++;

  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

ACE_Proactor *
ACE_Asynch_Operation::proactor (void)
{
  return this->proactor_;
}

// ************************************************************

ACE_Asynch_Read_Stream::ACE_Asynch_Read_Stream (void)
{
}

int
ACE_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                              u_long bytes_to_read,
                              const void *act)
{
  // Create the Asynch_Result.
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          message_block,
                          bytes_to_read,
                          act,
                          this->proactor_->get_handle ()),
                  -1);

  ssize_t return_val = this->shared_read (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

int
ACE_Asynch_Read_Stream::shared_read (ACE_Asynch_Read_Stream::Result *result)
{
#if defined (ACE_HAS_AIO_CALLS)
  // Act according to the completion strategy that is set in the
  // proactor.

  // Using RT Signals to notify the completion.
  if (this->proactor ()->posix_completion_strategy () ==
      ACE_Proactor::RT_SIGNALS)
    {
      // Setup AIOCB.
      // @@ Priority always 0?
      result->aiocb_ptr ()->aio_fildes = result->handle ();
      result->aiocb_ptr ()->aio_offset = result->Offset;
      result->aiocb_ptr ()->aio_buf = result->message_block ().wr_ptr ();
      result->aiocb_ptr ()->aio_nbytes = result->bytes_to_read ();
      result->aiocb_ptr ()->aio_reqprio = 0;
      result->aiocb_ptr ()->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
      result->aiocb_ptr ()->aio_sigevent.sigev_signo = ACE_SIG_AIO;
      result->aiocb_ptr ()->aio_sigevent.sigev_value.sival_ptr =
        (void *) result;

      // Fire off the aio write.
      if (aio_read (result->aiocb_ptr ()) == -1)
        // Queueing failed.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p\n",
                           "Asynch_Read_Stream: aio_read queueing failed"),
                          -1);
    }
  else
    {
      // AIO_CONTROL_BLOCKS strategy.

      // Make a new AIOCB and issue aio_read, if queueing is possible
      // store this with the Proactor, so that that can be used for
      // <aio_return> and <aio_error>.

      // Allocate aiocb block.
      aiocb *aiocb_ptr = 0;
      ACE_NEW_RETURN (aiocb_ptr,
                      aiocb,
                      -1);

      // Make sure there is space in the aiocb list.
      if (this->register_aio_with_proactor (0) == -1)
        {
          // Clean up the memory allocated.
          delete aiocb_ptr;

          // @@ Set errno to EAGAIN so that applications will know this as
          // a queueing failure and try again this aio_read it they want.
          errno = EAGAIN;

          return -1;
        }

      // Setup AIOCB.
      // @@ Priority always 0?
      // @@ Signal no, always?
      aiocb_ptr->aio_fildes = result->handle ();
      aiocb_ptr->aio_offset = result->Offset;
      aiocb_ptr->aio_buf = result->message_block ().wr_ptr ();
      aiocb_ptr->aio_nbytes = result->bytes_to_read ();
      aiocb_ptr->aio_reqprio = 0;
      aiocb_ptr->aio_sigevent.sigev_notify = SIGEV_NONE;
      //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
      aiocb_ptr->aio_sigevent.sigev_value.sival_ptr =
        (void *) result;

      // Fire off the aio write.
      if (aio_read (aiocb_ptr) == -1)
        {
          // Queueing failed.
          ACE_ERROR ((LM_ERROR,
                      "Error:%p\n",
                      "Asynch_Read_Stream: aio_read queueing failed"));

          // Clean up the memory allocated.
          delete aiocb_ptr;

          return -1;
        }

      // Success. Store the aiocb_ptr with Proactor.
      if (this->register_aio_with_proactor (aiocb_ptr) == -1)
        {
          // Clean up the memory allocated.
          delete aiocb_ptr;

          return -1;
        }
    }
  // Aio successfully issued and ptr stored.
  return 0;

#else /* ACE_HAS_AIO_CALLS */
  u_long bytes_read;

  // Initiate the read
  int initiate_result = ::ReadFile (result->handle (),
                                    result->message_block ().wr_ptr (),
                                    result->bytes_to_read (),
                                    &bytes_read,
                                    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ReadFile")), -1);
    }
#endif /* ACE_HAS_AIO_CALLS */
}

// ************************************************************

ACE_Asynch_Read_Stream::Result::Result (ACE_Handler &handler,
                                        ACE_HANDLE handle,
                                        ACE_Message_Block &message_block,
                                        u_long bytes_to_read,
                                        const void* act,
                                        ACE_HANDLE event)
  : ACE_Asynch_Result (handler, act, event),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    handle_ (handle)
{
}

u_long
ACE_Asynch_Read_Stream::Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_Asynch_Read_Stream::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_Asynch_Read_Stream::Result::handle (void) const
{
  return this->handle_;
}

void
ACE_Asynch_Read_Stream::Result::complete(u_long bytes_transferred,
                                          int success,
                                          const void *completion_key,
                                          u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_read_stream (*this);
}

// ************************************************************

ACE_Asynch_Write_Stream::ACE_Asynch_Write_Stream (void)
{
}

int
ACE_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                u_long bytes_to_write,
                                const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          message_block,
                          bytes_to_write,
                          act,
                          this->proactor_->get_handle ()),
                  -1);

  ssize_t return_val = this->shared_write (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

int
ACE_Asynch_Write_Stream::shared_write (ACE_Asynch_Write_Stream::Result *result)
{
#if defined (ACE_HAS_AIO_CALLS)
  // Act according to the completion strategy that is set in the
  // proactor.
  if (this->proactor ()->posix_completion_strategy () ==
      ACE_Proactor::RT_SIGNALS)
    {
      // Setup AIOCB.
      // @@ Priority always 0?
      result->aiocb_ptr ()->aio_fildes = result->handle ();
      result->aiocb_ptr ()->aio_offset = result->Offset;
      result->aiocb_ptr ()->aio_buf = result->message_block ().rd_ptr ();
      result->aiocb_ptr ()->aio_nbytes = result->bytes_to_write ();
      result->aiocb_ptr ()->aio_reqprio = 0;
      result->aiocb_ptr ()->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
      result->aiocb_ptr ()->aio_sigevent.sigev_signo = ACE_SIG_AIO;
      result->aiocb_ptr ()->aio_sigevent.sigev_value.sival_ptr =
        (void *) result;

      // Fire off the aio write.
      if (aio_write (result->aiocb_ptr ()) == -1)
        // Queueing failed.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p\n",
                           "Asynch_Write_Stream: aio_write queueing failed"),
                          -1);
    }
  else
    {
      // AIO_CONTROL_BLOCKS strategy.

      // Make a new AIOCB and issue aio_write, if queueing is possible
      // store this with the Proactor, so that that can be used for
      // <aio_return> and <aio_error>.

      // Allocate aiocb block.
      aiocb *aiocb_ptr = 0;
      ACE_NEW_RETURN  (aiocb_ptr,
                       aiocb,
                       -1);

      // Make sure there is space in the aiocb list.
      if (this->register_aio_with_proactor (0) == -1)
        {
          // Clean up the memory allocated.
          delete aiocb_ptr;

          // @@ Set errno to EAGAIN so that applications will know this as
          // a queueing failure and try again this aio_read it they want.
          errno = EAGAIN;

          return -1;
        }

      // Setup AIOCB.
      // @@ Priority always 0?
      // @@ Signal no, always?
      aiocb_ptr->aio_fildes = result->handle ();
      aiocb_ptr->aio_offset = result->Offset;
      aiocb_ptr->aio_buf = result->message_block ().rd_ptr ();
      aiocb_ptr->aio_nbytes = result->bytes_to_write ();
      aiocb_ptr->aio_reqprio = 0;
      aiocb_ptr->aio_sigevent.sigev_notify = SIGEV_NONE;
      //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
      aiocb_ptr->aio_sigevent.sigev_value.sival_ptr =
        (void *) result;

      // Fire off the aio write.
      if (aio_write (aiocb_ptr) == -1)
        {
          // Queueing failed.
          ACE_ERROR ((LM_ERROR,
                      "Error:%p\n",
                      "Asynch_Write_Stream: aio_write queueing failed"));

          // Clean up the memory allocated.
          delete aiocb_ptr;

          return -1;
        }

      // Success. Store the aiocb_ptr with Proactor.
      if (this->register_aio_with_proactor (aiocb_ptr) == -1)
        {
          // Clean up the memory allocated.
          delete aiocb_ptr;

          return -1;
        }
    }

  // Aio successfully issued.
  return 0;

#else /* ACE_HAS_AIO_CALLS */
  u_long bytes_written;

  // Initiate the write
  int initiate_result = ::WriteFile (result->handle (),
                                     result->message_block ().rd_ptr (),
                                     result->bytes_to_write (),
                                     &bytes_written,
                                     result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("WriteFile")), -1);
    }
#endif /* ACE_HAS_AIO_CALLS */
}

// ************************************************************

ACE_Asynch_Write_Stream::Result::Result (ACE_Handler &handler,
                                         ACE_HANDLE handle,
                                         ACE_Message_Block &message_block,
                                         u_long bytes_to_write,
                                         const void* act,
                                         ACE_HANDLE event)
  : ACE_Asynch_Result (handler, act, event),
    bytes_to_write_ (bytes_to_write),
    message_block_ (message_block),
    handle_ (handle)
{
}

u_long
ACE_Asynch_Write_Stream::Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

ACE_Message_Block &
ACE_Asynch_Write_Stream::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_Asynch_Write_Stream::Result::handle (void) const
{
  return this->handle_;
}

void
ACE_Asynch_Write_Stream::Result::complete (u_long bytes_transferred,
                                           int success,
                                           const void *completion_key,
                                           u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_write_stream (*this);
}

// ************************************************************

int
ACE_Asynch_Read_File::read (ACE_Message_Block &message_block,
                            u_long bytes_to_read,
                            u_long offset,
                            u_long offset_high,
                            const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          message_block,
                          bytes_to_read,
                          act,
                          offset,
                          offset_high,
                          this->proactor_->get_handle ()),
                  -1);

  return this->shared_read (result);
}

// ************************************************************

ACE_Asynch_Read_File::Result::Result (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      ACE_Message_Block &message_block,
                                      u_long bytes_to_read,
                                      const void* act,
                                      u_long offset,
                                      u_long offset_high,
                                      ACE_HANDLE event)
  : ACE_Asynch_Read_Stream::Result (handler, handle, message_block, bytes_to_read, act, event)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_Asynch_Read_File::Result::complete (u_long bytes_transferred,
                                        int success,
                                        const void *completion_key,
                                        u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion.
  this->handler_.handle_read_file (*this);
}

// ************************************************************

int
ACE_Asynch_Write_File::write (ACE_Message_Block &message_block,
                              u_long bytes_to_write,
                              u_long offset,
                              u_long offset_high,
                              const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          message_block,
                          bytes_to_write,
                          act,
                          offset,
                          offset_high,
                          this->proactor_->get_handle ()),
                  -1);

  return this->shared_write (result);
}

// ************************************************************

ACE_Asynch_Write_File::Result::Result (ACE_Handler &handler,
                                       ACE_HANDLE handle,
                                       ACE_Message_Block &message_block,
                                       u_long bytes_to_write,
                                       const void* act,
                                       u_long offset,
                                       u_long offset_high,
                                       ACE_HANDLE event)
  : ACE_Asynch_Write_Stream::Result (handler, handle, message_block, bytes_to_write, act, event)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_Asynch_Write_File::Result::complete (u_long bytes_transferred,
                                         int success,
                                         const void *completion_key,
                                         u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_write_file (*this);
}

// ************************************************************

#if defined (ACE_HAS_AIO_CALLS)
class ACE_Export ACE_Asynch_Accept_Handler : public ACE_Event_Handler
{
  // = TITLE
  //     For the POSIX implementation, this class takes care of doing
  //     Asynch_Accept.
  //
  // = DESCRIPTION
  //
public:
  ACE_Asynch_Accept_Handler (ACE_Reactor* reactor = 0,
                             ACE_Proactor* proactor = 0);
  // Constructor. Give the reactor so that it can activate/deactivate
  // the handlers. Give also the proactor used here, so that the
  // handler can send information through the notification pipe of the
  // proactor, in case AIO_CONTROL_BLOCKS strategy is used.

  ~ACE_Asynch_Accept_Handler (void);
  // Destructor.

  int register_accept_call (ACE_Asynch_Accept::Result* result);
  // Register this <accept> call with the local handler.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when accept event comes up on the <listen_handle>.

private:
  ACE_Asynch_Accept::Result* deregister_accept_call (void);
  // Undo the things done when registering.

  ACE_Unbounded_Queue<ACE_Asynch_Accept::Result*> result_queue_;
  // Queue of Result pointers that correspond to all the <accept>'s
  // pending.

  ACE_Reactor *reactor_;
  // Reactor used by the Asynch_Accept. We need this here to enable
  // and disable the <handle> now and then, depending on whether any
  // <accept> is pending or no.

  ACE_SYNCH_MUTEX lock_;
  // The lock to protect the  result queue which is shared. The queue
  // is updated by main thread in the register function call and
  // through the auxillary thread  in the deregister fun. So let us
  // mutex it.

  ACE_Proactor *proactor_;
  // Proactor used by the Asynch_Accept class.
};

ACE_Asynch_Accept_Handler::ACE_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                                      ACE_Proactor* proactor)
  : reactor_ (reactor),
    proactor_ (proactor)
{
}

ACE_Asynch_Accept_Handler::~ACE_Asynch_Accept_Handler (void)
{
}

int
ACE_Asynch_Accept_Handler::register_accept_call (ACE_Asynch_Accept::Result* result)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept_Handler::register_accept_call called\n"));

  // The queue is updated by main thread in the register function call
  // and thru the auxillary thread in the deregister fun. So let us
  // mutex it.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  // Insert this result to the queue.
  int insert_result = this->result_queue_.enqueue_tail (result);
  if (insert_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:ACE_Asynch_Accept_Handler::register_accept_call failed\n"),
                      -1);

  // If this is the only item, then it means there the set was empty
  // before. So enable the <handle> in the reactor.
  if (this->result_queue_.size () == 1)
    {
      int return_val = this->reactor_->resume_handler (result->listen_handle ());
      if (return_val != 0)
        return return_val;
    }
  
  return 0;
}

ACE_Asynch_Accept::Result *
ACE_Asynch_Accept_Handler::deregister_accept_call (void)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept_Handler::deregister_accept_call\n"));
  
  // The queue is updated by main thread in the register function call and
  // thru the auxillary thread  in the deregister fun. So let us mutex
  // it.
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  // Get the first item (result ptr) from the Queue.
  ACE_Asynch_Accept::Result* result = 0;
  int return_dequeue = this->result_queue_.dequeue_head (result);
  if (return_dequeue == -1)
    return 0;
  
  ACE_ASSERT (result != 0);

  // Disable the <handle> in the reactor if no <accept>'s are pending.
  if (this->result_queue_.size () == 0)
    {
      int return_val = this->reactor_->suspend_handler (result->listen_handle ()); 
      if (return_val != 0)
        return 0;
    }

  // Return the result pointer.
  return result;
}

int
ACE_Asynch_Accept_Handler::handle_input (ACE_HANDLE fd)
{
  // An <accept> has been sensed on the <listen_handle>. We should be
  // able to just go ahead and do the <accept> now on this <fd>. This
  // should be the same as the <listen_handle>.

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept_Handler::handle_input called\n"));

  // Deregister this info pertaining to this <accept> call.
  ACE_Asynch_Accept::Result* result = this->deregister_accept_call ();

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG,
              "(%t):ACE_Asynch_Accept_Handler::handle_input : fd = [%d], Result->listen_handle = [%d]\n",
              fd,
              result->listen_handle ()));

  // Issue <accept> now.
  // @@ We shouldnt block here since we have already done poll/select
  // thru reactor. But are we sure?
  ACE_HANDLE new_handle = ACE_OS::accept (result->listen_handle (), 0, 0);
  if (new_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "<accept> system call failed"),
                      -1);

  // Accept has completed.

  // Store the new handle.
  result->accept_handle_ = new_handle;

  // Notify the mail process about this completion
  // Signal the main process about this completion or send the result
  // pointer thru the notify pipe depending on what is Completion
  // Notification Strategy.
  switch (this->proactor_->posix_completion_strategy ())
    {
    case ACE_Proactor::RT_SIGNALS:
      // Queue a signal to the proactor.
      {
        // @@ Debugging.
        ACE_DEBUG ((LM_DEBUG,
                    "ACE_Asynch_Accept_Handler::handle_input: RT_SIGNALS\n"));

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
      }
      break;

    case ACE_Proactor::AIO_CONTROL_BLOCKS:
      // @@ Debugging.
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_Asynch_Accept_Handler::handle_input: AIO_CONTROL_BLOCKS\n"));

      // Send the Result through the notification pipe.
      if (this->proactor_->notify_asynch_accept (result) == -1)
        return -1;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%N:%l:Unknow POSIX_COMPLETION_STRATEGY\n"),
                        -1);
    }

  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

#if defined (ACE_HAS_AIO_CALLS)
// We need accept handlers here.
ACE_Asynch_Accept::ACE_Asynch_Accept (void)
  : accept_handler_ (0)
{
}
#else /* Not ACE_HAS_AIO_CALLS */
ACE_Asynch_Accept::ACE_Asynch_Accept (void)
{
}
#endif /* ACE_HAS_AIO_CALLS */

#if defined (ACE_HAS_AIO_CALLS)
int
ACE_Asynch_Accept::open (ACE_Handler &handler,
                         ACE_HANDLE handle,
                         const void *completion_key,
                         ACE_Proactor *proactor)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept::open called\n"));

  // Common things to register for any Asynch Operation. We need to
  // call the base class' <open>  method.
  int result_open = this->ACE_Asynch_Operation::open (handler,
                                                      handle,
                                                      completion_key,
                                                      proactor);
  if (result_open < 0)
    return result_open;

  // Init the Asynch_Accept_Handler now. Only now it can be inited,
  // because it needs to keep Proactor also with it.
  ACE_NEW_RETURN (this->accept_handler_,
                  ACE_Asynch_Accept_Handler (&this->reactor_,
                                             this->proactor_),
                  -1);

  // Register the handle with the reactor.
  this->reactor_.register_handler (this->handle_,
                                   this->accept_handler_,
                                   ACE_Event_Handler::ACCEPT_MASK);

  // Suspend the <handle> now. Enable only when the <accept> is issued
  // by the application.
  this->reactor_.suspend_handler (this->handle_);

  // Spawn the thread. It is the only thread we are going to have. It
  // will do the <handle_events> on the reactor.
  ACE_Thread_Manager::instance ()->spawn (ACE_Asynch_Accept::thread_function,
                                          (void *)&this->reactor_);

  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

int
ACE_Asynch_Accept::accept (ACE_Message_Block &message_block,
                           u_long bytes_to_read,
                           ACE_HANDLE accept_handle,
                           const void *act)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) || (defined (ACE_HAS_AIO_CALLS))

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept::accept called\n"));

  // Common code for both WIN and POSIX.

  // Sanity check: make sure that enough space has been allocated by
  // the caller.
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("Buffer too small\n")), -1);

#if !defined (ACE_HAS_AIO_CALLS)
  // WIN Specific.

  int close_accept_handle = 0;
  // If the <accept_handle> is invalid, we will create a new socket.
  if (accept_handle == ACE_INVALID_HANDLE)
    {
      accept_handle = ACE_OS::socket (PF_INET,
                                      SOCK_STREAM,
                                      0);
      if (accept_handle == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_OS::socket")), -1);
      else
        // Remember to close the socket down if failures occur.
        close_accept_handle = 1;
    }
#endif /* Not ACE_HAS_AIO_CALLS */

  // Common code for both WIN and POSIX.
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          accept_handle,
                          message_block,
                          bytes_to_read,
                          act,
                          this->proactor_->get_handle ()),
                  -1);

#if defined (ACE_HAS_AIO_CALLS)
  // Code specific to the POSIX Implementation.

  // Register this <accept> call with the local handler.
  this->accept_handler_->register_accept_call (result);

  return 0;
#else /* Not ACE_HAS_AIO_CALLS */
  // Code specific to WIN platforms.

  u_long bytes_read;

  // Initiate the accept.
  int initiate_result = ::AcceptEx ((SOCKET) result->listen_handle (),
                                    (SOCKET) result->accept_handle (),
                                    result->message_block ().wr_ptr (),
                                    result->bytes_to_read (),
                                    address_size,
                                    address_size,
                                    &bytes_read,
                                    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (close_accept_handle == 1)
        // Close the newly created socket
        ACE_OS::closesocket (accept_handle);

      // Cleanup dynamically allocated Asynch_Result.
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ReadFile")), -1);
    }
#endif /* ACE_HAS_AIO_CALLS */

#else /* ACE_HAS_WINNT4 .......|| ACE_HAS_AIO_CALLS */
  ACE_NOTSUP_RETURN (-1);
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) || (defined (ACE_HAS_AIO_CALLS) */
}

#if defined (ACE_HAS_AIO_CALLS)
void*
ACE_Asynch_Accept::thread_function (void* arg_reactor)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept::thread_function called\n"));

  // Retrieve the reactor pointer from the argument.
  ACE_Reactor* reactor = (ACE_Reactor *) arg_reactor;
  if (reactor == 0)
    reactor = ACE_Reactor::instance ();

  // For this reactor, this thread is the owner.
  reactor->owner (ACE_OS::thr_self ());

  // Handle events.
  int result = 0;
  while (result != -1)
    {
      result = reactor->handle_events ();
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_Asynch_Accept::Thread_Function : handle_events : result = [%d]\n",
                  result));
    }

  ACE_DEBUG ((LM_DEBUG, "Exiting ACE_Asynch_Accept::thread_function \n"));
  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

// ************************************************************

u_long
ACE_Asynch_Accept::Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_Asynch_Accept::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_Asynch_Accept::Result::listen_handle (void) const
{
  return this->listen_handle_;
}

ACE_HANDLE
ACE_Asynch_Accept::Result::accept_handle (void) const
{
  return this->accept_handle_;
}

ACE_Asynch_Accept::Result::Result (ACE_Handler &handler,
                                   ACE_HANDLE listen_handle,
                                   ACE_HANDLE accept_handle,
                                   ACE_Message_Block &message_block,
                                   u_long bytes_to_read,
                                   const void* act,
                                   ACE_HANDLE event)
  : ACE_Asynch_Result (handler, act, event),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    listen_handle_ (listen_handle),
    accept_handle_ (accept_handle)
{
}

void
ACE_Asynch_Accept::Result::complete (u_long bytes_transferred,
                                     int success,
                                     const void *completion_key,
                                     u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion.
  this->handler_.handle_accept (*this);
}

// ************************************************************

#if defined (ACE_HAS_AIO_CALLS)
class ACE_Export ACE_Asynch_Transmit_Handler : public ACE_Handler
{
  // = TITLE
  //     Auxillary handler for doing <Asynch_Transmit_File> in
  //     Unix. <ACE_Asynch_Transmit_File> internally uses this.
  //
  // = DESCRIPTION
  //     This is a helper class for implementing
  //     <ACE_Asynch_Transmit_File> in Unix systems.
public:
  ACE_Asynch_Transmit_Handler (ACE_Asynch_Transmit_File::Result *result);
  // Constructor. Result pointer will have all the information to do
  // the file transmission (socket, file, application handler, bytes
  // to write).

  virtual ~ACE_Asynch_Transmit_Handler (void);
  // Destructor.

  int transmit (void);
  // Do the transmission. All the info to do the transmission is in
  // the <result> member.

protected:
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete.

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the file complete.
private:
  int initiate_read_file (void);
  // Issue asynch read from  the file.

  ACE_Asynch_Transmit_File::Result *result_;
  // The asynch result pointer made from the initial transmit file
  // request.
  
  ACE_Asynch_Read_File rf_;
  // To read from the file to be transmitted.

  ACE_Asynch_Write_Stream ws_;
  // Write stream to write the header, trailer and the data.

  ACE_Message_Block *mb_;
  // Message bloack used to do the transmission.

  enum ACT
  {
    HEADER_ACT  = 1,
    DATA_ACT    = 2,
    TRAILER_ACT = 3
  };

  ACT header_act_;
  ACT data_act_;
  ACT trailer_act_;
  // ACT to transmit header, data and trailer.

  size_t file_offset_;
  // Current offset of the file being transmitted.

  size_t file_size_;
  // Total size of the file.

  size_t bytes_transferred_;
  // Number of bytes transferred on the stream.
};

// Constructor.
ACE_Asynch_Transmit_Handler::ACE_Asynch_Transmit_Handler (ACE_Asynch_Transmit_File::Result *result)
  : result_ (result),
    mb_ (0),
    header_act_ (this->HEADER_ACT),
    data_act_ (this->DATA_ACT),
    trailer_act_ (this->TRAILER_ACT),
    file_offset_ (result->offset ()),
    file_size_ (0),
    bytes_transferred_ (0)
{
  // Allocate memory for the message block.
  ACE_NEW (this->mb_,
           ACE_Message_Block (this->result_->bytes_per_send ()
                              + 1));
  // Init the file size.
  file_size_ = ACE_OS::filesize (this->result_->file ());
}

// Destructor.
ACE_Asynch_Transmit_Handler::~ACE_Asynch_Transmit_Handler (void)
{
  delete result_;
  mb_->release ();
}

// Do the transmission.
// Initiate transmitting the header. When that completes
// handle_write_stream will be called, there start transmitting the file.
int
ACE_Asynch_Transmit_Handler::transmit (void)
{
  // Open Asynch_Read_File.
  if (this->rf_.open (*this,
                      this->result_->file ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:read_file open failed\n"),
                      -1);

  // Open Asynch_Write_Stream.
  if (this->ws_.open (*this,
                      this->result_->socket ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:write_stream open failed\n"),
                      -1);

  // Transmit the header.
  if (this->ws_.write (*this->result_->header_and_trailer ()->header (),
                       this->result_->header_and_trailer ()->header_bytes (),
                       (void *) &this->header_act_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Asynch_Transmit_Handler:transmitting header:write_stream failed\n"),
                      -1);
  return 0;
}

void
ACE_Asynch_Transmit_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  // Update bytes transferred so far.
  this->bytes_transferred_ += result.bytes_transferred ();

  // Check the success parameter.
  if (result.success () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Asynch_Transmit_File failed.\n"));
      
      // Check the success parameter.
      if (result.success () == 0)
        {
          // Failure.
          ACE_ERROR ((LM_ERROR,
                      "Asynch_Transmit_File failed.\n"));
          
          ACE_SEH_TRY
            {
              this->result_->complete (this->bytes_transferred_,
                                       0,      // Failure.
                                       0,      // @@ Completion key.
                                       0);     // @@ Error no.
            }
          ACE_SEH_FINALLY
            {
              // This is crucial to prevent memory leaks. This deletes
              // the result pointer also.
              delete this;
            }
        }
    }

  // Write stream successful.

  // Partial write to socket.
  int unsent_data = result.bytes_to_write () - result.bytes_transferred ();
  if (unsent_data != 0)
    {
      // Reset pointers.
      result.message_block ().rd_ptr (result.bytes_transferred ());

      // Duplicate the message block and retry remaining data
      if (this->ws_.write (*result.message_block ().duplicate (),
                           unsent_data,
                           result.act ()) == -1)
        {
          // @@ Handle this error.
          ACE_ERROR ((LM_ERROR,
                      "Asynch_Transmit_Handler:write_stream failed\n"));
          return;
        }

      // @@ Handling *partial write* to a socket.  Let us not continue
      // further before this write finishes. Because proceeding with
      // another read and then write might change the order of the
      // file transmission, because partial write to the stream is
      // always possible.
      return;
    }
  
  // Not a partial write. A full write.

  // Check ACT to see what was sent.
  ACT act = *(ACT *) result.act ();

  switch (act)
    {
    case TRAILER_ACT:
      // If it is the "trailer" that is just sent, then transmit file
      // is complete. 
      // Call the application handler.
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   1,      // @@ Success.
                                   0,      // @@ Completion key.
                                   0);     // @@ Errno.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      break;

    case HEADER_ACT:
    case DATA_ACT:
      // If header/data was sent, initiate the file data transmission.
      if (this->initiate_read_file () == -1)
        // @@ Handle this error.
        ACE_ERROR ((LM_ERROR,
                    "Error:Asynch_Transmit_Handler:read_file couldnt be initiated\n"));
      break;
      
    default:
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_Handler::handle_write_stream::Unexpected act\n"));
    }
}

void
ACE_Asynch_Transmit_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  // Failure.
  if (result.success () == 0)
    {
      //
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   0,      // Failure.
                                   0,      // @@ Completion key.
                                   errno); // Error no.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      return;
    }

  // Read successful.
  if (result.bytes_transferred () == 0)
    return;

  // Increment offset and write data to network.
  this->file_offset_ += result.bytes_transferred ();
  if (this->ws_.write (result.message_block (),
                       result.bytes_transferred (),
                       (void *)&this->data_act_) == -1)
    {
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_File : write to the stream failed\n"));
      return;
    }
}

int
ACE_Asynch_Transmit_Handler::initiate_read_file (void)
{
  // Is there something to read.
  if (this->file_offset_ >= this->file_size_)
    {
      // File is sent. Send the trailer.
      if (this->ws_.write (*this->result_->header_and_trailer ()->trailer (),
                           this->result_->header_and_trailer ()->trailer_bytes (),
                           (void *)&this->trailer_act_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler:write_stream writing trailer failed\n"),
                          -1);
      return 0;
    }
  else
    {
      // Inititiate an asynchronous read from the file.
      if (this->rf_.read (*this->mb_,
                          this->mb_->size () - 1,
                          this->file_offset_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler::read from file failed\n"),
                          -1);
      return 0;
    }
}
#endif /* ACE_HAS_AIO_CALLS */

// ************************************************************

ACE_Asynch_Transmit_File::ACE_Asynch_Transmit_File (void)
{
}

int
ACE_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
                                         Header_And_Trailer *header_and_trailer,
                                         u_long bytes_to_write,
                                         u_long offset,
                                         u_long offset_high,
                                         u_long bytes_per_send,
                                         u_long flags,
                                         const void *act)
{
#if defined (ACE_HAS_AIO_CALLS)
  // Adjust these parameters if there are default values specified.
  ssize_t file_size = ACE_OS::filesize (file);

  if (file_size == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       ":Asynch_Transmit_File:Couldnt know the file size"),
                      -1);

  if (bytes_to_write == 0)
    bytes_to_write = file_size;

  if (offset > (size_t) file_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Transmit_File:File size is less than offset"),
                      -1);

  if (offset != 0)
    bytes_to_write = file_size - offset + 1;

  if (bytes_per_send == 0)
    bytes_per_send = bytes_to_write;

  // Configure the result parameter.
  Result *result = 0;

  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          file,
                          header_and_trailer,
                          bytes_to_write,
                          offset,
                          offset_high,
                          bytes_per_send,
                          flags,
                          act,
                          this->proactor_->get_handle ()),
                  -1);

  // Make the auxillary handler and initiate transmit.
  ACE_Asynch_Transmit_Handler *transmit_handler = 0;

  ACE_NEW_RETURN (transmit_handler,
                  ::ACE_Asynch_Transmit_Handler (result),
                  -1);

  ssize_t return_val = transmit_handler->transmit ();

  if (return_val == -1)
    // This deletes the <result> in it too.
    delete transmit_handler;

  return 0;

#elif (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  Result *result = 0;
  ACE_NEW_RETURN (result,
                  Result (*this->handler_,
                          this->handle_,
                          file,
                          header_and_trailer,
                          bytes_to_write,
                          offset,
                          offset_high,
                          bytes_per_send,
                          flags,
                          act,
                          this->proactor_->get_handle ()),
                  -1);

  ACE_LPTRANSMIT_FILE_BUFFERS transmit_buffers = 0;
  if (result->header_and_trailer () != 0)
    transmit_buffers = result->header_and_trailer ()->transmit_buffers ();

  // Initiate the transmit file
  int initiate_result = ::TransmitFile ((SOCKET) result->socket (),
                                        result->file (),
                                        result->bytes_to_write (),
                                        result->bytes_per_send (),
                                        result,
                                        transmit_buffers,
                                        result->flags ());
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("TransmitFile")), -1);
    }
#else /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) */
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_AIO_CALLS */
}

// ************************************************************

ACE_HANDLE
ACE_Asynch_Transmit_File::Result::socket (void) const
{
  return this->socket_;
}

ACE_HANDLE
ACE_Asynch_Transmit_File::Result::file (void) const
{
  return this->file_;
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_Asynch_Transmit_File::Result::header_and_trailer (void) const
{
  return this->header_and_trailer_;
}

u_long
ACE_Asynch_Transmit_File::Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

u_long
ACE_Asynch_Transmit_File::Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long
ACE_Asynch_Transmit_File::Result::flags (void) const
{
  return this->flags_;
}

ACE_Asynch_Transmit_File::Result::Result (ACE_Handler &handler,
                                          ACE_HANDLE socket,
                                          ACE_HANDLE file,
                                          Header_And_Trailer *header_and_trailer,
                                          u_long bytes_to_write,
                                          u_long offset,
                                          u_long offset_high,
                                          u_long bytes_per_send,
                                          u_long flags,
                                          const void *act,
                                          ACE_HANDLE event)
  : ACE_Asynch_Result (handler, act, event, offset, offset_high),
    socket_ (socket),
    file_ (file),
    header_and_trailer_ (header_and_trailer),
    bytes_to_write_ (bytes_to_write),
    bytes_per_send_ (bytes_per_send),
    flags_ (flags)
{
}

void
ACE_Asynch_Transmit_File::Result::complete (u_long bytes_transferred,
                                            int success,
                                            const void *completion_key,
                                            u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // We will not do this because (a) the header and trailer blocks may
  // be the same message_blocks and (b) in cases of failures we have
  // no idea how much of what (header, data, trailer) was sent.
  /*
    if (this->success_ && this->header_and_trailer_ != 0)
    {
    ACE_Message_Block *header = this->header_and_trailer_->header ();
    if (header != 0)
    header->rd_ptr (this->header_and_trailer_->header_bytes ());

    ACE_Message_Block *trailer = this->header_and_trailer_->trailer ();
    if (trailer != 0)
    trailer->rd_ptr (this->header_and_trailer_->trailer_bytes ());
    }
    */

  // Callback: notify <handler> of completion
  this->handler_.handle_transmit_file (*this);
}

// ************************************************************

ACE_Asynch_Transmit_File::Header_And_Trailer::Header_And_Trailer (ACE_Message_Block *header,
                                                                  u_long header_bytes,
                                                                  ACE_Message_Block *trailer,
                                                                  u_long trailer_bytes)
  : header_ (header),
    header_bytes_ (header_bytes),
    trailer_ (trailer),
    trailer_bytes_ (trailer_bytes)
{
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header_and_trailer (ACE_Message_Block *header,
                                                                  u_long header_bytes,
                                                                  ACE_Message_Block *trailer,
                                                                  u_long trailer_bytes)
{
  this->header (header);
  this->header_bytes (header_bytes);
  this->trailer (trailer);
  this->trailer_bytes (trailer_bytes);
}

ACE_Message_Block *
ACE_Asynch_Transmit_File::Header_And_Trailer::header (void) const
{
  return this->header_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header (ACE_Message_Block *message_block)
{
  this->header_ = message_block;
}

u_long
ACE_Asynch_Transmit_File::Header_And_Trailer::header_bytes (void) const
{
  return this->header_bytes_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header_bytes (u_long bytes)
{
  this->header_bytes_ = bytes;
}

ACE_Message_Block *
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer (void) const
{
  return this->trailer_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer (ACE_Message_Block *message_block)
{
  this->trailer_ = message_block;
}

u_long
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer_bytes (void) const
{
  return this->trailer_bytes_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer_bytes (u_long bytes)
{
  this->trailer_bytes_ = bytes;
}

ACE_LPTRANSMIT_FILE_BUFFERS
ACE_Asynch_Transmit_File::Header_And_Trailer::transmit_buffers (void)
{
  // If both are zero, return zero
  if (this->header_ == 0 && this->trailer_ == 0)
    return 0;
  else
    {
      // Something is valid

      // If header is valid, set the fields
      if (this->header_ != 0)
        {
          this->transmit_buffers_.Head = this->header_->rd_ptr ();
          this->transmit_buffers_.HeadLength = this->header_bytes_;
        }
      else
        {
          this->transmit_buffers_.Head = 0;
          this->transmit_buffers_.HeadLength = 0;
        }

      // If trailer is valid, set the fields
      if (this->trailer_ != 0)
        {
          this->transmit_buffers_.Tail = this->trailer_->rd_ptr ();
          this->transmit_buffers_.TailLength = this->trailer_bytes_;
        }
      else
        {
          this->transmit_buffers_.Tail = 0;
          this->transmit_buffers_.TailLength = 0;
        }

      // Return the transmit buffers
      return &this->transmit_buffers_;
    }
}


// ************************************************************

ACE_Handler::~ACE_Handler (void)
{
}

ACE_Handler::ACE_Handler (void)
  : proactor_ (0)
{
}

ACE_Handler::ACE_Handler (ACE_Proactor *d)
  : proactor_ (d)
{
}

void
ACE_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_accept (const ACE_Asynch_Accept::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_time_out (const ACE_Time_Value &tv,
                              const void *act)
{
  ACE_UNUSED_ARG (tv);
  ACE_UNUSED_ARG (act);
}

ACE_Proactor *
ACE_Handler::proactor (void)
{
  return this->proactor_;
}

void
ACE_Handler::proactor (ACE_Proactor *p)
{
  this->proactor_ = p;
}

ACE_HANDLE
ACE_Handler::handle (void) const
{
  return ACE_INVALID_HANDLE;
}

// ************************************************************

ACE_Service_Handler::ACE_Service_Handler (void)
{
}

ACE_Service_Handler::~ACE_Service_Handler (void)
{
}

void
ACE_Service_Handler::addresses (const ACE_INET_Addr &remote_address,
                                const ACE_INET_Addr &local_address)
{
  // Default behavior is to print out the addresses.
  ASYS_TCHAR local_address_buf[BUFSIZ], remote_address_buf[BUFSIZ];
  if (local_address.addr_to_string (local_address_buf, sizeof local_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("can't obtain local_address's address string")));

  if (remote_address.addr_to_string (remote_address_buf, sizeof remote_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("can't obtain remote_address's address string")));

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("On fd %d\n"), this->handle ()));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("local address %s\n"), local_address_buf));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("remote address %s\n"), remote_address_buf));
}

void
ACE_Service_Handler::act (const void *)
{
}

void
ACE_Service_Handler::open (ACE_HANDLE,
                           ACE_Message_Block &)
{
}

#if defined (ACE_HAS_AIO_CALLS)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Queue<ACE_Asynch_Accept::Result*>;
template class ACE_Node<ACE_Asynch_Accept::Result*>;
template class ACE_Unbounded_Queue_Iterator<ACE_Asynch_Accept::Result*>;
#elif  defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Queue<ACE_Asynch_Accept::Result*>
#pragma instantiate ACE_Node<ACE_Asynch_Accept::Result*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Asynch_Accept::Result*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_AIO_CALLS */

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS*/
