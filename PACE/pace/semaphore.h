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

#ifndef PACE_SEMAPHORE_H_INDIRECT
#define PACE_SEMAPHORE_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/semaphore.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/semaphore.h"
#elif defined (PACE_WIN32)
# include "pace/win32/semaphore.h"
#endif

#endif /* PACE_SEMAPHORE_H_INDIRECT */
