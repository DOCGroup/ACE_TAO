/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/assert.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_ASSERT_H
#define PACE_ASSERT_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/assert.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/assert.h"
#elif (PACE_WIN32)
# include "pace/win32/assert.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX macro assert.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.

     Use a macro because assert uses __FILE__ and __LINE__, which
     would not be useful with an inline function.
   */

#if (PACE_HAS_POSIX_MP_UOF)
#if defined (PACE_NDEBUG)
# define pace_assert(X)
#else  /* ! PACE_NDEBUG */
# define pace_assert(X) assert (X)
#endif /* ! PACE_NDEBUG */
#endif /* PACE_HAS_POSIX_MP_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/assert.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/assert.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/assert.inl"
#  endif
#endif /* PACE_HAS_INLINE */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_ASSERT_H */
