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

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_longjmp (pace_jmp_buf env, int val)
{
  longjmp (env, val);
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
void
pace_siglongjmp (pace_sigjmp_buf env, int val)
{
  PACE_UNUSED_ARG (env);
  PACE_UNUSED_ARG (val);
  PACE_ERRNO_NO_SUPPORT ();
  return;
}
#endif /* PACE_HAS_POSIX_SIG_UOF */
