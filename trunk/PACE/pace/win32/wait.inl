/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/wait.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_wait (int * statloc)
{
  pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_waitpid (pid_t pid, int * statloc, int options)
{
  /* ACE version uses a HANDLE */
  pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_MP_UOF */
