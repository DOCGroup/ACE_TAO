/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pthread.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/unistd.h"

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_atfork (void (*prepare) (void),
                     void (*parent) (void),
                     void (*child) (void))
{
  PACE_UNUSED_ARG (prepare);
  PACE_UNUSED_ARG (parent);
  PACE_UNUSED_ARG (child);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_destroy (pace_pthread_attr_t * attr)
{
  return pthread_attr_destroy (attr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
                                  int * detachstate)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (detachstate);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
                                   int * inheritsched)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (inheritsched);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
                                 pace_sched_param * param)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (param);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
                                  int * policy)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (policy);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getscope (const pace_pthread_attr_t * attr,
                            int * contentionscope)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (contentionscope);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
                                void ** stackaddr)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (stackaddr);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
                                size_t * stacksize)
{
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (stacksize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_init (pace_pthread_attr_t * attr)
{
  return pthread_attr_init (attr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
                                  int detachstate)
{
  return pthread_attr_setdetachstate (attr, detachstate);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
                                   int inheritsched)
{
  return pthread_attr_setinheritsched (attr, inheritsched);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedparam (pace_pthread_attr_t * attr,
                                 const pace_sched_param * param)
{
  return pthread_attr_setschedparam (attr, param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
                                  int policy)
{
  return pthread_attr_setschedpolicy (attr, policy);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setscope (pace_pthread_attr_t * attr,
                            int contentionscope)
{
  return pthread_attr_setscope (attr, contentionscope);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
                  