/* -*- C++ -*- */
// $Id$

// SOCK_Stream.i

#include "SOCK_Stream.h"
#if defined (ACE_HAS_SSL)

ASYS_INLINE
ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream");
}

ASYS_INLINE
ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream (ACE_HANDLE h)
  : stream_ (h)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream");
}

ASYS_INLINE
ACE_SSL_SOCK_Stream::~ACE_SSL_SOCK_Stream (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::~ACE_SSL_SOCK_Stream");
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n,
                           int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");
  // No send flags are supported in SSL.
  if (flags) ACE_NOTSUP_RETURN (-1);
  else
    return ((ssl_ && SSL_is_init_finished (ssl_)) ?
            ::SSL_write (ssl_, ACE_static_cast (const char*, buf), n) : -1);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
   if (flags)
     {
       if ((flags | MSG_PEEK) == MSG_PEEK)
         return ::SSL_peek (ssl_, ACE_static_cast (char*, buf), n);
       else
         ACE_NOTSUP_RETURN (-1);
     }
  return ((ssl_ && SSL_is_init_finished(ssl_)) ?
	  SSL_read (ssl_, ACE_static_cast (char *, buf), n) :
	  -1);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");
  return ((ssl_ && SSL_is_init_finished(ssl_)) ?
	  SSL_write (ssl_, ACE_static_cast (const char *, buf), n) :
	  -1);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
  return ((ssl_ && SSL_is_init_finished(ssl_)) ?
	  SSL_read (ssl_, ACE_static_cast (char*, buf), n) :
	  -1);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");
  return this->send (buf, len, 0, timeout);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
  return this->recv (buf, n, 0, timeout);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");
  return this->recv_n (buf, buf_size, 0);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf, int len) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");
  return this->send_n (buf, len, 0);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close_reader (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_reader");
  return stream_.close_reader ();
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close_writer (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_writer");
  return stream_.close_writer ();
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close");

  this->ssl_close ();

  return stream_.close ();
}

ASYS_INLINE void
ACE_SSL_SOCK_Stream::dump (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::dump");
  stream_.dump ();
}

ASYS_INLINE ACE_SOCK_Stream&
ACE_SSL_SOCK_Stream::peer () {
  ACE_TRACE ("ACE_SSL_SOCK_Stream::peer");
  return stream_;
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::control (int cmd, void* dummy) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::control");
  return stream_.control (cmd, dummy);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::set_option (int level,
                                    int option,
                                    void *optval,
                                    int optlen) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::set_option");
  return stream_.set_option (level, option, optval, optlen);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::get_option (int level,
                                    int option,
                                    void *optval,
                                    int *optlen) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::get_option");
  return stream_.get_option (level, option, optval, optlen);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::get_local_addr (ACE_Addr &addr) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::get_local_addr");
  return stream_.get_local_addr (addr);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::get_remote_addr (ACE_Addr &addr) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::get_remote_addr");
  return stream_.get_remote_addr (addr);
}

ASYS_INLINE ACE_HANDLE
ACE_SSL_SOCK_Stream::get_handle (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::get_handle");
  return stream_.get_handle ();
}

ASYS_INLINE void
ACE_SSL_SOCK_Stream::set_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::set_handle");
  stream_.set_handle (handle);
}

#endif /* ACE_HAS_SSL */
