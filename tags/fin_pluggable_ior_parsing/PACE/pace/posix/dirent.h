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

  PACE_INLINE DIR * pace_opendir (const char * dirname);

  PACE_INLINE struct dirent * pace_readdir (DIR * dirp);

  PACE_INLINE int pace_readdir_r (DIR * dirp, struct dirent * entry,
                                  struct dirent ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE void pace_rewinddir (DIR * dirp);

  PACE_INLINE int pace_closedir (DIR * dirp);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/dirent.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_DIRENT_H */
