/* -*- C -*-
 * $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_SYS_TYPES_H
#define PACE_SYS_TYPES_H

#include <sys/types.h>

/* specific to SunOS 5.7 ace.cs <luther@cs.wustl.edu> */
#include <inttypes.h>

/* Wrap a handle. */
# define PACE_HANDLE int
# define PACE_INVALID_HANDLE -1

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* (PACE_HAS_CPLUSPLUS) */


#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H */
