/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H
#define PACE_STDLIB_H

#if defined (PACE_HAS_POSIX)
# include "posix/stdlib.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/stdlib.h"
#elif defined (PACE_WIN32)
# include "win32/stdlib.h"
#endif

#endif /* PACE_STDLIB_H */
