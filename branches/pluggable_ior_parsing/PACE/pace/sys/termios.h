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

#ifndef PACE_SYS_TERMIOS_H_INDIRECT
#define PACE_SYS_TERMIOS_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/termios.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/termios.h"
#elif defined (PACE_WIN32)
# include "pace/win32/termios.h"
#endif

#endif /* PACE_SYS_TERMIOS_H_INDIRECT */
