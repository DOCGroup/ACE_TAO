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
pace_pid_t
pace_wait (int * statloc)
{
  PACE_UNUSED_ARG (statloc);

  {
    pace_pid_t retval = -1;
    PACE_ERRNO_NO_SUPPORT_RETURN (retval);
  }
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pace_pid_t
pace_waitpid (pace_pid_t pid, int * statloc, int options)
{
  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (statloc);
  PACE_UNUSED_ARG (options);

  /* ACE version uses a HANDLE */
  {
    pace_pid_t retval = -1;
    PACE_ERRNO_NO_SUPPORT_RETURN (retval);
  }
}
#endif /* PACE_HAS_POSIX_MP_UOF */
