/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/utime.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UTIME_H
#define PACE_UTIME_H

#if defined (PACE_HAS_POSIX)
# include "posix/utime.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/utime.h"
#elif defined (PACE_WIN32)
# include "win32/utime.h"
#endif

#endif /* PACE_UTIME_H */
