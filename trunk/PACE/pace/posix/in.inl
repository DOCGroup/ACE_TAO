/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/in.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <netinet/in.h>

PACE_INLINE
PACE_UINT32
pace_htonl (PACE_UINT32 hostlong)
{
  return htonl (hostlong);
}

PACE_INLINE
PACE_UINT16
pace_htons (PACE_UINT16 hostshort)
{
  return htons (hostshort);
}

PACE_INLINE
PACE_UINT32
pace_ntohl (PACE_UINT32 netlong)
{
  return ntohl (netlong);
}

PACE_INLINE
PACE_UINT16
pace_ntohs (PACE_UINT16 netshort)
{
  return ntohs (netshort);
}
