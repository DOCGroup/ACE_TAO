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
  ACE_TCHAR extra[3] = {0,0,0};

/*  
  Note: the semantics of the win32 function FindFirstFile take the 
  basename(filename) as a pattern to be matched within the dirname(filename). 
  This is contrary to the behavior of the posix function readdir which treats 
  basename(filename) as a directory to be opened and read. 

  For this reason, we append a slash-star or backslash-star to the supplied
  filename so the result is that FindFirstFile will do what we need.

  According to the documentation for FindFirstFile, either a '/' or a '\' may
  be used as a directory name separator.

  Of course, it is necessary to ensure that this is only done if the trailing
  filespec is not already there.

  Phil Mesnier
*/

  size_t lastchar = ACE_OS_String::strlen (filename);
  if (lastchar > 0) 
    {
      if (filename[lastchar-1] != '*') 
        {
          if (filename[lastchar-1] != '/' && filename[lastchar-1] != '\\') 
            ACE_OS_String::strcpy (extra, ACE_LIB_TEXT ("/*"));
          else
            ACE_OS_String::strcpy (extra, ACE_LIB_TEXT ("*"));
        }
    }
    
  ACE_NEW_RETURN (dir, DIR, 0);  
  ACE_NEW_RETURN (dir->directory_name_,
                  ACE_TCHAR[lastchar + ACE_OS_String::strlen (extra) + 1],
                  0);
  ACE_OS_String::strcpy (dir->directory_name_, filename);
  if (extra[0]) 
    ACE_OS_String::strcat (dir->directory_name_, extra);
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
          while (*d->fdata_.cFileName == '.'
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
        {
          // Make sure to close the handle explicitly to avoid a leak!
          ::FindClose (d->current_handle_);
          d->current_handle_ = INVALID_HANDLE_VALUE;
        }
    }

  if (d->current_handle_ != INVALID_HANDLE_VALUE)
    {
      d->dirent_.d_name = d->fdata_.cFileName;
      return &d->dirent_;
    }
  else
    return 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}
