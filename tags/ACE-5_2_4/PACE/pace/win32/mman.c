/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mman.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/sys/mman.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/mman.inl"
#endif /* ! PACE_HAS_INLINE */

#include <windows.h>
#include <stdio.h>

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
void *
mmap (void * addr, size_t len, int prot, int flags,
      PACE_HANDLE fildes, pace_off_t off)
{
  void *addr_mapping = 0;
  int nt_flags = 0;
  PACE_HANDLE file_mapping = PACE_INVALID_HANDLE;

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

  file_mapping = CreateFileMapping (fildes,
                                    0,
                                    prot,
                                    0,
                                    0,
                                    0);
  if (file_mapping == 0)
    PACE_FAIL_RETURN (MAP_FAILED);

# if defined (PACE_OS_EXTRA_MMAP_FLAGS)
  nt_flags |= PACE_OS_EXTRA_MMAP_FLAGS;
# endif /* PACE_OS_EXTRA_MMAP_FLAGS */

# if !defined (PACE_HAS_WINCE)
  addr_mapping = MapViewOfFileEx (file_mapping,
                                  nt_flags,
                                  0,
                                  off,
                                  len,
                                  addr);
# else
  ACE_UNUSED_ARG (addr);        /* WinCE does not allow specifying <addr>.*/
  addr_mapping = MapViewOfFile (file_mapping,
                                nt_flags,
                                0,
                                off,
                                len);
# endif /* ! PACE_HAS_WINCE */

  /* Only close this down if we used the temporary. */
  if (file_mapping == PACE_INVALID_HANDLE)
    CloseHandle (file_mapping);

  if (addr_mapping == 0)
    PACE_FAIL_RETURN (MAP_FAILED);

  else if (PACE_BIT_ENABLED (flags, MAP_FIXED)
           && addr_mapping != addr)
    {
      errno = EINVAL;
      return MAP_FAILED;
    }
  else
    return addr_mapping;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
mprotect (void * addr, size_t len, int prot)
{
  DWORD dummy; /* Sigh! */
  return VirtualProtect(addr, len, prot, &dummy) ? 0 : -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
msync (void * addr, size_t len, int flags)
{
  PACE_UNUSED_ARG (flags);
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (FlushViewOfFile (addr, len), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
munmap (void * addr, size_t len)
{
  PACE_UNUSED_ARG (len);
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (UnmapViewOfFile (addr), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
