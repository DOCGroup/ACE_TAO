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

#ifndef PACE_SCHED_H_INDIRECT
#define PACE_SCHED_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/sched.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/sched.h"
#elif defined (PACE_WIN32)
# include "pace/win32/sched.h"
#endif

#endif /* PACE_SCHED_H_INDIRECT */
