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
  siglongjmp (env, val);
  return;
}
#endif /* PACE_HAS_POSIX_SIG_UOF */
