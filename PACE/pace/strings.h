/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/strings.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRINGS_H
#define PACE_STRINGS_H

#if defined (PACE_HAS_POSIX)
# include "posix/strings.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/strings.h"
#elif defined (PACE_WIN32)
# include "win32/strings.h"
#endif

#endif /* PACE_STRINGS_H */
