/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/signal.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_SIGNAL_H
#define PACE_SIGNAL_H

#if defined (PACE_HAS_POSIX)
# include "posix/signal.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/signal.h"
#elif defined (PACE_WIN32)
# include "win32/signal.h"
#endif

#endif /* PACE_SIGNAL_H */
