/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/mman.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include "pace/fcntl.h"

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlock (const void * addr, pace_size_t len)
{
  PACE_TRACE("pace_mlock");

  return mlock (((char*)addr), len);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlockall (int flags)
{
  PACE_TRACE("pace_mlockall");

  return mlockall (flags);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_mmap (void * addr,
           pace_size_t len,
           int prot,
           int flags,
           PACE_HANDLE fildes,
           pace_off_t off)
{
  PACE_TRACE("pace_mmap");

  PACE_UNUSED_ARG(addr);
  PACE_UNUSED_ARG(len);
  PACE_UNUSED_ARG(prot);
  PACE_UNUSED_ARG(flags);
  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(off);
  PACE_ERRNO_NO_SUPPORT_RETURN ((void *) -1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlock (const void * addr, pace_size_t len)
{
  PACE_TRACE("pace_munlock");

  return munlock (((char*)addr), len);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mprotect (void * addr,
               pace_size_t len,
               int prot)
{
  PACE_TRACE("pace_mprotect");

  PACE_UNUSED_ARG(addr);
  PACE_UNUSED_ARG(len);
  PACE_UNUSED_ARG(prot);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_msync (void * addr,
            pace_size_t len,
            int flags)
{
  PACE_TRACE("pace_msync");

  PACE_UNUSED_ARG(addr);
  PACE_UNUSED_ARG(len);
  PACE_UNUSED_ARG(flags);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlockall ()
{
  PACE_TRACE("pace_munlockall");

  return munlockall ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munmap (void * addr, pace_size_t len)
{
  PACE_TRACE("pace_munmap");

  PACE_UNUSED_ARG(addr);
  PACE_UNUSED_ARG(len);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
PACE_HANDLE
pace_shm_open (const char * name,
               int oflag,
               pace_mode_t mode)
{
  PACE_TRACE("pace_shm_open");

  /* Just use open per ACE. */
  return pace_open (name, oflag, mode);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_shm_unlink (const char * name)
{
  PACE_TRACE("pace_shm_unlink");

  /* Just use unlink per ACE. */
  return pace_unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
