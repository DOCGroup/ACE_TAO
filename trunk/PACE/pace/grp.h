/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/grp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_GRP_H
#define PACE_GRP_H

#if defined (PACE_HAS_POSIX)
# include "posix/grp.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/grp.h"
#elif defined (PACE_WIN32)
# include "win32/grp.h"
#endif

#endif /* PACE_GRP_H */
