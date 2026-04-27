//=============================================================================
/**
 *  @file    Uring_Proactor.cpp
 *
 *  The Linux io_uring Proactor implementation.
 */
//=============================================================================

#include "Uring_Proactor.h"

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)

#include "Uring_Asynch_IO.h"
#include "ace/Countdown_Time.h"
#include "ace/Log_Category.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_sys_time.h"

#include <poll.h>
#include <sys/eventfd.h>
#include <unistd.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  enum
  {
    ACE_URING_SUBMIT_WAKE_USER_DATA = 1
  };

  unsigned
  queued_sqes (const io_uring &ring)
  {
    // For non-SQPOLL rings, khead tracks how many SQEs the kernel has
    // consumed from user space, so sqe_tail - *khead is the number of
    // prepared but unsubmitted/unconsumed entries.  For SQPOLL, defer to
    // liburing's helper because it uses the right acquire semantics.
    if (ring.flags & IORING_SETUP_SQPOLL)
      return ::io_uring_sq_ready (&ring);

    return ring.sq.sqe_tail - *ring.sq.khead;
  }
}

ACE_Uring_Proactor::ACE_Uring_Proactor (size_t entries)
  : ring_ ()
  , is_initialized_ (false)
  , submit_signal_pending_ (false)
  , submit_wakeup_handle_ (ACE_INVALID_HANDLE)
  , dispatch_thread_id_ ()
{
  ACE_TRACE ("ACE_Uring_Proactor::ACE_Uring_Proactor");

  int const ret = ::io_uring_queue_init (entries, &this->ring_, 0);
  if (ret < 0)
    {
      errno = -ret;
      ACELIB_ERROR ((LM_ERROR,
                     ACE_TEXT ("%p\n"),
                     ACE_TEXT ("ACE_Uring_Proactor::io_uring_queue_init")));
    }
  else
    {
      this->submit_wakeup_handle_ =
        ::eventfd (0, EFD_CLOEXEC | EFD_NONBLOCK);
      if (this->submit_wakeup_handle_ == ACE_INVALID_HANDLE)
        {
          ACELIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE_Uring_Proactor::eventfd")));
          ::io_uring_queue_exit (&this->ring_);
        }
      else
        {
          ACE_GUARD (ACE_Thread_Mutex, sq_guard, this->sq_mutex_);
          int const submit_result =
            this->arm_submit_wakeup_locked () == -1
              ? -1
              : ::io_uring_submit (&this->ring_);
          if (submit_result < 0)
            {
              errno = submit_result == -1 ? errno : -submit_result;
              ACELIB_ERROR ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("ACE_Uring_Proactor::arm_submit_wakeup_locked")));
              ::close (this->submit_wakeup_handle_);
              this->submit_wakeup_handle_ = ACE_INVALID_HANDLE;
              ::io_uring_queue_exit (&this->ring_);
            }
          else
            {
              this->is_initialized_ = true;
            }
        }
    }
}

ACE_Uring_Proactor::~ACE_Uring_Proactor (void)
{
  ACE_TRACE ("ACE_Uring_Proactor::~ACE_Uring_Proactor");
  this->close ();
}

int
ACE_Uring_Proactor::close (void)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, sq_guard, this->sq_mutex_, -1);
  ACE_GUARD_RETURN (ACE_Thread_Mutex, cq_guard, this->cq_mutex_, -1);

  if (!this->is_initialized_)
    return 0;

  this->is_initialized_ = false;
  ::io_uring_queue_exit (&this->ring_);
  if (this->submit_wakeup_handle_ != ACE_INVALID_HANDLE)
    {
      ::close (this->submit_wakeup_handle_);
      this->submit_wakeup_handle_ = ACE_INVALID_HANDLE;
    }
  return 0;
}

int
ACE_Uring_Proactor::register_handle (ACE_HANDLE, const void *)
{
  return 0;
}

int
ACE_Uring_Proactor::handle_events (ACE_Time_Value &wait_time)
{
  ACE_Countdown_Time countdown (&wait_time);
  int const result = this->process_cqes (DEFAULT_CQE_BATCH_SIZE, &wait_time);
  return result > 0 ? 1 : result;
}

int
ACE_Uring_Proactor::handle_events (void)
{
  int const result = this->process_cqes (DEFAULT_CQE_BATCH_SIZE);
  return result > 0 ? 1 : result;
}

int
ACE_Uring_Proactor::wake_up_dispatch_threads (void)
{
  return 0;
}

int
ACE_Uring_Proactor::close_dispatch_threads (int)
{
  return 0;
}

size_t
ACE_Uring_Proactor::number_of_threads (void) const
{
  return 1;
}

void
ACE_Uring_Proactor::number_of_threads (size_t)
{
}

ACE_HANDLE
ACE_Uring_Proactor::get_handle (void) const
{
  return ACE_INVALID_HANDLE;
}

int
ACE_Uring_Proactor::process_cqes (int max_to_process, const ACE_Time_Value *wait_time)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, dispatch_guard, this->dispatch_mutex_, -1);

  if (!this->is_initialized_)
    return -1;

  this->dispatch_thread_id_ = ACE_OS::thr_self ();

  if (max_to_process < 1)
    max_to_process = 1;

  int processed = 0;

  while (processed < max_to_process)
    {
      if (processed == 0)
        {
          ACE_GUARD_RETURN (ACE_Thread_Mutex, sq_guard, this->sq_mutex_, -1);
          int const submit_result = this->submit_pending_sqe ();
          if (submit_result < 0)
            {
              errno = -submit_result;
              return -1;
            }
        }

      struct io_uring_cqe *cqe = 0;
      ACE_Uring_Asynch_Result *result = 0;
      ACE_Uring_Asynch_Operation *owner = 0;
      size_t bytes_transferred = 0;
      int error = 0;
      int ret = 0;
      bool const should_wait = (processed == 0);

      {
        ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->cq_mutex_, -1);

        if (!this->is_initialized_)
          return -1;

        if (should_wait)
          {
            if (wait_time != 0)
              {
                if (*wait_time == ACE_Time_Value::zero)
                  ret = ::io_uring_peek_cqe (&this->ring_, &cqe);
                else
                  {
                    ACE_Time_Value const local_wait_time = *wait_time;
                    struct __kernel_timespec timeout;
                    timeout.tv_sec = local_wait_time.sec ();
                    timeout.tv_nsec = local_wait_time.usec () * 1000;
                    ret = ::io_uring_wait_cqe_timeout (&this->ring_,
                                                       &cqe,
                                                       &timeout);
                  }
              }
            else
              {
                ret = ::io_uring_wait_cqe (&this->ring_, &cqe);
              }
          }
        else
          {
            ret = ::io_uring_peek_cqe (&this->ring_, &cqe);
          }

        if (ret < 0)
          {
            if (ret == -ETIME || ret == -EAGAIN || ret == -EINTR)
              return processed;

            errno = -ret;
            return -1;
          }

        uintptr_t const data = ::io_uring_cqe_get_data64 (cqe);
        if (data == ACE_URING_SUBMIT_WAKE_USER_DATA)
          result = reinterpret_cast<ACE_Uring_Asynch_Result *> (ACE_URING_SUBMIT_WAKE_USER_DATA);
        else
          result = reinterpret_cast<ACE_Uring_Asynch_Result *> (data);
        error = (cqe->res < 0) ? -cqe->res : 0;
        bytes_transferred = (cqe->res > 0) ? cqe->res : 0;
        ::io_uring_cqe_seen (&this->ring_, cqe);
      }

      {
        ACE_GUARD_RETURN (ACE_Thread_Mutex, sq_guard, this->sq_mutex_, -1);

        if (result == reinterpret_cast<ACE_Uring_Asynch_Result *> (ACE_URING_SUBMIT_WAKE_USER_DATA))
          {
            this->drain_submit_wakeup_locked ();
            if (this->arm_submit_wakeup_locked () == -1)
              return -1;

            int const submit_result = this->submit_pending_sqe ();
            if (submit_result < 0)
              {
                errno = -submit_result;
                return -1;
              }
          }
        else if (result != 0)
          {
            owner = result->owner ();
            if (owner != 0)
              owner->unregister_result (result);
          }
      }

      if (result == reinterpret_cast<ACE_Uring_Asynch_Result *> (ACE_URING_SUBMIT_WAKE_USER_DATA))
        continue;

      ++processed;

      if (result == 0)
        continue;

      result->complete (bytes_transferred,
                        error == 0 ? 1 : 0,
                        0, // No completion key
                        error);
    }

  return processed;
}

ACE_Thread_Mutex &
ACE_Uring_Proactor::sq_mutex (void)
{
  return this->sq_mutex_;
}

bool
ACE_Uring_Proactor::is_initialized (void) const
{
  return this->is_initialized_;
}

struct io_uring_sqe *
ACE_Uring_Proactor::get_sqe (void)
{
  if (!this->is_initialized_)
    return 0;
  return ::io_uring_get_sqe (&this->ring_);
}

int
ACE_Uring_Proactor::submit_sqe (void)
{
  if (!this->is_initialized_)
    return -1;
  return ::io_uring_submit (&this->ring_);
}

int
ACE_Uring_Proactor::submit_sqe_if_necessary (void)
{
  if (!this->is_initialized_)
    return -1;

  unsigned int const ready = queued_sqes (this->ring_);
  if (ready == 0)
    return 0;

  if (ready < DEFAULT_SUBMIT_BATCH_SIZE
      && ready < this->ring_.sq.ring_entries)
    return 0;

  return ::io_uring_submit (&this->ring_);
}

int
ACE_Uring_Proactor::submit_pending_sqe (void)
{
  if (!this->is_initialized_)
    return -1;

  if (queued_sqes (this->ring_) == 0)
    return 0;

  return ::io_uring_submit (&this->ring_);
}

int
ACE_Uring_Proactor::signal_submitter (void)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, sq_guard, this->sq_mutex_, -1);

  return this->signal_submitter_locked ();
}

int
ACE_Uring_Proactor::signal_submitter_locked (void)
{
  if (!this->is_initialized_ || this->submit_wakeup_handle_ == ACE_INVALID_HANDLE)
    return -1;

  if (this->on_dispatch_thread () || this->submit_signal_pending_)
    return 0;

  uint64_t const one = 1;
  ssize_t const rc = ::write (this->submit_wakeup_handle_, &one, sizeof (one));
  if (rc == static_cast<ssize_t> (sizeof (one)))
    {
      this->submit_signal_pending_ = true;
      return 0;
    }

  return -1;
}

int
ACE_Uring_Proactor::arm_submit_wakeup_locked (void)
{
  struct io_uring_sqe *const sqe = ::io_uring_get_sqe (&this->ring_);
  if (sqe == 0)
    {
      errno = EAGAIN;
      return -1;
    }

  ::io_uring_prep_poll_add (sqe, this->submit_wakeup_handle_, POLLIN);
  ::io_uring_sqe_set_data64 (sqe, ACE_URING_SUBMIT_WAKE_USER_DATA);
  return 0;
}

void
ACE_Uring_Proactor::drain_submit_wakeup_locked (void)
{
  if (this->submit_wakeup_handle_ == ACE_INVALID_HANDLE)
    return;

  uint64_t value = 0;
  while (::read (this->submit_wakeup_handle_, &value, sizeof (value))
         == static_cast<ssize_t> (sizeof (value)))
    {
    }

  this->submit_signal_pending_ = false;
}

bool
ACE_Uring_Proactor::on_dispatch_thread (void) const
{
  return ACE_OS::thr_equal (ACE_OS::thr_self (),
                            this->dispatch_thread_id_.value ()) != 0;
}

ACE_Asynch_Read_Stream_Impl *ACE_Uring_Proactor::create_asynch_read_stream (void)
{
  ACE_Uring_Asynch_Read_Stream *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Read_Stream (this), 0);
  return ret;
}

ACE_Asynch_Write_Stream_Impl *ACE_Uring_Proactor::create_asynch_write_stream (void)
{
  ACE_Uring_Asynch_Write_Stream *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Write_Stream (this), 0);
  return ret;
}

ACE_Asynch_Read_File_Impl *ACE_Uring_Proactor::create_asynch_read_file (void)
{
  ACE_Uring_Asynch_Read_File *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Read_File (this), 0);
  return ret;
}

ACE_Asynch_Write_File_Impl *ACE_Uring_Proactor::create_asynch_write_file (void)
{
  ACE_Uring_Asynch_Write_File *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Write_File (this), 0);
  return ret;
}

ACE_Asynch_Accept_Impl *ACE_Uring_Proactor::create_asynch_accept (void)
{
  ACE_Uring_Asynch_Accept *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Accept (this), 0);
  return ret;
}

ACE_Asynch_Connect_Impl *ACE_Uring_Proactor::create_asynch_connect (void)
{
  ACE_Uring_Asynch_Connect *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Connect (this), 0);
  return ret;
}

ACE_Asynch_Transmit_File_Impl *ACE_Uring_Proactor::create_asynch_transmit_file (void)
{
  ACE_Uring_Asynch_Transmit_File *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Transmit_File (this), 0);
  return ret;
}

ACE_Asynch_Read_Dgram_Impl *ACE_Uring_Proactor::create_asynch_read_dgram (void)
{
  ACE_Uring_Asynch_Read_Dgram *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Read_Dgram (this), 0);
  return ret;
}

ACE_Asynch_Write_Dgram_Impl *ACE_Uring_Proactor::create_asynch_write_dgram (void)
{
  ACE_Uring_Asynch_Write_Dgram *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Write_Dgram (this), 0);
  return ret;
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_Uring_Proactor::create_asynch_read_stream_result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block &message_block,
   size_t bytes_to_read,
   const void *act,
   ACE_HANDLE,
   int,
   int)
{
  ACE_Uring_Asynch_Read_Stream_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Read_Stream_Result (handler_proxy,
                                                       handle,
                                                       &message_block,
                                                       bytes_to_read,
                                                       act,
                                                       0),
                  0);
  return ret;
}

ACE_Asynch_Write_Stream_Result_Impl *
ACE_Uring_Proactor::create_asynch_write_stream_result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block &message_block,
   size_t bytes_to_write,
   const void *act,
   ACE_HANDLE,
   int,
   int)
{
  ACE_Uring_Asynch_Write_Stream_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Write_Stream_Result (handler_proxy,
                                                        handle,
                                                        &message_block,
                                                        bytes_to_write,
                                                        act,
                                                        0),
                  0);
  return ret;
}

ACE_Asynch_Read_File_Result_Impl *
ACE_Uring_Proactor::create_asynch_read_file_result
  (const ACE_Handler::Proxy_Ptr &handler_proxy,
   ACE_HANDLE handle,
   ACE_Message_Block &message_block,
   size_t bytes_to_read,
   const void *act,
   u_long offset,
   u_long offset_high,
   ACE_HANDLE,
   int,
   int)
{
  ACE_Uring_Asynch_Read_File_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Read_File_Result (handler_proxy,
                                                     handle,
                                                     &message_block,
                                                     bytes_to_read,
                                                     act,
                                                     0,
                                                     offset,
                                                     offset_high),
                  0);
  return ret;
}

ACE_Asynch_Write_File_Result_Impl *
ACE_Uring_Proactor::create_asynch_write_file_result
  (const ACE_Handler::Proxy_Ptr &handler,
   ACE_HANDLE handle,
   ACE_Message_Block &message_block,
   size_t bytes_to_write,
   const void *act,
   u_long offset,
   u_long offset_high,
   ACE_HANDLE,
   int,
   int)
{
  ACE_Uring_Asynch_Write_File_Result *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Write_File_Result (handler, handle, &message_block, bytes_to_write, act, 0, offset, offset_high), 0);
  return ret;
}

ACE_Asynch_Read_Dgram_Result_Impl *
ACE_Uring_Proactor::create_asynch_read_dgram_result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE handle,
    ACE_Message_Block *message_block,
    size_t bytes_to_read,
    int flags,
    int protocol_family,
    const void *act,
    ACE_HANDLE,
    int,
    int)
{
  ACE_Uring_Asynch_Read_Dgram_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Read_Dgram_Result (handler_proxy,
                                                      handle,
                                                      message_block,
                                                      bytes_to_read,
                                                      flags,
                                                      protocol_family,
                                                      act,
                                                      0),
                  0);
  return ret;
}

ACE_Asynch_Write_Dgram_Result_Impl *
ACE_Uring_Proactor::create_asynch_write_dgram_result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE handle,
    ACE_Message_Block *message_block,
    size_t bytes_to_write,
    int flags,
    const void *act,
    ACE_HANDLE,
    int,
    int)
{
  ACE_Uring_Asynch_Write_Dgram_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Write_Dgram_Result (handler_proxy,
                                                       handle,
                                                       message_block,
                                                       bytes_to_write,
                                                       flags,
                                                       act,
                                                       0),
                  0);
  return ret;
}

ACE_Asynch_Accept_Result_Impl *
ACE_Uring_Proactor::create_asynch_accept_result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE listen_handle,
    ACE_HANDLE accept_handle,
    ACE_Message_Block &message_block,
    size_t bytes_to_read,
    const void *act,
    ACE_HANDLE,
    int,
    int)
{
  ACE_Uring_Asynch_Accept_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Accept_Result (handler_proxy,
                                                  listen_handle,
                                                  accept_handle,
                                                  &message_block,
                                                  bytes_to_read,
                                                  act,
                                                  0),
                  0);
  return ret;
}

ACE_Asynch_Connect_Result_Impl *
ACE_Uring_Proactor::create_asynch_connect_result (
    const ACE_Handler::Proxy_Ptr &handler_proxy,
    ACE_HANDLE connect_handle,
    const void *act,
    ACE_HANDLE,
    int,
    int)
{
  ACE_Uring_Asynch_Connect_Result *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Connect_Result (handler_proxy, connect_handle, act, 0), 0);
  return ret;
}

ACE_Asynch_Transmit_File_Result_Impl *
ACE_Uring_Proactor::create_asynch_transmit_file_result (
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
    ACE_HANDLE,
    int,
    int)
{
  ACE_Uring_Asynch_Transmit_File_Result *ret = 0;
  ACE_NEW_RETURN (ret,
                  ACE_Uring_Asynch_Transmit_File_Result (handler_proxy,
                                                         socket,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         0),
                  0);
  return ret;
}

ACE_Asynch_Result_Impl *
ACE_Uring_Proactor::create_asynch_timer (const ACE_Handler::Proxy_Ptr &handler_proxy, const void *act, const ACE_Time_Value &tv, ACE_HANDLE, int, int)
{
  ACE_Uring_Asynch_Timer *ret = 0;
  ACE_NEW_RETURN (ret, ACE_Uring_Asynch_Timer (handler_proxy, act, tv, 0), 0);
  return ret;
}

int
ACE_Uring_Proactor::post_wakeup_completions (int count)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->sq_mutex_, -1);

  if (!this->is_initialized_)
    return -1;

  for (int i = 0; i < count; ++i)
    {
      struct io_uring_sqe *const sqe = ::io_uring_get_sqe (&this->ring_);
      if (!sqe)
        break;
      ::io_uring_prep_nop (sqe);
      ::io_uring_sqe_set_data (sqe, 0);
    }
  return ::io_uring_submit (&this->ring_);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */
