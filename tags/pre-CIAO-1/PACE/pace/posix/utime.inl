/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/utime.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_utime (const char * path,
            const pace_utimbuf * times)
{
  return utime (path, times);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
