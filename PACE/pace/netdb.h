/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/netdb.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_NETDB_H
#define PACE_NETDB_H

#if defined (PACE_HAS_POSIX)
# include "posix/netdb.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/netdb.h"
#elif defined (PACE_WIN32)
# include "win32/netdb.h"
#endif

#endif /* PACE_NETDB_H */
