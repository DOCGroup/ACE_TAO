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
  return mq_close (mqdes);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_getattr (pace_mqd_t mqdes,
                 pace_mq_attr * mqstat)
{
  return mq_getattr (mqdes, mqstat);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_notify (pace_mqd_t mqdes,
                const pace_sigevent * notification)
{
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
  return mq_setattr (mqdes, mqstat, omqstat);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_unlink (const char * name)
{
  return mq_unlink (name);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
