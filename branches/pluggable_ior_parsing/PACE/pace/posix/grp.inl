/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/grp.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
struct group *
pace_getgrgid (pace_gid_t gid)
{
  return getgrgid (gid);
}

PACE_INLINE
int
pace_getgrgid_r (pace_gid_t gid,
                 struct group * grp,
                 char * buffer,
                 size_t bufsize,
                 struct group ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getgrgid_r (gid, grp, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
struct group *
pace_getgrnam (const char * name)
{
  return getgrnam (name);
}

PACE_INLINE
int
pace_getgrnam_r (const char * name,
                  struct group * grp,
                  char * buffer,
                  size_t bufsize,
                  struct group ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getgrnam_r (name, grp, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
