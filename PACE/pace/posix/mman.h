/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/mman.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_MMAN_H
#define PACE_SYS_MMAN_H

#include "pace/defines.h"
#include <sys/mman.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_mlock (const void * addr, size_t len);

  PACE_INLINE int pace_mlockall (int flags);

  PACE_INLINE void * pace_mmap (void * addr,
                                size_t len,
                                int prot,
                                int flags,
                                int fildes,
                                off_t off);

  PACE_INLINE int pace_mprotect (const void * addr,
                                 size_t len,
                                 int prot);

  PACE_INLINE int pace_msync (void * addr,
                              size_t len,
                              int flags);

  PACE_INLINE int pace_munlock (const void * addr, size_t len);

  PACE_INLINE int pace_munlockall ();

  PACE_INLINE int pace_munmap (void *addr, size_t len);

  PACE_INLINE int pace_shm_open (const char * name,
                                 int oflag,
                                 mode_t mode);
  /* Requires PACE_POSIX_C_SOURCE > 2. */

  PACE_INLINE int pace_shm_unlink (const char * name);
  /* Requires PACE_POSIX_C_SOURCE > 2. */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/mman.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_MMAN_H */
