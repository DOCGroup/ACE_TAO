/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H
#define PACE_SYS_TERMIOS_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/termios.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/termios.h"
#elif (PACE_WIN32)
# include "pace/win32/termios.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE pace_speed_t pace_cfgetospeed (const pace_termios * termiosp);

  PACE_INLINE int pace_cfsetospeed (pace_termios * termios_p, pace_speed_t speed);

  PACE_INLINE pace_speed_t pace_cfgetispeed (const pace_termios * termios_p);

  PACE_INLINE int pace_cfsetispeed (pace_termios * termios_p, pace_speed_t speed);

  PACE_INLINE int pace_tcdrain (int fildes);

  PACE_INLINE int pace_tcflow (int fildes, int action);

  PACE_INLINE int pace_tcflush (int fildes, int queue_selector);

  PACE_INLINE int pace_tcgetattr (int fildes, pace_termios * termios_p);

  PACE_INLINE int pace_tcsendbreak (int fildes, int duration);

  PACE_INLINE int pace_tcsetattr (int fildes,
                                  int optional_actions,
                                  const pace_termios * termios_p);


#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/termios.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/termios.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/termios.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_TERMIOS_H */
