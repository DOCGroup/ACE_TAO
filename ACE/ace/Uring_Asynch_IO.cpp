//=============================================================================
/**
 *  @file    Uring_Asynch_IO.cpp
 *
 *  The Linux io_uring asynchronous operation implementations.
 */
//=============================================================================

#include "Uring_Asynch_IO.h"

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "ace/Flag_Manip.h"
#include "ace/Message_Block.h"
#include "ace/Asynch_IO.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Addr.h"
#include "ace/Log_Category.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  int
  ace_uring_build_iovecs (ACE_Message_Block *message_block,
                          size_t requested_bytes,
                          bool write_operation,
                          struct iovec *&iovec_array,
                          unsigned int &iovec_count,
                          size_t &prepared_bytes)
  {
    iovec_array = 0;
    iovec_count = 0;
    prepared_bytes = 0;

    if (message_block == 0)
      {
        errno = EFAULT;
        return -1;
      }

    for (ACE_Message_Block *mb = message_block;
         mb != 0 && prepared_bytes < requested_bytes && iovec_count < ACE_IOV_MAX;
         mb = mb->cont ())
      {
        size_t const available = write_operation ? mb->length () : mb->space ();
        if (available == 0)
          continue;

        size_t const remaining = requested_bytes - prepared_bytes;
        size_t const len = available > remaining ? remaining : available;
        if (len == 0)
          break;

        ++iovec_count;
        prepared_bytes += len;
      }

    if (iovec_count == 0 || prepared_bytes == 0)
      {
        errno = EINVAL;
        return -1;
      }

    ACE_NEW_RETURN (iovec_array, struct iovec[iovec_count], -1);

    prepared_bytes = 0;
    unsigned int index = 0;
    for (ACE_Message_Block *mb = message_block;
         mb != 0 && prepared_bytes < requested_bytes && index < iovec_count;
         mb = mb->cont ())
      {
        size_t const available = write_operation ? mb->length () : mb->space ();
        if (available == 0)
          continue;

        size_t const remaining = requested_bytes - prepared_bytes;
        size_t const len = available > remaining ? remaining : available;
        iovec_array[index].iov_base = write_operation
          ? static_cast<void *> (mb->rd_ptr ())
          : static_cast<void *> (mb->wr_ptr ());
        iovec_array[index].iov_len = len;
        prepared_bytes += len;
        ++index;
      }

    iovec_count = index;
    return 0;
  }

  void
  ace_uring_advance_read_chain (ACE_Message_Block *message_block,
                                size_t bytes_transferred)
  {
    for (ACE_Message_Block *mb = message_block;
         mb != 0 && bytes_transferred > 0;
         mb = mb->cont ())
      {
        size_t len_part = mb->space ();
        if (len_part > bytes_transferred)
          len_part = bytes_transferred;

        mb->wr_ptr (len_part);
        bytes_transferred -= len_part;
      }
  }

  void
  ace_uring_advance_write_chain (ACE_Message_Block *message_block,
                                 size_t bytes_transferred)
  {
    for (ACE_Message_Block *mb = message_block;
         mb != 0 && bytes_transferred > 0;
         mb = mb->cont ())
      {
        size_t len_part = mb->length ();
        if (len_part > bytes_transferred)
          len_part = bytes_transferred;

        mb->rd_ptr (len_part);
        bytes_transferred -= len_part;
      }
  }
}

// ---------------------------------------------------------------------------
// Base Result
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Result::ACE_Uring_Asynch_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   const void *act,
   ACE_HANDLE handle,
   u_long offset,
   u_long offset_high,
   ACE_Proactor *proactor)
  : handler_ (0),
    handler_proxy_ (handler_proxy),
    act_ (act),
    completion_key_ (0),
    handle_ (handle),
    offset_ (offset),
    offset_high_ (offset_high),
    proactor_ (proactor),
    bytes_transferred_ (0),
    error_ (0),
    owner_ (0)
{
  ACE_Handler::Proxy *const proxy = this->handler_proxy_.get ();
  this->handler_ = proxy != 0 ? proxy->handler () : 0;
}

ACE_Uring_Asynch_Result::~ACE_Uring_Asynch_Result (void)
{
}

ACE_Handler *
ACE_Uring_Asynch_Result::handler (void) const
{
  ACE_Handler::Proxy *const proxy = this->handler_proxy_.get ();
  return proxy != 0 ? proxy->handler () : 0;
}

ACE_Handler *
ACE_Uring_Asynch_Result::dispatch_handler (void) const
{
  return this->handler_;
}

size_t
ACE_Uring_Asynch_Result::bytes_transferred (void) const
{
  return this->bytes_transferred_;
}

void
ACE_Uring_Asynch_Result::set_bytes_transferred (size_t n)
{
  this->bytes_transferred_ = n;
}

u_long
ACE_Uring_Asynch_Result::error (void) const
{
  return this->error_;
}

void
ACE_Uring_Asynch_Result::set_error (u_long err)
{
  this->error_ = err;
}

const void *
ACE_Uring_Asynch_Result::act (void) const
{
  return this->act_;
}

int
ACE_Uring_Asynch_Result::success (void) const
{
  return this->error_ == 0;
}

const void *
ACE_Uring_Asynch_Result::completion_key (void) const
{
  return this->completion_key_;
}

ACE_HANDLE
ACE_Uring_Asynch_Result::event (void) const
{
  return ACE_INVALID_HANDLE;
}

u_long
ACE_Uring_Asynch_Result::offset (void) const
{
  return this->offset_;
}

u_long
ACE_Uring_Asynch_Result::offset_high (void) const
{
  return this->offset_high_;
}

int
ACE_Uring_Asynch_Result::priority (void) const
{
  return 0;
}

int
ACE_Uring_Asynch_Result::signal_number (void) const
{
  return 0;
}

void
ACE_Uring_Asynch_Result::owner (ACE_Uring_Asynch_Operation *operation)
{
  this->owner_ = operation;
}

ACE_Uring_Asynch_Operation *
ACE_Uring_Asynch_Result::owner (void) const
{
  return this->owner_.value ();
}

void
ACE_Uring_Asynch_Result::completion_key (const void *completion_key)
{
  this->completion_key_ = completion_key;
}

int
ACE_Uring_Asynch_Result::post_completion (ACE_Proactor_Impl *proactor_impl)
{
  ACE_Uring_Proactor *const up = dynamic_cast<ACE_Uring_Proactor *> (proactor_impl);
  if (up == 0)
    return -1;

  if (!up->is_initialized ())
    return -1;

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, up->sq_mutex (), -1);
  struct io_uring_sqe *const sqe = up->get_sqe ();
  if (!sqe)
    return -1;
  ::io_uring_prep_nop (sqe);
  ::io_uring_sqe_set_data (sqe, this);
  return up->submit_sqe () < 0 ? -1 : 0;
}

// ---------------------------------------------------------------------------
// Timer
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Timer::ACE_Uring_Asynch_Timer
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   const void *act,
   const ACE_Time_Value &tv,
   ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy, act, ACE_INVALID_HANDLE, 0, 0, proactor),
    time_ (tv)
{
}

void
ACE_Uring_Asynch_Timer::complete (size_t, int, const void *, u_long)
{
  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    handler->handle_time_out (this->time_, this->act_);
  delete this;
}

// ---------------------------------------------------------------------------
// Base Operation
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Operation::ACE_Uring_Asynch_Operation (ACE_Uring_Proactor *proactor)
  : uring_proactor_ (proactor),
    proactor_ (0),
    completion_key_ (0),
    handle_ (ACE_INVALID_HANDLE)
{
}

ACE_Uring_Asynch_Operation::~ACE_Uring_Asynch_Operation (void)
{
  if (this->uring_proactor_ != 0 && this->uring_proactor_->is_initialized ())
    (void) this->cancel ();
}

int
ACE_Uring_Asynch_Operation::open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                  ACE_HANDLE handle,
                                  const void *completion_key,
                                  ACE_Proactor *proactor)
{
  this->handler_proxy_ = handler_proxy;
  this->completion_key_ = completion_key;
  this->handle_ = handle;
  this->proactor_ = proactor;

  // Grab the handle from the handler if none was provided
  // (matches POSIX behavior).
  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      ACE_Handler *const h = handler_proxy.get ()->handler ();
      if (h != 0)
        this->handle_ = h->handle ();
    }
  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  return 0;
}

int
ACE_Uring_Asynch_Operation::cancel (void)
{
  if (this->uring_proactor_ == 0)
    return -1;

  ACE_GUARD_RETURN (ACE_Thread_Mutex, sq_mon, this->uring_proactor_->sq_mutex (), -1);
  ACE_GUARD_RETURN (ACE_Thread_Mutex, pending_mon, this->pending_results_lock_, -1);

  if (this->pending_results_.is_empty ())
    return 1;

  ACE_Unbounded_Set<ACE_Uring_Asynch_Result *>::iterator const the_end =
    this->pending_results_.end ();
  for (ACE_Unbounded_Set<ACE_Uring_Asynch_Result *>::iterator i =
         this->pending_results_.begin ();
       i != the_end;
       ++i)
    {
      struct io_uring_sqe *sqe = this->uring_proactor_->get_sqe ();
      if (!sqe)
        {
          int const submit_result = this->uring_proactor_->submit_pending_sqe ();
          if (submit_result < 0)
            {
              errno = -submit_result;
              return -1;
            }

          sqe = this->uring_proactor_->get_sqe ();
          if (!sqe)
            {
              errno = EAGAIN;
              return -1;
            }
        }

      ::io_uring_prep_cancel (sqe, *i, 0);
      ::io_uring_sqe_set_data (sqe, 0);
    }

  int const submit_result = this->uring_proactor_->submit_pending_sqe ();
  if (submit_result < 0)
    {
      errno = -submit_result;
      return -1;
    }

  return 0;
}

ACE_Proactor *
ACE_Uring_Asynch_Operation::proactor (void) const
{
  return this->proactor_;
}

ACE_Handler *
ACE_Uring_Asynch_Operation::handler (void)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->pending_results_lock_, 0);
  ACE_Handler::Proxy *const proxy = this->handler_proxy_.get ();
  return proxy != 0 ? proxy->handler () : 0;
}

int
ACE_Uring_Asynch_Operation::queue_result (ACE_Uring_Asynch_Result *result)
{
  this->register_result (result);

  if (this->uring_proactor_->signal_submitter_locked () == -1)
    {
      int const submit_result = this->uring_proactor_->submit_pending_sqe ();
      if (submit_result >= 0)
        return 0;

      if (submit_result != -1)
        errno = -submit_result;

      return -1;
    }

  return 0;
}

int
ACE_Uring_Asynch_Operation::submit_result (ACE_Uring_Asynch_Result *result)
{
  return this->queue_result (result);
}

void
ACE_Uring_Asynch_Operation::register_result (ACE_Uring_Asynch_Result *result)
{
  if (result == 0)
    return;

  result->completion_key (this->completion_key_);

  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->pending_results_lock_);
  result->owner (this);
  this->pending_results_.insert (result);
}

void
ACE_Uring_Asynch_Operation::unregister_result (ACE_Uring_Asynch_Result *result)
{
  if (result == 0)
    return;

  ACE_GUARD (ACE_Thread_Mutex, ace_mon, this->pending_results_lock_);
  this->pending_results_.remove (result);
  result->owner (0);
}

// ---------------------------------------------------------------------------
// Read Stream and File Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Read_Stream_Result::ACE_Uring_Asynch_Read_Stream_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block *message_block,
   size_t bytes_to_read,
   const void *act,
   ACE_Proactor *proactor,
   u_long offset,
   u_long offset_high,
   bool vectored,
   struct iovec *iovec)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             handle,
                             offset,
                             offset_high,
                             proactor),
    message_block_ (message_block),
    bytes_to_read_ (bytes_to_read),
    vectored_ (vectored),
    iovec_ (iovec)
{
}

ACE_Uring_Asynch_Read_Stream_Result::~ACE_Uring_Asynch_Read_Stream_Result (void)
{
  delete [] this->iovec_;
}

size_t
ACE_Uring_Asynch_Read_Stream_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_Uring_Asynch_Read_Stream_Result::message_block (void) const
{
  return *this->message_block_;
}

ACE_HANDLE
ACE_Uring_Asynch_Read_Stream_Result::handle (void) const
{
  return this->handle_;
}

void
ACE_Uring_Asynch_Read_Stream_Result::complete (size_t bytes_transferred,
                                               int success,
                                               const void *,
                                               u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    {
      if (this->vectored_)
        ace_uring_advance_read_chain (this->message_block_, bytes_transferred);
      else
        this->message_block_->wr_ptr (bytes_transferred);
    }

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Read_Stream::Result result (this);
      handler->handle_read_stream (result);
    }
  delete this;
}

ACE_Uring_Asynch_Read_Stream::ACE_Uring_Asynch_Read_Stream
  (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

int
ACE_Uring_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                                    size_t num_bytes_to_read,
                                    const void *act,
                                    int,
                                    int)
{
  size_t const space = message_block.space ();
  if (num_bytes_to_read > space) num_bytes_to_read = space;

  ACE_Uring_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Read_Stream_Result (this->handler_proxy_,
                                                       this->handle_,
                                                       &message_block,
                                                       num_bytes_to_read,
                                                       act,
                                                       this->proactor_),
                  -1);

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_read (sqe,
                        this->handle_,
                        message_block.wr_ptr (),
                        (unsigned int) num_bytes_to_read,
                        0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Read_Stream::readv (ACE_Message_Block &message_block,
                                     size_t num_bytes_to_read,
                                     const void *act,
                                     int,
                                     int)
{
  struct iovec *iovec = 0;
  unsigned int iovec_count = 0;
  if (ace_uring_build_iovecs (&message_block,
                              num_bytes_to_read,
                              false,
                              iovec,
                              iovec_count,
                              num_bytes_to_read) == -1)
    return -1;

  ACE_Uring_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_NORETURN (result,
                    ACE_Uring_Asynch_Read_Stream_Result (this->handler_proxy_,
                                                         this->handle_,
                                                         &message_block,
                                                         num_bytes_to_read,
                                                         act,
                                                         this->proactor_,
                                                         0,
                                                         0,
                                                         true,
                                                         iovec));
  if (result == 0)
    {
      delete [] iovec;
      errno = ENOMEM;
      return -1;
    }

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_readv (sqe,
                         this->handle_,
                         iovec,
                         iovec_count,
                         0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

ACE_Uring_Asynch_Read_File::ACE_Uring_Asynch_Read_File
  (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Read_Stream (proactor)
{
}

int
ACE_Uring_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                  size_t num_bytes_to_read,
                                  u_long offset,
                                  u_long offset_high,
                                  const void *act,
                                  int,
                                  int)
{
  size_t const space = message_block.space ();
  if (num_bytes_to_read > space) num_bytes_to_read = space;

  ACE_Uring_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Read_File_Result (this->handler_proxy_,
                                                     this->handle_,
                                                     &message_block,
                                                     num_bytes_to_read,
                                                     act,
                                                     this->proactor_,
                                                     offset,
                                                     offset_high),
                  -1);

  ACE_UINT64 const full_offset =
    (static_cast<ACE_UINT64> (offset_high) << 32) | offset;

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_read (sqe,
                        this->handle_,
                        message_block.wr_ptr (),
                        (unsigned int) num_bytes_to_read,
                        full_offset);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                  size_t num_bytes_to_read,
                                  const void *act,
                                  int,
                                  int)
{
  size_t const space = message_block.space ();
  if (num_bytes_to_read > space) num_bytes_to_read = space;

  ACE_Uring_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Read_File_Result (this->handler_proxy_,
                                                     this->handle_,
                                                     &message_block,
                                                     num_bytes_to_read,
                                                     act,
                                                     this->proactor_,
                                                     0,
                                                     0),
                  -1);

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_read (sqe,
                        this->handle_,
                        message_block.wr_ptr (),
                        (unsigned int) num_bytes_to_read,
                        0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Read_File::readv (ACE_Message_Block &message_block,
                                   size_t num_bytes_to_read,
                                   u_long offset,
                                   u_long offset_high,
                                   const void *act,
                                   int,
                                   int)
{
  struct iovec *iovec = 0;
  unsigned int iovec_count = 0;
  if (ace_uring_build_iovecs (&message_block,
                              num_bytes_to_read,
                              false,
                              iovec,
                              iovec_count,
                              num_bytes_to_read) == -1)
    return -1;

  ACE_Uring_Asynch_Read_File_Result *result = 0;
  ACE_NEW_NORETURN (result,
                    ACE_Uring_Asynch_Read_File_Result (this->handler_proxy_,
                                                       this->handle_,
                                                       &message_block,
                                                       num_bytes_to_read,
                                                       act,
                                                       this->proactor_,
                                                       offset,
                                                       offset_high,
                                                       true,
                                                       iovec));
  if (result == 0)
    {
      delete [] iovec;
      errno = ENOMEM;
      return -1;
    }

  ACE_UINT64 const full_offset =
    (static_cast<ACE_UINT64> (offset_high) << 32) | offset;

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_readv (sqe,
                         this->handle_,
                         iovec,
                         iovec_count,
                         full_offset);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Read_File::readv (ACE_Message_Block &message_block,
                                   size_t num_bytes_to_read,
                                   const void *act,
                                   int priority,
                                   int signal_number)
{
  return this->readv (message_block,
                      num_bytes_to_read,
                      0,
                      0,
                      act,
                      priority,
                      signal_number);
}

ACE_Uring_Asynch_Read_File_Result::ACE_Uring_Asynch_Read_File_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block *message_block,
   size_t bytes_to_read,
   const void *act,
   ACE_Proactor *proactor,
   u_long offset,
   u_long offset_high,
   bool vectored,
   struct iovec *iovec)
  : ACE_Uring_Asynch_Read_Stream_Result (handler_proxy,
                                         handle,
                                         message_block,
                                         bytes_to_read,
                                         act,
                                         proactor,
                                         offset,
                                         offset_high,
                                         vectored,
                                         iovec)
{
}

void
ACE_Uring_Asynch_Read_File_Result::complete (size_t bytes_transferred,
                                             int success,
                                             const void *,
                                             u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    {
      if (this->vectored_)
        ace_uring_advance_read_chain (this->message_block_, bytes_transferred);
      else
        this->message_block_->wr_ptr (bytes_transferred);
    }

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Read_File::Result result (this);
      handler->handle_read_file (result);
    }
  delete this;
}

// ---------------------------------------------------------------------------
// Write Stream and File Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Write_Stream_Result::ACE_Uring_Asynch_Write_Stream_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block *message_block,
   size_t bytes_to_write,
   const void *act,
   ACE_Proactor *proactor,
   u_long offset,
   u_long offset_high,
   bool vectored,
   struct iovec *iovec)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             handle,
                             offset,
                             offset_high,
                             proactor),
    message_block_ (message_block),
    bytes_to_write_ (bytes_to_write),
    vectored_ (vectored),
    iovec_ (iovec)
{
}

ACE_Uring_Asynch_Write_Stream_Result::~ACE_Uring_Asynch_Write_Stream_Result (void)
{
  delete [] this->iovec_;
}

size_t
ACE_Uring_Asynch_Write_Stream_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

ACE_Message_Block &
ACE_Uring_Asynch_Write_Stream_Result::message_block (void) const
{
  return *this->message_block_;
}

ACE_HANDLE
ACE_Uring_Asynch_Write_Stream_Result::handle (void) const
{
  return this->handle_;
}

void
ACE_Uring_Asynch_Write_Stream_Result::complete (size_t bytes_transferred,
                                                int success,
                                                const void *,
                                                u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    {
      if (this->vectored_)
        ace_uring_advance_write_chain (this->message_block_, bytes_transferred);
      else
        this->message_block_->rd_ptr (bytes_transferred);
    }

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Write_Stream::Result result (this);
      handler->handle_write_stream (result);
    }
  delete this;
}

ACE_Uring_Asynch_Write_Stream::ACE_Uring_Asynch_Write_Stream
  (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

int
ACE_Uring_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                      size_t bytes_to_write,
                                      const void *act,
                                      int,
                                      int)
{
  size_t const length = message_block.length ();
  if (bytes_to_write > length) bytes_to_write = length;

  ACE_Uring_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Write_Stream_Result (this->handler_proxy_,
                                                        this->handle_,
                                                        &message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->proactor_),
                  -1);

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_write (sqe,
                         this->handle_,
                         message_block.rd_ptr (),
                         (unsigned int) bytes_to_write,
                         0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Write_Stream::writev (ACE_Message_Block &message_block,
                                       size_t bytes_to_write,
                                       const void *act,
                                       int,
                                       int)
{
  struct iovec *iovec = 0;
  unsigned int iovec_count = 0;
  if (ace_uring_build_iovecs (&message_block,
                              bytes_to_write,
                              true,
                              iovec,
                              iovec_count,
                              bytes_to_write) == -1)
    return -1;

  ACE_Uring_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_NORETURN (result,
                    ACE_Uring_Asynch_Write_Stream_Result (this->handler_proxy_,
                                                          this->handle_,
                                                          &message_block,
                                                          bytes_to_write,
                                                          act,
                                                          this->proactor_,
                                                          0,
                                                          0,
                                                          true,
                                                          iovec));
  if (result == 0)
    {
      delete [] iovec;
      errno = ENOMEM;
      return -1;
    }

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_writev (sqe,
                          this->handle_,
                          iovec,
                          iovec_count,
                          0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

ACE_Uring_Asynch_Write_File::ACE_Uring_Asynch_Write_File
  (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Write_Stream (proactor)
{
}

int
ACE_Uring_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                    size_t bytes_to_write,
                                    u_long offset,
                                    u_long offset_high,
                                    const void *act,
                                    int,
                                    int)
{
  size_t const length = message_block.length ();
  if (bytes_to_write > length) bytes_to_write = length;

  ACE_Uring_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Write_File_Result (this->handler_proxy_,
                                                      this->handle_,
                                                      &message_block,
                                                      bytes_to_write,
                                                      act,
                                                      this->proactor_,
                                                      offset,
                                                      offset_high),
                  -1);

  ACE_UINT64 const full_offset =
    (static_cast<ACE_UINT64> (offset_high) << 32) | offset;

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_write (sqe,
                         this->handle_,
                         message_block.rd_ptr (),
                         (unsigned int) bytes_to_write,
                         full_offset);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                    size_t bytes_to_write,
                                    const void *act,
                                    int,
                                    int)
{
  size_t const length = message_block.length ();
  if (bytes_to_write > length) bytes_to_write = length;

  ACE_Uring_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Write_File_Result (this->handler_proxy_,
                                                      this->handle_,
                                                      &message_block,
                                                      bytes_to_write,
                                                      act,
                                                      this->proactor_,
                                                      0,
                                                      0),
                  -1);

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_write (sqe,
                         this->handle_,
                         message_block.rd_ptr (),
                         (unsigned int) bytes_to_write,
                         0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Write_File::writev (ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     u_long offset,
                                     u_long offset_high,
                                     const void *act,
                                     int,
                                     int)
{
  struct iovec *iovec = 0;
  unsigned int iovec_count = 0;
  if (ace_uring_build_iovecs (&message_block,
                              bytes_to_write,
                              true,
                              iovec,
                              iovec_count,
                              bytes_to_write) == -1)
    return -1;

  ACE_Uring_Asynch_Write_File_Result *result = 0;
  ACE_NEW_NORETURN (result,
                    ACE_Uring_Asynch_Write_File_Result (this->handler_proxy_,
                                                        this->handle_,
                                                        &message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->proactor_,
                                                        offset,
                                                        offset_high,
                                                        true,
                                                        iovec));
  if (result == 0)
    {
      delete [] iovec;
      errno = ENOMEM;
      return -1;
    }

  ACE_UINT64 const full_offset =
    (static_cast<ACE_UINT64> (offset_high) << 32) | offset;

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_writev (sqe,
                          this->handle_,
                          iovec,
                          iovec_count,
                          full_offset);
  ::io_uring_sqe_set_data (sqe, result);
  return this->queue_result (result);
}

int
ACE_Uring_Asynch_Write_File::writev (ACE_Message_Block &message_block,
                                     size_t bytes_to_write,
                                     const void *act,
                                     int priority,
                                     int signal_number)
{
  return this->writev (message_block,
                       bytes_to_write,
                       0,
                       0,
                       act,
                       priority,
                       signal_number);
}

ACE_Uring_Asynch_Write_File_Result::ACE_Uring_Asynch_Write_File_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block *message_block,
   size_t bytes_to_write,
   const void *act,
   ACE_Proactor *proactor,
   u_long offset,
   u_long offset_high,
   bool vectored,
   struct iovec *iovec)
  : ACE_Uring_Asynch_Write_Stream_Result (handler_proxy,
                                          handle,
                                          message_block,
                                          bytes_to_write,
                                          act,
                                          proactor,
                                          offset,
                                          offset_high,
                                          vectored,
                                          iovec)
{
}

void
ACE_Uring_Asynch_Write_File_Result::complete (size_t bytes_transferred,
                                              int success,
                                              const void *,
                                              u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    {
      if (this->vectored_)
        ace_uring_advance_write_chain (this->message_block_, bytes_transferred);
      else
        this->message_block_->rd_ptr (bytes_transferred);
    }

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Write_File::Result result (this);
      handler->handle_write_file (result);
    }
  delete this;
}

// ---------------------------------------------------------------------------
// Accept Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Accept_Result::ACE_Uring_Asynch_Accept_Result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE listen_handle,
   ACE_HANDLE accept_handle,
   ACE_Message_Block *message_block,
   size_t bytes_to_read,
   const void *act,
   ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             listen_handle,
                             0,
                             0,
                             proactor),
    accept_handle_ (accept_handle),
    message_block_ (message_block),
    bytes_to_read_ (bytes_to_read),
    addr_len_ (sizeof (struct sockaddr_storage))
{
}

ACE_HANDLE
ACE_Uring_Asynch_Accept_Result::accept_handle (void) const
{
  return this->accept_handle_;
}

ACE_Message_Block &
ACE_Uring_Asynch_Accept_Result::message_block (void) const
{
  return *this->message_block_;
}

ACE_HANDLE
ACE_Uring_Asynch_Accept_Result::listen_handle (void) const
{
  return this->handle_;
}

size_t
ACE_Uring_Asynch_Accept_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

struct sockaddr *
ACE_Uring_Asynch_Accept_Result::addr (void)
{
  return reinterpret_cast<struct sockaddr *> (&this->client_addr_);
}

socklen_t *
ACE_Uring_Asynch_Accept_Result::addrlen (void)
{
  return &this->addr_len_;
}

void
ACE_Uring_Asynch_Accept_Result::complete (size_t bytes_transferred,
                                          int success,
                                          const void *,
                                          u_long error)
{
  // io_uring returns the accepted fd in cqe->res for accept operations.
  // Preserve that in accept_handle() and report zero transferred bytes,
  // matching ACE's accept result contract.
  this->bytes_transferred_ = 0;
  this->error_ = error;
  if (success)
    this->accept_handle_ = (ACE_HANDLE) bytes_transferred;

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Accept::Result result (this);
      handler->handle_accept (result);
    }
  delete this;
}

ACE_Uring_Asynch_Accept::ACE_Uring_Asynch_Accept
  (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

int
ACE_Uring_Asynch_Accept::accept (ACE_Message_Block &message_block,
                                 size_t bytes_to_read,
                                 ACE_HANDLE accept_handle,
                                 const void *act,
                                 int,
                                 int,
                                 int)
{
  ACE_Uring_Asynch_Accept_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Accept_Result (this->handler_proxy_,
                                                  this->handle_,
                                                  accept_handle,
                                                  &message_block,
                                                  bytes_to_read,
                                                  act,
                                                  this->proactor_),
                  -1);

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_accept (sqe, this->handle_, result->addr (), result->addrlen (), 0);
  ::io_uring_sqe_set_data (sqe, result);
  return this->submit_result (result);
}

// ---------------------------------------------------------------------------
// Connect Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Connect_Result::ACE_Uring_Asynch_Connect_Result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE connect_handle,
    const void *act,
    ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             connect_handle,
                             0,
                             0,
                             proactor)
  , connect_handle_ (connect_handle)
{
}

void
ACE_Uring_Asynch_Connect_Result::complete (size_t bytes_transferred,
                                           int /*success*/,
                                           const void *,
                                           u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Connect::Result result (this);
      handler->handle_connect (result);
    }
  delete this;
}

ACE_HANDLE ACE_Uring_Asynch_Connect_Result::connect_handle (void) const
{
  return this->connect_handle_;
}

void
ACE_Uring_Asynch_Connect_Result::connect_handle (ACE_HANDLE handle)
{
  this->connect_handle_ = handle;
}

ACE_Uring_Asynch_Connect::ACE_Uring_Asynch_Connect (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

int
ACE_Uring_Asynch_Connect::open (const ACE_Handler::Proxy_Ptr &handler_proxy,
                                ACE_HANDLE handle,
                                const void *completion_key,
                                ACE_Proactor *proactor)
{
  // Call the base class but ignore failure when handle is INVALID_HANDLE:
  // the connect socket is not known yet, it will be created per-connect call.
  ACE_Uring_Asynch_Operation::open (handler_proxy, handle, completion_key, proactor);
  this->handler_proxy_ = handler_proxy;
  this->proactor_ = proactor;
  return 0;
}

int
ACE_Uring_Asynch_Connect::connect (ACE_HANDLE connect_handle,
                                   const ACE_Addr &remote_sap,
                                   const ACE_Addr &local_sap,
                                   int reuse_addr,
                                   const void *act,
                                   int,
                                   int)
{
  ACE_Uring_Asynch_Connect_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Connect_Result (this->handler_proxy_,
                                                   connect_handle,
                                                   act,
                                                   this->proactor_),
                  -1);

  ACE_HANDLE handle = result->connect_handle ();
  bool created_handle = false;
  bool success = false;

  if (handle == ACE_INVALID_HANDLE)
    {
      int const protocol_family = remote_sap.get_type ();

      handle = ACE_OS::socket (protocol_family,
                               SOCK_STREAM,
                               0);
      // Save it.
      result->connect_handle (handle);
      created_handle = (handle != ACE_INVALID_HANDLE);
      if (handle == ACE_INVALID_HANDLE)
        {
          result->set_error (errno);
          ACELIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("ACE_Uring_Asynch_Connect::connect_i: %p\n"),
                         ACE_TEXT ("socket")));
        }
      else
        {
          // Reuse the address.
          int const one = 1;
          if (protocol_family != PF_UNIX &&
              reuse_addr != 0 &&
              ACE_OS::setsockopt (handle,
                                  SOL_SOCKET,
                                  SO_REUSEADDR,
                                  (const char *) &one,
                                  sizeof one) == -1)
            {
              result->set_error (errno);
              ACELIB_ERROR ((LM_ERROR,
                             ACE_TEXT ("ACE_Uring_Asynch_Connect::connect_i: %p\n"),
                             ACE_TEXT ("setsockopt")));
            }
        }
    }

  if (result->error () == 0 && local_sap != ACE_Addr::sap_any)
    {
      sockaddr *const laddr = reinterpret_cast<sockaddr *> (local_sap.get_addr ());
      size_t const size = local_sap.get_size ();

      if (ACE_OS::bind (handle, laddr, size) == -1)
        {
           result->set_error (errno);
           ACELIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("ACE_Uring_Asynch_Connect::connect_i: %p\n"),
                          ACE_TEXT ("bind")));
        }
    }

  // Set non-blocking mode.
  if (result->error () == 0 && ACE::set_flags (handle, ACE_NONBLOCK) != 0)
    {
      result->set_error (errno);
      ACELIB_ERROR ((LM_ERROR,
                     ACE_TEXT ("ACE_Uring_Asynch_Connect::connect_i: %p\n"),
                     ACE_TEXT ("set_flags")));
    }

  if (result->error () == 0)
    {
      ACE_GUARD_REACTION (ACE_Thread_Mutex,
                          ace_mon,
                          this->uring_proactor_->sq_mutex (),
                          {
                            if (created_handle && handle != ACE_INVALID_HANDLE)
                              {
                                ACE_OS::closesocket (handle);
                                result->connect_handle (ACE_INVALID_HANDLE);
                              }
                            delete result;
                            return -1;
                          });

      struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
      if (!sqe)
        {
          result->set_error (EAGAIN);
          errno = EAGAIN;
        }
      else
        {
          ::io_uring_prep_connect (sqe,
                                   handle,
                                   (struct sockaddr *) remote_sap.get_addr (),
                                   remote_sap.get_size ());
          ::io_uring_sqe_set_data (sqe, result);
          this->register_result (result);
          int const submit_result = this->uring_proactor_->submit_sqe ();
          if (submit_result < 0)
            {
              this->unregister_result (result);
              errno = -submit_result;
            }
          else
            {
              success = true;
            }
        }
    }

  if (success)
    return 0;

  if (created_handle && handle != ACE_INVALID_HANDLE)
    {
      ACE_OS::closesocket (handle);
      result->connect_handle (ACE_INVALID_HANDLE);
    }
  delete result;
  return -1;
}

// ---------------------------------------------------------------------------
// Datagram Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Read_Dgram_Result::ACE_Uring_Asynch_Read_Dgram_Result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE handle,
    ACE_Message_Block *message_block,
    size_t bytes_to_read,
    int flags,
    int protocol_family,
    const void *act,
    ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             handle,
                             0,
                             0,
                             proactor)
  , message_block_ (message_block)
  , bytes_to_read_ (bytes_to_read)
  , flags_ (flags)
{
  ACE_OS::memset (&this->msg_, 0, sizeof (this->msg_));
  this->iov_.iov_base = message_block->wr_ptr ();
  this->iov_.iov_len = (unsigned int)bytes_to_read;
  this->msg_.msg_iov = &this->iov_;
  this->msg_.msg_iovlen = 1;
  this->msg_.msg_name = &this->remote_addr_;
  this->msg_.msg_namelen = sizeof (this->remote_addr_);
  ACE_UNUSED_ARG (protocol_family);
}

ACE_Message_Block *
ACE_Uring_Asynch_Read_Dgram_Result::message_block (void) const
{
  return this->message_block_;
}

void
ACE_Uring_Asynch_Read_Dgram_Result::complete (size_t bytes_transferred,
                                              int success,
                                              const void *,
                                              u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    this->message_block_->wr_ptr (bytes_transferred);

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Read_Dgram::Result result (this);
      handler->handle_read_dgram (result);
    }
  delete this;
}

int
ACE_Uring_Asynch_Read_Dgram_Result::remote_address (ACE_Addr &addr) const
{
  socklen_t const name_len = this->msg_.msg_namelen;
  if (addr.get_addr () == 0
      || static_cast<socklen_t> (addr.get_size ()) < name_len)
    {
      errno = ENOSPC;
      return -1;
    }

  ACE_OS::memcpy (addr.get_addr (), &this->remote_addr_, name_len);
  addr.set_size (name_len);
  return 0;
}

int
ACE_Uring_Asynch_Read_Dgram_Result::flags (void) const
{
  return this->flags_;
}

ACE_HANDLE
ACE_Uring_Asynch_Read_Dgram_Result::handle (void) const
{
  return this->handle_;
}

size_t
ACE_Uring_Asynch_Read_Dgram_Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

struct msghdr *
ACE_Uring_Asynch_Read_Dgram_Result::msg (void)
{
  return &this->msg_;
}

ACE_Uring_Asynch_Read_Dgram::ACE_Uring_Asynch_Read_Dgram (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

ssize_t
ACE_Uring_Asynch_Read_Dgram::recv (ACE_Message_Block *message_block,
                                   size_t &/*number_of_bytes_recvd*/,
                                   int flags,
                                   int protocol_family,
                                   const void *act,
                                   int,
                                   int)
{
  ACE_Uring_Asynch_Read_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Read_Dgram_Result (this->handler_proxy_,
                                                      this->handle_,
                                                      message_block,
                                                      message_block->space (),
                                                      flags,
                                                      protocol_family,
                                                      act,
                                                      this->proactor_),
                  -1);
  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_recvmsg (sqe, this->handle_, result->msg (), flags);
  ::io_uring_sqe_set_data (sqe, result);
  return this->submit_result (result);
}

ACE_Uring_Asynch_Write_Dgram_Result::ACE_Uring_Asynch_Write_Dgram_Result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE handle,
    ACE_Message_Block *message_block,
    size_t bytes_to_write,
    int flags,
    const void *act,
    ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             handle,
                             0,
                             0,
                             proactor)
  , message_block_ (message_block)
  , bytes_to_write_ (bytes_to_write)
  , flags_ (flags)
  , remote_addr_len_ (0)
{
  ACE_OS::memset (&this->msg_, 0, sizeof (this->msg_));
  ACE_OS::memset (&this->remote_addr_, 0, sizeof (this->remote_addr_));
  this->iov_.iov_base = message_block->rd_ptr ();
  this->iov_.iov_len = (unsigned int)bytes_to_write;
  this->msg_.msg_iov = &this->iov_;
  this->msg_.msg_iovlen = 1;
}

ACE_Message_Block *
ACE_Uring_Asynch_Write_Dgram_Result::message_block (void) const
{
  return this->message_block_;
}

void
ACE_Uring_Asynch_Write_Dgram_Result::complete (size_t bytes_transferred,
                                               int success,
                                               const void *,
                                               u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  if (success && this->message_block_ != 0)
    this->message_block_->rd_ptr (bytes_transferred);

  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Write_Dgram::Result result (this);
      handler->handle_write_dgram (result);
    }
  delete this;
}

int
ACE_Uring_Asynch_Write_Dgram_Result::flags (void) const
{
  return this->flags_;
}

ACE_HANDLE
ACE_Uring_Asynch_Write_Dgram_Result::handle (void) const
{
  return this->handle_;
}

size_t
ACE_Uring_Asynch_Write_Dgram_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

struct msghdr *
ACE_Uring_Asynch_Write_Dgram_Result::msg (void)
{
  return &this->msg_;
}

int
ACE_Uring_Asynch_Write_Dgram_Result::remote_address (const ACE_Addr &addr)
{
  if (addr.get_addr () == 0
      || addr.get_size () == 0
      || static_cast<size_t> (addr.get_size ()) > sizeof (this->remote_addr_))
    {
      errno = EINVAL;
      return -1;
    }

  ACE_OS::memcpy (&this->remote_addr_, addr.get_addr (), addr.get_size ());
  this->remote_addr_len_ = static_cast<socklen_t> (addr.get_size ());
  this->msg_.msg_name = &this->remote_addr_;
  this->msg_.msg_namelen = this->remote_addr_len_;
  return 0;
}

ACE_Uring_Asynch_Write_Dgram::ACE_Uring_Asynch_Write_Dgram (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

ssize_t
ACE_Uring_Asynch_Write_Dgram::send (ACE_Message_Block *message_block,
                                    size_t &/*number_of_bytes_sent*/,
                                    int flags,
                                    const ACE_Addr &remote_addr,
                                    const void *act,
                                    int,
                                    int)
{
  ACE_Uring_Asynch_Write_Dgram_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_Uring_Asynch_Write_Dgram_Result (this->handler_proxy_,
                                                       this->handle_,
                                                       message_block,
                                                       message_block->length (),
                                                       flags,
                                                       act,
                                                       this->proactor_),
                  -1);
  if (result->remote_address (remote_addr) != 0)
    {
      delete result;
      return -1;
    }

  ACE_GUARD_REACTION (ACE_Thread_Mutex,
                      ace_mon,
                      this->uring_proactor_->sq_mutex (),
                      {
                        delete result;
                        return -1;
                      });
  struct io_uring_sqe *const sqe = this->uring_proactor_->get_sqe ();
  if (!sqe)
    {
      delete result;
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_sendmsg (sqe, this->handle_, result->msg (), flags);
  ::io_uring_sqe_set_data (sqe, result);
  return this->submit_result (result);
}

// ---------------------------------------------------------------------------
// Transmit File Impl
// ---------------------------------------------------------------------------

ACE_Uring_Asynch_Transmit_File_Result::ACE_Uring_Asynch_Transmit_File_Result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE socket,
    ACE_HANDLE file,
    ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
    size_t bytes_to_write,
    u_long offset,
    u_long offset_high,
    size_t bytes_per_send,
    u_long flags,
    const void *act,
    ACE_Proactor *proactor)
  : ACE_Uring_Asynch_Result (handler_proxy,
                             act,
                             socket,
                             offset,
                             offset_high,
                             proactor)
  , file_ (file)
  , header_and_trailer_ (header_and_trailer)
  , bytes_to_write_ (bytes_to_write)
  , bytes_per_send_ (bytes_per_send)
  , flags_ (flags)
{
}

ACE_HANDLE
ACE_Uring_Asynch_Transmit_File_Result::socket (void) const
{
  return this->handle_;
}

ACE_HANDLE
ACE_Uring_Asynch_Transmit_File_Result::file (void) const
{
  return this->file_;
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_Uring_Asynch_Transmit_File_Result::header_and_trailer (void) const
{
  return this->header_and_trailer_;
}

size_t
ACE_Uring_Asynch_Transmit_File_Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

size_t
ACE_Uring_Asynch_Transmit_File_Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long
ACE_Uring_Asynch_Transmit_File_Result::flags (void) const
{
  return this->flags_;
}

void
ACE_Uring_Asynch_Transmit_File_Result::complete (size_t bytes_transferred,
                                                 int /*success*/,
                                                 const void *,
                                                 u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->error_ = error;
  ACE_Handler *const handler = this->handler ();
  if (handler != 0)
    {
      ACE_Asynch_Transmit_File::Result result (this);
      handler->handle_transmit_file (result);
    }
  delete this;
}

ACE_Uring_Asynch_Transmit_File::ACE_Uring_Asynch_Transmit_File (ACE_Uring_Proactor *proactor)
  : ACE_Uring_Asynch_Operation (proactor)
{
}

int
ACE_Uring_Asynch_Transmit_File::transmit_file (
    ACE_HANDLE file,
    ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
    size_t bytes_to_write,
    u_long offset,
    u_long offset_high,
    size_t bytes_per_send,
    u_long flags,
    const void *act,
    int,
    int)
{
  ACE_UNUSED_ARG (file);
  ACE_UNUSED_ARG (header_and_trailer);
  ACE_UNUSED_ARG (bytes_to_write);
  ACE_UNUSED_ARG (offset);
  ACE_UNUSED_ARG (offset_high);
  ACE_UNUSED_ARG (bytes_per_send);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (act);

  // The previous implementation used splice(file -> socket), which is not
  // valid on Linux because one splice endpoint must be a pipe. Fail fast
  // until a real async transmit-file strategy is implemented for io_uring.
  errno = ENOTSUP;
  return -1;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */
