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

#ifndef PACE_NETDB_H_INDIRECT
#define PACE_NETDB_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/netdb.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/netdb.h"
#elif defined (PACE_WIN32)
# include "pace/win32/netdb.h"
#endif

#endif /* PACE_NETDB_H_INDIRECT */
