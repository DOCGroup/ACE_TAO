/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/stat.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chmod (const char * path, pace_mode_t mode)
{
  PACE_TRACE("pace_chmod");

  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fchmod (PACE_HANDLE fildes, pace_mode_t mode)
{
  PACE_TRACE("pace_fchmod");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_fstat (PACE_HANDLE fildes, pace_stat_s * buf)
{
  PACE_TRACE("pace_fstat");

  return fstat (fildes, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_mkdir (const char * path, pace_mode_t mode)
{
  PACE_TRACE("pace_mkdir");

  PACE_UNUSED_ARG (mode);
  return mkdir (PACE_NONCONST_ARG_CAST (char *) path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_F_UOF)
PACE_INLINE
int
pace_mkfifo (const char * path, pace_mode_t mode)
{
  PACE_TRACE("pace_mkfifo");

  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_F_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_stat (const char * path, pace_stat_s * buf)
{
  PACE_TRACE("pace_stat");

  return stat (PACE_NONCONST_ARG_CAST (char *) path, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
pace_mode_t
pace_umask (pace_mode_t cmask)
{
  PACE_TRACE("pace_umask");

  PACE_UNUSED_ARG(cmask);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FA_UOF */
