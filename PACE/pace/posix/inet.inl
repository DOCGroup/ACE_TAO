/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/inet.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/sys/socket.h"
#include "pace/sys/types.h"
#include "pace/netinet/in.h"

PACE_INLINE
unsigned long
pace_inet_addr (const char * cp)
{
  return inet_addr (cp);
}

PACE_INLINE
unsigned long
pace_inet_network (const char * cp)
{
  return inet_network (cp);
}

PACE_INLINE
struct in_addr
pace_inet_makeaddr (const int net, const int lna)
{
  return inet_makeaddr (net, lna);
}

PACE_INLINE
int
pace_inet_lnaof (const struct in_addr in)
{
  return inet_lnaof (in);
}

PACE_INLINE
int
pace_inet_netof (const struct in_addr in)
{
  return inet_netof (in);
}

PACE_INLINE
char *
pace_inet_ntoa (const struct in_addr in)
{
  return inet_ntoa (in);
}

PACE_INLINE
int
pace_inet_aton (const char * str, struct in_addr * addr)
{
  return inet_aton (str, addr);
}
