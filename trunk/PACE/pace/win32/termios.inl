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

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
speed_t
pace_cfgetospeed (const struct termios * termiosp)
{
  PACE_UNUSED_ARG (termiosp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetospeed (struct termios * termios_p, pace_speed_t speed)
{
  PACE_UNUSED_ARG (termios_p);
  PACE_UNUSED_ARG (speed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
speed_t
pace_cfgetispeed (const struct termios * termios_p)
{
  PACE_UNUSED_ARG (termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetispeed (struct termios * termios_p, pace_speed_t speed)
{
  PACE_UNUSED_ARG (termios_p);
  PACE_UNUSED_ARG (speed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcdrain (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcgetattr (PACE_HANDLE fildes, struct termios * termios_p)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflow (PACE_HANDLE fildes, int action)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (action);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflush (PACE_HANDLE fildes, int queue_selector)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (queue_selector);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsendbreak (PACE_HANDLE fildes, int duration)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (duration);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsetattr (PACE_HANDLE fildes,
                int optional_actions,
                const struct termios * termios_p)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (optional_actions);
  PACE_UNUSED_ARG (termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */
