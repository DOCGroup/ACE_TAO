/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H_VXWORKS
#define PACE_STRING_H_VXWORKS

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if !defined (PACE_NULL)
# define PACE_NULL NULL
#endif /* NULL */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STRING_H_VXWORKS */
