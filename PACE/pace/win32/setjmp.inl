/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/setjmp.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_sigsetjmp (sigjmp_buf env, int savemask)
{
  return sigsetjmp (env, savemask);
}

PACE_INLINE
void
pace_siglongjmp (sigjmp_buf env, int val)
{
  siglongjmp (env, val);
  return;
}
