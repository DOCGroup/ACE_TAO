/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/fcntl.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_creat (const char * path, pace_mode_t mode)
{
  /* Emulation is still buggy! creat (path, mode); */
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_fcntl (PACE_HANDLE fildes, int cmd, /* arg */ ... )
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (cmd);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

