/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/mqueue.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_close (pace_mqd_t mqdes)
{
  PACE_TRACE("pace_mq_close");

  return mq_close (mqdes);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_getattr (pace_mqd_t mqdes,
                 pace_mq_attr * mqstat)
{
  PACE_TRACE("pace_mq_getattr");

  return mq_getattr (mqdes, mqstat);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_notify (pace_mqd_t mqdes,
                const pace_sigevent * notification)
{
  PACE_TRACE("pace_mq_notify");

  return mq_notify (mqdes, notification);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
ssize_t
pace_mq_receive (pace_mqd_t mqdes,
                 char * msg_ptr,
                 size_t msg_len,
                 unsigned int * msg_prio)
{
  PACE_TRACE("pace_mq_receive");

  return mq_receive (mqdes, msg_ptr, msg_len, msg_prio);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_send (pace_mqd_t mqdes,
              const char * msg_ptr,
              size_t msg_len,
              unsigned int msg_prio)
{
  PACE_TRACE("pace_mq_send");

  return mq_send (mqdes, msg_ptr, msg_len, msg_prio);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_setattr (pace_mqd_t mqdes,
                 const pace_mq_attr * mqstat,
                 pace_mq_attr * omqstat)
{
  PACE_TRACE("pace_mq_setattr");

  return mq_setattr (mqdes, mqstat, omqstat);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_unlink (const char * name)
{
  PACE_TRACE("pace_mq_unlink");

  return mq_unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
