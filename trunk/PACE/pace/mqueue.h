/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/mqueue.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MQUEUE_H_INDIRECT
#define PACE_MQUEUE_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/mqueue.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/mqueue.h"
#elif defined (PACE_WIN32)
# include "pace/win32/mqueue.h"
#endif

#endif /* PACE_MQUEUE_H_INDIRECT */
