/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/errno.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_ERRNO_H
#define PACE_ERRNO_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/errno.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/errno.h"
#elif (PACE_WIN32)
# include "pace/win32/errno.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_ERRNO_H */
