/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/defines.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DEFINES_H
#define PACE_DEFINES_H

#if defined (PACE_HAS_POSIX)
# include "posix/defines.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/defines.h"
#elif defined (PACE_WIN32)
# include "win32/defines.h"
#endif

#endif /* PACE_DEFINES_H */
