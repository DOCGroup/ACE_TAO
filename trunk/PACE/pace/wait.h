/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/wait.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_WAIT_H
#define PACE_WAIT_H

#if defined (PACE_HAS_POSIX)
# include "posix/wait.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/wait.h"
#elif defined (PACE_WIN32)
# include "win32/wait.h"
#endif

#endif /* PACE_WAIT_H */
