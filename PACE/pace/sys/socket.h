/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    socket.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_SYS_SOCKET_H
#define PACE_SYS_SOCKET_H


#include "pace/defines.h"
#include "pace/sys/types.h"
#include <sys/socket.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_closesocket (PACE_HANDLE handle);

  PACE_INLINE int pace_getpeername (int s,
                                    struct  sockaddr * name,
                                    socklen_t * namelen);

  PACE_INLINE int pace_getsockname (int s,
                                    struct sockaddr * name,
                                    socklen_t * namelen);

  PACE_INLINE int pace_getsockopt (int s,
                                   int level,
                                   int optname,
                                   void  * optval,
                                   socklen_t * optlen);

  PACE_INLINE int pace_setsockopt (int s,
                                   int level,
                                   int  optname,
                                   const  void * optval,
                                   socklen_t optlen);

  PACE_INLINE int pace_socket (int domain,
                               int type,
                               int protocol);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/sys/socket.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_SYS_SOCKET_H */
