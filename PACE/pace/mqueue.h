/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/mqueue.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MQUEUE_H
#define PACE_MQUEUE_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/mqueue.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/mqueue.h"
#elif (PACE_WIN32)
# include "pace/win32/mqueue.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function mq_close.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.2.
   */
  PACE_INLINE int pace_mq_close (pace_mqd_t mqdes);

  /**
     PACE's implementation of the POSIX function mq_getattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.8.
   */
  PACE_INLINE int pace_mq_getattr (pace_mqd_t mqdes,
                                   pace_mq_attr * mqstat);

  /**
     PACE's implementation of the POSIX function mq_notify.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.6.
   */
  PACE_INLINE int pace_mq_notify (pace_mqd_t mqdes,
                                  const pace_sigevent * notification);

  /**
     PACE's implementation of the POSIX function mq_open.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.1.
   */
  # define pace_mq_open(X) mq_open X
  /* mqd_t mq_open (const char *name, int oflag, (*//* args *//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  /**
     PACE's implementation of the POSIX function mq_receive.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.5.
   */
  PACE_INLINE ssize_t pace_mq_receive (pace_mqd_t mqdes,
                                       char * msg_ptr,
                                       pace_size_t msg_len,
                                       unsigned int * nmsg_prio);

  /**
     PACE's implementation of the POSIX function mq_send.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.4.
   */
  PACE_INLINE int pace_mq_send (pace_mqd_t mqdes,
                                const char * mst_ptr,
                                pace_size_t msg_len,
                                unsigned int psg_prio);

  /**
     PACE's implementation of the POSIX function mq_setattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.7.
   */
  PACE_INLINE int pace_mq_setattr (pace_mqd_t mqdes,
                                   const pace_mq_attr * mqstat,
                                   pace_mq_attr * omqstat);

  /**
     PACE's implementation of the POSIX function mq_unlink.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.3.
   */
  PACE_INLINE int pace_mq_unlink (const char * name);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/mqueue.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/mqueue.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/mqueue.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_MQUEUE_H */
