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

#ifndef PACE_SYS_UTSNAME_H_WIN32
#define PACE_SYS_UTSNAME_H_WIN32

#include <sys/utsname.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTSNAME
#define PACE_UTSNAME
typedef struct utsname pace_utsname;
#endif /* PACE_UTSNAME */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_UTSNAME_H_WIN32 */
