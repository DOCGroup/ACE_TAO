/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/setjmp.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_setjmp (jmp_buf env)
{
  return setjmp (env);
}

PACE_INLINE
int
pace_sigsetjmp (sigjmp_buf env, int savemask)
{
  return sigsetjmp (env, savemask);
}

PACE_INLINE
void
pace_longjmp (jmp_buf env, int val)
{
  longjmp (env, val);
  return;
}

PACE_INLINE
void
pace_siglongjmp (sigjmp_buf env, int val)
{
  siglongjmp (env, val);
  return;
}
