/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TYPES_H
#define PACE_SYS_TYPES_H

#if defined (PACE_HAS_POSIX)
# include "posix/types.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/types.h"
#elif defined (PACE_WIN32)
# include "win32/types.h"
#endif

#endif /* PACE_SYS_TYPES_H */
