/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stat.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chmod (const char * path, pace_mode_t mode)
{
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fchmod (int fildes, pace_mode_t mode)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_fstat (int fildes, pace_stat_s * buf)
{
  return _fstat (fildes, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_mkdir (const char * path, pace_mode_t mode)
{
  return win32_mkdir (path, mode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_F_UOF)
PACE_INLINE
int
pace_mkfifo (const char * path, pace_mode_t mode)
{
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_F_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_stat (const char * path, pace_stat_s * buf)
{
  return _stat (path, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
pace_mode_t
pace_umask (pace_mode_t cmask)
{
  return _umask (cmask);
}
#endif /* PACE_HAS_POSIX_FA_UOF */
