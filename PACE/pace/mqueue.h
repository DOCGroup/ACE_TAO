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

#ifndef PACE_MQUEUE_H
#define PACE_MQUEUE_H

#if defined (PACE_HAS_POSIX)
# include "posix/mqueue.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/mqueue.h"
#elif defined (PACE_WIN32)
# include "win32/mqueue.h"
#endif

#endif /* PACE_MQUEUE_H */
