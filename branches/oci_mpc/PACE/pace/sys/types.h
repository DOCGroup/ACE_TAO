/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TYPES_H
#define PACE_SYS_TYPES_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/types.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/types.h"
#elif (PACE_WIN32)
# include "pace/win32/types.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H */
