/* $Id$

* ============================================================================
*
* = LIBRARY
*    pace
*
* = FILENAME
*    pace/posix/errno.h
*
* = AUTHOR
*    John Heitmann
*
* =========================================================================== *\
/

#ifndef PACE_STDDEF_H
#define PACE_STDDEF_H

#include <stddef.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_NULL NULL;
#define pace_ofsetof offsetof;

typedef ptrdiff_t pace_ptrdiff_t;
typedef size_t pace_size_t;
typedef wchar_t pace_wchar_t;

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDDEF_H */
