/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pwd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PWD_H
#define PACE_PWD_H

#include <pwd.h>
#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_PASSWD
#define PACE_PASSWD
  typedef struct passwd pace_passwd;
#endif /* PACE_PASSWD */

  /**
     PACE's implementation of the POSIX function getpwuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
  PACE_INLINE pace_passwd * pace_getpwuid (pace_uid_t uid);

  /**
     PACE's implementation of the POSIX function getpwuid_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
  PACE_INLINE int pace_getpwuid_r (pace_uid_t uid,
                                   pace_passwd * pwd,
                                   char * buffer,
                                   pace_size_t bufsize,
                                   pace_passwd ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getpwnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
  PACE_INLINE pace_passwd * pace_getpwnam (const char * name);

  /**
     PACE's implementation of the POSIX function getpwnam_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.2.
   */
  PACE_INLINE int pace_getpwnam_r (const char * name,
                                   pace_passwd * pwd,
                                   char * buffer,
                                   pace_size_t bufsize,
                                   pace_passwd ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/pwd.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_PWD_H */
