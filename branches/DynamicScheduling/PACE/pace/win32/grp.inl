/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/grp.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrgid (pace_gid_t gid)
{
  pace_group * retval = (pace_group*)0;
  PACE_UNUSED_ARG (gid);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (gid);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrnam (const char * name)
{
  pace_group * retval = (pace_group*)0;
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
