/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace ()
 *
 * = FILENAME
 *    pace/win32/grp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_GRP_H_WIN32
#define PACE_GRP_H_WIN32

# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_GROUP
#define PACE_GROUP
  typedef struct group {
    int val_;
  } pace_group;
#endif /* PACE_GROUP */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_GRP_H_WIN32 */
