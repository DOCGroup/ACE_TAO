/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_TIME_H_INDIRECT
#define PACE_TIME_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/time.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/time.h"
#elif defined (PACE_WIN32)
# include "pace/win32/time.h"
#endif

#endif /* PACE_TIME_H_INDIRECT */
