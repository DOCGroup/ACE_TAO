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
struct passwd *
pace_getpwuid (uid_t uid)
{
  return getpwuid (uid);
}

PACE_INLINE
int
pace_getpwuid_r (uid_t uid,
                 struct passwd * pwd,
                 char * buffer,
                 size_t bufsize,
                 struct passwd ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getpwuid_r (uid, pwd, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
struct passwd *
pace_getpwnam (const char * name)
{
  return getpwnam (name);
}

PACE_INLINE
int
pace_getpwnam_r (const char * name,
                 struct passwd * pwd,
                 char * buffer,
                 size_t bufsize,
                 struct passwd ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getpwnam_r (name, pwd, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
