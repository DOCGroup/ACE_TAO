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

#ifndef PACE_ARPA_INET_H_INDIRECT
#define PACE_ARPA_INET_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/inet.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/inet.h"
#elif defined (PACE_WIN32)
# include "pace/win32/inet.h"
#endif


#endif /* PACE_ARPA_INET_H_INDIRECT */
