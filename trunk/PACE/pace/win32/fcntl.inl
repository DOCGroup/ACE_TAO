/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/fcntl.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <windows.h>
#include <wtypes.h>

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_win32_emulation_creat (const char * path, pace_mode_t mode)
{
  DWORD access = GENERIC_READ,
	creation = OPEN_EXISTING,
	flags = 0,
	shared_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

  HANDLE h = (void*)0;

  if (PACE_BIT_ENABLED (mode, O_WRONLY))
    {
      access = GENERIC_WRITE;
    }
  else if (PACE_BIT_ENABLED (mode, O_RDWR))
    {
      access = GENERIC_READ | GENERIC_WRITE;
    }

  if ((mode & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL))
    {
      creation = CREATE_NEW;
    }
  else if ((mode & (_O_CREAT | _O_TRUNC)) == (_O_CREAT | _O_TRUNC))
    {
      creation = CREATE_ALWAYS;
    }
  else if (PACE_BIT_ENABLED (mode, _O_CREAT))
    {
      creation = OPEN_ALWAYS;
    }
  else if (PACE_BIT_ENABLED (mode, _O_TRUNC))
    {
      creation = TRUNCATE_EXISTING;
    }

  if (PACE_BIT_ENABLED (mode, _O_TEMPORARY))
    {
      flags |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;
    }

  if (PACE_BIT_ENABLED (mode, FILE_FLAG_WRITE_THROUGH))
    {
      flags |= FILE_FLAG_WRITE_THROUGH;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_OVERLAPPED))
    {
      flags |= FILE_FLAG_OVERLAPPED;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_NO_BUFFERING))
    {
      flags |= FILE_FLAG_NO_BUFFERING;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_RANDOM_ACCESS))
    {
      flags |= FILE_FLAG_RANDOM_ACCESS;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_SEQUENTIAL_SCAN))
    {
      flags |= FILE_FLAG_SEQUENTIAL_SCAN;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_DELETE_ON_CLOSE))
    {
      flags |= FILE_FLAG_DELETE_ON_CLOSE;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_BACKUP_SEMANTICS))
    {
      flags |= FILE_FLAG_BACKUP_SEMANTICS;
    }
  if (PACE_BIT_ENABLED (mode, FILE_FLAG_POSIX_SEMANTICS))
    {
      flags |= FILE_FLAG_POSIX_SEMANTICS;
    }

#if 0 //////////////////////////////////////////////////////////////////////

  /* Threads and version info that isn't implemented yet in PACE.
     ACE_MT (ACE_thread_mutex_t *ace_os_monitor_lock = 0;)
   */
  if (PACE_BIT_ENABLED (mode, _O_APPEND))
    {
      ACE_MT
        (
          ace_os_monitor_lock = (ACE_thread_mutex_t *)
            ACE_OS_Object_Manager::preallocated_object[
              ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK];
          ACE_OS::thread_mutex_lock (ace_os_monitor_lock);
        )
    }

#if !defined (ACE_HAS_WINCE)  /* CE doesn't have FILE_SHARE_DELETE */
  if (ACE_OS::get_win32_versioninfo().dwPlatformId ==
      VER_PLATFORM_WIN32_NT)
    shared_mode |= FILE_SHARE_DELETE;
#endif /* ACE_HAS_WINCE */

////////////////////////////////////////////////////////////
#endif /* 0 */

  h = CreateFile (path,
                  access,
                  shared_mode,
                  0,
                  creation,
                  flags,
                  0);

  if (PACE_BIT_ENABLED (mode, _O_APPEND))
    {
      if (h != PACE_WIN32_INVALID_HANDLE)
        {
          SetFilePointer (h, 0, 0, FILE_END);
        }
      /* Threads and version info that isn't implemented yet in PACE.
         ACE_MT (ACE_thread_mutex_t *ace_os_monitor_lock = 0;)
         ACE_MT (ACE_OS::thread_mutex_unlock (ace_os_monitor_lock);)
       */
    }
  
  if (h == PACE_WIN32_INVALID_HANDLE)
    {
      /* Where is this in ACE?
         ACE_FAIL_RETURN (h);
       */
      return -1;
    }
  else
    {
      /* POSIX needs an integer 
         return h;
         Should return lowest unused file descriptor but windows
         doesn't do that. So we return a (int) casted HANDLE.
       */
      return (int)*h;
    }
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_creat (const char * path, pace_mode_t mode)
{
  return pace_win32_emulation_creat (path, mode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */
