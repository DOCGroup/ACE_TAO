/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/grp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_GRP_H
#define PACE_GRP_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/grp.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/grp.h"
#elif (PACE_WIN32)
# include "pace/win32/grp.h"
#endif

# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function getgrgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
#if (PACE_HAS_POSIX_SD_UOF)
  PACE_Export PACE_INLINE pace_group * pace_getgrgid (pace_gid_t gid);
#endif /* PACE_HAS_POSIX_SD_UOF */

  /**
     PACE's implementation of the POSIX function getgrgid_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
#if (PACE_HAS_POSIX_SDR_UOF)
  PACE_Export PACE_INLINE int pace_getgrgid_r (pace_gid_t gid,
                                               pace_group * grp,
                                               char * buffer,
                                               size_t bufsize,
                                               pace_group ** result);
#endif /* PACE_HAS_POSIX_SDR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getgrnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
#if (PACE_HAS_POSIX_SD_UOF)
  PACE_Export PACE_INLINE pace_group * pace_getgrnam (const char * name);
#endif /* PACE_HAS_POSIX_SD_UOF */

  /**
     PACE's implementation of the POSIX function getgrnam_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
#if (PACE_HAS_POSIX_SDR_UOF)
  PACE_Export PACE_INLINE int pace_getgrnam_r (const char * name,
                                               pace_group * grp,
                                               char * buffer,
                                               size_t bufsize,
                                               pace_group ** result);
#endif /* PACE_HAS_POSIX_SDR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/grp.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/grp.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/grp.inl"
#  endif
#endif /* PACE_HAS_INLINE */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_GRP_H */
