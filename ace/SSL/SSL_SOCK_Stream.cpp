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
  if (flags) ACE_NOTSUP_RETURN (-1);

  // Mimics <ACE::send>.
  if (timeout == 0)
    return this->send (buf, len);

  int val;
  if (this->enter_send_timedwait (timeout, val) == -1)
    return -1;
  ssize_t bytes_written = this->send (buf, len);
  this->leave_send_timedwait (timeout, val);
  return bytes_written;
}

ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  // Mimics code in <ACE::recv>.
  int peek;
  if (flags)
    {
      if ((flags | MSG_PEEK) == MSG_PEEK)
        peek = 1;
      else
        ACE_NOTSUP_RETURN (-1);
    }
  if (timeout == 0)
    return this->recv (buf, n, flags);
  int val = 0;
  if (this->enter_recv_timedwait (timeout, val) == -1)
    return -1;
  ssize_t bytes_recv = this->recv (buf, n, flags);
  this->leave_recv_timedwait (timeout, val);
  return bytes_recv;
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
ACE_SSL_SOCK_Stream::send_n (const void *buf, size_t len, int flags,
                             const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");

  //no support for send flags in SSL
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  /* This code mimics ACE::send_n */
  // Total number of bytes written.
  size_t bytes_written;

  // Actual number of bytes written in each <send> attempt
  ssize_t n;

  for (bytes_written = 0;
       bytes_written < len;
       bytes_written += n)
    {
      n = this->send ((const char*) buf + bytes_written,
                      len - bytes_written,
                      flags,
                      timeout);
      if (n == -1)
        if (errno == EWOULDBLOCK)
          n = 0; // Keep trying to send
        else
          return -1;
    }
  return bytes_written;
}

ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf, size_t len, int flags,
			 const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");

  if (flags != 0)
    {
       if ((flags | MSG_PEEK) != MSG_PEEK)
        ACE_NOTSUP_RETURN (-1);
    }
  size_t bytes_received;

  ssize_t n;

  for (bytes_received = 0;
       bytes_received < len;
       bytes_received += n)
    {
      n = this->recv ((char*) buf + bytes_received,
                      len - bytes_received,
                      flags,
                      timeout);
      if (n == -1 || n == 0)
        break;
    }
  return bytes_received;
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
  ssize_t bytes_received;

  ssize_t n;

  for (bytes_received = 0;
       bytes_received < len;
       bytes_received += n)
    {
      n = this->recv ((char*) buf + bytes_received,
                      len - bytes_received,
                      flags);
      if (n == -1)
        {
          if (errno == EWOULDBLOCK)
            n = 0; //Keep trying
          else
            return -1;
        }
      else if (n == 0)
        break;
    }

  return bytes_received;
}

ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf, int len, int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");

  // Send flags are unsupported in SSL
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  /*  The following code mimics <ACE::send_n> */
  size_t bytes_written;
  ssize_t n;

  for (bytes_written = 0;
       bytes_written < (size_t) len;
       bytes_written += n)
    {
      n = this->send ((const char*) buf + bytes_written,
                      len - bytes_written,
                      flags);
      if (n == -1)
        {
          if (errno == EWOULDBLOCK)
            n = 0; //Keep trying to send.
          else
            return -1;
        }
    }
  return bytes_written;
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
      return stream_.enable (value);
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
      return stream_.disable (value);
    default:
      return -1;
    }
  return 0;
}




// The following four functions are copied straight from ACE::.
int
ACE_SSL_SOCK_Stream::enter_recv_timedwait (const ACE_Time_Value *timeout,
                                           int &val) const
{
  // Give value a default value to keep Purify happy!
  val = 0;
  ACE_HANDLE handle = this->get_handle ();
#if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

  struct pollfd fds;

  fds.fd = handle;
  fds.events = POLLIN;
  fds.revents = 0;

  int a = ACE_OS::poll (&fds, 1, *timeout);

#else
  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // Wait for input to arrive or for the timeout to elapse.
  int a = ACE_OS::select (int (handle) + 1,
                          (fd_set *) handle_set, // read_fds.
                          (fd_set *) 0, // write_fds.
                          (fd_set *) 0, // exception_fds.
                          timeout);
#endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

   switch ( a )
   {
   case 0:  // Timer expired.  return -1
      errno = ETIME;
      /* FALLTHRU */
   case -1: // we got here directly - select() returned -1.
      return -1;
   case 1: // OK to read now.
      /* FALLTHRU */
   default: // default is case a > 0; return a
      // really should assert if a != 1
      //assert( a == 1 );
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_DISABLED (val, ACE_NONBLOCK))
        // Set the handle into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return a;
   }
}

void
ACE_SSL_SOCK_Stream::leave_recv_timedwait (const ACE_Time_Value *timeout,
                                           int val) const
{
  if (timeout != 0
      && ACE_BIT_DISABLED (val,
                           ACE_NONBLOCK))
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (this->get_handle (), ACE_NONBLOCK);
    }
}

int
ACE_SSL_SOCK_Stream::enter_send_timedwait (const ACE_Time_Value* timeout,
                                           int &val) const
{
  // Give value a default value to keep Purify happy!
  val = 0;
  ACE_HANDLE handle = this->get_handle ();
#if defined (ACE_HAS_POLL) && defined (ACE_HAS_LIMITED_SELECT)

  struct pollfd fds;

  fds.fd = handle;
  fds.events = POLLOUT;
  fds.revents = 0;

  int a = ACE_OS::poll (&fds, 1, *timeout);

#else
  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // On timed writes we always go into select(); only if the
  // handle is available for writing within the specified amount
  // of time do we put it in non-blocking mode

  int a = ACE_OS::select (int (handle) + 1,
                          (fd_set *) 0,
                          (fd_set *) handle_set,
                          (fd_set *) 0,
                          timeout);
#endif /* ACE_HAS_POLL && ACE_HAS_LIMITED_SELECT */

   switch ( a )
   {
   case 0: // Timer expired.
      errno = ETIME;
      /* FALLTHRU */
   case -1: // we got here directly - select() returned -1.
      return -1;
   case 1: // Ok to write now.
      /* FALLTHRU */
   default: // default is case a > 0; return a
      // really should assert if a != 1
      //assert( a == 1 );
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_DISABLED (val, ACE_NONBLOCK))
        // Set the handle into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return a;
    }
}

void
ACE_SSL_SOCK_Stream::leave_send_timedwait (const ACE_Time_Value *timeout,
                                           int val) const
{
  if (timeout != 0
      && ACE_BIT_DISABLED (val, ACE_NONBLOCK))
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (this->get_handle (), ACE_NONBLOCK);
    }
}

#endif /* ACE_HAS_SSL */
