/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/socket.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <unistd.h>
#include <stdarg.h>

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_accept (int s,
             pace_sockaddr * socketaddress,
             pace_socklen_t * addresslen)
{
  PACE_TRACE("pace_accept");

  accept (s, socketaddress, addresslen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_bind (int s,
           const pace_sockaddr * socketaddress,
           pace_socklen_t addresslen)
{
  PACE_TRACE("pace_bind");

  bind (s, socketaddress, addresslen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_connect (int s,
              const pace_sockaddr * socketaddress,
              pace_socklen_t addresslen)
{
  PACE_TRACE("pace_connect");

  connect (s, socketaddress, addresslen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_getpeername (int s,
                  pace_sockaddr * socketaddress,
                  pace_socklen_t * addresslen)
{
  PACE_TRACE("pace_getpeername");

  getpeername (s, socketaddress, addresslen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_getsockname (int s,
                  pace_sockaddr * socketaddress,
                  pace_socklen_t * addresslen)
{
  PACE_TRACE("pace_getsockname");

  getsockname (s, socketaddress, addresslen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_getsockopt (int s,
                 int level,
                 int optname,
                 void * optval,
                 pace_socklen_t optlen)
{
  PACE_TRACE("pace_getsockopt");

  getsockopt (s, level, optname, optval, optlen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_setsockopt (int s,
                 int level,
                 int optname,
                 const * optval)
{
  PACE_TRACE("pace_setsockopt");

  setsockopt (s, level, optname, optval);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_setsockopt (int s,
                 int level,
                 int optname,
                 const * optval)
{
  PACE_TRACE("pace_setsockopt");

  setsockopt (s, level, optname, optval);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_isfdtype (int fildes, int fdtype)
{
  PACE_TRACE("pace_isfdtype");

  isfdtype (fildes, fdtype);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_listen (int s, int backlog)
{
  PACE_TRACE("pace_listen");

  listen (s, backlog);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_recv (int s,
           void * buf,
           pace_size_t len,
           int flags)
{
  PACE_TRACE("pace_recv");

  recv (s, buf, len, flags);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_recvfrom (int s,
               void * buf,
               pace_size_t len,
               int flags,
               pace_sockaddr * from,
               pace_socklen_t * fromlen)
{
  PACE_TRACE("pace_recvfrom");

  recvfrom (s, buf, len, flags, from, fromlen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_rcvmsg (int s,
             pace_msghdr * msg,
             int flags)
{
  PACE_TRACE("pace_rcvmsg");

  rcvmsg (s, msg, flags);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_send (int s,
           const void * buf,
           pace_size_t len,
           int flags)
{
  PACE_TRACE("pace_send");

  send (s, buf, len, flags);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_sendto (int s,
             const void * buf,
             pace_size_t len,
             int flags,
             const pace_sockaddr * to,
             pace_socklen_t * tolen)
{
  PACE_TRACE("pace_sendto");

  sendto (s, buf, len, flags, to, tolen);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
pace_ssize_t
pace_sendmsg (int s,
              const pace_msghdr * msg,
              int flags)
{
  PACE_TRACE("pace_sendmsg");

  sendmsg (s, msg, flags);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_shutdown (int s, int how)
{
  PACE_TRACE("pace_shutdown");

  shutdown (s, how);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_socket (int protofamily, int type, int protocol)
{
  PACE_TRACE("pace_socket");

  socket (protofamily, type, protocol);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_socketpair (int protofamily,
                 int type,
                 int protocol,
                 int sv[2])
{
  PACE_TRACE("pace_socketpair");

  socketpair (protofamily, type, protocol, sy);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */











