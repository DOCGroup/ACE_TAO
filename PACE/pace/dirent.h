/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H
#define PACE_DIRENT_H

#if defined (PACE_HAS_POSIX)
# include "posix/dirent.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/dirent.h"
#elif defined (PACE_WIN32)
# include "win32/dirent.h"
#endif

#endif /* PACE_DIRENT_H */
