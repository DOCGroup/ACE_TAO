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

#ifndef PACE_GRP_H_POSIX
#define PACE_GRP_H_POSIX

#include <grp.h>

# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_GROUP
#define PACE_GROUP
typedef struct group pace_group;
#endif /* PACE_GROUP */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_GRP_H_POSIX */
