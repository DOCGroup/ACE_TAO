/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/assert.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <assert.h>

PACE_INLINE
void
pace_assert (int expression)
{
#if defined (PACE_NDEBUG)
  PACE_UNUSED_ARG (expression);
#else  /* ! PACE_NDEBUG */
  assert (expression);
#endif /* ! PACE_NDEBUG */
}
