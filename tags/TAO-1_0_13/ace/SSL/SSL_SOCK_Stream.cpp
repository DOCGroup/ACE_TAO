// SSL_SOCK_Stream.cpp
// $Id$

#define ACE_BUILD_DLL

#include "SSL_SOCK_Stream.h"
#include "ace/Handle_Set.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Stream.i"
#endif

#if defined (ACE_HAS_SSL)

ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Stream)

ssize_t
ACE_SSL_SOCK_Stream::sendv (const iovec iov[],
                            size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::sendv");

  // Mimics ACE_OS::sendv.
  int result = 0;
  ssize_t bytes_sent = 0;
  for (size_t i = 0; i < n && result != -1; i++)
    {
      result = this->send (iov[i].iov_base,
                           iov[i].iov_len);
      bytes_sent += iov[i].iov_len;     // Gets ignored on error anyway
    }

  if (result == -1) bytes_sent = -1;
  return bytes_sent;
}

ssize_t
ACE_SSL_SOCK_Stream::recvv (iovec *io_vec,
                            const ACE_Time_Value *timeout = 0) const
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

  if (flags)
    ACE_NOTSUP_RETURN (-1);

  // Mimics <ACE::send>.
  if (timeout == 0)
    return this->send (buf, len);

  int val = 0;
  if (ACE::enter_send_timedwait (this->get_handle (),
                                 timeout,
                                 val) == -1)
    return -1;
  else
    {
      ssize_t bytes_transferred = this->send (buf, len);
      ACE::restore_non_blocking_mode (this->get_handle (), val);
      return bytes_transferred;
    }
}

ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  // Mimics code in <ACE::recv>.
  int peek = 0;

  if (flags)
    {
      if ((flags | MSG_PEEK) == MSG_PEEK)
        peek = 1;
      else
        ACE_NOTSUP_RETURN (-1);
    }

  if (timeout == 0)
    return this->recv (buf, n, flags);
  {
    int val = 0;
    if (ACE::enter_recv_timedwait (this->get_handle (),
                                   timeout,
                                   val) == -1)
      return -1;
    else
      {
        ssize_t bytes_transferred = this->recv (buf, n, flags);
        ACE::restore_non_blocking_mode (this->get_handle (), val);
        return bytes_transferred;
      }
  }
}


ssize_t
ACE_SSL_SOCK_Stream::send (size_t n,
                           ...) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  // Mimics <ACE_SOCK_IO::send (...)>.
  va_list argp;
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp,
                  iovec[total_tuples],
                  -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  va_start (argp, n);

  for (size_t i = 0; i < total_tuples; i++)
    {
      iovp[i].iov_base = va_arg (argp, char *);
      iovp[i].iov_len = va_arg (argp, ssize_t);
    }

  ssize_t result = this->sendv (iovp,
                                total_tuples);
#if !defined (ACE_HAS_ALLOCA)
  delete [] iovp;
#endif /* !defined (ACE_HAS_ALLOCA) */
  va_end (argp);
  return result;
}

ssize_t
ACE_SSL_SOCK_Stream::recv (size_t n,
                           ...) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
  size_t total_tuples = n / 2;
  va_list argp;
  va_start (argp, n);

  ssize_t result = 0;
  ssize_t bytes_recv = 0;
  for (size_t i = 0; i < total_tuples; i++)
    {
      result = this->recv_n (va_arg (argp, char *), va_arg (argp, ssize_t));
      if (result == -1)
        return -1;
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

  //no support for send flags in SSL
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
//       if (n == -1)
//         if (errno == EWOULDBLOCK)
//           n = 0; // Keep trying to send
//         else
//           return -1;
      if (n == -1)
        {
          // If blocked, try again.
          if (errno == EWOULDBLOCK)
            n = 0;

          //
          // No timeouts in this version.
          //

          // Other errors.
          return -1;
        }
      else if (n == 0)
        return 0;
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
//       if (n == -1 || n == 0)
//         break;
      if (n == -1)
        {
          // If blocked, try again.
          if (errno == EWOULDBLOCK)
            n = 0;

          //
          // No timeouts in this version.
          //

          // Other errors.
          return -1;
        }
      else if (n == 0)
        return 0;
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
//       if (n == -1)
//         {
//           if (errno == EWOULDBLOCK)
//             n = 0; //Keep trying
//           else
//             return -1;
//         }
//       else if (n == 0)
//         break;
      if (n == -1)
        {
          // If blocked, try again.
          if (errno == EWOULDBLOCK)
            n = 0;

          //
          // No timeouts in this version.
          //

          // Other errors.
          return -1;
        }
      else if (n == 0)
        return 0;
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
//       if (n == -1)
//         {
//           if (errno == EWOULDBLOCK)
//             n = 0; //Keep trying to send.
//           else
//             return -1;
//         }
      if (n == -1)
        {
          // If blocked, try again.
          if (errno == EWOULDBLOCK)
            n = 0;

          //
          // No timeouts in this version.
          //

          // Other errors.
          return -1;
        }
      else if (n == 0)
        return 0;
    }

  return bytes_transferred;
}


//Taken from OS.cpp, writev ()
ssize_t
ACE_SSL_SOCK_Stream::sendv_n (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::sendv_n");
  size_t length = 0;
  size_t i;

  // Determine the total length of all the buffers in <iov>.
  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;

#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  char *ptr = buf;

  for (i = 0; i < n; i++)
    {
      ACE_OS::memcpy (ptr, iov[i].iov_base, iov[i].iov_len);
      ptr += iov[i].iov_len;
    }

  ssize_t result = this->send_n (buf, length);
#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return result;
}

// Taken straight from OS.cpp, readv ()
ssize_t
ACE_SSL_SOCK_Stream::recvv_n (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recvv_n");
  ssize_t length = 0;
  size_t i;

  for (i = 0; i < n; i++)
    if (ACE_static_cast (int, iov[i].iov_len) < 0)
      return -1;
    else
      length += iov[i].iov_len;

  char *buf;
#   if defined (ACE_HAS_ALLOCA)
  buf = (char *) alloca (length);
#   else
  ACE_NEW_RETURN (buf,
                  char[length],
                  -1);
#   endif /* !defined (ACE_HAS_ALLOCA) */

  length = this->recv_n (buf, length);

  if (length != -1)
    {
      char *ptr = buf;
      int copyn = length;

      for (i = 0;
           i < n && copyn > 0;
           i++)
        {
          ACE_OS::memcpy (iov[i].iov_base, ptr,
                          // iov_len is int on some platforms, size_t on others
                          copyn > (int) iov[i].iov_len
                            ? (size_t) iov[i].iov_len
                            : (size_t) copyn);
          ptr += iov[i].iov_len;
          copyn -= iov[i].iov_len;
        }
    }

#   if !defined (ACE_HAS_ALLOCA)
  delete [] buf;
#   endif /* !defined (ACE_HAS_ALLOCA) */
  return length;
}


int
ACE_SSL_SOCK_Stream::enable (int value) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::enable");
  switch (value)
    {
    case SIGURG:
    case ACE_SIGURG:
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return this->stream_.enable (value);
    default:
      return -1;
    }
  return 0;
}

int
ACE_SSL_SOCK_Stream::disable (int value) const
{
  ACE_TRACE("ACE_SSL_SOCK_Stream::disable");
  switch (value)
    {
    case SIGURG:
    case ACE_SIGURG:
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return this->stream_.disable (value);
    default:
      return -1;
    }
  return 0;
}

#endif /* ACE_HAS_SSL */
