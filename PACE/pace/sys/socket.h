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

#ifndef PACE_SYS_SOCKET_H_INDIRECT
#define PACE_SYS_SOCKET_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/socket.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/socket.h"
#elif defined (PACE_WIN32)
# include "pace/win32/socket.h"
#endif

#endif /* PACE_SYS_SOCKET_H_INDIRECT */
