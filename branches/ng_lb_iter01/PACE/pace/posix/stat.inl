/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stat.inl
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
  return chmod (path, mode);
}
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fchmod (PACE_HANDLE fildes, pace_mode_t mode)
{
  return fchmod (fildes, mode);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_fstat (PACE_HANDLE fildes, pace_stat_s * buf)
{
  return fstat (fildes, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_mkdir (const char * path, pace_mode_t mode)
{
  return mkdir (path, mode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_F_UOF)
PACE_INLINE
int
pace_mkfifo (const char * path, pace_mode_t mode)
{
  return mkfifo (path, mode);
}
#endif /* PACE_HAS_POSIX_F_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_stat (const char * path, pace_stat_s * buf)
{
  return stat (path, buf);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
pace_mode_t
pace_umask (pace_mode_t cmask)
{
  return umask (cmask);
}
#endif /* PACE_HAS_POSIX_FA_UOF */
