/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    times.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_SYS_TIMES_H
#define PACE_SYS_TIMES_H


#include "pace/defines.h"
#include <sys/times.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE clock_t pace_times (struct tms * buffer);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "times.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_SYS_TIMES_H */
