/* $Id$ */
#ifndef PACE_EMU_MQUEUE_H
#define PACE_EMU_MQUEUE_H

#include "pace/signal.h"

#define PACE_MQ_ATTR
typedef struct mq_attr
{
  long mq_flags;
  pace_size_t mq_maxmsg;
  pace_size_t mq_msgsize;
  pace_size_t mq_curmsgs;
} pace_mq_attr;

struct mqd
{
  int fd;
  long length;
  int oflag;
  char* mptr;
};

#define PACE_MQD_T
typedef struct mqd* pace_mqd_t;

pace_mqd_t mq_open (const char* name,
                    int oflag,
                    pace_mode_t mode,
                    pace_mq_attr* attr);

int mq_close (pace_mqd_t mqdes);
int mq_unlink (const char* name);
int mq_send (pace_mqd_t mqdes,
             const char* ptr,
             pace_size_t length,
             unsigned int priority);
pace_ssize_t mq_receive (pace_mqd_t mqdes,
                         char * msg_ptr,
                         pace_size_t msg_len,
                         unsigned int * nmsg_prio);
int mq_getattr (pace_mqd_t mqdes, pace_mq_attr * mqstat);
int mq_setattr (pace_mqd_t mqdes,
                const pace_mq_attr * mqstat,
                pace_mq_attr * omqstat);
int mq_notify (pace_mqd_t mqd, const pace_sigevent* notification);

#endif /* PACE_EMU_MQUEUE_H */
