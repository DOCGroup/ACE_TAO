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

#ifndef PACE_SYS_TIMES_H_INDIRECT
#define PACE_SYS_TIMES_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/times.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/times.h"
#elif defined (PACE_WIN32)
# include "pace/win32/times.h"
#endif

#endif /* PACE_SYS_TIMES_H_INDIRECT */
