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

#ifndef PACE_PTHREAD_H_INDIRECT
#define PACE_PTHREAD_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/pthread.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/pthread.h"
#elif defined (PACE_WIN32)
# include "pace/win32/pthread.h"
#endif

#endif /* PACE_PTHREAD_H_INDIRECT */
