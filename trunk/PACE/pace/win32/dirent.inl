/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/dirent.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/string.h"
#include "pace/stdlib.h"

#if (PACE_HAS_POSIX_FS_UOF)
int
pace_win32_emulate_closedir (PACE_DIR * dirp)
{
  if (dirp->current_handle_ != (void*)0)
    {
      FindClose (dirp->current_handle_);
    }
  dirp->current_handle_ = (void*)0;
  dirp->started_reading_ = 0;
  return 1;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_DIR *
pace_win32_emulate_opendir (const char * dirname)
{
  PACE_DIR * dirp = (PACE_DIR*) pace_malloc (sizeof (PACE_DIR));
  const pace_size_t dir_size = pace_strlen (dirname) + 1;
  dirp->directory_name_ = (char*) pace_malloc (dir_size);
  pace_strcpy (dirp->directory_name_, dirname);
  dirp->current_handle_ = PACE_WIN32_INVALID_HANDLE;
  dirp->started_reading_ = 0;
  return dirp;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
pace_dirent *
pace_win32_emulate_readdir (PACE_DIR * dirp)
{
  if (dirp->started_reading_)
    {
      dirp->current_handle_ = FindFirstFile (dirp->directory_name_,
                                             &(dirp->fdata_));
      if (dirp->current_handle_ != PACE_WIN32_INVALID_HANDLE)
        {
          FindClose (dirp->current_handle_);
          dirp->current_handle_ = PACE_WIN32_INVALID_HANDLE;
        }
      else /* Skip "." and ".." */
        {
          int retval = 1;
          while (*(dirp->fdata_.cFileName) == '.'
                 && retval
                 && dirp->fdata_.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            {
              retval = FindNextFile (dirp->current_handle_,
                                     &(dirp->fdata_));
            }
          if (retval == 0)
            {
              dirp->current_handle_ = PACE_WIN32_INVALID_HANDLE;
            }
        }
      dirp->started_reading_ = 1;
    }
  else
    {
      int retval = FindNextFile (dirp->current_handle_,
                                 &(dirp->fdata_));
      if (retval == 0)
        {
          dirp->current_handle_ = PACE_WIN32_INVALID_HANDLE;
        }
    }

  if (dirp->current_handle_ != PACE_WIN32_INVALID_HANDLE)
    {
      dirp->dirent_.d_name = dirp->fdata_.cFileName;
      return &(dirp->dirent_);
    }
  else
    {
      return (pace_dirent*)0;
    }
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
int
pace_win32_emulate_readdir_r (PACE_DIR * dirp,
                              pace_dirent * entry,
                              pace_dirent ** result)
{
  PACE_UNUSED_ARG (dirp);
  PACE_UNUSED_ARG (entry);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
void
pace_win32_emulate_rewinddir (PACE_DIR * dirp)
{
  PACE_UNUSED_ARG (dirp);
  PACE_ERRNO_NO_SUPPORT ();
  return;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
PACE_DIR *
pace_opendir (const char * dirname)
{
  return pace_win32_emulate_opendir (dirname);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
pace_dirent *
pace_readdir (PACE_DIR * dirp)
{
  return pace_win32_emulate_readdir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
int
pace_readdir_r (PACE_DIR * dirp,
                pace_dirent * entry,
                pace_dirent ** result)
{
  return pace_win32_emulate_readdir_r (dirp, entry, result);
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
void
pace_rewinddir (PACE_DIR * dirp)
{
  pace_win32_emulate_rewinddir (dirp);
  return;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_closedir (PACE_DIR * dirp)
{
  return pace_win32_emulate_closedir (dirp);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
