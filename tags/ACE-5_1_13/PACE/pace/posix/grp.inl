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

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrgid (pace_gid_t gid)
{
  return getgrgid (gid);
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
#if (PACE_LYNXOS)
  *result = getgrgid_r (grp, gid, buffer, bufsize);
  if (*result == 0)
    return errno;
  return 0;
#else
  return getgrgid_r (gid, grp, buffer, bufsize, result);
#endif /* ! PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_SDR_UOF */

#if (PACE_HAS_POSIX_SD_UOF)
PACE_INLINE
pace_group *
pace_getgrnam (const char * name)
{
  return getgrnam (name);
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
#if (PACE_LYNXOS)
  *result = getgrnam_r (grp, PACE_NONCONST_ARG_CAST (char *) name,
                        buffer, bufsize);
  if (*result == 0)
    return errno;
  return 0;
#else
  return getgrnam_r (name, grp, buffer, bufsize, result);
#endif /* ! PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_SDR_UOF */
