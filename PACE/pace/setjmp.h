/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/setjmp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H
#define PACE_SETJMP_H

#if defined (PACE_HAS_POSIX)
# include "posix/setjmp.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/setjmp.h"
#elif defined (PACE_WIN32)
# include "win32/setjmp.h"
#endif

#endif /* PACE_SETJMP_H */
