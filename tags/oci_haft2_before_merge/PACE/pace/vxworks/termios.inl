/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/termios.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */


#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
pace_speed_t
pace_cfgetospeed (const pace_termios *termiosp)
{
  PACE_TRACE("pace_cfgetospeed");

  PACE_UNUSED_ARG(termiosp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetospeed (pace_termios *termios_p, pace_speed_t speed)
{
  PACE_TRACE("pace_cfsetospeed");

  PACE_UNUSED_ARG(termios_p);
  PACE_UNUSED_ARG(speed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
pace_speed_t
pace_cfgetispeed (const pace_termios *termios_p)
{
  PACE_TRACE("pace_cfgetispeed");

  PACE_UNUSED_ARG(termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_cfsetispeed (pace_termios *termios_p, pace_speed_t speed)
{
  PACE_TRACE("pace_cfsetispeed");

  PACE_UNUSED_ARG(termios_p);
  PACE_UNUSED_ARG(speed);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcdrain (PACE_HANDLE fildes)
{
  PACE_TRACE("pace_tcdrain");

  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcgetattr (PACE_HANDLE fildes, pace_termios *termios_p)
{
  PACE_TRACE("pace_tcgetattr");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflow (PACE_HANDLE fildes, int action)
{
  PACE_TRACE("pace_tcflow");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(action);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcflush (PACE_HANDLE fildes, int queue_selector)
{
  PACE_TRACE("pace_tcflush");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(queue_selector);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsendbreak (PACE_HANDLE fildes, int duration)
{
  PACE_TRACE("pace_tcsendbreak");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(duration);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_tcsetattr (PACE_HANDLE fildes,
                int optional_actions,
                const pace_termios *termios_p)
{
  PACE_TRACE("pace_tcsetattr");

  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(optional_actions);
  PACE_UNUSED_ARG(termios_p);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_DS_UOF */
