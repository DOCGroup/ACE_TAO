/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H
#define PACE_DIRENT_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/dirent.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/dirent.h"
#elif (PACE_WIN32)
# include "pace/win32/dirent.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function opendir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE PACE_DIR * pace_opendir (const char * dirname);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function readdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE pace_dirent * pace_readdir (PACE_DIR * dirp);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function readdir_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE int pace_readdir_r (PACE_DIR * dirp,
                                              pace_dirent * entry,
                                              pace_dirent ** result);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function rewinddir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE void pace_rewinddir (PACE_DIR * dirp);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function closedir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.1.2.
   */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_closedir (PACE_DIR * dirp);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/dirent.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/dirent.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/dirent.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_DIRENT_H */
