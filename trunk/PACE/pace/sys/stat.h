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

#ifndef PACE_SYS_STAT_H
#define PACE_SYS_STAT_H

#if defined (PACE_HAS_POSIX)
# include "posix/stat.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/stat.h"
#elif defined (PACE_WIN32)
# include "win32/stat.h"
#endif

#endif /* PACE_SYS_STAT_H */
