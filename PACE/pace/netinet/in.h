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

#ifndef PACE_NETINET_IN_H_INDIRECT
#define PACE_NETINET_IN_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/in.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/in.h"
#elif defined (PACE_WIN32)
# include "pace/win32/in.h"
#endif

#endif /* PACE_NETINET_IN_H_INDIRECT */
