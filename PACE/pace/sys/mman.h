/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/mman.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_MMAN_H
#define PACE_SYS_MMAN_H

#if defined (PACE_HAS_POSIX)
# include "posix/mman.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/mman.h"
#elif defined (PACE_WIN32)
# include "win32/mman.h"
#endif

#endif /* PACE_SYS_MMAN_H */
