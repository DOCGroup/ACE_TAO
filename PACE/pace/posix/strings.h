/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    strings.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_STRINGS_H
#define PACE_STRINGS_H


#include "pace/defines.h"
#include "pace/unistd.h"


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_strcasecmp (const char * s1,
                              const char * s2);

  PACE_INLINE int pace_strncasecmp (const char * s1,
                               const char * s2,
                               size_t n);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "strings.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_STRINGS_H */
