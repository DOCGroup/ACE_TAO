/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/pthread.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PTHREAD_H
#define PACE_PTHREAD_H

#if defined (PACE_HAS_POSIX)
# include "posix/pthread.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/pthread.h"
#elif defined (PACE_WIN32)
# include "win32/pthread.h"
#endif

#endif /* PACE_PTHREAD_H */
