/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    termios.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include <unistd.h>


PACE_INLINE
speed_t
pace_cfgetospeed (const struct termios *termiosp)
{
  return cfgetospeed (termiosp);
}

PACE_INLINE
int
pace_cfsetospeed (struct termios *termios_p, speed_t speed)
{
  return cfsetospeed (termios_p, speed);
}

PACE_INLINE
speed_t
pace_cfgetispeed (const struct termios *termios_p)
{
  return cfgetispeed (termios_p);
}

PACE_INLINE
int
pace_cfsetispeed (struct termios *termios_p, speed_t speed)
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
pace_tcgetattr (int fildes, struct termios *termios_p)
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
pid_t
pace_tcgetpgrp (int fildes)
{
  return tcgetpgrp (fildes);
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
                const struct termios *termios_p)
{
  return tcsetattr (fildes, optional_actions, termios_p);
}

PACE_INLINE
int
pace_tcsetpgrp (int fildes, pid_t pgrp_id)
{
  return tcsetpgrp (fildes, pgrp_id);
}



