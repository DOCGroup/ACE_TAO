// $Id$

#define ACE_BUILD_DLL
#include "ace/Asynch_IO.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) \
    || (defined (ACE_HAS_AIO_CALLS))
// This only works on Win32 platforms

#include "ace/Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"

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
  // @@
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
  // @@ aio_cancel will come here soon.
  return 0;
#else /* ACE_HAS_AIO_CALLS */
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && (defined (_MSC_VER) && (_MSC_VER > 1020))
  return (int) ::CancelIo (this->handle_);
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && (defined (_MSC_VER) && (_MSC_VER > 1020)) */
#endif /* ACE_HAS_AIO_CALLS */
}

#if defined (ACE_HAS_AIO_CALLS)
// If the ptr is o, just check whether there is any slot free and
// return 0 if yes, else return -1. If a valid ptr is passed, keep it
// in a free slot.
int
ACE_Asynch_Operation::register_aio_with_proactor (aiocb *aiocb_ptr)
{
  ACE_DEBUG ((LM_DEBUG, "register_aio_with_proactor\n"));
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
                       "Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Slot(s) available. Find a free one.
  size_t ai;
  for (ai = 0;
       ai < this->proactor_->aiocb_list_max_size_;
       ai++)
    if (this->proactor_->aiocb_list_[ai] == 0)
      break;

  // Check again.
  if (ai == this->proactor_->aiocb_list_max_size_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Asynch_Operation: No space to store the <aio> info.\n"),
                      -1);

  // Store the pointers.
  this->proactor_->aiocb_list_[ai] = aiocb_ptr;
  this->proactor_->aiocb_list_cur_size_ ++;
  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */


// ************************************************************

ACE_Asynch_Read_Stream::ACE_Asynch_Read_Stream (void)
{
}

int
ACE_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                              u_long bytes_to_read,
                              const void *act)
{
  // Create the Asynch_Result
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
      // No space.
      ACE_ERROR ((LM_ERROR,
                  "(%p %t) Asynch_Read_Stream: No space to queue aio_read\n"));

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
                  "(%p %t) Asynch_Read_Stream: aio_read queueing failed\n"));

      // Clean up the memory allocated.
      delete aiocb_ptr;

      return -1;
    }

  // Success. Store the aiocb_ptr with Proactor.
  if (this->register_aio_with_proactor (aiocb_ptr) == -1)
    {
      // Couldnt store the aiocb.
      ACE_ERROR ((LM_ERROR,
                  "(%p %t) Asynch_Read_Stream:Fatal error\n"));

      // Clean up the memory allocated.
      delete aiocb_ptr;

      return -1;
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
      // No space.
      ACE_ERROR ((LM_ERROR,
                  "(%p %t) Asynch_Write_Stream: No space to queue aio_read\n"));

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
                  "(%p %t) Asynch_Write_Stream: aio_write queueing failed\n"));

      // Clean up the memory allocated.
      delete aiocb_ptr;

      return -1;
    }

  // Success. Store the aiocb_ptr with Proactor.
  if (this->register_aio_with_proactor (aiocb_ptr) == -1)
    {
      // Couldnt store the aiocb.
      ACE_ERROR ((LM_ERROR,
                  "(%p %t) Asynch_Write_Stream: Fatal error\n"));

      // Clean up the memory allocated.
      delete aiocb_ptr;

      return -1;
    }

  // Aio successfully issued and ptr stored.
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
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Read_File::Result::complete\n"));

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

ACE_Asynch_Accept::ACE_Asynch_Accept (void)
{
}

int
ACE_Asynch_Accept::accept (ACE_Message_Block &message_block,
                           u_long bytes_to_read,
                           ACE_HANDLE accept_handle,
                           const void *act)
{
#if defined (ACE_HAS_AIO_CALLS)
  ACE_UNUSED_ARG (message_block);
  ACE_UNUSED_ARG (bytes_to_read);
  ACE_UNUSED_ARG (accept_handle);
  ACE_UNUSED_ARG (act);
  return 0;
#else /* ACE_HAS_AIO_CALLS */
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  // Sanity check: make sure that enough space has been allocated by the caller.
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("Buffer too small\n")), -1);

  int close_accept_handle = 0;
  // If the <accept_handle> is invalid, we will create a new socket
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

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ReadFile")), -1);
    }
#else /* ACE_HAS_WINNT4 ... */
  ACE_NOTSUP_RETURN (-1);
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) */
#endif /* ACE_HAS_AIO_CALLS */
}

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

  // Callback: notify <handler> of completion
  this->handler_.handle_accept (*this);
}

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
                       "(%p) Asynch_Transmit_File:Couldnt know the file size\n"),
                      -1);

  if (bytes_to_write == 0)
    bytes_to_write = file_size;

  if (offset > (size_t) file_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%p %t)Asynch_Transmit_File:File size is less than offset\n"),
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
    // This deletes the result in it.
    delete transmit_handler;

  return return_val;
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

#if defined (ACE_HAS_AIO_CALLS)
// Constructor.
ACE_Asynch_Transmit_Handler::ACE_Asynch_Transmit_Handler (ACE_Asynch_Transmit_File::Result *result)
  : result_ (result),
    mb_ (0),
    header_act_ (0),
    data_act_ (0),
    trailer_act_ (0),
    file_offset_ (result->offset ()),
    file_size_ (0),
    bytes_transferred_ (0)
{
  // Allocate memory for the message block.
  ACE_NEW (this->mb_,
           ACE_Message_Block (this->result_->bytes_per_send ()
                              + 1));

  // @@ Alex, must we (1) use dynamic memory for this and (2) allocate
  // 3 separate chunks of memory?  This seems costly.  Is there any
  // way to optimize it, e.g., by combining multiple
  // operations/buffers into fewer ones?

  // Memory for the ACTs.
  ACE_NEW (this->header_act_,
           ACT);

  ACE_NEW (this->data_act_,
           ACT);

  ACE_NEW (this->trailer_act_,
           ACT);

  *this->header_act_ = this->HEADER_ACT;
  *this->data_act_ = this->DATA_ACT;
  *this->trailer_act_ = this->TRAILER_ACT;

  // Init the file size.
  file_size_ = ACE_OS::filesize (this->result_->file ());
}

// Destructor.
ACE_Asynch_Transmit_Handler::~ACE_Asynch_Transmit_Handler (void)
{
  delete result_;
  delete mb_;
  delete this->header_act_;
  delete this->data_act_;
  delete this->trailer_act_;
}

// Do the transmission.
// Initiate transmitting the header. When that completes
// handle_write_stream will be called, there start transmitting the file.
int
ACE_Asynch_Transmit_Handler::transmit (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Asynch_Transmit_Handler::transmit\n"));

  // Open Asynch_Read_File.
  if (this->rf_.open (*this,
                      this->result_->file ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%p %t):ACE_Asynch_Transmit_Handler:read_file open failed\n"),
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
                       (void *) this->header_act_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%p %t):Asynch_Transmit_Handler:transmitting header:write_stream failed\n"),
                      -1);
  return 0;
}

void
ACE_Asynch_Transmit_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_DEBUG ((LM_DEBUG, "Asynch_Transmit_Handler:handle_write_stream called\n"));

  // Update bytes transferred so far.
  this->bytes_transferred_ += result.bytes_transferred ();

  // Check the success parameter.
  if (result.success () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Asynch_Transmit_File failed.\n"));

      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   0,      // Failure.
                                   0,      // @@ Completion key.
                                   errno); // Error no.
        }
      ACE_SEH_FINALLY
        {
          // This is crucial to prevent memory leaks
          delete this;
        }
    }

  // Write stream successful.

  // Partial write to socket.
  int unsent_data = result.bytes_to_write () - result.bytes_transferred ();
  if (unsent_data != 0)
    {
      // Reset pointers
      result.message_block ().rd_ptr (result.bytes_transferred ());

      // Duplicate the message block and retry remaining data
      if (this->ws_.write (*result.message_block ().duplicate (),
                           unsent_data,
                           result.act ()) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%p %t):Asynch_Transmit_Handler:write_stream failed\n"));
          return;
        }

      // @@ Handling *partial write* to a socket.  Let us not continue
      // further before this write finishes. Because proceeding with
      // another read and then write might change the order of the
      // file transmission, because partial write to the stream is
      // always possible.
      return;
    }

  // Not a partial write.

  // Check ACT to see what was sent.
  ACT act = *(ACT *) result.act ();

  switch (act)
    {
    case TRAILER_ACT:
      // If it is the "trailer" that is just sent, then transmit file
      // is complete.

      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   1,      // @@ Success.
                                   0,      // @@ Completion key.
                                   errno); // @@ Errno.
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
        ACE_ERROR ((LM_ERROR,
                    "(%p %t):Asynch_Transmit_Handler:read_file couldnt be initiated\n"));
      break;

    default:
      ACE_ERROR ((LM_ERROR,
                  "(%p %t):ACE_Asynch_Transmit_File::Aux:handle_write_stream::Unexpected act\n"));
    }
}

void
ACE_Asynch_Transmit_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  // Failure.
  if (result.success () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%p %t):Asynch_Transmit_Handler : read from the file failed\n"));
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
                       (void *) this->data_act_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%p %t):ACE_Asynch_Transmit_File : write to the stream failed\n"));
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
      ACE_DEBUG ((LM_DEBUG,
                  "Trailer %s\n",
                  this->result_->header_and_trailer ()->trailer ()->rd_ptr ()));
      if (this->ws_.write (*this->result_->header_and_trailer ()->trailer (),
                           this->result_->header_and_trailer ()->trailer_bytes (),
                           (void *)this->trailer_act_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%p %t):Asynch_Transmit_Handler:write_stream writing trailer failed\n"),
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
                           "(%p %t) Asynch_Transmit_Handler::read from file failed\n"),
                          -1);
      return 0;
    }
}
#endif /* ACE_HAS_AIO_CALLS */

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

/*
void
ACE_Handler::handle_notify (const ACE_Asynch_Notify::Result &result)
{
  ACE_UNUSED_ARG (result);
}
*/

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
ACE_Service_Handler::open (ACE_HANDLE,
                           ACE_Message_Block &)
{
}

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS*/
