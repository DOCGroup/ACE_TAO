// $Id$

#include "OS.h"
#include "OS_Dirent.h"
#include "OS_String.h"
#include "OS_Memory.h"

#if 0
#include "ace/Logging/Log_Msg.h"
#endif

ACE_RCSID(ace, OS_Dirent, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "OS_Dirent.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

ACE_DIR *
ACE_OS_Dirent::opendir_emulation (const ACE_TCHAR *filename)
{
#if defined (ACE_WIN32)
  ACE_DIR *dir;
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

  ACE_NEW_RETURN (dir, ACE_DIR, 0);
  ACE_NEW_RETURN (dir->directory_name_,
                  ACE_TCHAR[lastchar + ACE_OS_String::strlen (extra) + 1],
                  0);
  ACE_OS_String::strcpy (dir->directory_name_, filename);
  if (extra[0])
    ACE_OS_String::strcat (dir->directory_name_, extra);
  dir->current_handle_ = INVALID_HANDLE_VALUE;
  dir->started_reading_ = 0;
  dir->dirent_ = 0;
  return dir;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (filename);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

void
ACE_OS_Dirent::closedir_emulation (ACE_DIR *d)
{
#if defined (ACE_WIN32)
  if (d->current_handle_ != INVALID_HANDLE_VALUE)
    ::FindClose (d->current_handle_);

  d->current_handle_ = INVALID_HANDLE_VALUE;
  d->started_reading_ = 0;
  ACE_OS_Memory::free (d->dirent_);
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (d);
#endif /* ACE_WIN32 */
}

dirent *
ACE_OS_Dirent::readdir_emulation (ACE_DIR *d)
{
#if defined (ACE_WIN32)
  ACE_OS_Memory::free (d->dirent_);
  d->dirent_ = 0;

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
            retval = ACE_TEXT_FindNextFile (d->current_handle_, &d->fdata_);
          if (retval == 0)
            {
              ::FindClose (d->current_handle_);
              d->current_handle_ = INVALID_HANDLE_VALUE;
            }
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
      d->dirent_ = (dirent *)
        ACE_OS_Memory::malloc (sizeof (dirent)
                               + ((ACE_OS_String::strlen (d->fdata_.cFileName) + 1
                                   * sizeof (ACE_TCHAR))));
      ACE_OS_String::strcpy (d->dirent_->d_name, d->fdata_.cFileName);
      d->dirent_->d_reclen = sizeof (dirent)
        + (ACE_OS_String::strlen (d->dirent_->d_name) * sizeof (ACE_TCHAR));
      return d->dirent_;
    }
  else
    return 0;
#else /* ACE_WIN32 */
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

extern "C"
{
  typedef int (*ACE_SCANDIR_COMPARATOR) (const void *, const void *);
}

int
ACE_OS_Dirent::scandir_emulation (const ACE_TCHAR *dirname,
                                  dirent **namelist[],
                                  int (*selector) (const dirent *entry),
                                  int (*comparator) (const dirent **f1,
                                                     const dirent **f2))
{
  ACE_DIR *dirp = ACE_OS_Dirent::opendir (dirname);

  if (dirp == 0)
    return -1;
  // A sanity check here.  "namelist" had better not be zero.
  else if (namelist == 0)
    return -1;

  dirent **vector = 0;
  dirent *dp;
  int arena_size = 0;

  int nfiles = 0;
  int fail = 0;

  // @@ This code shoulduse readdir_r() rather than readdir().
  for (dp = ACE_OS_Dirent::readdir (dirp);
       dp != 0;
       dp = ACE_OS_Dirent::readdir (dirp))
    {
      if (selector && (*selector)(dp) == 0)
        continue;

      // If we get here, we have a dirent that the user likes.
      if (nfiles == arena_size)
        {
          dirent **newv;
          if (arena_size == 0)
            arena_size = 10;
          else
            arena_size *= 2;

          newv = (dirent **) ACE_OS_Memory::realloc (vector,
                                                     arena_size * sizeof (dirent *));
          if (newv == 0)
            {
              fail = 1;
              break;
            }
          vector = newv;
        }

      int dsize =
        sizeof (dirent) +
        ((ACE_OS_String::strlen (dp->d_name) + 1) * sizeof (ACE_TCHAR));
      dirent *newdp = (dirent *) ACE_OS_Memory::malloc (dsize);

      if (newdp == 0)
        {
          fail = 1;
          break;
        }

      vector[nfiles++] = (dirent *) ACE_OS_String::memcpy (newdp, dp, dsize);
    }

  if (fail)
    {
      ACE_OS_Dirent::closedir (dirp);
      while (nfiles-- > 0)
        ACE_OS_Memory::free (vector[nfiles]);
      ACE_OS_Memory::free (vector);
      return -1;
    }

  ACE_OS_Dirent::closedir (dirp);

  *namelist = vector;

  if (comparator)
    ACE_OS::qsort (*namelist,
                   nfiles,
                   sizeof (dirent *),
                   (ACE_SCANDIR_COMPARATOR) comparator);

  return nfiles;
}
