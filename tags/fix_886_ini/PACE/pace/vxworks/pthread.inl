/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/pthread.inl
 *
 * = AUTHOR
 *    Joe Hoffert. The *VAST* majority of the pthread code for VxWorks
 *    has been supplied by Hughes Network Systems via Braeton Taylor.
 *    
 *
 * ============================================================================= */

#include "pace/unistd.h"

#include "pace/stdio.h"

/*
 * PACE - POSIX Functions
 */


#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
int
pace_pthread_atfork (pace_atfork_pf prepare,
                     pace_atfork_pf parent,
                     pace_atfork_pf child)
{
  /*
   * In VxWorks, there is only one process.
   */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
int
pace_pthread_atfork (void (*prepare) (),
                     void (*parent) (),
                     void (*child) ())
{
  /*
   * In VxWorks, there is only one process.
   */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_destroy (pace_pthread_attr_t * attr)
{
  /*
   * Free memory allocated to the attributes object.
   */
  if ((*attr) == 0)
    return ERROR;
 
  free ((char *) *attr);
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
                                  int * detachstate)
{
  *detachstate = (*attr)->dstate;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
                                   int * inheritsched)
{
  /*
   * Get the priority inheritence of the thread attribute structure.
   * Only PTHREAD_EXPLICIT_SCHED is returned.
   */
  *inheritsched = PTHREAD_EXPLICIT_SCHED;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
                                 pace_sched_param * param)
{
  param->sched_priority = (*attr)->schedule.sched_priority;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
                                  int * policy)
{
  /*
   * VxWorks only supports SCHED_FIFO.
   */

  *policy = SCHED_FIFO;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getscope (const pace_pthread_attr_t * attr,
                            int * contentionscope)
{
  /*
   * Since all VxWorks tasks are system wide scope,
   * no support for the scope except for PTHREAD_SCOPE_SYSTEM
   */

  *contentionscope = PTHREAD_SCOPE_SYSTEM;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
                                void ** stackaddr)
{
  /*
   * VxWorks has no support for stack address getting 
   */

  return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
                                size_t * stacksize)
{
  if (*attr == 0)
    return ERROR;

  *stacksize = (*attr)->stacksize;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
                                   int inheritsched)
{
  /*
   * For VxWorks, no inheritance of priority between the spawning task
   * and the spawned task. Only PTHREAD_EXPLICIT_SCHED is supported.
   */
  if (inheritsched == PTHREAD_EXPLICIT_SCHED)
    {
      return OK;
    }

  return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
                                  int policy)
{
  /*
   * In VxWorks, the schedule policy is set in system scope, 
   * the whole system is running with one single scheduler, and
   * is always pre-emptive priority based. The system wide 
   * scheduler can only be changed by kernelTimeSlice() 
   *
   * To maintain the RTOS characteristics, only SCHED_FIFO is
   * supported.
   */
  if (policy != SCHED_FIFO)
    return ERROR;
  else
    return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setscope (pace_pthread_attr_t * attr,
                            int contentionscope)
{
  /*
   * Since all VxWorks tasks are system wide scope,
   * no support for the scope except for PTHREAD_SCOPE_SYSTEM
   */
  if (contentionscope != PTHREAD_SCOPE_SYSTEM)
    return ERROR;
  else
    return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
                                void * stackaddr)
{
  /*
   * VxWorks does not support stack address setting.
   */
  return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstacksize (pace_pthread_attr_t * attr,
                                size_t stacksize)
{
  /*
   * Default size will be assigned if the stacksize is 0
   */
  if (*attr == 0) return ERROR;

  if (stacksize != 0)
    (*attr)->stacksize = stacksize;
  else
    (*attr)->stacksize = PTHREAD_DEFAULT_STACK_SIZE;

  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_broadcast (pace_pthread_cond_t * cond)
{
  /* 
   * This call unblocks all the threads that are blocked 
   * on the specified condition variable cond.  
   */
  return semFlush(*cond);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_destroy (pace_pthread_cond_t * cond)
{
  if (*cond != NULL)
    return semDelete(*cond);

  return EINVAL;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_init (pace_pthread_cond_t * cond,
                        const pace_pthread_condattr_t * attr)
{
  *cond = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
  if (*cond == NULL)
    return ERROR;

  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_signal (pace_pthread_cond_t * cond)
{
  return semGive(*cond);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_destroy (pace_pthread_condattr_t * attr)
{
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_getpshared (const pace_pthread_condattr_t * attr,
                                  int * pshared)
{
  /*
   * All the resources in present VxWorks are shared system wide.
   */
  *pshared = PTHREAD_PROCESS_SHARED;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_init (pace_pthread_condattr_t * attr)
{
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_setpshared (pace_pthread_condattr_t * attr,
                                 int pshared)
{
  /*
   * All the resources in present VxWorks are shared system wide.
   */
  if (pshared != PTHREAD_PROCESS_SHARED)
    return ERROR;
 
  return OK;  
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_equal (pace_pthread_t t1, pace_pthread_t t2)
{
  if (pacevx_pthread_verify(t1) && pacevx_pthread_verify(t2))
    {
      if (t1->tid != t2->tid)
        return 0;
    }

  return 1;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_exit (void * value_ptr)
{
  /*
   * Normal thread exit. All the cleanup routine will be popped, if any.
   * If the thread is detached, free the storage; otherwise wait for join.
   */
  pace_pthread_t pthread;

  if ((pthread = pace_pthread_self()) != NULL)
    {
      pacevx_pthread_proc_exit(pthread, value_ptr);
 
      exit(0);
    }
  exit(-1);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_kill (pace_pthread_t thread, int sig)
{
  if (pacevx_pthread_verify(thread))
    return kill(thread->tid, sig);
  else
    return EINVAL;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_destroy (pace_pthread_mutex_t * mutex)
{
  if (*mutex != NULL)
    return semDelete(*mutex);

  return EINVAL;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_getprioceiling (pace_pthread_mutex_t * mutex,
                                   int * prioceiling)
{
  /* 
   * Mutex priority is not supported in VxWorks;
   * it depends on the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  *prioceiling = SCHED_RR_HIGH_PRI;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_setprioceiling (pace_pthread_mutex_t * mutex,
                                   int prioceiling,
                                   int * old_ceiling)
{
  /*
   * Mutex priority is not supported in VxWorks;
   * it depends on the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  *old_ceiling = SCHED_RR_HIGH_PRI;

  if (prioceiling != SCHED_RR_HIGH_PRI)
    return ERROR;

  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_destroy (pace_pthread_mutexattr_t * attr)
{
  free(*attr);
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprioceiling (pace_pthread_mutexattr_t * attr,
                                       int * prioceiling)
{
  /* 
   * Mutex priority is not supported in VxWorks, 
   * it depends the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  *prioceiling = SCHED_RR_HIGH_PRI;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprotocol (const pace_pthread_mutexattr_t * attr,
                                    int * protocol)
{
/*
 * Does not support PTHREAD_PRIO_PROTECT for VxWorks
 */
  if (attr == 0)
    return EINVAL;

  *protocol = (*attr)->protocol;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setprioceiling (pace_pthread_mutexattr_t * attr,
                                       int prioceiling)
{
  /* 
   * Mutex priority is not supported in VxWorks;
   * it depends on the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  if (prioceiling == SCHED_RR_HIGH_PRI)
     return OK;
  else
     return ERROR;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getpshared (const pace_pthread_mutexattr_t * attr,
                                   int * pshared)
{
  /*
   * Only supports PTHREAD_PROCESS_SHARED 
   */
  *pshared = PTHREAD_PROCESS_SHARED;
  return OK;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_pthread_t
pace_pthread_self ()
{
  WIND_TCB *pTcb;

  if ((pTcb = taskTcb(taskIdSelf())) == NULL)
    return (pace_pthread_t)NULL;

  return (pace_pthread_t)(pTcb->_USER_SPARE4);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_testcancel ()
{
  /* Do nothing per ACE (i.e., ACE_OS::thr_testcancel). */
  return;
}
#endif /* PACE_HAS_NONUOF_FUNCS */
