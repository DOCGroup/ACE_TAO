// $Id$

#include "ace/WIN32_Asynch_IO.h"

// ACE_RCSID(ace, Asynch_IO, "$Id$")

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))

#include "ace/WIN32_Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/Task_T.h"

u_long
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
ACE_WIN32_Asynch_Result::set_bytes_transferred (u_long nbytes)
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
        || (defined (__BORLANDC__) && (__BORLANDC__ >= 0x530)))
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

u_long
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

ACE_WIN32_Asynch_Read_Stream_Result::ACE_WIN32_Asynch_Read_Stream_Result (ACE_Handler &handler,
                                                                          ACE_HANDLE handle,
                                                                          ACE_Message_Block &message_block,
                                                                          u_long bytes_to_read,
                                                                          const void* act,
                                                                          ACE_HANDLE event,
                                                                          int priority,
                                                                          int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Stream_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    handle_ (handle)
{
}

void
ACE_WIN32_Asynch_Read_Stream_Result::complete (u_long bytes_transferred,
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
  ACE_Asynch_Read_Stream::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_stream (result);
}

ACE_WIN32_Asynch_Read_Stream_Result::~ACE_WIN32_Asynch_Read_Stream_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
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

ACE_WIN32_Asynch_Read_Stream::ACE_WIN32_Asynch_Read_Stream (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                                    u_long bytes_to_read,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
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

ACE_WIN32_Asynch_Read_Stream::~ACE_WIN32_Asynch_Read_Stream (void)
{
}

int
ACE_WIN32_Asynch_Read_Stream::shared_read (ACE_WIN32_Asynch_Read_Stream_Result *result)
{
  u_long bytes_read;

  result->set_error (0); // Clear error before starting IO.

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

u_long
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

ACE_WIN32_Asynch_Write_Stream_Result::ACE_WIN32_Asynch_Write_Stream_Result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_write,
                                                                            const void* act,
                                                                            ACE_HANDLE event,
                                                                            int priority,
                                                                            int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Stream_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_write_ (bytes_to_write),
    message_block_ (message_block),
    handle_ (handle)
{
}

void
ACE_WIN32_Asynch_Write_Stream_Result::complete (u_long bytes_transferred,
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
  this->message_block_.rd_ptr (bytes_transferred);

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

u_long
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

ACE_WIN32_Asynch_Write_Stream::ACE_WIN32_Asynch_Write_Stream (ACE_WIN32_Proactor *win32_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_WIN32_Asynch_Operation (win32_proactor)
{
}

int
ACE_WIN32_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                      u_long bytes_to_write,
                                      const void *act,
                                      int priority,
                                      int signal_number)
{
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

ACE_WIN32_Asynch_Write_Stream::~ACE_WIN32_Asynch_Write_Stream (void)
{
}

int
ACE_WIN32_Asynch_Write_Stream::shared_write (ACE_WIN32_Asynch_Write_Stream_Result *result)
{
  u_long bytes_written;

  result->set_error (0); // Clear error before starting IO.

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

ACE_WIN32_Asynch_Read_File_Result::ACE_WIN32_Asynch_Read_File_Result (ACE_Handler &handler,
                                                                      ACE_HANDLE handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      u_long offset,
                                                                      u_long offset_high,
                                                                      ACE_HANDLE event,
                                                                      int priority,
                                                                      int signal_number)
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
                                         signal_number)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_WIN32_Asynch_Read_File_Result::complete (u_long bytes_transferred,
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

u_long
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

u_long
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
                                  u_long bytes_to_read,
                                  u_long offset,
                                  u_long offset_high,
                                  const void *act,
                                  int priority,
                                  int signal_number)
{
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

ACE_WIN32_Asynch_Read_File::~ACE_WIN32_Asynch_Read_File (void)
{
}

int
ACE_WIN32_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                  u_long bytes_to_read,
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

ACE_WIN32_Asynch_Write_File_Result::ACE_WIN32_Asynch_Write_File_Result (ACE_Handler &handler,
                                                                        ACE_HANDLE handle,
                                                                        ACE_Message_Block &message_block,
                                                                        u_long bytes_to_write,
                                                                        const void* act,
                                                                        u_long offset,
                                                                        u_long offset_high,
                                                                        ACE_HANDLE event,
                                                                        int priority,
                                                                        int signal_number)
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
                                          signal_number)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void
ACE_WIN32_Asynch_Write_File_Result::complete (u_long bytes_transferred,
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

u_long
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

u_long
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
                                    u_long bytes_to_write,
                                    u_long offset,
                                    u_long offset_high,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
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

ACE_WIN32_Asynch_Write_File::~ACE_WIN32_Asynch_Write_File (void)
{
}

int
ACE_WIN32_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                    u_long bytes_to_write,
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

u_long
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

ACE_WIN32_Asynch_Accept_Result::ACE_WIN32_Asynch_Accept_Result (ACE_Handler &handler,
                                                                ACE_HANDLE listen_handle,
                                                                ACE_HANDLE accept_handle,
                                                                ACE_Message_Block &message_block,
                                                                u_long bytes_to_read,
                                                                const void* act,
                                                                ACE_HANDLE event,
                                                                int priority,
                                                                int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Accept_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_read_ (bytes_to_read),
    message_block_ (message_block),
    listen_handle_ (listen_handle),
    accept_handle_ (accept_handle)
{
}

void
ACE_WIN32_Asynch_Accept_Result::complete (u_long bytes_transferred,
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

u_long
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
ACE_WIN32_Asynch_Accept::accept  (ACE_Message_Block &message_block,
                                  u_long bytes_to_read,
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
                                    result->bytes_to_read (),
                                    address_size,
                                    address_size,
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

u_long
ACE_WIN32_Asynch_Transmit_File_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long
ACE_WIN32_Asynch_Transmit_File_Result::flags (void) const
{
  return this->flags_;
}

ACE_WIN32_Asynch_Transmit_File_Result::ACE_WIN32_Asynch_Transmit_File_Result (ACE_Handler &handler,
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
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Transmit_File_Result_Impl (),
    ACE_WIN32_Asynch_Result (handler, act, event, offset, offset_high, priority, signal_number),
    socket_ (socket),
    file_ (file),
    header_and_trailer_ (header_and_trailer),
    bytes_to_write_ (bytes_to_write),
    bytes_per_send_ (bytes_per_send),
    flags_ (flags)
{
}

void
ACE_WIN32_Asynch_Transmit_File_Result::complete (u_long bytes_transferred,
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

u_long
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
                                               u_long bytes_to_write,
                                               u_long offset,
                                               u_long offset_high,
                                               u_long bytes_per_send,
                                               u_long flags,
                                               const void *act,
                                               int priority,
                                               int signal_number)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
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
                                        result->bytes_to_write (),
                                        result->bytes_per_send (),
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

u_long
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

u_long
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

ACE_WIN32_Asynch_Read_Dgram_Result::ACE_WIN32_Asynch_Read_Dgram_Result (ACE_Handler &handler,
                                                                        ACE_HANDLE handle,
                                                                        ACE_Message_Block *message_block,
                                                                        u_long bytes_to_read,
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
  ACE_ASSERT(protocol_family == PF_INET); // only supporting INET addresses

  ACE_NEW(remote_address_, ACE_INET_Addr);
  addr_len_ = remote_address_->get_size();
}

void
ACE_WIN32_Asynch_Read_Dgram_Result::complete (u_long bytes_transferred,
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
  for (ACE_Message_Block* mb = this->message_block_; (mb != 0) && (bytes_transferred > 0); mb = mb->cont ())
  {
    if (mb->size () >= bytes_transferred)
    {
      mb->wr_ptr (bytes_transferred);
      bytes_transferred = 0;
    }
    else
    {
      mb->wr_ptr (mb->size ());
      bytes_transferred -= mb->size ();
    }
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
                                   size_t &number_of_bytes_recvd,
                                   int flags,
                                   int protocol_family,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
  number_of_bytes_recvd = 0;

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Read_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Read_Dgram_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      message_block->total_size(),
                                                      flags,
                                                      protocol_family,
                                                      act,
                                                      this->win32_proactor_->get_handle (),
                                                      priority,
                                                      signal_number),
                  -1);

  // do the scatter/gather recv
  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;
  for (const ACE_Message_Block* msg = message_block; msg != 0; msg = msg->cont ())
  {
    iov[iovcnt].iov_base  = msg->wr_ptr ();
    iov[iovcnt].iov_len   = msg->size ();
    ++iovcnt;
    if (iovcnt >= ACE_IOV_MAX)
    {
      delete result;
      return -1;
    }
  }

  size_t bytes_recvd = 0;
  int initiate_result = ACE_OS::recvfrom (result->handle (),
                                          iov,
                                          iovcnt,
                                          bytes_recvd,
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
    number_of_bytes_recvd = bytes_recvd;
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

u_long
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

u_long
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

ACE_WIN32_Asynch_Write_Dgram_Result::ACE_WIN32_Asynch_Write_Dgram_Result (ACE_Handler &handler,
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
    ACE_WIN32_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    bytes_to_write_ (bytes_to_write),
    message_block_ (message_block),
    flags_ (flags),
    handle_ (handle)
{
}

void
ACE_WIN32_Asynch_Write_Dgram_Result::complete (u_long bytes_transferred,
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
  for (ACE_Message_Block* mb = this->message_block_; (mb != 0) && (bytes_transferred > 0); mb = mb->cont ())
  {
    if (mb->length () >= bytes_transferred)
    {
      mb->rd_ptr (bytes_transferred);
      bytes_transferred = 0;
    }
    else
    {
      size_t len = mb->length ();
      mb->rd_ptr (len);
      bytes_transferred -= len;
    }
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

  // Create the Asynch_Result.
  ACE_WIN32_Asynch_Write_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_WIN32_Asynch_Write_Dgram_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       message_block->total_length(),
                                                       flags,
                                                       act,
                                                       this->win32_proactor_->get_handle (),
                                                       priority,
                                                       signal_number),
                  -1);

  // do the scatter/gather recv
  iovec iov[ACE_IOV_MAX];
  int iovcnt = 0;
  for (const ACE_Message_Block* msg = message_block; msg != 0; msg = msg->cont ())
  {
    iov[iovcnt].iov_base  = msg->rd_ptr ();
    iov[iovcnt].iov_len   = msg->length ();
    ++iovcnt;
    if (iovcnt >= ACE_IOV_MAX)
    {
      delete result;
      return -1;
    }
  }

  size_t bytes_sent = 0;
  int initiate_result = ACE_OS::sendto (result->handle (),
                                        iov,
                                        iovcnt,
                                        bytes_sent,
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
    number_of_bytes_sent = bytes_sent;
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

#endif /* ACE_WIN32 || ACE_HAS_WINCE */
