/* -*- C -*-
 * $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    types.h
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

# if defined (PACE_PSOS_TM)
typedef long long longlong_t;
typedef long      id_t;
# endif /* PACE_PSOS_TM */

# define PACE_UINT32 uint32_t
# define PACE_UINT16 uint16_t

# if defined PACE_HAS_WCHAR
# if defined PACE_USES_WCHAR
# define PACE_TCHAR wchar
# define PACE_TEXT(X) X
# endif /* PACE_USES_WCHAR */
# else /* !PACE_HAS_WCHAR */
# define PACE_TCHAR char
# define PACE_TEXT(X) X
# endif /* PACE_HAS_WCHAR */

# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* (PACE_HAS_CPLUSPLUS) */


# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */


#endif /* PACE_SYS_TYPES_H */
