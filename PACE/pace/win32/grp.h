/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace (posix ace)
 *
 * = FILENAME
 *    pace/posix/grp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_GRP_H
#define PACE_GRP_H

#include <grp.h>

# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_GROUP
#define PACE_GROUP
typedef struct group pace_group;
#endif /* PACE_GROUP */

  /**
     PACE's implementation of the POSIX function getgrgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
  PACE_INLINE pace_group * pace_getgrgid (pace_gid_t gid);

  /**
     PACE's implementation of the POSIX function getgrgid_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
  PACE_INLINE int pace_getgrgid_r (pace_gid_t gid,
                                   pace_group * grp,
                                   char * buffer,
                                   size_t bufsize,
                                   pace_group ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getgrnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
  PACE_INLINE pace_group * pace_getgrnam (const char * name);

  /**
     PACE's implementation of the POSIX function getgrnam_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 9.2.1.
   */
  PACE_INLINE int pace_getgrnam_r (const char * name,
                                   pace_group * grp,
                                   char * buffer,
                                   size_t bufsize,
                                   pace_group ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/posix/grp.inl"
# endif /* PACE_HAS_INLINE */

#endif /* PACE_GRP_H */
