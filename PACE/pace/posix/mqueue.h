/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    mqueue.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_MQUEUE_H
#define PACE_MQUEUE_H


#include "pace/defines.h"

/* Linux doesn't support this file at all. We will allow the interface, but
   all return values will be PACE_ERRNO_LACKS_RT. */
# if defined (linux)
typedef unsigned int mqd_t;
struct mq_attr { };
struct sigevent { };
#include "pace/unistd.h"
# else /* !linux */
#include <mqueue.h>
#endif /* !linux */


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_mq_close (mqd_t mqdes);

  PACE_INLINE int pace_mq_getattr (mqd_t mqdes,
                                   struct mq_attr * mqstat);

  PACE_INLINE int pace_mq_notify (mqd_t mqdes,
                                  const struct sigevent * notification);

  # define pace_mq_open(X) mq_open X
  /* mqd_t mq_open (const char *name, int oflag, (*//* args *//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE ssize_t pace_mq_receive (mqd_t mqdes,
                                       char * msg_ptr,
                                       size_t msg_len,
                                       unsigned int * nmsg_prio);

  PACE_INLINE int pace_mq_send (mqd_t mqdes,
                                const char * mst_ptr,
                                size_t msg_len,
                                unsigned int psg_prio);

  PACE_INLINE int pace_mq_setattr (mqd_t mqdes,
                                   const struct mq_attr * mqstat,
                                   struct mq_attr * omqstat);

  PACE_INLINE int pace_mq_unlink (const char * name);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "mqueue.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_MQUEUE_H */
