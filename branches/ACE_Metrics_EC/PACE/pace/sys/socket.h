/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/socket.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SOCKET_H
#define PACE_SOCKET_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/socket.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/socket.h"
#elif (PACE_WIN32)
# include "pace/win32/socket.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

/**
   PACE's implementation of the POSIX function accept.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_accept (int s,
                                             pace_sockaddr * socketaddress,
                                             pace_socklen_t * addresslen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function bind.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_bind (int s,
                                           const pace_sockaddr * socketaddress,
                                           pace_socklen_t addresslen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function connect.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_connect (int s,
                                              const pace_sockaddr * socketaddress,
                                              pace_socklen_t addresslen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function getpeername.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_getpeername (int s,
                                                  pace_sockaddr * socketaddress,
                                                  pace_socklen_t * addresslen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function getsockname.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_getsockname (int s,
                                                  pace_sockaddr * socketaddress,
                                                  pace_socklen_t * addresslen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function getsockopt.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_getsockopt (int s,
                                                 int level,
                                                 int optname,
                                                 void * optval,
                                                 pace_socklen_t optlen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function setsockopt.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_setsockopt (int s,
                                                 int level,
                                                 int optname,
                                                 const * optval);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function isfdtype.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_isfdtype (int fildes, int fdtype);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function listen.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_listen (int s, int backlog);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function recv.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_recv (int s,
                                                    void * buf,
                                                    pace_size_t len,
                                                    int flags);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function recvfrom.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_recvfrom (int s,
                                                        void * buf,
                                                        pace_size_t len,
                                                        int flags,
                                                        pace_sockaddr * from,
                                                        pace_socklen_t * fromlen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function recvmsg.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_rcvmsg (int s,
                                                      pace_msghdr * msg,
                                                      int flags);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function send.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_send (int s,
                                                    const void * buf,
                                                    pace_size_t len,
                                                    int flags);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function sendto.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_sendto (int s,
                                                      const void * buf,
                                                      pace_size_t len,
                                                      int flags,
                                                      const pace_sockaddr * to,
                                                      pace_socklen_t * tolen);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function sendmsg.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE pace_ssize_t pace_sendmsg (int s,
                                                       const pace_msghdr * msg,
                                                       int flags);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function shutdown.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_shutdown (int s, int how);
#endif /* PACE_HAS_POSIX_SOCK_UOF */

/**
   PACE's implementation of the POSIX function socket.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_socket (int protofamily,
                                             int type,
                                             int protocol);
#endif /* PACE_HAS_POSIX_SOCK_UOF */
    
/**
   PACE's implementation of the POSIX function socketpair.
   See IEEE Standards Project Approved 30 January 2000.
   (IEEE Std P1003.1g, D6.6, March 1997 Edition)
*/
#if (PACE_HAS_POSIX_SOCK_UOF)
    PACE_Export PACE_INLINE int pace_socketpair (int protofamily,
                                                 int type,
                                                 int protocol,
                                                 int sv[2]);
#endif /* PACE_HAS_POSIX_SOCK_UOF */
    
#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/socket.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/socket.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/socket.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SOCKET_H */

