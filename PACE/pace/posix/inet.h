/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/inet.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_ARPA_INET_H
#define PACE_ARPA_INET_H

#include "pace/defines.h"
#include <arpa/inet.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE unsigned long pace_inet_addr (const char * cp);

  PACE_INLINE unsigned long pace_inet_network (const char * cp);

  PACE_INLINE struct in_addr pace_inet_makeaddr (const int net, const int lna);

  PACE_INLINE int pace_inet_lnaof (const struct in_addr in);

  PACE_INLINE int pace_inet_netof (const struct in_addr in);

  PACE_INLINE char * pace_inet_ntoa (const struct in_addr in);

  PACE_INLINE int pace_inet_aton (const char * str, struct in_addr * addr);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/inet.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_ARPA_INET_H */
