/* $Id$

* ============================================================================
*
* = LIBRARY
*    pace
*
* = FILENAME
*    pace/stddef.h
*
* = AUTHOR
*    John Heitmann
*
* =========================================================================== */

#ifndef PACE_STDDEF_H
#define PACE_STDDEF_H

#include <stddef.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_NULL
#define PACE_NULL NULL;
#endif /* PACE_NULL */

#ifndef PACE_OFFSETOF
# define PACE_OFFSETOF
# define pace_offsetof offsetof;
#endif /* PACE_OFFSETOF */

#ifndef PACE_PTRDIFF_T
#define PACE_PTRDIFF_T
typedef ptrdiff_t pace_ptrdiff_t;
#endif /* PACE_PTRDIFF_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_WCHAR_T
#define PACE_WCHAR_T
typedef wchar_t pace_wchar_t;
#endif /* PACE_WCHAR_T */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDDEF_H */
