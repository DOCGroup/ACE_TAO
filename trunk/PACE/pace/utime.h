/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    utime.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_UTIME_H
#define PACE_UTIME_H


#include "pace/defines.h"
#include "pace/sys/types.h"
#include <utime.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_utime (const char * path,
                              const struct utimbuf * times);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/utime.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_UTIME_H */
