/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/stat.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_STAT_H_INDIRECT
#define PACE_SYS_STAT_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/stat.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/stat.h"
#elif defined (PACE_WIN32)
# include "pace/win32/stat.h"
#endif

#endif /* PACE_SYS_STAT_H_INDIRECT */
