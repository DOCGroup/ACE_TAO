/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/unistd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UNISTD_H
#define PACE_UNISTD_H

#if defined (PACE_HAS_POSIX)
# include "posix/unistd.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/unistd.h"
#elif defined (PACE_WIN32)
# include "win32/unistd.h"
#endif

#endif /* PACE_UNISTD_H */
