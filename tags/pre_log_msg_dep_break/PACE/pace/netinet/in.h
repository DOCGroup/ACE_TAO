/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    in.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_NETINET_IN_H
#define PACE_NETINET_IN_H


#include "pace/defines.h"
#include "pace/sys/types.h"


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE PACE_UINT32 pace_htonl (PACE_UINT32 hostlong);

  PACE_INLINE PACE_UINT16 pace_htons (PACE_UINT16 hostshort);

  PACE_INLINE PACE_UINT32 pace_ntohl (PACE_UINT32 netlong);

  PACE_INLINE PACE_UINT16 pace_ntohs (PACE_UINT16 netshort);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/netinet/in.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_NETINET_IN_H */
