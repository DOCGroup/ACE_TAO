/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mqueue.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MQUEUE_H_WIN32
#define PACE_MQUEUE_H_WIN32

#include "pace/signal.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_MQD_T
#define PACE_MQD_T
  /* Temporary fix until we emulate these funcs.
     typedef mqd_t pace_mqd_t;
   */
  typedef int pace_mqd_t;
#endif /* PACE_MQD_T */

#ifndef PACE_MQ_ATTR
#define PACE_MQ_ATTR
  /* Temporary fix until we emulate these funcs.
     typedef struct mq_attr pace_mq_attr
   */
  typedef struct mq_attr { int a_; } pace_mq_attr;
#endif /* PACE_MQ_ATTR */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_MQUEUE_H_WIN32 */
