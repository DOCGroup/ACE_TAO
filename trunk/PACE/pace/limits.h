/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/limits.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_LIMITS_H
#define PACE_LIMITS_H

#if defined (PACE_HAS_POSIX)
# include "posix/limits.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/limits.h"
#elif defined (PACE_WIN32)
# include "win32/limits.h"
#endif

#endif /* PACE_LIMITS_H */
