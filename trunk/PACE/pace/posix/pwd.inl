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

PACE_INLINE
pace_passwd *
pace_getpwuid (pace_uid_t uid)
{
  return getpwuid (uid);
}

PACE_INLINE
int
pace_getpwuid_r (pace_uid_t uid,
                 pace_passwd * pwd,
                 char * buffer,
                 pace_size_t bufsize,
                 pace_passwd ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getpwuid_r (uid, pwd, buffer, bufsize, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (uid);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
pace_passwd *
pace_getpwnam (const char * name)
{
  return getpwnam (name);
}

PACE_INLINE
int
pace_getpwnam_r (const char * name,
                 pace_passwd * pwd,
                 char * buffer,
                 pace_size_t bufsize,
                 pace_passwd ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getpwnam_r (name, pwd, buffer, bufsize, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (pwd);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
