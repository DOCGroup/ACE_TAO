/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mman.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <windows.h>
#include <io.h>
#include <stdio.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlock (const void * addr, pace_size_t len)
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlockall (int flags)
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_mmap (void * addr,
           size_t len,
           int prot,
           int flags,
           PACE_HANDLE fildes,
           pace_off_t off)
{
  void * addr_mapping = (void*)0;
  int nt_flags = 0;
  PACE_HANDLE local_handle = PACE_INVALID_HANDLE;
  PACE_HANDLE * file_mapping = CreateFileMapping (fildes,
                                                  0,
                                                  prot,
                                                  0,
                                                  0,
                                                  0);
  if (*file_mapping == 0)
    {
      PACE_FAIL_RETURN (MAP_FAILED);
    }

  if (PACE_BIT_ENABLED (flags, MAP_PRIVATE))
    {
      prot = PAGE_WRITECOPY;
      nt_flags = FILE_MAP_COPY;
    }
  else if (PACE_BIT_ENABLED (flags, MAP_SHARED))
    {
      if (PACE_BIT_ENABLED (prot, PAGE_READONLY))
        nt_flags = FILE_MAP_READ;
      if (PACE_BIT_ENABLED (prot, PAGE_READWRITE))
        nt_flags = FILE_MAP_WRITE;
    }

# if !defined (PACE_HAS_WINCE)
  addr_mapping = MapViewOfFileEx (*file_mapping,
                                  nt_flags,
                                  0,
                                  off,
                                  len,
                                  addr);
# else
  /* WinCE doesn't allow specifying <addr>. */
  PACE_UNUSED_ARG (addr);        
  addr_mapping = MapViewOfFile (*file_mapping,
                                nt_flags,
                                0,
                                off,
                                len);
# endif /* ! ACE_HAS_WINCE */

  /* Only close this down if we used the temporary. */
  if (file_mapping == &local_handle)
    {
      CloseHandle (*file_mapping);
    }

  if (addr_mapping == 0)
    {
      PACE_FAIL_RETURN (MAP_FAILED);
    }

  else if (PACE_BIT_ENABLED (flags, MAP_FIXED)
           && addr_mapping != addr)
    {
      errno = EINVAL;
      return MAP_FAILED;
    }
  else
    {
      return addr_mapping;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlock (const void * addr, size_t len)
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mprotect (void * addr,
               size_t len,
               int prot)
{
  DWORD dummy; /* Sigh! */
  return VirtualProtect(addr, len, prot, &dummy) ? 0 : -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_msync (void * addr,
            size_t len,
            int flags)
{
  PACE_UNUSED_ARG (flags);
  if (FlushViewOfFile (addr, len))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlockall ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munmap (void * addr, size_t len)
{
  PACE_UNUSED_ARG (len);
  if (UnmapViewOfFile (addr))
    {
      return 0;
    }
  else
    {
      errno = GetLastError ();
      return -1;
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
PACE_HANDLE
pace_shm_open (const char * name,
               int oflag,
               pace_mode_t mode)
{
  /* Would be similar to ACE_OS::open
     which (currently uses threads and Object Manager).
   */
  PACE_HANDLE retval = PACE_INVALID_HANDLE;
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (oflag);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_shm_unlink (const char * name)
{
  return _unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
