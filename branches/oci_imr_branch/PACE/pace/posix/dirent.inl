/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/dirent.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
PACE_DIR *
pace_opendir (const char * dirname)
{
  return opendir (dirname);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
pace_dirent *
pace_readdir (PACE_DIR * dirp)
{
  return readdir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
int
pace_readdir_r (PACE_DIR * dirp, pace_dirent * entry,
                pace_dirent ** result)
{
#if (PACE_LYNXOS)
  *result = readdir_r (dirp, entry);
  if (*result == 0)
    return errno;
  return 0;
#else
  return readdir_r (dirp, entry, result);
#endif /* PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
void
pace_rewinddir (PACE_DIR * dirp)
{
  rewinddir (dirp);
  return;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_closedir (PACE_DIR * dirp)
{
  return closedir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
