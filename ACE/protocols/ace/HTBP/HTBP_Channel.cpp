/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    HTBP_Channel.cpp
 *
 *  $Id$
 *
 *  @author Phil Mesnier, Priyanka Gontla
 */
//=============================================================================
#include "HTBP_Channel.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Channel.inl"
#endif

#include "HTBP_Session.h"
#include "HTBP_Filter_Factory.h"

#include "ace/Message_Block.h"
#include "ace/Reactor.h"
#include "ace/os_include/netinet/os_tcp.h"
#include "ace/OS_NS_time.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Initialization and termination methods.
/// Constructor.
ACE::HTBP::Channel::Channel (ACE::HTBP::Session *s)
  : filter_ (0),
    session_ (s),
    ace_stream_ (),
    notifier_ (0),
    leftovers_ (1001),
    data_len_ (0),
    data_consumed_ (0),
    state_ (Init),
    error_buffer_ (0)
{
  ACE_NEW (this->notifier_,ACE::HTBP::Notifier(this));
  this->filter_ = ACE::HTBP::Filter_Factory::get_filter (this->session_ != 0);
  this->request_count_ = static_cast<unsigned long> (ACE_OS::time());
}

/// Constructor, takes ownership of the supplied stream
ACE::HTBP::Channel::Channel (ACE_SOCK_Stream &s)
  : filter_ (0),
    session_ (0),
    ace_stream_ (s.get_handle()),
    notifier_ (0),
    leftovers_ (1001),
    data_len_ (0),
    data_consumed_ (0),
    state_ (Init),
    error_buffer_ (0)

{
#if !defined (ACE_LACKS_TCP_NODELAY)
  int no_delay = 1;
  int result = this->ace_stream_.set_option (ACE_IPPROTO_TCP,
                                             TCP_NODELAY,
                                             (void *) &no_delay,
                                             sizeof (no_delay));
  if (result == -1)
    ACE_DEBUG ((LM_DEBUG, "HTBP::Channel ctor(stream), %p\n", "set_option" ));
#endif /* ! ACE_LACKS_TCP_NODELAY */

  this->filter_ = ACE::HTBP::Filter_Factory::get_filter (this->session_ != 0);
  this->request_count_ = static_cast<unsigned long> (ACE_OS::time());
}

ACE::HTBP::Channel::Channel (ACE_HANDLE h)
  : filter_ (0),
    session_ (0),
    ace_stream_ (h),
    notifier_ (0),
    leftovers_ (1001),
    data_len_ (0),
    data_consumed_ (0),
    state_ (Init),
    error_buffer_ (0)
{
#if !defined (ACE_LACKS_TCP_NODELAY)
  int no_delay = 1;
  int result = this->ace_stream_.set_option (ACE_IPPROTO_TCP,
                                             TCP_NODELAY,
                                             (void *) &no_delay,
                                             sizeof (no_delay));
  if (result == -1)
    ACE_DEBUG ((LM_DEBUG, "HTBP::Channel(handle) ctor, %p\n", "set_option" ));
#endif /* ! ACE_LACKS_TCP_NODELAY */

  this->filter_ = ACE::HTBP::Filter_Factory::get_filter (this->session_ != 0);
  this->request_count_ = static_cast<unsigned long> (ACE_OS::time());
}

/// Destructor.
ACE::HTBP::Channel::~Channel (void)
{
  delete this->notifier_;
  delete this->filter_;
}

  /// Dump the state of an object.
void
ACE::HTBP::Channel::dump (void) const
{
}

unsigned long
ACE::HTBP::Channel::request_count (void)
{
  return this->request_count_++;
}

void
ACE::HTBP::Channel::register_notifier (ACE_Reactor *r)
{
  if (r == 0)
    return;
  if (this->notifier_ == 0)
    {
      ACE_NEW (this->notifier_,ACE::HTBP::Notifier(this));
    }
  else
    {
      if (notifier_->get_handle() == ACE_INVALID_HANDLE)
        {
          delete this->notifier_;
          ACE_NEW (this->notifier_,ACE::HTBP::Notifier(this));
        }
    }

  r->register_handler(notifier_,ACE_Event_Handler::READ_MASK);
}

ACE::HTBP::Notifier *
ACE::HTBP::Channel::notifier (void)
{
  return this->notifier_;
}

ACE_HANDLE
ACE::HTBP::Channel::get_handle (void) const
{
  return this->ace_stream_.get_handle ();
}

void
ACE::HTBP::Channel::data_consumed (size_t n)
{
  this->data_consumed_ += n;
  if (this->data_consumed_ == this->data_len_)
    {
      this->filter_->recv_data_trailer(this);
      this->filter_->send_ack(this);
    }
}

int
ACE::HTBP::Channel::load_buffer (void)
{
  this->leftovers_.crunch();
  if (this->state() == Detached ||
      this->state() == Ack_Sent)
    {
      this->data_len_ = 0;
      this->data_consumed_ = 0;
    }

  ssize_t nread = 0;
  errno = 0;
#if 0
  if (this->session_ &&
      (this->session_->sock_flags() & ACE_NONBLOCK == ACE_NONBLOCK))
#endif
    {
      nread =
        ACE::handle_read_ready (this->ace_stream().get_handle(),
                                &ACE_Time_Value::zero);
      if (nread == -1 && errno == ETIME)
        errno = EWOULDBLOCK;
    }
  if (nread != -1)
    nread = this->ace_stream().recv (this->leftovers_.wr_ptr(),
                                     this->leftovers_.space()-1);
  if (nread < 1)
    {
      if (nread == 0 || (errno != EWOULDBLOCK && errno != EAGAIN))
        {
          this->state_ = Closed;
#if 0
          ACE_ERROR ((LM_ERROR,
                      "load_buffer[%d] %p\n",
                      this->ace_stream_.get_handle(),"recv"));
#endif
        }
      return nread;
    }
  this->leftovers_.wr_ptr(nread);
  *this->leftovers_.wr_ptr() = '\0';
  return nread;
}

int
ACE::HTBP::Channel::flush_buffer (void)
{
  if (this->session_)
    return this->session_->flush_outbound_queue();
  return 0;
}

int
ACE::HTBP::Channel::send_ack (void)
{
  return this->filter_->send_ack(this);
}

int
ACE::HTBP::Channel::recv_ack (void)
{
  if (load_buffer() == -1)
    return -1;
  return this->filter_->recv_ack(this);
}

void
ACE::HTBP::Channel::state (ACE::HTBP::Channel::State s)
{
  if (s == Detached)
    {
      this->session_->detach(this);
      this->session_ = 0;
    }
  this->state_ = s;
}

int
ACE::HTBP::Channel::consume_error (void)
{
  if (error_buffer_ == 0)
    {
      ACE_NEW_RETURN (error_buffer_,
                      ACE_Message_Block (this->data_len_ + 1),
                      0);
    }

  ssize_t result = 0;
  size_t n = error_buffer_->size();
  char *buf = error_buffer_->wr_ptr();

  if (this->leftovers_.length() > 0)
    {
      result = ACE_MIN (n,this->leftovers_.length());
      ACE_OS::memcpy (buf,this->leftovers_.rd_ptr(), result);
      this->leftovers_.rd_ptr(result);
      buf += result;
    }

  if (result < (ssize_t)n &&
      result < (ssize_t)data_len_)
    {
      n -= result;
      result += this->ace_stream_.recv(buf, n);
    }
  if (result > 0)
    {
      this->error_buffer_->wr_ptr(result);
      this->data_consumed_ += result;
      if (this->data_consumed_ == this->data_len_)
        {
          *this->error_buffer_->wr_ptr() = '\0';
          if (ACE::debug())
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("ACE::HTBP::Channel::consume_error ")
                        ACE_TEXT("Received entire error buffer: \n%s\n"),
                        this->error_buffer_->rd_ptr()));
          delete error_buffer_;
          error_buffer_ = 0;

          return 1;
        }
    }
  return 0;
}

//---------------------------------------------------------------------------
// = I/O functions.

/// The ACE::HTBP::Channel is a sibling of the ACE_SOCK_IO class, rather than a
/// decendant. This is due to the requirement to wrap all messages with
/// an HTTP request or reply wrapper, and to send application data in only
/// one direction on one stream.

int
ACE::HTBP::Channel::pre_recv(void)
{
  if (ACE::debug())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::pre_recv ")
                ACE_TEXT ("in initial state = %d\n"),state_));
  if (this->state_ == Init ||
      this->state_ == Detached ||
      this->state_ == Header_Pending ||
      this->state_ == Ack_Sent)
    {
      if (this->load_buffer() == -1 && this->leftovers_.length() == 0)
        {
          if (errno != EWOULDBLOCK)
            this->state_ = Closed;
          if (ACE::debug())
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::pre_recv ")
                        ACE_TEXT ("pre_recv returning -1, state = %d, %p\n"),
                        state_, ACE_TEXT("load_buffer()")));
          return -1;
        }
      if (this->filter_->recv_data_header(this) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::pre_recv ")
                    ACE_TEXT ("recv_data_header failed, %p\n"),
                    ACE_TEXT("pre_recv")));
    }
  switch (this->state_)
    {
    case Data_Queued:
    case Ack_Sent:
    case Ready:
      return 0;
    case Header_Pending:
      errno = EWOULDBLOCK;
      return -1;
    default:
      if (ACE::debug())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::pre_recv ")
                    ACE_TEXT("channel[%d] state = %d, %p\n"),
                    this->get_handle(),
                    this->state_,
                    ACE_TEXT("pre_recv")));
    }
  return -1;
}

/// Recv an <n> byte buffer from the connected socket.
ssize_t
ACE::HTBP::Channel::recv (void *buf,
                 size_t n,
                 int flags,
                 const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->pre_recv() == -1 && this->leftovers_.length() == 0)
    return -1;
  if (this->leftovers_.length() > 0)
    {
      result = ACE_MIN (n,this->leftovers_.length());
      ACE_OS::memcpy (buf,this->leftovers_.rd_ptr(), result);
      this->leftovers_.rd_ptr(result);
      buf = (char *)buf + result;
    }

  if (result < (ssize_t)n &&
      result < (ssize_t)data_len_)
    {
      n -= result;
      result += this->ace_stream_.recv(buf, n, flags, timeout);
    }
  if (result > 0)
    data_consumed((size_t)result);
  return result;
}

  /// Recv an <n> byte buffer from the connected socket.
ssize_t
ACE::HTBP::Channel::recv (void *buf,
                 size_t n,
                 const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->pre_recv() == -1)
    return -1;

  result = 0;
  if (this->leftovers_.length() > 0)
    {
      result = ACE_MIN (n,this->leftovers_.length());
      ACE_OS::memcpy (buf,this->leftovers_.rd_ptr(), result);
      this->leftovers_.rd_ptr(result);
      buf = (char *)buf + result;
    }

  if ((size_t)result < n && (size_t)result < this->data_len())
    {
      n -= result;
      result += this->ace_stream_.recv(buf, n, timeout);
    }

  if (result > 0)
    this->data_consumed((size_t)result);
  return result;
}

  /// Recv an <iovec> of size <n> from the connected socket.
ssize_t
ACE::HTBP::Channel::recvv (iovec iov[],
                  int iovcnt,
                  const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->pre_recv() == -1)
    return -1;

  if (this->leftovers_.length())
    {
      int ndx = 0;
      iovec *iov2 = new iovec[iovcnt];
      ACE_Auto_Array_Ptr<iovec> guard (iov2);
      for (int i = 0; i < iovcnt; i++)
        {
          size_t n = ACE_MIN ((size_t) iov[i].iov_len ,
                              (size_t) this->leftovers_.length());
          if (n > 0)
            {
              ACE_OS::memcpy (iov[i].iov_base,this->leftovers_.rd_ptr(), n);
              this->leftovers_.rd_ptr(n);
              result += n;
            }
          if (n < (size_t) iov[i].iov_len)
            {
              iov2[ndx].iov_len = iov[i].iov_len - n;
              iov2[ndx].iov_base = (char *)iov[i].iov_base + n;
              ndx++;
            }
        }
      if (ndx > 0)
        result += this->ace_stream_.recvv(iov2,ndx,timeout);
    }
  else
    result = this->ace_stream_.recvv(iov,iovcnt,timeout);

  if (result > 0)
    this->data_consumed((size_t)result);
  return result;
}

ssize_t
ACE::HTBP::Channel::recvv (iovec *io_vec,
                  const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->pre_recv() == -1)
    return -1;
  if (ACE::debug())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ACE::HTBP::Channel::recvv ")
                ACE_TEXT("recvv, leftover len = %d\n"),
                this->leftovers_.length()));
  if (this->leftovers_.length())
    {
      io_vec->iov_base = 0;
      io_vec->iov_len = 0;
      ACE_NEW_RETURN (io_vec->iov_base,
                      char[this->leftovers_.length()],-1);
      io_vec->iov_len = this->leftovers_.length();
      ACE_OS::memcpy (io_vec->iov_base,
                      this->leftovers_.rd_ptr(),
                      io_vec->iov_len);
      result = io_vec->iov_len;
      this->leftovers_.length(0);
    }
  else
    result = this->ace_stream_.recvv(io_vec,timeout);

  if (result > 0)
    this->data_consumed((size_t)result);
  return result;
}

ssize_t
ACE::HTBP::Channel::send (const void *buf,
                 size_t n,
                 int flags,
                 const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->filter_->send_data_header(n,this) == -1)
    return -1;
  result = this->ace_stream_.send(buf,n,flags,timeout);
  if (result == -1)
    return -1;
  if (this->filter_->send_data_trailer(this) == -1)
    return -1;
  return result;
}

ssize_t
ACE::HTBP::Channel::send (const void *buf,
                  size_t n,
                  const ACE_Time_Value *timeout)
{
  ssize_t result = 0;
  if (this->filter_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::send: filter ")
                       ACE_TEXT ("is null\n")),-1);
  if (this->filter_->send_data_header(n,this) == -1)
    return -1;
  result = this->ace_stream_.send (buf,n,timeout);
  if (result == -1)
    return -1;
  if (this->filter_->send_data_trailer(this) == -1)
    return -1;
  return result;
}

ssize_t
ACE::HTBP::Channel::sendv (const iovec iov[],
                           int iovcnt,
                           const ACE_Time_Value *timeout)
{
  if (this->ace_stream_.get_handle() == ACE_INVALID_HANDLE)
    this->session_->inbound();

  ssize_t result = 0;
  size_t n = 0;

  for (int i = 0; i < iovcnt; n += iov[i++].iov_len)
    {
      // No action.
    }

  if (this->filter_->send_data_header(n,this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::sendv ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("send_data_header")),-1);

  result = this->ace_stream_.sendv (iov,iovcnt,timeout);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::sendv ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("ace_stream_.sendv")),-1);

  if (this->filter_->send_data_trailer(this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) ACE::HTBP::Channel::sendv ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("send_data_trailer\n")),-1);
  return result;
}

int
ACE::HTBP::Channel::enable (int value) const
{
  this->ace_stream_.enable(value);

  return 0; //this->ace_stream_.enable(value);
}

int
ACE::HTBP::Channel::disable (int value) const
{
  this->ace_stream_.disable(value);

  return 0;//this->ace_stream_.disable(value);
}

ACE_END_VERSIONED_NAMESPACE_DECL
