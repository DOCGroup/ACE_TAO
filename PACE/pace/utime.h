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

#ifndef PACE_UTIME_H_INDIRECT
#define PACE_UTIME_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/utime.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/utime.h"
#elif defined (PACE_WIN32)
# include "pace/win32/utime.h"
#endif

#endif /* PACE_UTIME_H_INDIRECT */
