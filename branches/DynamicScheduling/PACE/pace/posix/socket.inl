/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/socket.inl
 *
 * = AUTHOR
 *    Luther Baker
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
    setsockopt (s, level, optname, optval);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_isfdtype (int fildes, int fdtype)
{
    isfdtype (fildes, fdtype);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_listen (int s, int backlog)
{
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
    sendmsg (s, msg, flags);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_shutdown (int s, int how)
{
    shutdown (s, how);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */

#if (PACE_HAS_POSIX_SOCK_UOF)
PACE_INLINE
int
pace_socket (int protofamily, int type, int protocol)
{
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
    socketpair (protofamily, type, protocol, sy);
}
#endif /* PACE_HAS_POSIX_SOCK_UOF */











