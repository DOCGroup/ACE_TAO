/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/assert.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_ASSERT_H
#define PACE_ASSERT_H

#include "pace/defines.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE void pace_assert (int expression);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/assert.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_ASSERT_H */
