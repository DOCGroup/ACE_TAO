/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stat.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_STAT_H
#define PACE_SYS_STAT_H

#include "pace/sys/types.h"
#include <sys/stat.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_chmod (const char * path, mode_t mode);

  PACE_INLINE int pace_fchmod (int fildes, mode_t mode);

  PACE_INLINE int pace_fstat (int fildes, struct stat * buf);

  PACE_INLINE int pace_mkdir (const char * path, mode_t mode);

  PACE_INLINE int pace_mkfifo (const char * path, mode_t mode);

  PACE_INLINE int pace_stat (const char * path, struct stat * buf);

  PACE_INLINE mode_t pace_umask (mode_t cmask);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/stat.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_STAT_H */
