/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/netinet/in.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_NETINET_IN_H
#define PACE_NETINET_IN_H

#if defined (PACE_HAS_POSIX)
# include "posix/in.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/in.h"
#elif defined (PACE_WIN32)
# include "win32/in.h"
#endif

#endif /* PACE_NETINET_IN_H */
