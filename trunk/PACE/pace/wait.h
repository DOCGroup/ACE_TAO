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

#ifndef PACE_WAIT_H_INDIRECT
#define PACE_WAIT_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/wait.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/wait.h"
#elif defined (PACE_WIN32)
# include "pace/win32/wait.h"
#endif

#endif /* PACE_WAIT_H_INDIRECT */
