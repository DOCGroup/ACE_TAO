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
#include <process.h>

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/pthread.inl"
#endif /* ! PACE_HAS_INLINE */

//////////////////////////////////////////////////////////////////////
// pthread_t
//////////////////////////////////////////////////////////////////////

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_create (pace_pthread_t * thread,
		const pace_pthread_attr_t * attr,
		void * (*start_routine) (void*),
		void * arg)
{
  /* Get around a gcc bug. */
  typedef unsigned (__stdcall *bthrexr)(void*);

  unsigned flags = 0x0, thr_addr = 0x0;
  if (attr->sparam_.sched_priority != THREAD_PRIORITY_NORMAL)
    {
      // CREATE_SUSPENDED is the only flag win32 currently supports
      flags = CREATE_SUSPENDED;
    }

  thread = (pace_pthread_t)
    _beginthreadex (0,
		    attr->stack_size_,
		    (bthrexr) start_routine,
		    arg,
		    flags,
		    &thr_addr);

  if (flags == CREATE_SUSPENDED && thread != 0)
    {
      SetThreadPriority (thread, attr->sparam_.sched_priority);
      ResumeThread (thread);
    }

  if (thread == 0)
    {
      return 0;
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }

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
int
pthread_getschedparam (pace_pthread_t thread,
		       int * policy,
		       pace_sched_param * param)
{
  PACE_UNUSED_ARG (policy);
  if (param != (pace_sched_param*)0)
    {
      param->sched_priority = GetThreadPriority (thread);
      return 0;
    }
  /* Invalid pointer to pace_sched_param. */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_setschedparam (pace_pthread_t thread,
			     int policy,
			     const pace_sched_param * param)
{
  PACE_UNUSED_ARG(policy);

  if (SetThreadPriority (thread, param->sched_priority))
    {
      return 0;
    }
  /* ERROR: with return value of SetThreadPriority. */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

//////////////////////////////////////////////////////////////////////
// pthread_attr_t
//////////////////////////////////////////////////////////////////////

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pthread_attr_init (pace_pthread_attr_t * attr)
{
  /* These need to be the platform defaults!
   * Since I made this, I need to maintain this invariant!
   * Eventually, #define 'words' would be better than
   * constants here! */

  attr->init_ = 1;
  attr->detach_state_ = 0;
  attr->policy_ = 0;
  attr->sparam_.sched_priority = 0;
  attr->inherit_sched_ = 0;
  attr->contention_scope_ = 0;
  attr->guard_size_ = 0;
  attr->stackaddr_set_ = 0;
  attr->stack_addr_ = (void*)0;
  attr->stack_size_ = 0;

  return 0;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

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
pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
			     int * detachstate)
{
  if (attr->init_ == 1)
    {
      *detachstate = attr->detach_state_;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
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
pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
			      int * inheritsched)
{
  if (attr->init_ == 1)
    {
      *inheritsched = attr->inherit_sched_;
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
pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
			    pace_sched_param * param)
{
  if (attr->init_ == 1)
    {
      param->sched_priority = attr->sparam_.sched_priority;
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
      attr->sparam_.sched_priority = param->sched_priority;
      return 0;
    }
  /* ERROR: not initilalized properly! */
  return -1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
				 int * policy)
{
  if (attr->init_ == 1)
    {
      *policy = attr->policy_;
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
pthread_attr_getscope (const pace_pthread_attr_t * attr,
		       int * contentionscope)
{
  if (attr->init_ == 1)
    {
      *contentionscope = attr->contention_scope_;
      return 0;
    }
  /* ERROR: not initialized properly! */
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
pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
			   void ** stackaddr)
{
  if (attr->init_ == 1)
    {
      *stackaddr = attr->stack_addr_;
      return 0;
    }
  /* ERROR: not initilalized properly! */
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
pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
			   size_t * stacksize)
{
  if (attr->init_ == 1)
    {
      *stacksize = attr->stack_size_;
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
