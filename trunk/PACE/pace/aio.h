/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/aio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_AIO_H
#define PACE_AIO_H

#if defined (PACE_HAS_POSIX)
# include "posix/aio.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/aio.h"
#elif defined (PACE_WIN32)
# include "win32/aio.h"
#endif

#endif /* PACE_AIO_H */
