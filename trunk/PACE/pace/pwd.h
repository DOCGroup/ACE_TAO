/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pwd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_PWD_H
#define PACE_PWD_H


#include "pace/defines.h"
#include <pwd.h>

/* Linux wants to use sys/types.h to define uid_t.
 * Sun is happy with pwd.h.
 */
# if defined (linux)
# include "pace/sys/types.h"
# endif /* linux */


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE struct passwd * pace_getpwuid (uid_t uid);

  PACE_INLINE int pace_getpwuid_r (uid_t uid,
                                   struct passwd * pwd,
                                   char * buffer,
                                   size_t bufsize,
                                   struct passwd ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE struct passwd * pace_getpwnam (const char * name);

  PACE_INLINE int pace_getpwnam_r (const char * name,
                                   struct passwd * pwd,
                                   char * buffer,
                                   size_t bufsize,
                                   struct passwd ** result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/pwd.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_PWD_H */
