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

#ifndef PACE_SIGNAL_H_INDIRECT
#define PACE_SIGNAL_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/signal.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/signal.h"
#elif defined (PACE_WIN32)
# include "pace/win32/signal.h"
#endif

#endif /* PACE_SIGNAL_H_INDIRECT */
