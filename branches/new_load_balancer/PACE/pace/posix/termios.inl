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
PACE_INLINE
pace_speed_t
pace_cfgetospeed (const pace_termios *termiosp)
{
  return cfgetospeed (termiosp);
}

PACE_INLINE
int
pace_cfsetospeed (pace_termios *termios_p, pace_speed_t speed)
{
  return cfsetospeed (termios_p, speed);
}

PACE_INLINE
pace_speed_t
pace_cfgetispeed (const pace_termios *termios_p)
{
  return cfgetispeed (termios_p);
}

PACE_INLINE
int
pace_cfsetispeed (pace_termios *termios_p, pace_speed_t speed)
{
  return cfsetispeed (termios_p, speed);
}

PACE_INLINE
int
pace_tcdrain (int fildes)
{
  return tcdrain (fildes);
}

PACE_INLINE
int
pace_tcgetattr (int fildes, pace_termios *termios_p)
{
  return tcgetattr (fildes, termios_p);
}

PACE_INLINE
int
pace_tcflow (int fildes, int action)
{
  return tcflow (fildes, action);
}

PACE_INLINE
int
pace_tcflush (int fildes, int queue_selector)
{
  return tcflush (fildes, queue_selector);
}

PACE_INLINE
int
pace_tcsendbreak (int fildes, int duration)
{
  return tcsendbreak (fildes, duration);
}

PACE_INLINE
int
pace_tcsetattr (int fildes,
                int optional_actions,
                const pace_termios *termios_p)
{
  return tcsetattr (fildes, optional_actions, termios_p);
}
