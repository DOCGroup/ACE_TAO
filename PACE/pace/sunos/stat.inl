/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    stat.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


PACE_INLINE
int
pace_chmod (const char * path, mode_t mode)
{
  return chmod (path, mode);
}

PACE_INLINE
int
pace_fchmod (int fildes, mode_t mode)
{
  return fchmod (fildes, mode);
}

PACE_INLINE
int
pace_fstat (int fildes, struct stat * buf)
{
  return fstat (fildes, buf);
}

PACE_INLINE
int
pace_mkdir (const char * path, mode_t mode)
{
  return mkdir (path, mode);
}

PACE_INLINE
int
pace_mkfifo (const char * path, mode_t mode)
{
  return mkfifo (path, mode);
}

PACE_INLINE
int
pace_stat (const char * path, struct stat * buf)
{
  return stat (path, buf);
}

PACE_INLINE
mode_t
pace_umask (mode_t cmask)
{
  return umask (cmask);
}
