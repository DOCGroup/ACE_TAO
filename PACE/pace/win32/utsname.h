/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utsname.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_UTSNAME_H
#define PACE_SYS_UTSNAME_H

#include <sys/utsname.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTSNAME
#define PACE_UTSNAME
typedef struct utsname pace_utsname;
#endif /* PACE_UTSNAME */

  PACE_INLINE int pace_uname (struct utsname * name);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/utsname.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_UTSNAME_H */
