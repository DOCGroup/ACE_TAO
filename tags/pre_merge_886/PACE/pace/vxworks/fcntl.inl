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
  return creat (path, mode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
