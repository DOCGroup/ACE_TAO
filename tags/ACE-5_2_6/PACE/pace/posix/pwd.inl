/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/pwd.inl
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
  return getpwuid (uid);
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
#if (PACE_LYNXOS)
  if (getpwuid_r (pwd, uid, buffer, bufsize) == 0)
    /* Successful search */
    {
      **result = *pwd;
      return 0;
    }

  return -1;
#else
  return getpwuid_r (uid, pwd, buffer, bufsize, result);
#endif /* ! PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_passwd *
pace_getpwnam (const char * name)
{
  return getpwnam (name);
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
# if (PACE_LYNXOS)
  if (getpwnam_r (pwd, PACE_NONCONST_ARG_CAST (char *) name,
                  buffer, bufsize) == 0)
    /* Successful search */
    {
      **result = *pwd;
      return 0;
    }

  return -1;
#else
  return getpwnam_r (name, pwd, buffer, bufsize, result);
#endif /* ! PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
