/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/setjmp.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_longjmp (pace_jmp_buf env, int val)
{
  PACE_TRACE("pace_longjmp");

  longjmp (env, val);
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
void
pace_siglongjmp (pace_sigjmp_buf env, int val)
{
  PACE_TRACE("pace_siglongjmp");

  siglongjmp (env, val);
  return;
}
#endif /* PACE_HAS_POSIX_SIG_UOF */
