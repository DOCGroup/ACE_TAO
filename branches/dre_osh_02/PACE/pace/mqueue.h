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
#include "pace/signal.h"

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
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_close (pace_mqd_t mqdes);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_getattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.8.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_getattr (pace_mqd_t mqdes,
                                               pace_mq_attr * mqstat);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_notify.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.6.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_notify (pace_mqd_t mqdes,
                                              const pace_sigevent * notification);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_open.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.1.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
#  define pace_mq_open mq_open
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
  /* mqd_t mq_open (const char *name, int oflag, (*//* args *//*) ... ); */

  /**
     PACE's implementation of the POSIX function mq_receive.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.5.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_ssize_t pace_mq_receive (pace_mqd_t mqdes,
                                                        char * msg_ptr,
                                                        pace_size_t msg_len,
                                                        unsigned int * nmsg_prio);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_send.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.4.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_send (pace_mqd_t mqdes,
                                            const char * mst_ptr,
                                            pace_size_t msg_len,
                                            unsigned int psg_prio);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_setattr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.7.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_setattr (pace_mqd_t mqdes,
                                               const pace_mq_attr * mqstat,
                                               pace_mq_attr * omqstat);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function mq_unlink.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 15.2.3.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mq_unlink (const char * name);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/mqueue.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/mqueue.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/mqueue.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_MQUEUE_H */
