/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/fnctl.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_FCNTL_H
#define PACE_FCNTL_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/fcntl.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/fcntl.h"
#elif (PACE_WIN32)
# include "pace/win32/fcntl.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function creat.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.2.
   */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_creat (const char * path, pace_mode_t mode);
  /* WIN32 doesn't correctly return a file desciptor. WIN32 doesn't use
     integers to represent file handles. Instead, we return 1 for success
     and -1 for failure.
   */
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function fcntl.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.5.2.
   */
#if (PACE_HAS_POSIX_FM_UOF)
  /* int fcntl (int fildes, int cmd, *//* arg *//* ... ); */
# if defined (PACE_WIN32)
  int fcntl (PACE_HANDLE fildes, int cmd, /* arg */ ... );
# endif /* PACE_WIN32 */

# if defined (PACE_VXWORKS) && PACE_VXWORKS
  PACE_Export PACE_INLINE int pace_fcntl(PACE_HANDLE fildes,
                                         int cmd,
                                         long arg);
# else
#  define pace_fcntl fcntl
# endif /* ! PACE_VXWORKS */
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function open.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.1.
   */
#if (PACE_HAS_POSIX_DI_UOF)
  /* int open (const char *path, int oflag, *//* pace_mode_t mode *//* ... );*/
# if defined (PACE_HAS_CPLUPLUS)
#  define pace_open ::open
# else
#  define pace_open open
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_POSIX_DI_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/fcntl.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/fcntl.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/fcntl.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_FCNTL_H_INDIRECT */
