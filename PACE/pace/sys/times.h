/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/times.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TIMES_H
#define PACE_SYS_TIMES_H

#if defined (PACE_HAS_POSIX)
# include "posix/times.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/times.h"
#elif defined (PACE_WIN32)
# include "win32/times.h"
#endif

#endif /* PACE_SYS_TIMES_H */
