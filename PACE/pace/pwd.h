/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/pwd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PWD_H
#define PACE_PWD_H

#if defined (PACE_HAS_POSIX)
# include "posix/pwd.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/pwd.h"
#elif defined (PACE_WIN32)
# include "win32/pwd.h"
#endif

#endif /* PACE_PWD_H */
