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

#ifndef PACE_STDIO_H_INDIRECT
#define PACE_STDIO_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/stdio.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/stdio.h"
#elif defined (PACE_WIN32)
# include "pace/win32/stdio.h"
#endif

#endif /* PACE_STDIO_H_INDIRECT */


