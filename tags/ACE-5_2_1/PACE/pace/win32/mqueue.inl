/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mqueue.inl
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
  PACE_UNUSED_ARG (mqdes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_getattr (pace_mqd_t mqdes,
                 pace_mq_attr * mqstat)
{
  PACE_UNUSED_ARG (mqdes);
  PACE_UNUSED_ARG (mqstat);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_notify (pace_mqd_t mqdes,
                const pace_sigevent * notification)
{
  PACE_UNUSED_ARG (mqdes);
  PACE_UNUSED_ARG (notification);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_ssize_t
pace_mq_receive (pace_mqd_t mqdes,
                 char * msg_ptr,
                 size_t msg_len,
                 unsigned int * msg_prio)
{
  PACE_UNUSED_ARG (mqdes);
  PACE_UNUSED_ARG (msg_ptr);
  PACE_UNUSED_ARG (msg_len);
  PACE_UNUSED_ARG (msg_prio);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
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
  PACE_UNUSED_ARG (mqdes);
  PACE_UNUSED_ARG (msg_ptr);
  PACE_UNUSED_ARG (msg_len);
  PACE_UNUSED_ARG (msg_prio);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_setattr (pace_mqd_t mqdes,
                 const pace_mq_attr * mqstat,
                 pace_mq_attr * omqstat)
{
  PACE_UNUSED_ARG (mqdes);
  PACE_UNUSED_ARG (mqstat);
  PACE_UNUSED_ARG (omqstat);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mq_unlink (const char * name)
{
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
