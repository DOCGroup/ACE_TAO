/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/grp.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrgid (pace_gid_t gid)
{
  PACE_TRACE("pace_getgrgid");

  /* getgrgid() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (gid);
  return 0;
}
#endif /* PACE_HAS_POSIX_SD_UOF */

#if (PACE_HAS_POSIX_SDR_UOF)
PACE_INLINE
int
pace_getgrgid_r (pace_gid_t gid,
                 pace_group * grp,
                 char * buffer,
                 size_t bufsize,
                 pace_group ** result)
{
  PACE_TRACE("pace_getgrgid_r");

  /* getgrgid_r() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (gid);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  return 0;
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrnam (const char * name)
{
  PACE_TRACE("pace_getgrnam");

  /* getgrnam() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (name);
  return 0;
}
#endif /* PACE_HAS_POSIX_SD_UOF */

#if (PACE_HAS_POSIX_SDR_UOF)
PACE_INLINE
int
pace_getgrnam_r (const char * name,
                 pace_group * grp,
                 char * buffer,
                 size_t bufsize,
                 pace_group ** result)
{
  PACE_TRACE("pace_getgrnam_r");

  /* getgrnam_r() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  return 0;
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
