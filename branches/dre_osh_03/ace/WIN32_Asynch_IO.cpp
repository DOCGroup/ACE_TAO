#include "ace/WIN32_Asynch_IO.h"

ACE_RCSID (ace,
           Win32_Asynch_IO,
           "$Id$")

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))

#include "ace/WIN32_Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/Task_T.h"

size_t
ACE_WIN32_Asynch_Result::bytes_transferred (void) const
{
  return this->bytes_transferred_;
}

const void *
ACE_WIN32_Asynch_Result::act (void) const
{
  return this->act_;
}

int
ACE_WIN32_Asynch_Result::success (void) const
{
  return this->success_;
}

const void *
ACE_WIN32_Asynch_Result::completion_key (void) const
{
  return this->completion_key_;
}

u_long
ACE_WIN32_Asynch_Result::error (void) const
{
  return this->error_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Result::event (void) const
{
  return this->hEvent;
}

u_long
ACE_WIN32_Asynch_Result::offset (void) const
{
  return this->Offset;
}

u_long
ACE_WIN32_Asynch_Result::offset_high (void) const
{
  return this->OffsetHigh;
}

int
ACE_WIN32_Asynch_Result::priority (void) const
{
  ACE_NOTSUP_RETURN (0);
}

int
ACE_WIN32_Asynch_Result::signal_number (void) const
{
  ACE_NOTSUP_RETURN (0);
}

int
ACE_WIN32_Asynch_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  // Get to the platform specific implementation.
  ACE_WIN32_Proactor *win32_proactor = ACE_dynamic_cast (ACE_WIN32_Proactor *,
                                                         proactor);

  if (win32_proactor == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("Dynamic cast to WIN32 Proactor failed\n")),
                      -1);

  // Post myself.
  return win32_proactor->post_completion (this);
}

void
ACE_WIN32_Asynch_Result::set_bytes_transferred (size_t nbytes)
{
  this->bytes_transferred_ = nbytes;
}

void
ACE_WIN32_Asynch_Result::set_error (u_long errcode)
{
  this->error_ = errcode;
}

ACE_WIN32_Asynch_Result::~ACE_WIN32_Asynch_Result (void)
{
}

ACE_WIN32_Asynch_Result::ACE_WIN32_Asynch_Result (ACE_Handler &handler,
                                                  const void* act,
                                                  ACE_HANDLE event,
                                                  u_long offset,
                                                  u_long offset_high,
                                                  int priority,
                                                  int signal_number)
  : ACE_Asynch_Result_Impl (),
    OVERLAPPED (),
    handler_ (handler),
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

  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (signal_number);
}

int
ACE_WIN32_Asynch_Operation::open (ACE_Handler &handler,
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

  // Register with the <proactor>.
  return this->win32_proactor_->register_handle (this->handle_,
                                                 completion_key);
}

int
ACE_WIN32_Asynch_Operation::cancel (void)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) \
    && (   (defined (_MSC_VER) && (_MSC_VER > 1020)) \
        || (defined (__BORLANDC__) && (__BORLANDC__ >= 0x530)) \
        || (defined (__MINGW32)))
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.

  // @@ This API returns 0 on failure. So, I am returning -1 in that
  //    case. Is that right? (Alex).

  int result = (int) ::CancelIo (this->handle_);

  if (result == 0)
    // Couldnt cancel the operations.
    return 2;

  // result is non-zero. All the operations are cancelled then.
  return 0;

#else /* Not ACE_HAS_WINNT4 && ACE_HAS_WINNT4!=0 && _MSC... */
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_AIO_CALLS */
}

ACE_Proactor *
ACE_WIN32_Asynch_Operation::proactor (void) const
{
  return this->proactor_;
}

ACE_WIN32_Asynch_Operation::ACE_WIN32_Asynch_Operation (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    win32_proactor_ (win32_proactor),
    proactor_ (0),
    handler_ (0),
    handle_ (ACE_INVALID_HANDLE)
{
}

ACE_WIN32_Asynch_Operation::~ACE_WIN32_Asynch_Operation (void)
{
}

// ************************************************************

size_t
ACE_WIN32_Asynch_Read_Stream_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_WIN32_Asynch_Read_Stream_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_Stream_Result::handle (void) const
{
  return this->handle_;
}

ACE_WIN32_Asynch_Read_Stream_Result::ACE_WIN32_Asynch_Read_Stream_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block &message_block,
  size_t bytes_to_read,
  const void* act,
  ACE_HANDLE event,
  int priority,
  int signal_number,
  int scatter_enabled)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Stream_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler,
                             act,
                             event,
                             0,
                             0,
                             priority,
                             signal_number),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    handle_ (handle),
    scatter_enabled_ (scatter_enabled)
{
}

void
ACE_WIN32_Asynch_Read_Stream_Result::complete (size_t bytes_transferred,
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
  if (!this->scatter_enabled ())
    this->message_block_.wr_ptr (bytes_transferred);
  else
  {
    for (ACE_Message_Block* mb = &this->message_block_;
         (mb != 0) && (bytes_transferred > 0);
         mb = mb->cont ())
    {
      size_t len_part = mb->space ();

      if (len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->wr_ptr (len_part);

      bytes_transferred -= len_part;
    }
  }

  // Create the interface result class.
  ACE_Asynch_Read_Stream::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_stream (result);
}

ACE_WIN32_Asynch_Read_Stream_Result::~ACE_WIN32_Asynch_Read_Stream_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Read_Stream_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Read_Stream_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Read_Stream_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Read_Stream_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Read_Stream_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_Stream_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Read_Stream_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Read_Stream_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Read_Stream_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Read_Stream_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Read_Stream_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

int
ACE_WIN32_Asynch_Read_Stream_Result::scatter_enabled (void) const
{
  return this->scatter_enabled_;
}

ACE_WIN32_Asynch_Read_Stream::ACE_WIN32_Asynch_Read_Stream (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                                    size_t bytes_to_read,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
  size_t space = message_block.space ();
  if (bytes_to_read > space)
    bytes_to_read = space;

  if (bytes_to_read == 0)
    {
      errno = ENOSPC;
      return -1;
    }

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_Stream_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       this->win32_proactor_->get_handle (),
                                                       priority,
                                                       signal_number),
                  -1);

  // Shared read
  ssize_t return_val = this->shared_read (result);

  // Upon errors
  if (return_val == -1)
    delete result;

  return return_val;
}

int
ACE_WIN32_Asynch_Read_Stream::readv (ACE_Message_Block &message_block,
                                     size_t bytes_to_read,
                                     const void *act,
                                     int priority,
                                     int signal_number)
{

  iovec  iov[ACE_IOV_MAX];
  int    iovcnt = 0;

  // We should not read more than user requested,
  // but it is allowed to read less

  for (const ACE_Message_Block* msg = &message_block;
       msg != 0 && bytes_to_read > 0 && iovcnt < ACE_IOV_MAX;
       msg = msg->cont () , ++iovcnt )
  {
    size_t msg_space = msg->space ();

    // OS should correctly process zero length buffers
    // if ( msg_space == 0 )
    //   ACE_ERROR_RETURN ((LM_ERROR,
    //                      ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_Stream::readv:")
    //                      ACE_LIB_TEXT ("No space in the message block\n")),
    //                     -1);

    if (msg_space > bytes_to_read)
      msg_space = bytes_to_read;
    bytes_to_read -= msg_space;

    // Make as many iovec as needed to fit all of msg_space.
    size_t wr_ptr_offset = 0;
    while (msg_space > 0 && iovcnt < ACE_IOV_MAX)
      {
        u_long this_chunk_length;
        if (msg_space > ULONG_MAX)
          this_chunk_length = ULONG_MAX;
        else
          this_chunk_length = ACE_static_cast (u_long, msg_space);
        // Collect the data in the iovec.
        iov[iovcnt].iov_base = msg->wr_ptr () + wr_ptr_offset;
        iov[iovcnt].iov_len  = this_chunk_length;
        msg_space -= this_chunk_length;
        wr_ptr_offset += this_chunk_length;

        // Increment iovec counter if there's more to do.
        if (msg_space > 0)
          iovcnt++;
      }
    if (msg_space > 0)       // Ran out of iovecs before msg_space exhausted
      {
        errno = ERANGE;
        return -1;
      }
  }

  // Re-calculate number bytes to read
  bytes_to_read = 0;

  for (int i = 0; i < iovcnt ; ++i)
    bytes_to_read += iov[i].iov_len;

  if (bytes_to_read == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_Stream::readv:")
                         ACE_LIB_TEXT ("Attempt to read 0 bytes\n")),
                        -1);

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_Stream_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       this->win32_proactor_->get_handle (),
                                                       priority,
                                                       signal_number,
                                                       1), // scatter read enabled
                  -1);

  // do the scatter recv

  result->set_error (0); // Clear error before starting IO.

  DWORD bytes_recvd = 0;
  u_long flags = 0;

  int initiate_result = ::WSARecv (ACE_reinterpret_cast (SOCKET, result->handle ()),
                                   ACE_reinterpret_cast (WSABUF *, iov),
                                   iovcnt,
                                   &bytes_recvd,
                                   &flags,
                                   result,
                                   0);

  if (0 == initiate_result)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  ACE_ASSERT (initiate_result == SOCKET_ERROR);

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
  {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      initiate_result = 0;
      break;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_LIB_TEXT ("%p\n"),
                    ACE_LIB_TEXT ("WSARecv")));
      }

      delete result;
      initiate_result = -1;
      break;
  }

  return initiate_result;
}

ACE_WIN32_Asynch_Read_Stream::~ACE_WIN32_Asynch_Read_Stream (void)
{
}

int
ACE_WIN32_Asynch_Read_Stream::shared_read (ACE_WIN32_Asynch_Read_Stream_Result *result)
{
  // ReadFile API limits us to DWORD range.
  if (result->bytes_to_read () > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD bytes_to_read = ACE_static_cast (DWORD, result->bytes_to_read ());
  u_long bytes_read;

  result->set_error (0); // Clear error before starting IO.

  // Initiate the read
  int initiate_result = ::ReadFile (result->handle (),
                                    result->message_block ().wr_ptr (),
                                    bytes_to_read,
                                    &bytes_read,
                                    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ReadFile")));
        }

      return -1;
    }
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Read_Stream::open (ACE_Handler &handler,
                                    ACE_HANDLE handle,
                                    const void *completion_key,
                                    ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Read_Stream::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Read_Stream::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

size_t
ACE_WIN32_Asynch_Write_Stream_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

ACE_Message_Block &
ACE_WIN32_Asynch_Write_Stream_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_Stream_Result::handle (void) const
{
  return this->handle_;
}

ACE_WIN32_Asynch_Write_Stream_Result::ACE_WIN32_Asynch_Write_Stream_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block &message_block,
  size_t bytes_to_write,
  const void* act,
  ACE_HANDLE event,
  int priority,
  int signal_number,
  int gather_enabled)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Stream_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_write_ (bytes_to_write),
    message_block_ (message_block),
    handle_ (handle),
    gather_enabled_ (gather_enabled)
{
}

void
ACE_WIN32_Asynch_Write_Stream_Result::complete (size_t bytes_transferred,
                                                int success,
                                                const void *completion_key,
                                                u_long error)
{
  // Copy the data which was returned by <GetQueuedCompletionStatus>.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  if (!this->gather_enabled ())
    this->message_block_.rd_ptr (bytes_transferred);
  else
  {
    for (ACE_Message_Block* mb = &this->message_block_;
         (mb != 0) && (bytes_transferred > 0);
         mb = mb->cont ())
    {
      size_t len_part = mb->length ();

      if ( len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->rd_ptr (len_part);

      bytes_transferred -= len_part;
    }
  }

  // Create the interface result class.
  ACE_Asynch_Write_Stream::Result result (this);

  // Call the application handler.
  this->handler_.handle_write_stream (result);
}

ACE_WIN32_Asynch_Write_Stream_Result::~ACE_WIN32_Asynch_Write_Stream_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Write_Stream_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Write_Stream_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Write_Stream_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Write_Stream_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Write_Stream_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_Stream_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Write_Stream_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Write_Stream_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Write_Stream_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Write_Stream_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Write_Stream_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

int
ACE_WIN32_Asynch_Write_Stream_Result::gather_enabled (void) const
{
  return this->gather_enabled_;
}

ACE_WIN32_Asynch_Write_Stream::ACE_WIN32_Asynch_Write_Stream (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                      size_t bytes_to_write,
                                      const void *act,
                                      int priority,
                                      int signal_number)
{
  size_t len = message_block.length();

  if (bytes_to_write > len)
     bytes_to_write = len ;

  if (bytes_to_write == 0)
    ACE_ERROR_RETURN
      ((LM_ERROR,
        ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_Stream::write:")
        ACE_LIB_TEXT ("Attempt to write 0 bytes\n")),
       -1);

  ACE_WIN32_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_Stream_Result (*this->handler_,
                                                        this->handle_,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->win32_proactor_->get_handle (),
                                                        priority,
                                                        signal_number),
                  -1);

  // Shared write
  ssize_t return_val = this->shared_write (result);

  // Upon errors
  if (return_val == -1)
    delete result;

  return return_val;
}

int
ACE_WIN32_Asynch_Write_Stream::writev (ACE_Message_Block &message_block,
                                       size_t bytes_to_write,
                                       const void *act,
                                       int priority,
                                       int signal_number)
{
  iovec  iov[ACE_IOV_MAX];
  int    iovcnt = 0;

  // We should not write more than user requested,
  // but it is allowed to write less

  for (const ACE_Message_Block* msg = &message_block;
       msg != 0 && bytes_to_write > 0 && iovcnt < ACE_IOV_MAX;
       msg = msg->cont () , ++iovcnt)
  {
    size_t msg_len = msg->length ();

    // OS should process zero length block correctly
    // if ( msg_len == 0 )
    //   ACE_ERROR_RETURN ((LM_ERROR,
    //                      ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_Stream::writev:")
    //                      ACE_LIB_TEXT ("Zero-length message block\n")),
    //                     -1);

    if (msg_len > bytes_to_write)
      msg_len = bytes_to_write;
    bytes_to_write -= msg_len;

    // Make as many iovec as needed to fit all of msg_len.
    size_t rd_ptr_offset = 0;
    while (msg_len > 0 && iovcnt < ACE_IOV_MAX)
      {
        u_long this_chunk_length;
        if (msg_len > ULONG_MAX)
          this_chunk_length = ULONG_MAX;
        else
          this_chunk_length = ACE_static_cast (u_long, msg_len);
        // Collect the data in the iovec.
        iov[iovcnt].iov_base = msg->rd_ptr () + rd_ptr_offset;
        iov[iovcnt].iov_len  = this_chunk_length;
        msg_len -= this_chunk_length;
        rd_ptr_offset += this_chunk_length;

        // Increment iovec counter if there's more to do.
        if (msg_len > 0)
          iovcnt++;
      }
    if (msg_len > 0)       // Ran out of iovecs before msg_space exhausted
      {
        errno = ERANGE;
        return -1;
      }
  }

  // Re-calculate number bytes to write
  bytes_to_write = 0;

  for ( int i=0; i < iovcnt ; ++i )
    bytes_to_write += iov[i].iov_len;

  if ( bytes_to_write == 0 )
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_Stream::writev:")
                         ACE_LIB_TEXT ("Attempt to write 0 bytes\n")),
                        -1);


  ACE_WIN32_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_Stream_Result (*this->handler_,
                                                        this->handle_,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->win32_proactor_->get_handle (),
                                                        priority,
                                                        signal_number,
                                                        1), // gather write enabled
                  -1);

  // do the gather send

  u_long bytes_sent = 0;

  int initiate_result = ::WSASend (ACE_reinterpret_cast (SOCKET, result->handle ()),
                                   ACE_reinterpret_cast (WSABUF *, iov),
                                   iovcnt,
                                   &bytes_sent,
                                   0, // flags
                                   result,
                                   0);

  if (0 == initiate_result)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  ACE_ASSERT (initiate_result == SOCKET_ERROR);

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
  {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      initiate_result = 0;
      break;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_LIB_TEXT ("%p\n"),
                    ACE_LIB_TEXT ("WSASend")));
      }

      delete result;
      initiate_result = -1;
      break;
  }

  return initiate_result;
}

ACE_WIN32_Asynch_Write_Stream::~ACE_WIN32_Asynch_Write_Stream (void)
{
}

int
ACE_WIN32_Asynch_Write_Stream::shared_write (ACE_WIN32_Asynch_Write_Stream_Result *result)
{
  u_long bytes_written;
  if (result->bytes_to_write () > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD bytes_to_write = ACE_static_cast (DWORD, result->bytes_to_write ());

  result->set_error (0); // Clear error before starting IO.

  // Initiate the write
  int initiate_result = ::WriteFile (result->handle (),
                                     result->message_block ().rd_ptr (),
                                     bytes_to_write,
                                     &bytes_written,
                                     result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("WriteFile")));
        }
      return -1;
    }
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Write_Stream::open (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     const void *completion_key,
                                     ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Write_Stream::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Write_Stream::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

ACE_WIN32_Asynch_Read_File_Result::ACE_WIN32_Asynch_Read_File_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block &message_block,
  size_t bytes_to_read,
  const void* act,
  u_long offset,
  u_long offset_high,
  ACE_HANDLE event,
  int priority,
  int signal_number,
  int scatter_enabled)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Stream_Result_Impl (),
    ACE_Asynch_Read_File_Result_Impl (),
    ACE_WIN32_Asynch_Read_Stream_Result (handler,
                                         handle,
                                         message_block,
                                         bytes_to_read,
                                         act,
                                         event,
                                         priority,
                                         signal_number,
                                         scatter_enabled)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_WIN32_Asynch_Read_File_Result::complete (size_t bytes_transferred,
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
  if (!this->scatter_enabled ())
    this->message_block_.wr_ptr (bytes_transferred);
  else
  {
    static const size_t page_size = ACE_OS::getpagesize();

    for (ACE_Message_Block* mb = &this->message_block_;
         (mb != 0) && (bytes_transferred > 0);
         mb = mb->cont ())
    {
      // mb->space () is ought to be >= page_size.
      // this is verified in the readv method
      // ACE_ASSERT (mb->space () >= page_size);

      size_t len_part = page_size ;

      if ( len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->wr_ptr (len_part);

      bytes_transferred -= len_part;
    }
  }

  // Create the interface result class.
  ACE_Asynch_Read_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_file (result);
}

ACE_WIN32_Asynch_Read_File_Result::~ACE_WIN32_Asynch_Read_File_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Read_File_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Read_File_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Read_File_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Read_File_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Read_File_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_File_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Read_File_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Read_File_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Read_File_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Read_File_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

// The following methods belong to
// ACE_WIN32_Asynch_Read_Stream_Result. They are here to avoid VC++
// warnings. These methods route their call to the
// ACE_WIN32_Asynch_Read_Stream_Result base class.

size_t
ACE_WIN32_Asynch_Read_File_Result::bytes_to_read (void) const
{
  return ACE_WIN32_Asynch_Read_Stream_Result::bytes_to_read ();
}

ACE_Message_Block &
ACE_WIN32_Asynch_Read_File_Result::message_block (void) const
{
  return ACE_WIN32_Asynch_Read_Stream_Result::message_block ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_File_Result::handle (void) const
{
  return ACE_WIN32_Asynch_Read_Stream_Result::handle ();
}

int
ACE_WIN32_Asynch_Read_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

// ************************************************************

ACE_WIN32_Asynch_Read_File::ACE_WIN32_Asynch_Read_File (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_Asynch_Read_File_Impl (),
    ACE_WIN32_Asynch_Read_Stream (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                  size_t bytes_to_read,
                                  u_long offset,
                                  u_long offset_high,
                                  const void *act,
                                  int priority,
                                  int signal_number)
{
  size_t space = message_block.space ();
  if ( bytes_to_read > space )
    bytes_to_read = space;

  if ( bytes_to_read == 0 )
    ACE_ERROR_RETURN
      ((LM_ERROR,
        ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_File::read:")
        ACE_LIB_TEXT ("Attempt to read 0 bytes or no space in the message block\n")),
       -1);


  ACE_WIN32_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_File_Result (*this->handler_,
                                                     this->handle_,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     this->win32_proactor_->get_handle (),
                                                     priority,
                                                     signal_number),
                  -1);

  // Shared read
  ssize_t return_val = this->shared_read (result);

  // Upon errors
  if (return_val == -1)
    delete result;

  return return_val;
}

int
ACE_WIN32_Asynch_Read_File::readv (ACE_Message_Block &message_block,
                                   size_t bytes_to_read,
                                   u_long offset,
                                   u_long offset_high,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
#if ((ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
  static const size_t page_size = ACE_OS::getpagesize();

  FILE_SEGMENT_ELEMENT buffer_pointers[ACE_IOV_MAX + 1];
  int buffer_pointers_count = 0;

  // Each buffer must be at least the size of a system memory page
  // and must be aligned on a system memory page size boundary

  // We should not read more than user requested,
  // but it is allowed to read less

  size_t total_space = 0;

  for (const ACE_Message_Block* msg = &message_block;
       msg != 0 && buffer_pointers_count < ACE_IOV_MAX && total_space < bytes_to_read;
       msg = msg->cont(), ++buffer_pointers_count )
  {
    size_t msg_space = msg->space ();

    if (msg_space < page_size)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_File::readv:")
                         ACE_LIB_TEXT ("Invalid message block size\n")),
                        -1);

    buffer_pointers[buffer_pointers_count].Buffer = msg->wr_ptr ();
    total_space += page_size;
  }

  // not read more than buffers space
  if (bytes_to_read > total_space)
    bytes_to_read = total_space;

  // ReadFileScatter API limits us to DWORD range.
  if (bytes_to_read > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD dword_bytes_to_read = ACE_static_cast (DWORD, bytes_to_read);

  // last one should be completely 0
  buffer_pointers[buffer_pointers_count].Buffer = 0;

  ACE_WIN32_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_File_Result (*this->handler_,
                                                     this->handle_,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     this->win32_proactor_->get_handle (),
                                                     priority,
                                                     signal_number,
                                                     1), // scatter read enabled
                  -1);

  // do the scatter read
  result->set_error (0); // Clear error before starting IO.

  int initiate_result = ::ReadFileScatter (result->handle (),
                                           buffer_pointers,
                                           dword_bytes_to_read,
                                           0, // reserved, must be NULL
                                           result);

  if (0 != initiate_result)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
  {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      initiate_result = 0;
      break;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_LIB_TEXT ("%p\n"),
                    ACE_LIB_TEXT ("ReadFileScatter")));
      }

      delete result;
      initiate_result = -1;
      break;
  }

  return initiate_result;
#else /*#if ( (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))*/
  ACE_NOTSUP_RETURN (-1);
#endif /*#if ( (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))*/
}


ACE_WIN32_Asynch_Read_File::~ACE_WIN32_Asynch_Read_File (void)
{
}

int
ACE_WIN32_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                  size_t bytes_to_read,
                                  const void *act,
                                  int priority,
                                  int signal_number)
{
  return ACE_WIN32_Asynch_Read_Stream::read (message_block,
                                             bytes_to_read,
                                             act,
                                             priority,
                                             signal_number);
}

int
ACE_WIN32_Asynch_Read_File::readv (ACE_Message_Block &message_block,
                                   size_t bytes_to_read,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
  return ACE_WIN32_Asynch_Read_Stream::readv (message_block,
                                              bytes_to_read,
                                              act,
                                              priority,
                                              signal_number);
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Read_File::open (ACE_Handler &handler,
                                  ACE_HANDLE handle,
                                  const void *completion_key,
                                  ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Read_File::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Read_File::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

ACE_WIN32_Asynch_Write_File_Result::ACE_WIN32_Asynch_Write_File_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block &message_block,
  size_t bytes_to_write,
  const void* act,
  u_long offset,
  u_long offset_high,
  ACE_HANDLE event,
  int priority,
  int signal_number,
  int gather_enabled)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Stream_Result_Impl (),
    ACE_Asynch_Write_File_Result_Impl (),
    ACE_WIN32_Asynch_Write_Stream_Result (handler,
                                          handle,
                                          message_block,
                                          bytes_to_write,
                                          act,
                                          event,
                                          priority,
                                          signal_number,
                                          gather_enabled)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_WIN32_Asynch_Write_File_Result::complete (size_t bytes_transferred,
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
  if (!this->gather_enabled ())
    this->message_block_.rd_ptr (bytes_transferred);
  else
  {
    static const size_t page_size = ACE_OS::getpagesize();

    for (ACE_Message_Block* mb = &this->message_block_;
         (mb != 0) && (bytes_transferred > 0);
         mb = mb->cont ())
    {
      // mb->length () is ought to be >= page_size.
      // this is verified in the writev method
      // ACE_ASSERT (mb->length () >= page_size);

      size_t len_part = page_size;

      if ( len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->rd_ptr (len_part);

      bytes_transferred -= len_part;
    }

  }

  // Create the interface result class.
  ACE_Asynch_Write_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_write_file (result);
}

ACE_WIN32_Asynch_Write_File_Result::~ACE_WIN32_Asynch_Write_File_Result  (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Write_File_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Write_File_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Write_File_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Write_File_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Write_File_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_File_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Write_File_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Write_File_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Write_File_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Write_File_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

// The following methods belong to
// ACE_WIN32_Asynch_Write_Stream_Result. They are here to avoid VC++
// warnings. These methods route their call to the
// ACE_WIN32_Asynch_Write_Stream_Result base class.

size_t
ACE_WIN32_Asynch_Write_File_Result::bytes_to_write (void) const
{
  return ACE_WIN32_Asynch_Write_Stream_Result::bytes_to_write ();
}

ACE_Message_Block &
ACE_WIN32_Asynch_Write_File_Result::message_block (void) const
{
  return ACE_WIN32_Asynch_Write_Stream_Result::message_block ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_File_Result::handle (void) const
{
  return ACE_WIN32_Asynch_Write_Stream_Result::handle ();
}

int
ACE_WIN32_Asynch_Write_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

ACE_WIN32_Asynch_Write_File::ACE_WIN32_Asynch_Write_File (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_Asynch_Write_File_Impl (),
    ACE_WIN32_Asynch_Write_Stream (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                    size_t bytes_to_write,
                                    u_long offset,
                                    u_long offset_high,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
  size_t len = message_block.length ();
  if ( bytes_to_write > len )
     bytes_to_write = len;

  if ( bytes_to_write == 0 )
    ACE_ERROR_RETURN
      ((LM_ERROR,
        ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_File::write:")
        ACE_LIB_TEXT ("Attempt to read 0 bytes\n")),
       -1);

  ACE_WIN32_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_File_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      this->win32_proactor_->get_handle (),
                                                      priority,
                                                      signal_number),
                  -1);

  // Shared write
  ssize_t return_val = this->shared_write (result);

  // Upon errors
  if (return_val == -1)
    delete result;

  return return_val;
}

int
ACE_WIN32_Asynch_Write_File::writev (ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     u_long offset,
                                     u_long offset_high,
                                     const void *act,
                                     int priority,
                                     int signal_number)
{
#if ((ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
  static const size_t page_size = ACE_OS::getpagesize();

  FILE_SEGMENT_ELEMENT buffer_pointers[ACE_IOV_MAX + 1];
  int buffer_pointers_count = 0;

  // Each buffer must be at least the size of a system memory page
  // and must be aligned on a system memory page size boundary

  // We should not read more than user requested,
  // but it is allowed to read less

  size_t total_len = 0;

  for (const ACE_Message_Block* msg = &message_block;
       msg != 0 && buffer_pointers_count < ACE_IOV_MAX && total_len < bytes_to_write;
       msg = msg->cont (), ++buffer_pointers_count )
  {
    size_t msg_len = msg->length ();

    // Don't allow writing less than page_size, unless
    // the size of the message block is big enough (so we don't write from
    // memory which does not belong to the message block), and the message
    // block is the last in the chain.
    if (msg_len < page_size &&
        (msg->size () - (msg->rd_ptr () - msg->base ()) < page_size || // message block too small
         bytes_to_write - total_len > page_size ))// NOT last chunk
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_File::writev:")
                         ACE_LIB_TEXT ("Invalid message block length\n")),
                        -1);

    buffer_pointers[buffer_pointers_count].Buffer = msg->rd_ptr ();
    total_len += page_size;
  }

  // not write more than we have in buffers
  if (bytes_to_write > total_len)
    bytes_to_write = total_len;
  // WriteFileGather API limits us to DWORD range.
  if (bytes_to_write > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD dword_bytes_to_write = ACE_static_cast (DWORD, bytes_to_write);

  // last one should be completely 0
  buffer_pointers[buffer_pointers_count].Buffer = 0;

  ACE_WIN32_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_File_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      this->win32_proactor_->get_handle (),
                                                      priority,
                                                      signal_number,
                                                      1), // gather write enabled
                  -1);

  result->set_error(0);

  // do the gather write
  int initiate_result = ::WriteFileGather (result->handle (),
                                           buffer_pointers,
                                           dword_bytes_to_write,
                                           0, // reserved, must be NULL
                                           result);

  if (0 != initiate_result)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
  switch (errno)
  {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      initiate_result = 0;
      break;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (ACE::debug ())
      {
        ACE_DEBUG ((LM_ERROR,
                    ACE_LIB_TEXT ("%p\n"),
                    ACE_LIB_TEXT ("WriteFileGather")));
      }

      delete result;
      initiate_result = -1;
      break;
  }

  return initiate_result;
#else /*#if ((ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))*/

  ACE_NOTSUP_RETURN (-1);

#endif /* */
}


ACE_WIN32_Asynch_Write_File::~ACE_WIN32_Asynch_Write_File (void)
{
}

int
ACE_WIN32_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                    size_t bytes_to_write,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
  return ACE_WIN32_Asynch_Write_Stream::write (message_block,
                                               bytes_to_write,
                                               act,
                                               priority,
                                               signal_number);
}

int
ACE_WIN32_Asynch_Write_File::writev (ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     const void *act,
                                     int priority,
                                     int signal_number)
{
  return ACE_WIN32_Asynch_Write_Stream::writev (message_block,
                                                bytes_to_write,
                                                act,
                                                priority,
                                                signal_number);
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Write_File::open (ACE_Handler &handler,
                                   ACE_HANDLE handle,
                                   const void *completion_key,
                                   ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Write_File::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Write_File::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

size_t
ACE_WIN32_Asynch_Accept_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_WIN32_Asynch_Accept_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Accept_Result::listen_handle (void) const
{
  return this->listen_handle_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Accept_Result::accept_handle (void) const
{
  return this->accept_handle_;
}

ACE_WIN32_Asynch_Accept_Result::ACE_WIN32_Asynch_Accept_Result (
  ACE_Handler &handler,
  ACE_HANDLE listen_handle,
  ACE_HANDLE accept_handle,
  ACE_Message_Block &message_block,
  size_t bytes_to_read,
  const void* act,
  ACE_HANDLE event,
  int priority,
  int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Accept_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler,
                             act,
                             event,
                             0,
                             0,
                             priority,
                             signal_number),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    listen_handle_ (listen_handle),
    accept_handle_ (accept_handle)
{
}

void
ACE_WIN32_Asynch_Accept_Result::complete (size_t bytes_transferred,
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

  // Create the interface result class.
  ACE_Asynch_Accept::Result result (this);

  // Call the application handler.
  this->handler_.handle_accept (result);
}

ACE_WIN32_Asynch_Accept_Result::~ACE_WIN32_Asynch_Accept_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Accept_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Accept_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Accept_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Accept_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Accept_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Accept_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Accept_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Accept_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Accept_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Accept_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Accept_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

ACE_WIN32_Asynch_Accept::ACE_WIN32_Asynch_Accept (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Accept_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Accept::accept (ACE_Message_Block &message_block,
                                 size_t bytes_to_read,
                                 ACE_HANDLE accept_handle,
                                 const void *act,
                                 int priority,
                                 int signal_number)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  // Sanity check: make sure that enough space has been allocated by
  // the caller.
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_LIB_TEXT ("Buffer too small\n")), -1);

  // WIN Specific.

  // AcceptEx API limits us to DWORD range.
  if (bytes_to_read > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD dword_bytes_to_read = ACE_static_cast (DWORD, bytes_to_read);

  int close_accept_handle = 0;
  // If the <accept_handle> is invalid, we will create a new socket.
  if (accept_handle == ACE_INVALID_HANDLE)
    {
      accept_handle = ACE_OS::socket (PF_INET,
                                      SOCK_STREAM,
                                      0);
      if (accept_handle == ACE_INVALID_HANDLE)
        {
          if (ACE::debug ())
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_LIB_TEXT ("%p\n"),
                          ACE_LIB_TEXT ("ACE_OS::socket")));
            }
          return -1;
        }
      else
        // Remember to close the socket down if failures occur.
        close_accept_handle = 1;
    }

  // Common code for both WIN and POSIX.
  ACE_WIN32_Asynch_Accept_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Accept_Result (*this->handler_,
                                                  this->handle_,
                                                  accept_handle,
                                                  message_block,
                                                  bytes_to_read,
                                                  act,
                                                  this->win32_proactor_->get_handle (),
                                                  priority,
                                                  signal_number),
                  -1);

  u_long bytes_read;

  // Initiate the accept.
  int initiate_result = ::AcceptEx ((SOCKET) result->listen_handle (),
                                    (SOCKET) result->accept_handle (),
                                    result->message_block ().wr_ptr (),
                                    dword_bytes_to_read,
                                    ACE_static_cast (DWORD, address_size),
                                    ACE_static_cast (DWORD, address_size),
                                    &bytes_read,
                                    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
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

      if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("ReadFile")));
        }
      return -1;
    }
#else /* ACE_HAS_WINNT4 .......|| ACE_HAS_AIO_CALLS */
  ACE_UNUSED_ARG (message_block);
  ACE_UNUSED_ARG (bytes_to_read);
  ACE_UNUSED_ARG (accept_handle);
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (signal_number);
  ACE_NOTSUP_RETURN (-1);
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) || (defined (ACE_HAS_AIO_CALLS) */
}

ACE_WIN32_Asynch_Accept::~ACE_WIN32_Asynch_Accept (void)
{
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Accept::open (ACE_Handler &handler,
                               ACE_HANDLE handle,
                               const void *completion_key,
                               ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Accept::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Accept::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_HANDLE
ACE_WIN32_Asynch_Connect_Result::connect_handle (void) const
{
  return this->connect_handle_;
}

void ACE_WIN32_Asynch_Connect_Result::connect_handle ( ACE_HANDLE handle )
{
  this->connect_handle_ = handle;
}


ACE_WIN32_Asynch_Connect_Result::ACE_WIN32_Asynch_Connect_Result
            (ACE_Handler &handler,
             ACE_HANDLE connect_handle,
             const void* act,
             ACE_HANDLE event,
             int priority,
             int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Connect_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    connect_handle_ ( connect_handle )
{
  ;
}

void
ACE_WIN32_Asynch_Connect_Result::complete (size_t bytes_transferred,
                                           int success,
                                           const void *completion_key,
                                           u_long error)
{
  // Copy the data.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Create the interface result class.
  ACE_Asynch_Connect::Result result (this);

  // Call the application handler.
  this->handler_.handle_connect (result);
}

ACE_WIN32_Asynch_Connect_Result::~ACE_WIN32_Asynch_Connect_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Connect_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Connect_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Connect_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Connect_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Connect_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Connect_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Connect_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Connect_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Connect_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Connect_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Connect_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

ACE_WIN32_Asynch_Connect::ACE_WIN32_Asynch_Connect (ACE_WIN32_Proactor * win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Connect_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor),
    flg_open_ (0),
    task_lock_count_ (0)
{
}

ACE_WIN32_Asynch_Connect::~ACE_WIN32_Asynch_Connect (void)
{
  this->close ();
  this->reactor (0); // to avoid purge_pending_notifications
}

ACE_Proactor *
ACE_WIN32_Asynch_Connect::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Connect::get_handle (void) const
{

  ACE_ASSERT (0);
  return ACE_INVALID_HANDLE;
}

void
ACE_WIN32_Asynch_Connect::set_handle (ACE_HANDLE)
{
  ACE_ASSERT (0) ;
}

int
ACE_WIN32_Asynch_Connect::open (ACE_Handler &handler,
                                ACE_HANDLE,
                                const void *completion_key,
                                ACE_Proactor *proactor)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::open\n"));

  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

  // if we are already opened,
  // we could not create a new handler without closing the previous
  if (this->flg_open_ != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::open:")
                       ACE_LIB_TEXT ("connector already open \n")),
                      -1);

  //int result =
  ACE_WIN32_Asynch_Operation::open (handler,
                                    ACE_INVALID_HANDLE,
                                    completion_key,
                                    proactor);

  // Ignore result as we pass ACE_INVALID_HANDLE
  //if (result == -1)
  //  return result;

  this->flg_open_ = 1;

  return 0;
}

int
ACE_WIN32_Asynch_Connect::connect (ACE_HANDLE connect_handle,
                                   const ACE_Addr & remote_sap,
                                   const ACE_Addr & local_sap,
                                   int reuse_addr,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::connect\n"));

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    if (this->flg_open_ == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect")
                         ACE_LIB_TEXT ("connector was not opened before\n")),
                        -1);

    // Common code for both WIN and WIN32.
    // Create future Asynch_Connect_Result
    ACE_WIN32_Asynch_Connect_Result *result = 0;
    ACE_NEW_RETURN (result,
                    ACE_WIN32_Asynch_Connect_Result (*this->handler_,
                                                     connect_handle,
                                                     act,
                                                     this->win32_proactor_->get_handle (),
                                                     priority,
                                                     signal_number),
                    -1);

    int rc = connect_i (result,
                        remote_sap,
                        local_sap,
                        reuse_addr);

    // update handle
    connect_handle = result->connect_handle ();

    if (rc != 0)
      return post_result (result, 1);

    //  Enqueue result we will wait for completion

    if (this->result_map_.bind (connect_handle, result) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect:")
                    ACE_LIB_TEXT ("result map binding failed\n")));
        result->set_error (EFAULT);
        return post_result (result, 1);
      }

    this->task_lock_count_++;
  }

  ACE_Asynch_Pseudo_Task & task =
    this->win32_proactor_->get_asynch_pseudo_task ();

  int rc_task = task.register_io_handler (connect_handle,
                                          this,
                                          ACE_Event_Handler::CONNECT_MASK,
                                          0);  // not to suspend after register

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    this->task_lock_count_--;

    int post_enable = 1;

    if (rc_task == -2 && task_lock_count_ == 0)  // task is closing
      {
        post_enable = 0;
        task.unlock_finish ();
      }

    if (rc_task < 0)
      {
        ACE_WIN32_Asynch_Connect_Result *result = 0;

        this->result_map_.unbind (connect_handle, result);

        if (result != 0)
          {
            result->set_error (EFAULT);

            return post_result (result, post_enable);
          }
      }
  }

  return 0;
}

int ACE_WIN32_Asynch_Connect::post_result (ACE_WIN32_Asynch_Connect_Result * result,
                                           int post_enable)
{
  if (this->flg_open_ != 0 && post_enable != 0)
    {
      if (this->win32_proactor_ ->post_completion (result) == 0)
        return 0;

      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("Error:(%P | %t):%p\n"),
                  ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::post_result: ")
                  ACE_LIB_TEXT (" <post_completion> failed")));
    }

   ACE_HANDLE handle = result->connect_handle ();

   if (handle != ACE_INVALID_HANDLE)
     ACE_OS::closesocket (handle);

   delete result;

   return -1;
}

//@@ New method connect_i
//  return code :
//   -1   errors  before  attempt to connect
//    0   connect started
//    1   connect finished ( may be unsuccessfully)

int
ACE_WIN32_Asynch_Connect::connect_i (ACE_WIN32_Asynch_Connect_Result *result,
                                     const ACE_Addr & remote_sap,
                                     const ACE_Addr & local_sap,
                                     int  reuse_addr)
{
  result->set_bytes_transferred (0);

  ACE_HANDLE handle = result->connect_handle ();

  if (handle == ACE_INVALID_HANDLE)
    {
      int protocol_family = remote_sap.get_type ();

      handle = ACE_OS::socket (protocol_family,
                               SOCK_STREAM,
                               0);

      // save it
      result->connect_handle (handle);

      if (handle == ACE_INVALID_HANDLE)
        {
          result->set_error (errno);

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect_i: ")
                             ACE_LIB_TEXT (" ACE_OS::socket failed\n")),
                            -1);
        }

      // Reuse the address
      int one = 1;
      if (protocol_family != PF_UNIX  &&
          reuse_addr != 0 &&
          ACE_OS::setsockopt (handle,
                              SOL_SOCKET,
                              SO_REUSEADDR,
                              (const char*) &one,
                              sizeof one) == -1)
        {
          result->set_error (errno);

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect_i: ")
                             ACE_LIB_TEXT (" ACE_OS::setsockopt failed\n")),
                            -1);
        }
    }

  if (local_sap != ACE_Addr::sap_any)
    {
      sockaddr * laddr = ACE_reinterpret_cast (sockaddr *,
                                               local_sap.get_addr ());
      int size = local_sap.get_size ();

      if (ACE_OS::bind (handle, laddr, size) == -1)
        {
           result->set_error (errno);

           ACE_ERROR_RETURN ((LM_ERROR,
                              ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect_i: ")
                              ACE_LIB_TEXT (" ACE_OS::bind failed\n")),
                             -1);
        }
    }

  // set non blocking mode

  if (ACE::set_flags (handle, ACE_NONBLOCK) != 0)
    {
      result->set_error (errno);

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%N:%l:ACE_WIN32_Asynch_Connect::connect_i: ")
                         ACE_LIB_TEXT (" ACE::set_flags failed\n")),
                        -1);
    }

  for (;;)
    {
      int rc = ACE_OS::connect (handle,
                                ACE_reinterpret_cast (sockaddr *,
                                                      remote_sap.get_addr ()),
                                remote_sap.get_size ());

      if (rc < 0)  // failure
        {
          if (errno == EWOULDBLOCK || errno == EINPROGRESS)
            return 0; // connect started

          if (errno == EINTR)
             continue;

          result->set_error (errno);
        }
      return 1 ;  // connect finished
    }

  ACE_NOTREACHED (return 0);
}


//@@ New method cancel_uncompleted
// It performs cancellation of all pending requests
//
// Parameter flg_notify can be
//     0  - don't send notifications about canceled accepts
//    !0  - notify user about canceled accepts
//          according WIN32 standards we should receive notifications
//          on canceled AIO requests
//
//  Return value : number of cancelled requests
//

int
ACE_WIN32_Asynch_Connect::cancel_uncompleted (int flg_notify, ACE_Handle_Set & set)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::cancel_uncompleted\n"));

  int retval = 0;

  MAP_MANAGER::ITERATOR iter (result_map_);

  MAP_MANAGER::ENTRY *   me = 0;

  set.reset ();

  for (; iter.next (me) != 0; retval++, iter.advance ())
    {
       ACE_HANDLE handle = me->ext_id_;
       ACE_WIN32_Asynch_Connect_Result* result = me->int_id_ ;

       set.set_bit (handle);

       result->set_bytes_transferred (0);
       result->set_error (ERROR_OPERATION_ABORTED);
       this->post_result (result, flg_notify);
    }

  result_map_.unbind_all ();

  return retval;
}

int
ACE_WIN32_Asynch_Connect::cancel (void)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::cancel\n"));

  //We are not really ACE_WIN32_Asynch_Operation
  //so we could not call ::aiocancel ()
  // or just write
  //return ACE_WIN32_Asynch_Operation::cancel ();
  //We delegate real cancelation to cancel_uncompleted (1)

  int rc = -1 ;  // ERRORS

  ACE_Handle_Set set;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    int num_cancelled = cancel_uncompleted (flg_open_, set);

    if (num_cancelled == 0)
       rc = 1;        // AIO_ALLDONE
    else if (num_cancelled > 0)
       rc = 0;        // AIO_CANCELED

    if (this->flg_open_ == 0)
       return rc;

    this->task_lock_count_++;
  }

  ACE_Asynch_Pseudo_Task & task =
    this->win32_proactor_->get_asynch_pseudo_task ();

  int rc_task = task.remove_io_handler (set);

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    this->task_lock_count_--;

    if (rc_task == -2 && task_lock_count_ == 0)  // task is closing
       task.unlock_finish ();
  }

  return rc;
}

int
ACE_WIN32_Asynch_Connect::close (void)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::close\n"));

  ACE_Handle_Set set;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    int num_cancelled = cancel_uncompleted (flg_open_, set);

    if (num_cancelled == 0 || this->flg_open_ == 0)
      {
        this->flg_open_ = 0;
        return 0;
      }

    this->task_lock_count_++;
  }

  ACE_Asynch_Pseudo_Task & task =
    this->win32_proactor_->get_asynch_pseudo_task ();

  int rc_task = task.remove_io_handler (set);

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1));

    this->task_lock_count_--;

    if (rc_task == -2 && task_lock_count_ == 0)  // task is closing
       task.unlock_finish ();

    this->flg_open_ = 0;
  }

  return 0;
}

int
ACE_WIN32_Asynch_Connect::handle_exception (ACE_HANDLE fd)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::handle_exception\n"));
  return handle_output (fd);
}

int
ACE_WIN32_Asynch_Connect::handle_input (ACE_HANDLE fd)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::handle_input\n"));
  return handle_output (fd);
}

int
ACE_WIN32_Asynch_Connect::handle_output (ACE_HANDLE fd)
{
  ACE_TRACE (ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::handle_output\n"));

  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0));

  ACE_WIN32_Asynch_Connect_Result* result = 0;

  if (this->result_map_.unbind (fd, result) != 0) // not found
    return -1;

  int sockerror  = 0 ;
  int lsockerror = sizeof sockerror;

  ACE_OS::getsockopt (fd,
                      SOL_SOCKET,
                      SO_ERROR,
                      (char*) & sockerror,
                      & lsockerror);

  result->set_bytes_transferred (0);
  result->set_error (sockerror);
  this->post_result (result, this->flg_open_);

  //ACE_Asynch_Pseudo_Task & task =
  //       this->win32_proactor_->get_asynch_pseudo_task();

  // remove_io_handler() contains flag DONT_CALL
  //task.remove_io_handler ( fd );

  //return 0;
  return -1;
}


int
ACE_WIN32_Asynch_Connect::handle_close (ACE_HANDLE fd, ACE_Reactor_Mask)
{
  ACE_TRACE(ACE_LIB_TEXT ("ACE_WIN32_Asynch_Connect::handle_close\n"));

  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0));

  ACE_Asynch_Pseudo_Task & task =
         this->win32_proactor_->get_asynch_pseudo_task ();

  if (task.is_active () == 0)  // task is closing
    {
      if (this->flg_open_ != 0)  // we are open
        {
          this->flg_open_ = 0;

          // it means other thread is waiting for reactor token_
          if (this->task_lock_count_ > 0)
            task.lock_finish ();
        }

      ACE_Handle_Set set;
      this->cancel_uncompleted (0, set);

      return 0;
    }

  // remove_io_handler() contains flag DONT_CALL
  // so it is save
  task.remove_io_handler (fd);

  ACE_WIN32_Asynch_Connect_Result* result = 0;

  if (this->result_map_.unbind (fd, result) != 0) // not found
    return -1;

  result->set_bytes_transferred (0);
  result->set_error (ERROR_OPERATION_ABORTED);
  this->post_result (result, this->flg_open_);

  return 0;
}

// *********************************************************************

ACE_HANDLE
ACE_WIN32_Asynch_Transmit_File_Result::socket (void) const
{
  return this->socket_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Transmit_File_Result::file (void) const
{
  return this->file_;
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_WIN32_Asynch_Transmit_File_Result::header_and_trailer (void) const
{
  return this->header_and_trailer_;
}

size_t
ACE_WIN32_Asynch_Transmit_File_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

size_t
ACE_WIN32_Asynch_Transmit_File_Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::flags (void) const
{
  return this->flags_;
}

ACE_WIN32_Asynch_Transmit_File_Result::ACE_WIN32_Asynch_Transmit_File_Result (
  ACE_Handler &handler,
  ACE_HANDLE socket,
  ACE_HANDLE file,
  ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
  size_t bytes_to_write,
  u_long offset,
  u_long offset_high,
  size_t bytes_per_send,
  u_long flags,
  const void *act,
  ACE_HANDLE event,
  int priority,
  int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Transmit_File_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler,
                             act,
                             event,
                             offset,
                             offset_high,
                             priority,
                             signal_number),
    socket_ (socket),
    file_ (file),
    header_and_trailer_ (header_and_trailer),
    bytes_to_write_ (bytes_to_write),
    bytes_per_send_ (bytes_per_send),
    flags_ (flags)
{
}

void
ACE_WIN32_Asynch_Transmit_File_Result::complete (size_t bytes_transferred,
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

  // Create the interface result class.
  ACE_Asynch_Transmit_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_transmit_file (result);
}

ACE_WIN32_Asynch_Transmit_File_Result::~ACE_WIN32_Asynch_Transmit_File_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

size_t
ACE_WIN32_Asynch_Transmit_File_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Transmit_File_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Transmit_File_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Transmit_File_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Transmit_File_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Transmit_File_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Transmit_File_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Transmit_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

ACE_WIN32_Asynch_Transmit_File::ACE_WIN32_Asynch_Transmit_File (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Transmit_File_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
                                               ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                               size_t bytes_to_write,
                                               u_long offset,
                                               u_long offset_high,
                                               size_t bytes_per_send,
                                               u_long flags,
                                               const void *act,
                                               int priority,
                                               int signal_number)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))

  // TransmitFile API limits us to DWORD range.
  if (bytes_to_write > MAXDWORD || bytes_per_send > MAXDWORD)
    {
      errno = ERANGE;
      return -1;
    }
  DWORD dword_bytes_to_write = ACE_static_cast (DWORD, bytes_to_write);
  DWORD dword_bytes_per_send = ACE_static_cast (DWORD, bytes_per_send);

  ACE_WIN32_Asynch_Transmit_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Transmit_File_Result (*this->handler_,
                                                         this->handle_,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         this->win32_proactor_->get_handle (),
                                                         priority,
                                                         signal_number),
                  -1);

  ACE_LPTRANSMIT_FILE_BUFFERS transmit_buffers = 0;
  if (result->header_and_trailer () != 0)
    transmit_buffers = result->header_and_trailer ()->transmit_buffers ();

  // Initiate the transmit file
  int initiate_result = ::TransmitFile ((SOCKET) result->socket (),
                                        result->file (),
                                        dword_bytes_to_write,
                                        dword_bytes_per_send,
                                        result,
                                        transmit_buffers,
                                        result->flags ());
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  ACE_OS::set_errno_to_last_error ();
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

      if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("TransmitFile")));
        }
      return -1;
    }
#else /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) */
  ACE_UNUSED_ARG (file);
  ACE_UNUSED_ARG (header_and_trailer);
  ACE_UNUSED_ARG (bytes_to_write);
  ACE_UNUSED_ARG (offset);
  ACE_UNUSED_ARG (offset_high);
  ACE_UNUSED_ARG (bytes_per_send);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (act);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (signal_number);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_AIO_CALLS */
}

ACE_WIN32_Asynch_Transmit_File::~ACE_WIN32_Asynch_Transmit_File (void)
{
}

// Methods belong to ACE_WIN32_Asynch_Operation base class. These
// methods are defined here to avoid VC++ warnings. They route the
// call to the ACE_WIN32_Asynch_Operation base class.

int
ACE_WIN32_Asynch_Transmit_File::open (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      const void *completion_key,
                                      ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Transmit_File::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Transmit_File::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

size_t
ACE_WIN32_Asynch_Read_Dgram_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block*
ACE_WIN32_Asynch_Read_Dgram_Result::message_block (void) const
{
  return this->message_block_;
}


int
ACE_WIN32_Asynch_Read_Dgram_Result::remote_address (ACE_Addr& addr) const
{
 int retVal = -1;  // failure

  // make sure the addresses are of the same type
  if (addr.get_type () == this->remote_address_->get_type ())
  { // copy the remote_address_ into addr
    addr.set_addr (this->remote_address_->get_addr (),
                   this->remote_address_->get_size ());
    retVal = 0; // success
  }

  return retVal;
}

sockaddr *
ACE_WIN32_Asynch_Read_Dgram_Result::saddr () const
{
  return (sockaddr *) this->remote_address_->get_addr ();
}


int
ACE_WIN32_Asynch_Read_Dgram_Result::flags (void) const
{
  return this->flags_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_Dgram_Result::handle (void) const
{
  return this->handle_;
}

size_t
ACE_WIN32_Asynch_Read_Dgram_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Read_Dgram_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Read_Dgram_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Read_Dgram_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Read_Dgram_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Read_Dgram_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Read_Dgram_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Read_Dgram_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Read_Dgram_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Read_Dgram_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Read_Dgram_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

ACE_WIN32_Asynch_Read_Dgram_Result::ACE_WIN32_Asynch_Read_Dgram_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block *message_block,
  size_t bytes_to_read,
  int flags,
  int protocol_family,
  const void* act,
  ACE_HANDLE event,
  int priority,
  int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Dgram_Result_Impl(),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    remote_address_ (0),
    addr_len_ (0),
    flags_ (flags),
    handle_ (handle)
{
  ACE_ASSERT (protocol_family == PF_INET); // only supporting INET addresses

  ACE_NEW (remote_address_, ACE_INET_Addr);
  addr_len_ = remote_address_->get_size ();

  ACE_UNUSED_ARG (protocol_family);
}

void
ACE_WIN32_Asynch_Read_Dgram_Result::complete (size_t bytes_transferred,
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
  for (ACE_Message_Block* mb = this->message_block_;
       (mb != 0) && (bytes_transferred > 0);
       mb = mb->cont ())
    {
      size_t len_part = mb->space ();

      if ( len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->wr_ptr (len_part);

      bytes_transferred -= len_part;
    }

  // Adjust the address length
  this->remote_address_->set_size (this->addr_len_);

  // Create the interface result class.
  ACE_Asynch_Read_Dgram::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_dgram (result);
}

ACE_WIN32_Asynch_Read_Dgram_Result::~ACE_WIN32_Asynch_Read_Dgram_Result (void)
{
  delete this->remote_address_;
}

//***************************************************************************

ACE_WIN32_Asynch_Read_Dgram::~ACE_WIN32_Asynch_Read_Dgram (void)
{
}

ssize_t
ACE_WIN32_Asynch_Read_Dgram::recv (ACE_Message_Block *message_block,
                                   size_t & number_of_bytes_recvd,
                                   int flags,
                                   int protocol_family,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
  number_of_bytes_recvd = 0;

  size_t bytes_to_read = 0;

  iovec  iov[ACE_IOV_MAX];
  int    iovcnt = 0;

  for (const ACE_Message_Block* msg = message_block;
       msg != 0 && iovcnt < ACE_IOV_MAX;
       msg = msg->cont () , ++iovcnt )
  {
    size_t msg_space = msg->space ();

    // OS should correctly process zero length buffers
    // if ( msg_space == 0 )
    //   ACE_ERROR_RETURN ((LM_ERROR,
    //                      ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_Dgram::recv:")
    //                      ACE_LIB_TEXT ("No space in the message block\n")),
    //                     -1);

    bytes_to_read += msg_space;

    // Make as many iovec as needed to fit all of msg_len.
    size_t wr_ptr_offset = 0;
    while (msg_space > 0 && iovcnt < ACE_IOV_MAX)
      {
        u_long this_chunk_length;
        if (msg_space > ULONG_MAX)
          this_chunk_length = ULONG_MAX;
        else
          this_chunk_length = ACE_static_cast (u_long, msg_space);
        // Collect the data in the iovec.
        iov[iovcnt].iov_base = msg->wr_ptr () + wr_ptr_offset;
        iov[iovcnt].iov_len  = this_chunk_length;
        msg_space -= this_chunk_length;
        wr_ptr_offset += this_chunk_length;

        // Increment iovec counter if there's more to do.
        if (msg_space > 0)
          iovcnt++;
      }
    if (msg_space > 0)       // Ran out of iovecs before msg_space exhausted
      {
        errno = ERANGE;
        return -1;
      }
  }

  if (bytes_to_read == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Read_Dgram::recv:")
                         ACE_LIB_TEXT ("Attempt to read 0 bytes\n")),
                        -1);

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Read_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_Dgram_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      bytes_to_read,
                                                      flags,
                                                      protocol_family,
                                                      act,
                                                      this->win32_proactor_->get_handle (),
                                                      priority,
                                                      signal_number),
                  -1);

  // do the scatter/gather recv
  int initiate_result = ACE_OS::recvfrom (result->handle (),
                                          iov,
                                          iovcnt,
                                          number_of_bytes_recvd,
                                          result->flags_,
                                          result->saddr (),
                                          &(result->addr_len_),
                                          result,
                                          0);
  if (initiate_result == SOCKET_ERROR)
  {
    // If initiate failed, check for a bad error.
    ACE_OS::set_errno_to_last_error ();
    switch (errno)
    {
      case ERROR_IO_PENDING:
        // The IO will complete proactively: the OVERLAPPED will still
        // get queued.
        initiate_result = 0;
        break;

      default:
        // Something else went wrong: the OVERLAPPED will not get
        // queued.

        if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("WSARecvFrom")));
        }

        delete result;
        initiate_result = -1;
        break;
    }

  }
  else
  {
    // Immediate success: the OVERLAPPED will still get queued.
    // number_of_bytes_recvd contains the number of bytes recvd
    // addr contains the peer address
    // flags was updated

    // number_of_bytes_recvd = bytes_recvd;
    initiate_result = 1;
  }

  return initiate_result;
}

int
ACE_WIN32_Asynch_Read_Dgram::open (ACE_Handler &handler,
                                   ACE_HANDLE handle,
                                   const void *completion_key,
                                   ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Read_Dgram::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Read_Dgram::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

ACE_WIN32_Asynch_Read_Dgram::ACE_WIN32_Asynch_Read_Dgram (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Dgram_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

//***********************************************

size_t
ACE_WIN32_Asynch_Write_Dgram_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

ACE_Message_Block*
ACE_WIN32_Asynch_Write_Dgram_Result::message_block () const
{
  return this->message_block_;
}

int
ACE_WIN32_Asynch_Write_Dgram_Result::flags (void) const
{
  return this->flags_;
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_Dgram_Result::handle (void) const
{
  return this->handle_;
}

size_t
ACE_WIN32_Asynch_Write_Dgram_Result::bytes_transferred (void) const
{
  return ACE_WIN32_Asynch_Result::bytes_transferred ();
}

const void *
ACE_WIN32_Asynch_Write_Dgram_Result::act (void) const
{
  return ACE_WIN32_Asynch_Result::act ();
}

int
ACE_WIN32_Asynch_Write_Dgram_Result::success (void) const
{
  return ACE_WIN32_Asynch_Result::success ();
}

const void *
ACE_WIN32_Asynch_Write_Dgram_Result::completion_key (void) const
{
  return ACE_WIN32_Asynch_Result::completion_key ();
}

u_long
ACE_WIN32_Asynch_Write_Dgram_Result::error (void) const
{
  return ACE_WIN32_Asynch_Result::error ();
}

ACE_HANDLE
ACE_WIN32_Asynch_Write_Dgram_Result::event (void) const
{
  return ACE_WIN32_Asynch_Result::event ();
}

u_long
ACE_WIN32_Asynch_Write_Dgram_Result::offset (void) const
{
  return ACE_WIN32_Asynch_Result::offset ();
}

u_long
ACE_WIN32_Asynch_Write_Dgram_Result::offset_high (void) const
{
  return ACE_WIN32_Asynch_Result::offset_high ();
}

int
ACE_WIN32_Asynch_Write_Dgram_Result::priority (void) const
{
  return ACE_WIN32_Asynch_Result::priority ();
}

int
ACE_WIN32_Asynch_Write_Dgram_Result::signal_number (void) const
{
  return ACE_WIN32_Asynch_Result::signal_number ();
}

int
ACE_WIN32_Asynch_Write_Dgram_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_WIN32_Asynch_Result::post_completion (proactor);
}

ACE_WIN32_Asynch_Write_Dgram_Result::ACE_WIN32_Asynch_Write_Dgram_Result (
  ACE_Handler &handler,
  ACE_HANDLE handle,
  ACE_Message_Block *message_block,
  size_t bytes_to_write,
  int flags,
  const void* act,
  ACE_HANDLE event,
  int priority,
  int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Dgram_Result_Impl(),
    ACE_WIN32_Asynch_Result (handler,
                             act,
                             event,
                             0,
                             0,
                             priority,
                             signal_number),
    bytes_to_write_ (bytes_to_write),
    message_block_ (message_block),
    flags_ (flags),
    handle_ (handle)
{
}

void
ACE_WIN32_Asynch_Write_Dgram_Result::complete (size_t bytes_transferred,
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
  for (ACE_Message_Block* mb = this->message_block_;
       (mb != 0) && (bytes_transferred > 0);
       mb = mb->cont ())
    {
      size_t len_part = mb->length ();

      if ( len_part > bytes_transferred)
        len_part = bytes_transferred;

      mb->rd_ptr (len_part);

      bytes_transferred -= len_part;
    }

  // Create the interface result class.
  ACE_Asynch_Write_Dgram::Result result (this);

  // Call the application handler.
  this->handler_.handle_write_dgram (result);
}

ACE_WIN32_Asynch_Write_Dgram_Result::~ACE_WIN32_Asynch_Write_Dgram_Result (void)
{
}


//***********************************************

ACE_WIN32_Asynch_Write_Dgram::~ACE_WIN32_Asynch_Write_Dgram (void)
{
}

ssize_t
ACE_WIN32_Asynch_Write_Dgram::send (ACE_Message_Block *message_block,
                                    size_t &number_of_bytes_sent,
                                    int flags,
                                    const ACE_Addr &addr,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
  number_of_bytes_sent = 0;

  size_t bytes_to_write = 0;

  iovec  iov[ACE_IOV_MAX];
  int    iovcnt = 0;

  for (const ACE_Message_Block* msg = message_block;
       msg != 0 && iovcnt < ACE_IOV_MAX;
       msg = msg->cont () , ++iovcnt )
  {
    size_t msg_len = msg->length ();

    // OS should process zero length block correctly
    // if ( msg_len == 0 )
    //   ACE_ERROR_RETURN ((LM_ERROR,
    //                      ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_Dgram::send:")
    //                      ACE_LIB_TEXT ("Zero-length message block\n")),
    //                     -1);

    bytes_to_write += msg_len;

    // Make as many iovec as needed to fit all of msg_len.
    size_t rd_ptr_offset = 0;
    while (msg_len > 0 && iovcnt < ACE_IOV_MAX)
      {
        u_long this_chunk_length;
        if (msg_len > ULONG_MAX)
          this_chunk_length = ULONG_MAX;
        else
          this_chunk_length = ACE_static_cast (u_long, msg_len);
        // Collect the data in the iovec.
        iov[iovcnt].iov_base = msg->rd_ptr () + rd_ptr_offset;
        iov[iovcnt].iov_len  = this_chunk_length;
        msg_len -= this_chunk_length;
        rd_ptr_offset += this_chunk_length;

        // Increment iovec counter if there's more to do.
        if (msg_len > 0)
          iovcnt++;
      }
    if (msg_len > 0)       // Ran out of iovecs before msg_space exhausted
      {
        errno = ERANGE;
        return -1;
      }
  }

  if ( bytes_to_write == 0 )
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("ACE_WIN32_Asynch_Write_Dgram::send:")
                         ACE_LIB_TEXT ("Attempt to write 0 bytes\n")),
                        -1);

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Write_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_Dgram_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       bytes_to_write,
                                                       flags,
                                                       act,
                                                       this->win32_proactor_->get_handle (),
                                                       priority,
                                                       signal_number),
                  -1);

  // do the scatter/gather send

  int initiate_result = ACE_OS::sendto (result->handle (),
                                        iov,
                                        iovcnt,
                                        number_of_bytes_sent,
                                        result->flags_,
                                        (sockaddr *) addr.get_addr (),
                                        addr.get_size(),
                                        result,
                                        0);


  if (initiate_result == SOCKET_ERROR)
  {
    // If initiate failed, check for a bad error.
    ACE_OS::set_errno_to_last_error ();
    switch (errno)
    {
      case ERROR_IO_PENDING:
        // The IO will complete proactively: the OVERLAPPED will still
        // get queued.
        initiate_result = 0;
        break;

      default:
        // Something else went wrong: the OVERLAPPED will not get
        // queued.

        if (ACE::debug ())
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_LIB_TEXT ("%p\n"),
                      ACE_LIB_TEXT ("WSASendTo")));
        }

        delete result;
        initiate_result = -1;
        break;
    }

  }
  else
  {
    // Immediate success: the OVERLAPPED will still get queued.
    // number_of_bytes_recvd contains the number of bytes recvd
    // addr contains the peer address
    // flags was updated

    // number_of_bytes_sent = bytes_sent;
    initiate_result = 1;
  }

  return initiate_result;
}

int
ACE_WIN32_Asynch_Write_Dgram::open (ACE_Handler &handler,
                                    ACE_HANDLE handle,
                                    const void *completion_key,
                                    ACE_Proactor *proactor)
{
  return ACE_WIN32_Asynch_Operation::open (handler,
                                           handle,
                                           completion_key,
                                           proactor);
}

int
ACE_WIN32_Asynch_Write_Dgram::cancel (void)
{
  return ACE_WIN32_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_WIN32_Asynch_Write_Dgram::proactor (void) const
{
  return ACE_WIN32_Asynch_Operation::proactor ();
}

ACE_WIN32_Asynch_Write_Dgram::ACE_WIN32_Asynch_Write_Dgram (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Dgram_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Entry<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Const_Iterator_Base<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Const_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>;

#elif  defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Const_Iterator_Base<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Const_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_WIN32_Asynch_Connect_Result *, ACE_SYNCH_NULL_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_WIN32 || ACE_HAS_WINCE */
