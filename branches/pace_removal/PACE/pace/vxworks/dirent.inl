/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/dirent.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
PACE_DIR *
pace_opendir (const char * dirname)
{
  PACE_TRACE("pace_opendir");

  return opendir (PACE_NONCONST_ARG_CAST(char *) dirname);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
pace_dirent *
pace_readdir (PACE_DIR * dirp)
{
  PACE_TRACE("pace_readdir");

  return readdir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
int
pace_readdir_r (PACE_DIR * dirp, pace_dirent * entry,
                pace_dirent ** result)
{
  PACE_TRACE("pace_readdir_r");

  PACE_UNUSED_ARG (entry);
  /* <result> has better not be 0! */
  *result = pace_readdir (dirp);
  return 0;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
void
pace_rewinddir (PACE_DIR * dirp)
{
  PACE_TRACE("pace_rewinddir");

  rewinddir (dirp);
  return;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_closedir (PACE_DIR * dirp)
{
  PACE_TRACE("pace_closedir");

  return closedir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
