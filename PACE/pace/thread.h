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

#ifndef PACE_THREAD_H_INDIRECT
#define PACE_THREAD_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/thread.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/thread.h"
#elif defined (PACE_WIN32)
# include "pace/win32/thread.h"
#endif

#endif /* PACE_THREAD_H_INDIRECT */
