/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/fnctl.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_FCNTL_H
#define PACE_FCNTL_H

#if defined (PACE_HAS_POSIX)
# include "posix/fcntl.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/fcntl.h"
#elif defined (PACE_WIN32)
# include "win32/fcntl.h"
#endif

#endif /* PACE_FCNTL_H */
