/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/utime.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_utime (const char * path,
            const pace_utimbuf * times)
{
  PACE_TRACE("pace_utime");

  return utime (PACE_NONCONST_ARG_CAST (char *) path,
                PACE_NONCONST_ARG_CAST (pace_utimbuf *) times);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
