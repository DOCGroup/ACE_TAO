// -*- C++ -*-
//
// $Id$

#include "ace/Handle_Set.h"

#include <openssl/err.h>

#include "SSL_SOCK_Stream.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Stream.i"
#endif

ACE_RCSID (ACE_SSL,
           SSL_SOCK_Stream,
           "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Stream)

ssize_t
ACE_SSL_SOCK_Stream::sendv (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::sendv");

  // There is subtle problem in this method that occurs when using
  // non-blocking IO.  The semantics of a non-blocking scatter write
  // (sendv()) are not possible to retain with the emulation in this
  // method.

  ssize_t bytes_sent = 0;

  for (size_t i = 0; i < n; ++i)
    {
      int result = this->send (iov[i].iov_base,
                               iov[i].iov_len);
      if (result == -1)
        return -1;
      else
        bytes_sent += result;
    }

  return bytes_sent;
}

ssize_t
ACE_SSL_SOCK_Stream::recvv (iovec *io_vec,
                            const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recvv");

  // From <ACE_SOCK_IO::recvv>.
#if defined (FIONREAD)
  ACE_Handle_Set handle_set;
  handle_set.reset ();
  handle_set.set_bit (this->get_handle ());

  io_vec->iov_base = 0;

  // Check the status of the current socket.
  switch (ACE_OS::select (int (this->get_handle ()) + 1,
                          handle_set,
                          0, 0,
                          timeout))
    {
    case -1:
      return -1;
      /* NOTREACHED */
    case 0:
      errno = ETIME;
      return -1;
      /* NOTREACHED */
    default:
      // Goes fine, fallthrough to get data
      break;
    }

  u_long inlen;


  if (ACE_OS::ioctl (this->get_handle (),
                     FIONREAD,
                     (u_long *) &inlen) == -1)
    return -1;
  else if (inlen > 0)
    {
      ACE_NEW_RETURN (io_vec->iov_base,
                      char[inlen],
                      -1);
      io_vec->iov_len = this->recv (io_vec->iov_base,
                                    inlen);
      return io_vec->iov_len;
    }
  else
    return 0;
#else
  ACE_UNUSED_ARG (io_vec);
  ACE_UNUSED_ARG (timeout);
  ACE_NOTSUP_RETURN (-1);
#endif /* FIONREAD */
}

ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t len,
                           int flags,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  if (timeout == 0)
    return this->send (buf, len);

  int val = 0;
  if (ACE::enter_send_timedwait (this->get_handle (),
                                 timeout,
                                 val) == -1)
    return -1;

  ssize_t bytes_transferred = this->send (buf, len);

  ACE::restore_non_blocking_mode (this->get_handle (), val);

  return bytes_transferred;
}

ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  if (timeout == 0)
    return this->recv (buf, n, flags);

  int val = 0;
  if (ACE::enter_recv_timedwait (this->get_handle (),
                                 timeout,
                                 val) == -1)
    return -1;

  ssize_t bytes_transferred = this->recv (buf, n, flags);

  ACE::restore_non_blocking_mode (this->get_handle (), val);

  return bytes_transferred;
}


ssize_t
ACE_SSL_SOCK_Stream::send (size_t n, ...) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  size_t total_tuples = n / 2;

  va_list argp;
  va_start (argp, n);

  ssize_t bytes_sent = 0;

  // NOTE: This method used to fill an IO vector (e.g. iovec) and then
  //       send it using a scatter write (sendv()).  However, it is
  //       not possible to emulate a non-blocking scatter write over
  //       SSL.  As such, there is no point in attempting to use
  //       scatter writes over SSL.
  for (size_t i = 0; i < total_tuples; ++i)
    {
      ssize_t result = this->send_n (va_arg (argp, char *),
                                     va_arg (argp, ssize_t));

      if (result == -1)
        {
          va_end (argp);
          return -1;
        }
      else
        bytes_sent += result;
    }

  va_end (argp);

  return bytes_sent;
}

ssize_t
ACE_SSL_SOCK_Stream::recv (size_t n, ...) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  size_t total_tuples = n / 2;

  va_list argp;
  va_start (argp, n);

  ssize_t bytes_recv = 0;

  for (size_t i = 0; i < total_tuples; ++i)
    {
      ssize_t result = this->recv_n (va_arg (argp, char *),
                                     va_arg (argp, ssize_t));

      if (result == -1)
        {
          va_end (argp);
          return -1;
        }
      else
        bytes_recv += result;
    }

  va_end (argp);

  return bytes_recv;
}

ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf,
                             size_t len,
                             int flags,
                             const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");

  // No support for send flags in SSL.
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  /* This code mimics ACE::send_n */
  // Total number of bytes written.
  size_t bytes_transferred = 0;

  // Actual number of bytes written in each <send> attempt
  ssize_t n = 0;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = this->send ((const char*) buf + bytes_transferred,
                      len - bytes_transferred,
                      flags,
                      timeout);

      if (n < 0)
	{
          if (errno == EWOULDBLOCK)
            {
              // If blocked, try again.
	      n = 0;
	      continue;
            }
          else
            return -1;
	}
      else if (n == 0)
        break;
    }

  return bytes_transferred;
}

ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf,
                             size_t len,
                             int flags,
                             const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");

  if (flags != 0)
    {
       if ((flags | MSG_PEEK) != MSG_PEEK)
        ACE_NOTSUP_RETURN (-1);
    }

  size_t bytes_transferred = 0;
  ssize_t n = 0;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = this->recv ((char*) buf + bytes_transferred,
                      len - bytes_transferred,
                      flags,
                      timeout);

      if (n < 0)
	{
          if (errno == EWOULDBLOCK)
            {
              // If blocked, try again.
	      n = 0;
	      continue;
            }
          else
            return -1;
	}
      else if (n == 0)
        break;
    }

  return bytes_transferred;
}

ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf, int len, int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");

  if (flags != 0)
    {
      if ((flags | MSG_PEEK) != MSG_PEEK)
        ACE_NOTSUP_RETURN (-1);
    }

  ssize_t bytes_transferred = 0;
  ssize_t n = 0;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = this->recv ((char*) buf + bytes_transferred,
                      len - bytes_transferred,
                      flags);

      if (n < 0)
	{
          if (errno == EWOULDBLOCK)
            {
              // If blocked, try again.
	      n = 0;
	      continue;
            }
          else
            return -1;
	}
      else if (n == 0)
	break;
    }

  return bytes_transferred;
}

ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf, int len, int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");

  // Send flags are unsupported in SSL
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  /*  The following code mimics <ACE::send_n> */
  size_t bytes_transferred = 0;
  ssize_t n = 0;

  for (bytes_transferred = 0;
       bytes_transferred < (size_t) len;
       bytes_transferred += n)
    {
      n = this->send ((const char*) buf + bytes_transferred,
                      len - bytes_transferred,
                      flags);

      if (n < 0)
	{
          if (errno == EWOULDBLOCK)
            {
              // If blocked, try again.
	      n = 0;
	      continue;
            }
          else
            return -1;
	}
      else if (n == 0)
        break;
    }

  return bytes_transferred;
}

ssize_t
ACE_SSL_SOCK_Stream::sendv_n (const iovec iov[], size_t iovcnt) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::sendv_n");

  ssize_t bytes_sent = 0;

  for (size_t i = 0; i < iovcnt; ++i)
    {
      int result = this->send_n (iov[i].iov_base,
                                 iov[i].iov_len);

      if (result == -1)
        return -1;
      else
        bytes_sent += result;
    }

  return bytes_sent;
}

ssize_t
ACE_SSL_SOCK_Stream::recvv_n (iovec iov[], size_t iovcnt) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recvv_n");

  ssize_t bytes_read = 0;

  for (size_t i = 0; i < iovcnt; ++i)
    {
      int result = this->recv_n (iov[i].iov_base,
                                 iov[i].iov_len);

      if (result == -1)
        return -1;
      else
        bytes_read += result;
    }

  return bytes_read;
}
