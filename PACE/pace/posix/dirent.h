/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H
#define PACE_DIRENT_H

#include "pace/sys/types.h"
#include <dirent.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /** 
     PACE's implementation of the POSIX function opendir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
  PACE_INLINE DIR * pace_opendir (const char * dirname);

  /** 
     PACE's implementation of the POSIX function readdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
  PACE_INLINE struct dirent * pace_readdir (DIR * dirp);

  /** 
     PACE's implementation of the POSIX function readdir_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
  PACE_INLINE int pace_readdir_r (DIR * dirp, struct dirent * entry,
                                  struct dirent ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function rewinddir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
  PACE_INLINE void pace_rewinddir (DIR * dirp);

  /** 
     PACE's implementation of the POSIX function closedir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
  PACE_INLINE int pace_closedir (DIR * dirp);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/dirent.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_DIRENT_H */
