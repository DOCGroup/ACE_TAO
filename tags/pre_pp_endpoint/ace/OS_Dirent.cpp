// $Id$

#include "ace/OS_Dirent.h"
#include "ace/OS_String.h"

ACE_RCSID(ace, OS_Dirent, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_Dirent.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

DIR *
ACE_OS_Dirent::opendir_emulation (const ACE_TCHAR *filename)
{
#if defined (ACE_WIN32)
  DIR *dir;
  ACE_NEW_RETURN (dir, DIR, 0);
  ACE_NEW_RETURN (dir->directory_name_,
                  ACE_TCHAR[ACE_OS_String::strlen (filename)+1],
                  0);
  ACE_OS_String::strcpy (dir->directory_name_, filename);
  dir->current_handle_ = INVALID_HANDLE_VALUE;
  dir->started_reading_ = 0;
  return dir;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (filename);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

void
ACE_OS_Dirent::closedir_emulation (DIR *d)
{
#if defined (ACE_WIN32)
  if (d->current_handle_ != INVALID_HANDLE_VALUE)
    ::FindClose (d->current_handle_);

  d->current_handle_ = INVALID_HANDLE_VALUE;
  d->started_reading_ = 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (d);  
#endif /* ACE_WIN32 */
}

struct dirent *
ACE_OS_Dirent::readdir_emulation (DIR *d)
{
#if defined (ACE_WIN32)
  if (!d->started_reading_)
    {
      d->current_handle_ = ACE_TEXT_FindFirstFile (d->directory_name_,
                                                   &(d->fdata_));

      if (d->current_handle_ != INVALID_HANDLE_VALUE)
        {
          // Skip "." and ".."
          int retval = 1;
          while (*(d->fdata_.cFileName) == '.'
                 && retval
                 && d->fdata_.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            {
              retval = ACE_TEXT_FindNextFile (d->current_handle_,
                                              &(d->fdata_));
            }
          if (retval == 0)
            d->current_handle_ = INVALID_HANDLE_VALUE;
        }

      d->started_reading_ = 1;
    }
  else
    {
      int retval = ACE_TEXT_FindNextFile (d->current_handle_,
                                          &(d->fdata_));
      if (retval == 0)
        d->current_handle_ = INVALID_HANDLE_VALUE;
    }

  if (d->current_handle_ != INVALID_HANDLE_VALUE)
    {
      d->dirent_.d_name = d->fdata_.cFileName;
      return &(d->dirent_);
    }
  else
    return 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}
