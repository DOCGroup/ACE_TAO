/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_SYS_TERMIOS_H
#define PACE_SYS_TERMIOS_H


#include "pace/defines.h"
#include "pace/sys/types.h"
#include <termios.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS) */

  PACE_INLINE speed_t pace_cfgetospeed (const struct termios * termiosp);

  PACE_INLINE int pace_cfsetospeed (struct termios * termios_p, speed_t speed);

  PACE_INLINE speed_t pace_cfgetispeed (const struct termios * termios_p);

  PACE_INLINE int pace_cfsetispeed (struct termios * termios_p, speed_t speed);

  PACE_INLINE int pace_tcdrain (int fildes);

  PACE_INLINE int pace_tcflow (int fildes, int action);

  PACE_INLINE int pace_tcflush (int fildes, int queue_selector);

  PACE_INLINE int pace_tcgetattr (int fildes, struct termios * termios_p);

  PACE_INLINE pid_t pace_tcgetpgrp (int fildes);

  PACE_INLINE int pace_tcsendbreak (int fildes, int duration);

  PACE_INLINE int pace_tcsetattr (int fildes,
                                  int optional_actions,
                                  const struct termios * termios_p);

  PACE_INLINE int pace_tcsetpgrp (int fildes, pid_t pgrp_id);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "termios.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_SYS_TERMIOS_H */
