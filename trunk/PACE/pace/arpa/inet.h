/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/arpa/inet.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_ARPA_INET_H
#define PACE_ARPA_INET_H

#if defined (PACE_HAS_POSIX)
# include "posix/inet.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/inet.h"
#elif defined (PACE_WIN32)
# include "win32/inet.h"
#endif


#endif /* PACE_ARPA_INET_H */
