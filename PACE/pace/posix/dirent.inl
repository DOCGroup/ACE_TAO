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

PACE_INLINE
DIR *
pace_opendir (const char * dirname)
{
  return opendir (dirname);
}

PACE_INLINE
struct dirent *
pace_readdir (DIR * dirp)
{
  return readdir (dirp);
}

PACE_INLINE
int
pace_readdir_r (DIR * dirp, struct dirent * entry,
                struct dirent ** result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return readdir_r (dirp, entry, result);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
void
pace_rewinddir (DIR * dirp)
{
  rewinddir (dirp);
  return;
}

PACE_INLINE
int
pace_closedir (DIR * dirp)
{
  return closedir (dirp);
}
