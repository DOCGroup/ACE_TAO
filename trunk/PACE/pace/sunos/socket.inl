/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    socket.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include <unistd.h>


PACE_INLINE
int
pace_closesocket (PACE_HANDLE handle)
{
  return close (handle);
}

PACE_INLINE
int
pace_getpeername (int s,
                  struct  sockaddr * name,
                  socklen_t * namelen)
{
  return getpeername (s, name, namelen);
}

PACE_INLINE
int
pace_getsockname (int s,
                  struct sockaddr * name,
                  socklen_t * namelen)
{
  return getsockname (s, name, namelen);
}

PACE_INLINE
int
pace_getsockopt (int s,
                 int level,
                 int optname,
                 void  * optval,
                 socklen_t * optlen)
{
  return getsockopt (s, level, optname, optval, optlen);
}

PACE_INLINE
int
pace_setsockopt (int s,
                 int level,
                 int  optname,
                 const  void * optval,
                 socklen_t optlen)
{
  return setsockopt (s, level, optname, optval, optlen);
}

PACE_INLINE
int
pace_socket (int domain,
             int type,
             int protocol)
{
  return socket (domain, type, protocol);
}
