/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/thread.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_THREAD_H
#define PACE_THREAD_H

#if defined (PACE_HAS_POSIX)
# include "posix/thread.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/thread.h"
#elif defined (PACE_WIN32)
# include "win32/thread.h"
#endif

#endif /* PACE_THREAD_H */
