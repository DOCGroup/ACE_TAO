/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utime.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UTIME_H_WIN32
#define PACE_UTIME_H_WIN32

#include "pace/sys/types.h"
#include <sys/utime.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTIMBUF
#define PACE_UTIMBUF
  typedef struct _utimbuf pace_utimbuf;
#endif /* PACE_UTIMBUF */

#if (PACE_HAS_POSIX_FS_UOF)
  int win32_utime (const char * path, const pace_utimbuf * times);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UTIME_H_WIN32 */
