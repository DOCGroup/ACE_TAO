/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/stat.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_STAT_H
#define PACE_SYS_STAT_H

#include "pace/config/defines.h"
#include "pace/sys/types.h"


#if (PACE_HAS_POSIX)
# include "pace/posix/stat.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/stat.h"
#elif (PACE_WIN32)
# include "pace/win32/stat.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if (PACE_HAS_POSIX_FA_UOF)
  PACE_Export PACE_INLINE int pace_chmod (const char * path, pace_mode_t mode);
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_fchmod (PACE_HANDLE fildes,
                                           pace_mode_t mode);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_fstat (PACE_HANDLE fildes,
pace_stat_s * buf);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_mkdir (const char * path, pace_mode_t mode);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_F_UOF)
  PACE_Export PACE_INLINE int pace_mkfifo (const char * path,
                                           pace_mode_t mode);
#endif /* PACE_HAS_POSIX_F_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_stat (const char * path, pace_stat_s * buf);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
  PACE_Export PACE_INLINE pace_mode_t pace_umask (pace_mode_t cmask);
#endif /* PACE_HAS_POSIX_FA_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/stat.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/stat.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/stat.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_STAT_H */
