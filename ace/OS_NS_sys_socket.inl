// -*- C++ -*-
// $Id$

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_macros.h"
#include "ace/OS_NS_sys_uio.h"

#if defined (ACE_HAS_VOIDPTR_SOCKOPT)
typedef void *ACE_SOCKOPT_TYPE1;
#elif defined (ACE_HAS_CHARPTR_SOCKOPT)
typedef char *ACE_SOCKOPT_TYPE1;
#else
typedef const char *ACE_SOCKOPT_TYPE1;
#endif /* ACE_HAS_VOIDPTR_SOCKOPT */

ACE_INLINE ACE_HANDLE
ACE_OS::accept (ACE_HANDLE handle,
                struct sockaddr *addr,
                int *addrlen)
{
  ACE_OS_TRACE ("ACE_OS::accept");
#if defined (ACE_PSOS)
#  if !defined (ACE_PSOS_DIAB_PPC)
  ACE_SOCKCALL_RETURN (::accept ((ACE_SOCKET) handle,
                                 (struct sockaddr_in *) addr,
                                 (ACE_SOCKET_LEN *) addrlen),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
#  else
  ACE_SOCKCALL_RETURN (::accept ((ACE_SOCKET) handle,
                                 (struct sockaddr *) addr,
                                 (ACE_SOCKET_LEN *) addrlen),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
#  endif /* defined ACE_PSOS_DIAB_PPC */
#else
  // On a non-blocking socket with no connections to accept, this
  // system call will return EWOULDBLOCK or EAGAIN, depending on the
  // platform.  UNIX 98 allows either errno, and they may be the same
  // numeric value.  So to make life easier for upper ACE layers as
  // well as application programmers, always change EAGAIN to
  // EWOULDBLOCK.  Rather than hack the ACE_OSCALL_RETURN macro, it's
  // handled explicitly here.  If the ACE_OSCALL macro ever changes,
  // this function needs to be reviewed.  On Win32, the regular macros
  // can be used, as this is not an issue.

#  if defined (ACE_WIN32)
  ACE_SOCKCALL_RETURN (::accept ((ACE_SOCKET) handle,
                                 addr,
                                 (ACE_SOCKET_LEN *) addrlen),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
#  else
#    if defined (ACE_HAS_BROKEN_ACCEPT_ADDR)
  // Apparently some platforms like VxWorks can't correctly deal with
  // a NULL addr.

   sockaddr_in fake_addr;
   int fake_addrlen;

   if (addrlen == 0)
     addrlen = &fake_addrlen;

   if (addr == 0)
     {
       addr = (sockaddr *) &fake_addr;
       *addrlen = sizeof fake_addr;
     }
#    endif /* VXWORKS */
  ACE_HANDLE ace_result = ::accept ((ACE_SOCKET) handle,
                                    addr,
                                    (ACE_SOCKET_LEN *) addrlen) ;
  if (ace_result == ACE_INVALID_HANDLE && errno == EAGAIN)
    errno = EWOULDBLOCK;
  return ace_result;

#  endif /* defined (ACE_WIN32) */
#endif /* defined (ACE_PSOS) */
}

ACE_INLINE int
ACE_OS::bind (ACE_HANDLE handle, struct sockaddr *addr, int addrlen)
{
  ACE_OS_TRACE ("ACE_OS::bind");
#if defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC)
  ACE_SOCKCALL_RETURN (::bind ((ACE_SOCKET) handle,
                               (struct sockaddr_in *) addr,
                               (ACE_SOCKET_LEN) addrlen),
                       int, -1);
#else /* !defined (ACE_PSOS) || defined (ACE_PSOS_DIAB_PPC) */
  ACE_SOCKCALL_RETURN (::bind ((ACE_SOCKET) handle,
                               addr,
                               (ACE_SOCKET_LEN) addrlen), int, -1);
#endif /* defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC) */
}

ACE_INLINE int
ACE_OS::closesocket (ACE_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::closesocket");
#if defined (ACE_WIN32)
  ACE_SOCKCALL_RETURN (::closesocket ((SOCKET) handle), int, -1);
#elif defined (ACE_PSOS_DIAB_PPC)
  ACE_OSCALL_RETURN (::pna_close (handle), int, -1);
#else
  ACE_OSCALL_RETURN (::close (handle), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::connect (ACE_HANDLE handle,
                 struct sockaddr *addr,
                 int addrlen)
{
  ACE_OS_TRACE ("ACE_OS::connect");
#if defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC)
  ACE_SOCKCALL_RETURN (::connect ((ACE_SOCKET) handle,
                                  (struct sockaddr_in *) addr,
                                  (ACE_SOCKET_LEN) addrlen),
                       int, -1);
#else  /* !defined (ACE_PSOS) || defined (ACE_PSOS_DIAB_PPC) */
  ACE_SOCKCALL_RETURN (::connect ((ACE_SOCKET) handle,
                                  addr,
                                  (ACE_SOCKET_LEN) addrlen), int, -1);
#endif /* defined (ACE_PSOS)  && !defined (ACE_PSOS_DIAB_PPC) */
}

ACE_INLINE int
ACE_OS::enum_protocols (int *protocols,
                        ACE_Protocol_Info *protocol_buffer,
                        u_long *buffer_length)
{
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)

  ACE_SOCKCALL_RETURN (::WSAEnumProtocols (protocols,
                                           protocol_buffer,
                                           buffer_length),
                       int,
                       SOCKET_ERROR);

#else
  ACE_UNUSED_ARG (protocols);
  ACE_UNUSED_ARG (protocol_buffer);
  ACE_UNUSED_ARG (buffer_length);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_WINSOCK2 */
}

ACE_INLINE int
ACE_OS::getpeername (ACE_HANDLE handle, struct sockaddr *addr,
                     int *addrlen)
{
  ACE_OS_TRACE ("ACE_OS::getpeername");
#if defined (ACE_PSOS) && !defined ACE_PSOS_DIAB_PPC
  ACE_SOCKCALL_RETURN (::getpeername ((ACE_SOCKET) handle,
                                      (struct sockaddr_in *) addr,
                                      (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#else
  ACE_SOCKCALL_RETURN (::getpeername ((ACE_SOCKET) handle,
                                      addr,
                                      (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#endif /* defined (ACE_PSOS) */
}

ACE_INLINE int
ACE_OS::getsockname (ACE_HANDLE handle,
                     struct sockaddr *addr,
                     int *addrlen)
{
  ACE_OS_TRACE ("ACE_OS::getsockname");
#if defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC)
  ACE_SOCKCALL_RETURN (::getsockname ((ACE_SOCKET) handle,
                                      (struct sockaddr_in *) addr,
                                      (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#else
  ACE_SOCKCALL_RETURN (::getsockname ((ACE_SOCKET) handle,
                                      addr,
                                      (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#endif /* defined (ACE_PSOS) */
}

ACE_INLINE int
ACE_OS::getsockopt (ACE_HANDLE handle,
                    int level,
                    int optname,
                    char *optval,
                    int *optlen)
{
  ACE_OS_TRACE ("ACE_OS::getsockopt");
  ACE_SOCKCALL_RETURN (::getsockopt ((ACE_SOCKET) handle,
                                     level,
                                     optname,
                                     optval,
                                     (ACE_SOCKET_LEN *) optlen),
                       int,
                       -1);
}

ACE_INLINE int
ACE_OS::listen (ACE_HANDLE handle, int backlog)
{
  ACE_OS_TRACE ("ACE_OS::listen");
  ACE_SOCKCALL_RETURN (::listen ((ACE_SOCKET) handle, backlog), int, -1);
}

ACE_INLINE int
ACE_OS::recv (ACE_HANDLE handle, char *buf, size_t len, int flags)
{
  ACE_OS_TRACE ("ACE_OS::recv");

  // On UNIX, a non-blocking socket with no data to receive, this
  // system call will return EWOULDBLOCK or EAGAIN, depending on the
  // platform.  UNIX 98 allows either errno, and they may be the same
  // numeric value.  So to make life easier for upper ACE layers as
  // well as application programmers, always change EAGAIN to
  // EWOULDBLOCK.  Rather than hack the ACE_OSCALL_RETURN macro, it's
  // handled explicitly here.  If the ACE_OSCALL macro ever changes,
  // this function needs to be reviewed.  On Win32, the regular macros
  // can be used, as this is not an issue.
#if defined (ACE_WIN32)
  ACE_SOCKCALL_RETURN (::recv ((ACE_SOCKET) handle, buf,
                               ACE_static_cast (int, len), flags), int, -1);
#else
  int ace_result_;
  ace_result_ = ::recv ((ACE_SOCKET) handle, buf, len, flags);
  if (ace_result_ == -1 && errno == EAGAIN)
    errno = EWOULDBLOCK;
  return ace_result_;
#endif /* defined (ACE_WIN32) */
}

ACE_INLINE int
ACE_OS::recvfrom (ACE_HANDLE handle,
                  char *buf,
                  size_t len,
                  int flags,
                  struct sockaddr *addr,
                  int *addrlen)
{
  ACE_OS_TRACE ("ACE_OS::recvfrom");
#if defined (ACE_PSOS)
#  if !defined ACE_PSOS_DIAB_PPC
  ACE_SOCKCALL_RETURN (::recvfrom ((ACE_SOCKET) handle, buf, len, flags,
                                   (struct sockaddr_in *) addr, (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#  else
  ACE_SOCKCALL_RETURN (::recvfrom ((ACE_SOCKET) handle, buf, len, flags,
                                   (struct sockaddr *) addr, (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#  endif /* defined ACE_PSOS_DIAB_PPC */
#elif defined (ACE_WIN32)
  int shortened_len = ACE_static_cast (int, len);
  int result = ::recvfrom ((ACE_SOCKET) handle,
                           buf,
                           shortened_len,
                           flags,
                           addr,
                           (ACE_SOCKET_LEN *) addrlen);
  if (result == SOCKET_ERROR)
    {
      ACE_OS::set_errno_to_wsa_last_error ();
      if (errno == WSAEMSGSIZE &&
          ACE_BIT_ENABLED (flags, MSG_PEEK))
        return shortened_len;
      else
        return -1;
    }
  else
    return result;
#else /* non Win32 and non PSOS */
  ACE_SOCKCALL_RETURN (::recvfrom ((ACE_SOCKET) handle, buf, len, flags,
                                   addr, (ACE_SOCKET_LEN *) addrlen),
                       int, -1);
#endif /* defined (ACE_PSOS) */
}

ACE_INLINE int
ACE_OS::recvfrom (ACE_HANDLE handle,
                  iovec *buffers,
                  int buffer_count,
                  size_t &number_of_bytes_recvd,
                  int &flags,
                  struct sockaddr *addr,
                  int *addrlen,
                  ACE_OVERLAPPED *overlapped,
                  ACE_OVERLAPPED_COMPLETION_FUNC func)
{
  ACE_OS_TRACE ("ACE_OS::recvfrom");

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  DWORD bytes_recvd;
  DWORD the_flags = flags;
  int result = ::WSARecvFrom ((SOCKET) handle,
                              (WSABUF*)buffers,
                              buffer_count,
                              &bytes_recvd,
                              &the_flags,
                              addr,
                              addrlen,
                              overlapped,
                              func);
  if (result != 0) {
    ACE_OS::set_errno_to_last_error ();
  }
  flags = the_flags;
  number_of_bytes_recvd = ACE_static_cast (size_t, bytes_recvd);
  return result;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buffers);
  ACE_UNUSED_ARG (buffer_count);
  ACE_UNUSED_ARG (number_of_bytes_recvd);
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (addr);
  ACE_UNUSED_ARG (addrlen);
  ACE_UNUSED_ARG (overlapped);
  ACE_UNUSED_ARG (func);
  ACE_NOTSUP_RETURN (-1);
#endif /* defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */
}

ACE_INLINE int
ACE_OS::recvmsg (ACE_HANDLE handle, struct msghdr *msg, int flags)
{
  ACE_OS_TRACE ("ACE_OS::recvmsg");
#if !defined (ACE_LACKS_RECVMSG)
# if (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  DWORD bytes_received = 0;

  int result = ::WSARecvFrom ((SOCKET) handle,
                              (WSABUF *) msg->msg_iov,
                              msg->msg_iovlen,
                              &bytes_received,
                              (DWORD *) &flags,
                              msg->msg_name,
                              &msg->msg_namelen,
                              0,
                              0);

  if (result != 0)
    {
      ACE_OS::set_errno_to_last_error ();
      return -1;
    }
  else
    return (ssize_t) bytes_received;
# else /* ACE_HAS_WINSOCK2 */
  ACE_SOCKCALL_RETURN (::recvmsg (handle, msg, flags), int, -1);
# endif /* ACE_HAS_WINSOCK2 */
#else
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (msg);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_LACKS_RECVMSG */
}

ACE_INLINE ssize_t
ACE_OS::recvv (ACE_HANDLE handle,
               iovec *buffers,
               int n)
{
#if defined (ACE_HAS_WINSOCK2)

  DWORD bytes_received = 0;
  int result = 1;

  // Winsock 2 has WSARecv and can do this directly, but Winsock 1 needs
  // to do the recvs piece-by-piece.

# if (ACE_HAS_WINSOCK2 != 0)
  DWORD flags = 0;
  result = ::WSARecv ((SOCKET) handle,
                      (WSABUF *) buffers,
                      n,
                      &bytes_received,
                      &flags,
                      0,
                      0);
# else
  int i, chunklen;
  char *chunkp = 0;

  // Step through the buffers requested by caller; for each one, cycle
  // through reads until it's filled or an error occurs.
  for (i = 0; i < n && result > 0; i++)
    {
      chunkp = buffers[i].iov_base;     // Point to part of chunk being read
      chunklen = buffers[i].iov_len;    // Track how much to read to chunk
      while (chunklen > 0 && result > 0)
        {
          result = ::recv ((SOCKET) handle, chunkp, chunklen, 0);
          if (result > 0)
            {
              chunkp += result;
              chunklen -= result;
              bytes_received += result;
            }
        }
    }
# endif /* ACE_HAS_WINSOCK2 != 0 */

  if (result == SOCKET_ERROR)
    {
      ACE_OS::set_errno_to_wsa_last_error ();
      return -1;
    }
  else
    return (ssize_t) bytes_received;
#else
  return ACE_OS::readv (handle, buffers, n);
#endif /* ACE_HAS_WINSOCK2 */
}

ACE_INLINE int
ACE_OS::send (ACE_HANDLE handle, const char *buf, size_t len, int flags)
{
  ACE_OS_TRACE ("ACE_OS::send");

  // On UNIX, a non-blocking socket with no data to receive, this
  // system call will return EWOULDBLOCK or EAGAIN, depending on the
  // platform.  UNIX 98 allows either errno, and they may be the same
  // numeric value.  So to make life easier for upper ACE layers as
  // well as application programmers, always change EAGAIN to
  // EWOULDBLOCK.  Rather than hack the ACE_OSCALL_RETURN macro, it's
  // handled explicitly here.  If the ACE_OSCALL macro ever changes,
  // this function needs to be reviewed.  On Win32, the regular macros
  // can be used, as this is not an issue.
#if defined (ACE_WIN32)
  ACE_SOCKCALL_RETURN (::send ((ACE_SOCKET) handle,
                               buf,
                               ACE_static_cast (int, len),
                               flags), int, -1);
#else
  int ace_result_;
#  if defined (VXWORKS) || defined (HPUX) || defined (ACE_PSOS)
  ace_result_ = ::send ((ACE_SOCKET) handle, (char *) buf, len, flags);
#  else
  ace_result_ = ::send ((ACE_SOCKET) handle, buf, len, flags);
#  endif /* VXWORKS */
  if (ace_result_ == -1 && errno == EAGAIN)
    errno = EWOULDBLOCK;
  return ace_result_;
#endif /* defined (ACE_WIN32) */
}

ACE_INLINE int
ACE_OS::sendmsg (ACE_HANDLE handle,
                 const struct msghdr *msg,
                 int flags)
{
  ACE_OS_TRACE ("ACE_OS::sendmsg");
#if !defined (ACE_LACKS_SENDMSG)
# if (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  DWORD bytes_sent = 0;
  int result = ::WSASendTo ((SOCKET) handle,
                            (WSABUF *) msg->msg_iov,
                            msg->msg_iovlen,
                            &bytes_sent,
                            flags,
                            msg->msg_name,
                            msg->msg_namelen,
                            0,
                            0);

  if (result != 0)
    {
      ACE_OS::set_errno_to_last_error ();
      return -1;
    }
  else
    return (ssize_t) bytes_sent;
# elif defined (ACE_LACKS_POSIX_PROTOTYPES) ||  defined (ACE_PSOS)
  ACE_SOCKCALL_RETURN (::sendmsg (handle, (struct msghdr *) msg, flags), int, -1);
# else
  ACE_SOCKCALL_RETURN (::sendmsg (handle, (ACE_SENDMSG_TYPE *) msg, flags), int, -1);
# endif /* ACE_LACKS_POSIX_PROTOTYPES */
#else
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (msg);
  ACE_UNUSED_ARG (handle);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_LACKS_SENDMSG */
}

ACE_INLINE int
ACE_OS::sendto (ACE_HANDLE handle,
                const char *buf,
                size_t len,
                int flags,
                const struct sockaddr *addr,
                int addrlen)
{
  ACE_OS_TRACE ("ACE_OS::sendto");
#if defined (VXWORKS)
  ACE_SOCKCALL_RETURN (::sendto ((ACE_SOCKET) handle, (char *) buf, len, flags,
                                 ACE_const_cast (struct sockaddr *, addr), addrlen),
                       int, -1);
#elif defined (ACE_PSOS)
#  if !defined (ACE_PSOS_DIAB_PPC)
  ACE_SOCKCALL_RETURN (::sendto ((ACE_SOCKET) handle, (char *) buf, len, flags,
                                 (struct sockaddr_in *) addr, addrlen),
                       int, -1);
#  else
  ACE_SOCKCALL_RETURN (::sendto ((ACE_SOCKET) handle, (char *) buf, len, flags,
                                 (struct sockaddr *) addr, addrlen),
                       int, -1);
#  endif /*defined ACE_PSOS_DIAB_PPC */
#else
#  if defined (ACE_WIN32)
  ACE_SOCKCALL_RETURN (::sendto ((ACE_SOCKET) handle, buf,
                                 ACE_static_cast (int, len), flags,
                                 ACE_const_cast (struct sockaddr *, addr), addrlen),
                       int, -1);
#  else
  ACE_SOCKCALL_RETURN (::sendto ((ACE_SOCKET) handle, buf, len, flags,
                                 ACE_const_cast (struct sockaddr *, addr), addrlen),
                       int, -1);
#  endif /* ACE_WIN32 */
#endif /* VXWORKS */
}

ACE_INLINE int
ACE_OS::sendto (ACE_HANDLE handle,
                const iovec *buffers,
                int buffer_count,
                size_t &number_of_bytes_sent,
                int flags,
                const struct sockaddr *addr,
                int addrlen,
                ACE_OVERLAPPED *overlapped,
                ACE_OVERLAPPED_COMPLETION_FUNC func)
{
  ACE_OS_TRACE ("ACE_OS::sendto");
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  DWORD bytes_sent;
  int result = ::WSASendTo ((SOCKET) handle,
                            (WSABUF*)buffers,
                            buffer_count,
                            &bytes_sent,
                            flags,
                            addr,
                            addrlen,
                            overlapped,
                            func);
  if (result != 0) {
    ACE_OS::set_errno_to_last_error ();
  }
  number_of_bytes_sent = ACE_static_cast (size_t, bytes_sent);
  return result;
#else
  ACE_UNUSED_ARG (overlapped);
  ACE_UNUSED_ARG (func);

  number_of_bytes_sent = 0;

  int result = 0;

  for (int i = 0; i < buffer_count; i++)
    {
       result = ACE_OS::sendto (handle,
                                ACE_reinterpret_cast (char *ACE_CAST_CONST,
                                                      buffers[i].iov_base),
                                buffers[i].iov_len,
                                flags,
                                addr,
                                addrlen);
       if (result == -1)
         break;
       number_of_bytes_sent += result;
    }

  return result;
#endif /* defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */
}

ACE_INLINE ssize_t
ACE_OS::sendv (ACE_HANDLE handle,
               const iovec *buffers,
               int n)
{
#if defined (ACE_HAS_WINSOCK2)
  DWORD bytes_sent = 0;
  int result = 0;

  // Winsock 2 has WSASend and can do this directly, but Winsock 1
  // needs to do the sends one-by-one.
# if (ACE_HAS_WINSOCK2 != 0)
  result = ::WSASend ((SOCKET) handle,
                      (WSABUF *) buffers,
                      n,
                      &bytes_sent,
                      0,
                      0,
                      0);
# else
  int i;
  for (i = 0; i < n && result != SOCKET_ERROR; i++)
    {
      result = ::send ((SOCKET) handle,
                       buffers[i].iov_base,
                       buffers[i].iov_len,
                       0);
      // Gets ignored on error anyway
      bytes_sent += buffers[i].iov_len;

      // If the transfer isnt complete just drop out of the loop.
      if (result < (int)buffers[i].iov_len)
        break;
    }
# endif /* ACE_HAS_WINSOCK2 != 0 */

  if (result == SOCKET_ERROR)
    {
      ACE_OS::set_errno_to_wsa_last_error ();
      return -1;
    }
  else
    return (ssize_t) bytes_sent;

#else
  return ACE_OS::writev (handle, buffers, n);
#endif /* ACE_HAS_WINSOCK2 */
}

ACE_INLINE int
ACE_OS::setsockopt (ACE_HANDLE handle,
                    int level,
                    int optname,
                    const char *optval,
                    int optlen)
{
  ACE_OS_TRACE ("ACE_OS::setsockopt");

  #if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) && defined(SO_REUSEPORT)
  // To work around an inconsistency with Microsofts implementation of
  // sockets, we will check for SO_REUSEADDR, and ignore it. Winsock
  // always behaves as if SO_REUSEADDR=1. Some implementations have the
  // same behaviour as Winsock, but use a new name for it. SO_REUSEPORT.
  // If you want the normal behaviour for SO_REUSEADDR=0, then NT 4 sp4 and later
  // supports SO_EXCLUSIVEADDRUSE. This also requires using an updated Platform SDK
  // so it was decided to ignore the option for now. (Especially since ACE always
  // sets SO_REUSEADDR=1, which we can mimic by doing nothing.)
  if (level == SOL_SOCKET) {
    if (optname == SO_REUSEADDR) {
      return 0; // Not supported by Winsock
    }
    if (optname == SO_REUSEPORT) {
      optname = SO_REUSEADDR;
    }
  }
  #endif /*ACE_HAS_WINSOCK2*/

  ACE_SOCKCALL_RETURN (::setsockopt ((ACE_SOCKET) handle,
                                     level,
                                     optname,
                                     (ACE_SOCKOPT_TYPE1) optval,
                                     optlen),
                       int,
                       -1);
}

ACE_INLINE int
ACE_OS::shutdown (ACE_HANDLE handle, int how)
{
  ACE_OS_TRACE ("ACE_OS::shutdown");
  ACE_SOCKCALL_RETURN (::shutdown ((ACE_SOCKET) handle, how), int, -1);
}

ACE_INLINE ACE_HANDLE
ACE_OS::socket (int domain,
                int type,
                int proto)
{
  ACE_OS_TRACE ("ACE_OS::socket");
  ACE_SOCKCALL_RETURN (::socket (domain,
                                 type,
                                 proto),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
}

ACE_INLINE ACE_HANDLE
ACE_OS::socket (int domain,
                int type,
                int proto,
                ACE_Protocol_Info *protocolinfo,
                ACE_SOCK_GROUP g,
                u_long flags)
{
  ACE_OS_TRACE ("ACE_OS::socket");

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  ACE_SOCKCALL_RETURN (::WSASocket (domain,
                                    type,
                                    proto,
                                    protocolinfo,
                                    g,
                                    flags),
                       ACE_HANDLE,
                       ACE_INVALID_HANDLE);
#else
  ACE_UNUSED_ARG (protocolinfo);
  ACE_UNUSED_ARG (g);
  ACE_UNUSED_ARG (flags);

  return ACE_OS::socket (domain,
                         type,
                         proto);
#endif /* ACE_HAS_WINSOCK2 */
}

ACE_INLINE int
ACE_OS::socketpair (int domain, int type,
                    int protocol, ACE_HANDLE sv[2])
{
  ACE_OS_TRACE ("ACE_OS::socketpair");
#if defined (ACE_WIN32) || defined (ACE_LACKS_SOCKETPAIR)
  ACE_UNUSED_ARG (domain);
  ACE_UNUSED_ARG (type);
  ACE_UNUSED_ARG (protocol);
  ACE_UNUSED_ARG (sv);

  ACE_NOTSUP_RETURN (-1);
#else
  ACE_OSCALL_RETURN (::socketpair (domain, type, protocol, sv),
                     int, -1);
#endif /* ACE_WIN32 */
}

