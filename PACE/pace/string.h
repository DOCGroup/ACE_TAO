/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H
#define PACE_STRING_H

#if defined (PACE_HAS_POSIX)
# include "posix/string.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/string.h"
#elif defined (PACE_WIN32)
# include "win32/string.h"
#endif

#endif /* PACE_STRING_H */
