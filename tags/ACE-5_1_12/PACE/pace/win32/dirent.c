/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/dirent.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/dirent.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/dirent.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_FS_UOF)
int
closedir (PACE_DIR * dirp)
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
opendir (const char * dirname)
{
  PACE_DIR * dirp = (PACE_DIR*) pace_malloc (sizeof (PACE_DIR));
  const pace_size_t dir_size = pace_strlen (dirname) + 1;
  dirp->directory_name_ = (char*) pace_malloc (dir_size);
  pace_strcpy (dirp->directory_name_, dirname);
  dirp->current_handle_ = PACE_INVALID_HANDLE;
  dirp->started_reading_ = 0;
  return dirp;
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
pace_dirent *
readdir (PACE_DIR * dirp)
{
  if (dirp->started_reading_)
    {
      dirp->current_handle_ = FindFirstFile (dirp->directory_name_,
                                             &(dirp->fdata_));
      if (dirp->current_handle_ != PACE_INVALID_HANDLE)
        {
          FindClose (dirp->current_handle_);
          dirp->current_handle_ = PACE_INVALID_HANDLE;
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
              dirp->current_handle_ = PACE_INVALID_HANDLE;
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
          dirp->current_handle_ = PACE_INVALID_HANDLE;
        }
    }

  if (dirp->current_handle_ != PACE_INVALID_HANDLE)
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
