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

  /**
     PACE's implementation of the POSIX function cfgetospeed.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.1.3.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE pace_speed_t pace_cfgetospeed (const pace_termios * termiosp);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function cfsetospeed.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.1.3.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_cfsetospeed (pace_termios * termios_p,
                                                pace_speed_t speed);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function cfgetispeed.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.1.3.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE pace_speed_t pace_cfgetispeed (const pace_termios * termios_p);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function cfsetispeed.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.1.3.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_cfsetispeed (pace_termios * termios_p,
                                                pace_speed_t speed);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcdrain.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcdrain (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcflow.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcflow (PACE_HANDLE fildes, int action);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcflush.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcflush (PACE_HANDLE fildes,
                                            int queue_selector);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcgetattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.1.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcgetattr (PACE_HANDLE fildes,
                                              pace_termios * termios_p);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcsendbreak.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcsendbreak (PACE_HANDLE fildes,
                                                int duration);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function tcsetattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.1.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_tcsetattr (PACE_HANDLE fildes,
                                              int optional_actions,
                                              const pace_termios * termios_p);
#endif /* PACE_HAS_POSIX_DS_UOF */


#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/termios.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/termios.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/termios.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TERMIOS_H */
