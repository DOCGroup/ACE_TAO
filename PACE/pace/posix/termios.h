/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H
#define PACE_SYS_TERMIOS_H

#include "pace/sys/types.h"
#include <termios.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS) */

#ifndef PACE_CC_T
#define PACE_CC_T
typedef cc_t pace_cc_t;
#endif /* PACE_CC_T */

#ifndef PACE_SPEED_T
#define PACE_SPEED_T
typedef speed_t pace_speed_t;
#endif /* PACE_SPEED_T */

#ifndef PACE_TCFLAG_T
#define PACE_TCFLAG_T
typedef tcflag_t pace_tcflag_t;
#endif /* PACE_TCFLAG_T */

#ifndef PACE_TERMIOS
#define PACE_TERMIOS
typedef struct termios pace_termios;
#endif /* PACE_TERMIOS */


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
# include "pace/posix/termios.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_TERMIOS_H */
