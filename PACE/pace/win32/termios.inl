/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/termios.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/unistd.h"
#include <termios.h>

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
speed_t
pace_cfgetospeed (const struct termios *termiosp)
{
  return cfgetospeed (termiosp);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetospeed (struct termios *termios_p, pace_speed_t speed)
{
  return cfsetospeed (termios_p, speed);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
speed_t
pace_cfgetispeed (const struct termios *termios_p)
{
  return cfgetispeed (termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetispeed (struct termios *termios_p, pace_speed_t speed)
{
  return cfsetispeed (termios_p, speed);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcdrain (int fildes)
{
  return tcdrain (fildes);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcgetattr (int fildes, struct termios *termios_p)
{
  return tcgetattr (fildes, termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflow (int fildes, int action)
{
  return tcflow (fildes, action);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflush (int fildes, int queue_selector)
{
  return tcflush (fildes, queue_selector);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsendbreak (int fildes, int duration)
{
  return tcsendbreak (fildes, duration);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsetattr (int fildes,
                int optional_actions,
                const struct termios *termios_p)
{
  return tcsetattr (fildes, optional_actions, termios_p);
}
#endif /* PACE_HAS_POSIX_DS_UOF */
