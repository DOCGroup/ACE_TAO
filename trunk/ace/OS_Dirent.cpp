// $Id$

#include "ace/OS_Dirent.h"

// @@ (brunsch) We have to include OS.h here for the string operations.
//    Once the string stuff gets split off, then we can just include that
//    instead.
#include "ace/OS.h"

ACE_RCSID(ace, OS_Dirent, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_Dirent.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

DIR *
ACE_OS_Dirent::opendir (const ACE_TCHAR *filename)
{
#if defined (ACE_HAS_DIRENT)
#  if defined (ACE_PSOS)

  // The pointer to the DIR buffer must be passed to ACE_OS_Dirent::closedir
  // in order to free it and avoid a memory leak.
  DIR *dir;
  u_long result;
  ACE_NEW_RETURN (dir, DIR, 0);
#if defined (ACE_PSOS_DIAB_PPC)
  result = ::open_dir (ACE_const_cast (char *, filename), &(dir->xdir));
#else
  result = ::open_dir (ACE_const_cast (char *, filename), dir);
#endif /* defined ACE_PSOS_DIAB_PPC */
  if (result == 0)
    return dir;
  else
    {
      errno = result;
      return 0;
    }

#  else /* ! defined (ACE_PSOS) */
#if defined (ACE_WIN32)
  DIR *dir;
  ACE_NEW_RETURN (dir, DIR, 0);
  ACE_NEW_RETURN (dir->directory_name_,
                  ACE_TCHAR[ACE_OS::strlen (filename)],
                  0);
  ACE_OS::strcpy (dir->directory_name_, filename);
  dir->current_handle_ = INVALID_HANDLE_VALUE;
  dir->started_reading_ = 0;
  return dir;
#else
  // VxWorks' ::opendir () is declared with a non-const argument.
  return ::opendir (ACE_const_cast (char *, filename));
#endif /* defined (ACE_WIN32) */
#  endif /* ! defined (ACE_PSOS) */
#else
  ACE_UNUSED_ARG (filename);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_DIRENT */
}
