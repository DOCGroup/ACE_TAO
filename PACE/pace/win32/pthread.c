/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pthread.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/pthread.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/pthread.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_destroy (pace_pthread_attr_t * attr)
{
  if (attr->init_ == 1)
    {
      attr->init_ = 0;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_init (pace_pthread_attr_t * attr)
{
  if (attr->init_ == 1)
    {
      /* ERROR: not initilalized properly! */
      return -1;
    }
  attr->init_ = 1;
  return 0;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
			     int detachstate)
{
  if (attr->init_ == 1)
    {
      attr->detach_state_ = detachstate;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
			      int inheritsched)
{
  if (attr->init_ == 1)
    {
      attr->inherit_sched_ = inheritsched;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setschedparam (pace_pthread_attr_t * attr,
			    const pace_sched_param * param)
{
  if (attr->init_ == 1)
    {
      attr->sparam_.priority_ = param->priority_;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
				 int policy)
{
  if (attr->init_ == 1)
    {
      attr->policy_ = policy;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setscope (pace_pthread_attr_t * attr,
		       int contentionscope)
{
  if (attr->init_ == 1)
    {
      attr->contention_scope_ = contentionscope;
      return 0;
    }
  /* ERROR: not initialized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
			   void * stackaddr)
{
  if (attr->init_ == 1)
    {
      attr->stack_addr_ = stackaddr;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_setstacksize (pace_pthread_attr_t * attr,
			   size_t stacksize)
{
  if (attr->init_ == 1)
    {
      attr->stack_size_ = stacksize;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_create (pace_pthread_t * thread,
		const pace_pthread_attr_t * attr,
		void * (*start_routine) (void*),
		void * arg)
{
  /* not working yet .. fill me in */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_getschedparam (pace_pthread_t thread,
		       int * policy,
		       pace_sched_param * param)
{
  if (param != (pace_sched_param*)0)
    {
      param->priority_ = GetThreadPriority (thread);
      return 0;
    }
  /* Invalid pointer to pace_sched_param. */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
pace_pthread_t
pthread_self ()
{
  return GetCurrentThread ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_setschedparam (pace_pthread_t thread,
			     int policy,
			     const pace_sched_param * param)
{
  if (SetThreadPriority (thread, param->priority_))
    {
      return 0;
    }
  /* ERROR: with return value of SetThreadPriority. */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
