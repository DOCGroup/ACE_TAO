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

PACE_INLINE
int
pace_mq_close (mqd_t mqdes)
{
# if defined (PACE_HAS_RT)
  return mq_close (mqdes);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
int
pace_mq_getattr (mqd_t mqdes,
                 struct mq_attr * mqstat)
{
# if defined (PACE_HAS_RT)
  return mq_getattr (mqdes, mqstat);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
int
pace_mq_notify (mqd_t mqdes,
                const struct sigevent * notification)
{
# if defined (PACE_HAS_RT)
  return mq_notify (mqdes, notification);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
ssize_t
pace_mq_receive (mqd_t mqdes,
                 char * msg_ptr,
                 size_t msg_len,
                 unsigned int * msg_prio)
{
# if defined (PACE_HAS_RT)
  return mq_receive (mqdes, msg_ptr, msg_len, msg_prio);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
int
pace_mq_send (mqd_t mqdes,
              const char * msg_ptr,
              size_t msg_len,
              unsigned int msg_prio)
{
# if defined (PACE_HAS_RT)
  return mq_send (mqdes, msg_ptr, msg_len, msg_prio);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
int
pace_mq_setattr (mqd_t mqdes,
                 const struct mq_attr * mqstat,
                 struct mq_attr * omqstat)
{
# if defined (PACE_HAS_RT)
  return mq_setattr (mqdes, mqstat, omqstat);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}

PACE_INLINE
int
pace_mq_unlink (const char * name)
{
# if defined (PACE_HAS_RT)
  return mq_unlink (name);
# else /* !PACE_HAS_RT */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_RT);
# endif /* !PACE_HAS_RT */
}
