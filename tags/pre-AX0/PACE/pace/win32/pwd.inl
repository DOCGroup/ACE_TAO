/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pwd.inl
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
  pace_passwd * retval = (pace_passwd*)0;
  PACE_UNUSED_ARG (uid);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (uid);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_passwd *
pace_getpwnam (const char * name)
{
  pace_passwd * retval = (pace_passwd*)0;
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
