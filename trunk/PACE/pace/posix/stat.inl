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

PACE_INLINE
int
pace_chmod (const char * path, pace_mode_t mode)
{
  return chmod (path, mode);
}

PACE_INLINE
int
pace_fchmod (int fildes, pace_mode_t mode)
{
  return fchmod (fildes, mode);
}

PACE_INLINE
int
pace_fstat (int fildes, pace_stat_s * buf)
{
  return fstat (fildes, buf);
}

PACE_INLINE
int
pace_mkdir (const char * path, pace_mode_t mode)
{
  return mkdir (path, mode);
}

PACE_INLINE
int
pace_mkfifo (const char * path, pace_mode_t mode)
{
  return mkfifo (path, mode);
}

PACE_INLINE
int
pace_stat (const char * path, pace_stat_s * buf)
{
  return stat (path, buf);
}

PACE_INLINE
pace_mode_t
pace_umask (pace_mode_t cmask)
{
  return umask (cmask);
}
