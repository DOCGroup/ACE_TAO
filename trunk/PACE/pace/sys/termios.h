/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H
#define PACE_SYS_TERMIOS_H

#if defined (PACE_HAS_POSIX)
# include "posix/termios.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/termios.h"
#elif defined (PACE_WIN32)
# include "win32/termios.h"
#endif

#endif /* PACE_SYS_TERMIOS_H */
