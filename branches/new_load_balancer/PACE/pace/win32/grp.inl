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

PACE_INLINE
pace_group *
pace_getgrgid (pace_gid_t gid)
{
  return getgrgid (gid);
}

PACE_INLINE
int
pace_getgrgid_r (pace_gid_t gid,
                 pace_group * grp,
                 char * buffer,
                 size_t bufsize,
                 pace_group ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getgrgid_r (gid, grp, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (gid);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
pace_group *
pace_getgrnam (const char * name)
{
  return getgrnam (name);
}

PACE_INLINE
int
pace_getgrnam_r (const char * name,
                  pace_group * grp,
                  char * buffer,
                  size_t bufsize,
                  pace_group ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getgrnam_r (name, grp, buffer, bufsize, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (grp);
  PACE_UNUSED_ARG (buffer);
  PACE_UNUSED_ARG (bufsize);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
