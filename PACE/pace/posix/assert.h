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

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /** 
     PACE's implementation of the POSIX macro assert.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1..
   */
  PACE_INLINE void pace_assert (int expression);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/assert.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_ASSERT_H */
