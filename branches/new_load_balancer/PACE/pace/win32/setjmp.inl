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
pace_setjmp (pace_jmp_buf env)
{
  return setjmp (env);
}

PACE_INLINE
int
pace_sigsetjmp (pace_sigjmp_buf env, int savemask)
{
  return sigsetjmp (env, savemask);
}

PACE_INLINE
void
pace_longjmp (pace_jmp_buf env, int val)
{
  longjmp (env, val);
  return;
}

PACE_INLINE
void
pace_siglongjmp (pace_sigjmp_buf env, int val)
{
  siglongjmp (env, val);
  return;
}
