/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/pwd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PWD_H
#define PACE_PWD_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/pwd.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/pwd.h"
#elif (PACE_WIN32)
# include "pace/win32/pwd.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function getpwuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
#if (PACE_HAS_POSIX_SD_UOF)
  PACE_Export PACE_INLINE pace_passwd * pace_getpwuid (pace_uid_t uid);
#endif /* PACE_HAS_POSIX_SD_UOF */

  /**
     PACE's implementation of the POSIX function getpwuid_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
#if (PACE_HAS_POSIX_SDR_UOF)
  PACE_Export PACE_INLINE int pace_getpwuid_r (pace_uid_t uid,
                                               pace_passwd * pwd,
                                               char * buffer,
                                               pace_size_t bufsize,
                                               pace_passwd ** result);
#endif /* PACE_HAS_POSIX_SDR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getpwnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
#if (PACE_HAS_POSIX_SD_UOF)
  PACE_Export PACE_INLINE pace_passwd * pace_getpwnam (const char * name);
#endif /* PACE_HAS_POSIX_SD_UOF */

  /**
     PACE's implementation of the POSIX function getpwnam_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
#if (PACE_HAS_POSIX_SDR_UOF)
  PACE_Export PACE_INLINE int pace_getpwnam_r (const char * name,
                                               pace_passwd * pwd,
                                               char * buffer,
                                               pace_size_t bufsize,
                                               pace_passwd ** result);
#endif /* PACE_HAS_POSIX_SDR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/pwd.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/pwd.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/pwd.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_PWD_H */
