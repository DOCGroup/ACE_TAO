/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/socket.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_SOCKET_H
#define PACE_SYS_SOCKET_H

#if defined (PACE_HAS_POSIX)
# include "posix/socket.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/socket.h"
#elif defined (PACE_WIN32)
# include "win32/socket.h"
#endif

#endif /* PACE_SYS_SOCKET_H */
