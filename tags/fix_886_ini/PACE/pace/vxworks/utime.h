/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/utime.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_UTIME_H_VXWORKS
#define PACE_UTIME_H_VXWORKS

#include "pace/sys/types.h"
#include <utime.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTIMBUF
#define PACE_UTIMBUF
typedef struct utimbuf pace_utimbuf;
#endif /* PACE_UTIMBUF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UTIME_H_VXWORKS */
