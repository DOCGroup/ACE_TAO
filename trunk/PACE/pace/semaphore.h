/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/semaphore.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SEMAPHORE_H
#define PACE_SEMAPHORE_H

#if defined (PACE_HAS_POSIX)
# include "posix/semaphore.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/semaphore.h"
#elif defined (PACE_WIN32)
# include "win32/semaphore.h"
#endif

#endif /* PACE_SEMAPHORE_H */
