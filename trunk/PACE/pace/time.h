/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_TIME_H
#define PACE_TIME_H

#if defined (PACE_HAS_POSIX)
# include "posix/time.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/time.h"
#elif defined (PACE_WIN32)
# include "win32/time.h"
#endif

#endif /* PACE_TIME_H */
