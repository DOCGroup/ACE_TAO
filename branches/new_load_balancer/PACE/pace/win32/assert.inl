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
  assert (expression);
  return;
}
