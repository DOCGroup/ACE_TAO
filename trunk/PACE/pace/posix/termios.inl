/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/termios.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/unistd.h"
#include <termios.h>

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
pace_speed_t
pace_cfgetospeed (const pace_termios *termiosp)
{
  return cfgetospeed (termiosp);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetospeed (pace_termios *termios_p, pace_speed_t speed)
{
  return cfsetospeed (termios_p, speed);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
pace_speed_t
pace_cfgetispeed (const pace_termios *termios_p)
{
  return cfgetispeed (termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetispeed (pace_termios *termios_p, pace_speed_t speed)
{
  return cfsetispeed (termios_p, speed);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcdrain (PACE_HANDLE fildes)
{
  return tcdrain (fildes);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcgetattr (PACE_HANDLE fildes, pace_termios *termios_p)
{
  return tcgetattr (fildes, termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflow (PACE_HANDLE fildes, int action)
{
  return tcflow (fildes, action);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflush (PACE_HANDLE fildes, int queue_selector)
{
  return tcflush (fildes, queue_selector);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsendbreak (PACE_HANDLE fildes, int duration)
{
  return tcsendbreak (fildes, duration);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsetattr (PACE_HANDLE fildes,
                int optional_actions,
                const pace_termios *termios_p)
{
  return tcsetattr (fildes, optional_actions, termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */
