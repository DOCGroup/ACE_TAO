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
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return readdir_r (dirp, entry, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (dirp);
  PACE_UNUSED_ARG (entry);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
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
