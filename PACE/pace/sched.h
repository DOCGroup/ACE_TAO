/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H
#define PACE_SCHED_H

#if defined (PACE_HAS_POSIX)
# include "posix/sched.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/sched.h"
#elif defined (PACE_WIN32)
# include "win32/sched.h"
#endif

#endif /* PACE_SCHED_H */
