/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/stdio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDIO_H
#define PACE_STDIO_H

#if defined (PACE_HAS_POSIX)
# include "posix/stdio.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/stdio.h"
#elif defined (PACE_WIN32)
# include "win32/stdio.h"
#endif

#endif /* PACE_STDIO_H */


