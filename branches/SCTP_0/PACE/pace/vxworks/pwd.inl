/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/pwd.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_passwd *
pace_getpwuid (pace_uid_t uid)
{
  PACE_TRACE("pace_getpwuid");

  /* getpwuid() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (uid);
  return 0;
}
#endif /* PACE_HAS_POSIX_SD_UOF */

#if (PACE_HAS_POSIX_SDR_UOF)
PACE_INLINE
int
pace_getpwuid_r (pace_uid_t uid,
                 pace_passwd * pwd,
                 char * buffer,
                 pace_size_t bufsize,
                 pace_passwd ** result)
{
  PACE_TRACE("pace_getpwuid_r");

  /* getpwuid_r() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (uid);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  return 0;
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_passwd *
pace_getpwnam (const char * name)
{
  PACE_TRACE("pace_getpwnam");

  /* getpwuid() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (name);
  return 0;
}
#endif /* PACE_HAS_POSIX_SD_UOF */

#if (PACE_HAS_POSIX_SDR_UOF)
PACE_INLINE
int
pace_getpwnam_r (const char * name,
                 pace_passwd * pwd,
                 char * buffer,
                 pace_size_t bufsize,
                 pace_passwd ** result)
{
  PACE_TRACE("pace_getpwnam_r");

  /* getpwuid_r() is not supported:  just one user anyways */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  return 0;
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
