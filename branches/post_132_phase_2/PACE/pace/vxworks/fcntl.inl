/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/fcntl.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_creat (const char * path, pace_mode_t mode)
{
  PACE_TRACE("pace_creat");

  return creat (path, mode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
int
pace_fcntl (PACE_HANDLE fildes, int cmd, long arg)
{
  PACE_TRACE("pace_fcntl");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(cmd);
  PACE_UNUSED_ARG(arg);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
