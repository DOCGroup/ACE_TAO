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
PACE_INLINE
int
pace_pthread_atfork (
# if defined (PACE_HAS_CPLUSPLUS)
                     pace_atfork_pf prepare,
                     pace_atfork_pf parent,
                     pace_atfork_pf child
# else /* ! PACE_HAS_CPLUSPLUS */
                     void (*prepare) (),
                     void (*parent) (),
                     void (*child) ()
# endif /* PACE_HAS_CPLUSPLUS */
                     )
{
  PACE_TRACE("pace_pthread_atfork");
  /*
   * In VxWorks, there is only one process.
   */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_destroy (pace_pthread_attr_t * attr)
{
  PACE_TRACE("pace_pthread_attr_destroy");
  /*
   * Free memory allocated to the attributes object.
   */
  if ((*attr) == 0)
    return ERROR;
 
  free ((char *) *attr);
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
                                  int * detachstate)
{
  PACE_TRACE("pace_pthread_attr_getdetachstate");

  *detachstate = (*attr)->dstate;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
                                   int * inheritsched)
{
  PACE_TRACE("pace_pthread_attr_getinheritsched");

  /*
   * Get the priority inheritence of the thread attribute structure.
   * Only PTHREAD_EXPLICIT_SCHED is returned.
   */
  *inheritsched = PTHREAD_EXPLICIT_SCHED;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
                                 pace_sched_param * param)
{
  PACE_TRACE("pace_pthread_attr_getschedparam");

  param->sched_priority = (*attr)->schedule.sched_priority;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
                                  int * policy)
{
  PACE_TRACE("pace_pthread_attr_getschedpolicy");

  /*
   * VxWorks only supports SCHED_FIFO.
   */

  *policy = SCHED_FIFO;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getscope (const pace_pthread_attr_t * attr,
                            int * contentionscope)
{
  PACE_TRACE("pace_pthread_attr_getscope");

  /*
   * Since all VxWorks tasks are system wide scope,
   * no support for the scope except for PTHREAD_SCOPE_SYSTEM
   */

  *contentionscope = PTHREAD_SCOPE_SYSTEM;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
                                void ** stackaddr)
{
  PACE_TRACE("pace_pthread_attr_getstackaddr");

  /*
   * VxWorks has no support for stack address getting 
   */

  return ERROR;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
                                size_t * stacksize)
{
  PACE_TRACE("pace_pthread_attr_getstacksize");

  if (*attr == 0)
    return ERROR;

  *stacksize = (*attr)->stacksize;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_init (pace_pthread_attr_t * attr)
{
  PACE_TRACE("pace_pthread_attr_init");

  return pthread_attr_init(attr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
                                  int detachstate)
{
  PACE_TRACE("pace_pthread_attr_setdetachstate");

  return pthread_attr_setdetachstate (attr, detachstate);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
                                   int inheritsched)
{
  PACE_TRACE("pace_pthread_attr_setinheritsched");

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
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedparam (pace_pthread_attr_t * attr,
                                 const pace_sched_param * param)
{
  PACE_TRACE("pace_pthread_attr_setschedparam");

  return pthread_attr_setschedparam (attr, param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
                                  int policy)
{
  PACE_TRACE("pace_pthread_attr_setschedpolicy");

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
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setscope (pace_pthread_attr_t * attr,
                            int contentionscope)
{
  PACE_TRACE("pace_pthread_attr_setscope");

  /*
   * Since all VxWorks tasks are system wide scope,
   * no support for the scope except for PTHREAD_SCOPE_SYSTEM
   */
  if (contentionscope != PTHREAD_SCOPE_SYSTEM)
    return ERROR;
  else
    return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
                                void * stackaddr)
{
  PACE_TRACE("pace_pthread_attr_setstackaddr");

  /*
   * VxWorks does not support stack address setting.
   */
  return ERROR;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstacksize (pace_pthread_attr_t * attr,
                                size_t stacksize)
{
  PACE_TRACE("pace_pthread_attr_setstacksize");

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
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cancel (pace_pthread_t thread)
{
  PACE_TRACE("pace_pthread_cancel");

  return pthread_cancel (thread);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_broadcast (pace_pthread_cond_t * cond)
{
  PACE_TRACE("pace_pthread_cond_broadcast");

  /* 
   * This call unblocks all the threads that are blocked 
   * on the specified condition variable cond.  
   */
  return semFlush(*cond);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_destroy (pace_pthread_cond_t * cond)
{
  PACE_TRACE("pace_pthread_cond_destroy");

  if (*cond != NULL)
    return semDelete(*cond);

  return EINVAL;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_init (pace_pthread_cond_t * cond,
                        const pace_pthread_condattr_t * attr)
{
  PACE_TRACE("pace_pthread_cond_init");

  *cond = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
  if (*cond == NULL)
    return ERROR;

  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_signal (pace_pthread_cond_t * cond)
{
  PACE_TRACE("pace_pthread_cond_signal");

  return semGive(*cond);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_timedwait (pace_pthread_cond_t * cond,
                             pace_pthread_mutex_t * mutex,
                             const pace_timespec * abstime)
{
  PACE_TRACE("pace_pthread_cond_timedwait");

  return pthread_cond_timedwait(cond,
                                mutex,
                                abstime);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_wait (pace_pthread_cond_t * cond,
                        pace_pthread_mutex_t * mutex)
{
  PACE_TRACE("pace_pthread_cond_wait");

  return pthread_cond_wait(cond,
                           mutex);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_destroy (pace_pthread_condattr_t * attr)
{
  PACE_TRACE("pace_pthread_condattr_destroy");

  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_getpshared (const pace_pthread_condattr_t * attr,
                                  int * pshared)
{
  PACE_TRACE("pace_pthread_condattr_getpshared");

  /*
   * All the resources in present VxWorks are shared system wide.
   */
  *pshared = PTHREAD_PROCESS_SHARED;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_init (pace_pthread_condattr_t * attr)
{
  PACE_TRACE("pace_pthread_condattr_init");

  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_setpshared (pace_pthread_condattr_t * attr,
                                 int pshared)
{
  PACE_TRACE("pace_pthread_condattr_setpshared");

  /*
   * All the resources in present VxWorks are shared system wide.
   */
  if (pshared != PTHREAD_PROCESS_SHARED)
    return ERROR;
 
  return OK;  
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_create (pace_pthread_t * thread,
                     const pace_pthread_attr_t * attr,
                     void * (*start_routine) (void*),
                     void * arg)
{
  PACE_TRACE("pace_pthread_create");

  return pthread_create (thread,
                         attr,
                         start_routine,
                         arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_detach (pace_pthread_t thread)
{
  PACE_TRACE("pace_pthread_detach");

  return pthread_detach (thread);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_equal (pace_pthread_t t1, pace_pthread_t t2)
{
  PACE_TRACE("pace_pthread_equal");

  if (pacevx_pthread_verify(t1) && pacevx_pthread_verify(t2))
    {
      if (t1->tid != t2->tid)
        return 0;
    }

  return 1;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_exit (void * value_ptr)
{
  /*
   * Normal thread exit. All the cleanup routines will be popped, if any.
   * If the thread is detached, free the storage; otherwise wait for join.
   */
  pace_pthread_t pthread;

  PACE_TRACE("pace_pthread_exit");

  if ((pthread = pace_pthread_self()) != NULL)
    {
      pacevx_pthread_proc_exit(pthread, value_ptr);
 
      exit(0);
    }

  exit(-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_getschedparam (pace_pthread_t thread,
                            int * policy,
                            pace_sched_param * param)
{
  PACE_TRACE("pace_pthread_getschedparam");

  return pthread_getschedparam (thread,
                                policy,
                                param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_pthread_getspecific (pace_pthread_key_t key)
{
  PACE_TRACE("pace_pthread_getspecific");

  return pthread_getspecific (key);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_join (pace_pthread_t thread, void ** value_ptr)
{
  PACE_TRACE("pace_pthread_join");

  return pthread_join (thread,
                       value_ptr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_key_create (pace_pthread_key_t * key,
                         void (*destructor)(void*))
{
  PACE_TRACE("pace_pthread_key_create");

  return pthread_key_create (key,
                             destructor);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_key_delete (pace_pthread_key_t key)
{
  PACE_TRACE("pace_pthread_key_delete");

  return pthread_key_delete (key);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_kill (pace_pthread_t thread, int sig)
{
  PACE_TRACE("pace_pthread_kill");

  if (pacevx_pthread_verify(thread))
    return kill(thread->tid, sig);
  else
    return EINVAL;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_destroy (pace_pthread_mutex_t * mutex)
{
  PACE_TRACE("pace_pthread_mutex_destroy");

  if (*mutex != NULL)
    return semDelete(*mutex);

  return EINVAL;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_getprioceiling (pace_pthread_mutex_t * mutex,
                                   int * prioceiling)
{
  PACE_TRACE("pace_pthread_mutex_getprioceiling");

  /* 
   * Mutex priority is not supported in VxWorks;
   * it depends on the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  *prioceiling = SCHED_RR_HIGH_PRI;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_init (pace_pthread_mutex_t * mutex,
                         const pace_pthread_mutexattr_t * attr)
{
  PACE_TRACE("pace_pthread_mutex_init");

  return pthread_mutex_init (mutex,
                             attr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_lock (pace_pthread_mutex_t * mutex)
{
  PACE_TRACE("pace_pthread_mutex_lock");

  return pthread_mutex_lock (mutex);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_setprioceiling (pace_pthread_mutex_t * mutex,
                                   int prioceiling,
                                   int * old_ceiling)
{
  PACE_TRACE("pace_pthread_mutex_setprioceiling");

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
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_trylock (pace_pthread_mutex_t * mutex)
{
  PACE_TRACE("pace_pthread_mutex_trylock");

  return pthread_mutex_trylock (mutex);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_unlock (pace_pthread_mutex_t * mutex)
{
  PACE_TRACE("pace_pthread_mutex_unlock");

  return pthread_mutex_unlock (mutex);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_destroy (pace_pthread_mutexattr_t * attr)
{
  PACE_TRACE("pace_pthread_mutexattr_destroy");

  free(*attr);
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprioceiling (pace_pthread_mutexattr_t * attr,
                                       int * prioceiling)
{
  PACE_TRACE("pace_pthread_mutexattr_getprioceiling");

  /* 
   * Mutex priority is not supported in VxWorks, 
   * it depends the thread using it and has the same priority
   * ceiling as this thread (SCHED_RR_HIGH_PRI).
   */
  *prioceiling = SCHED_RR_HIGH_PRI;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprotocol (const pace_pthread_mutexattr_t * attr,
                                    int * protocol)
{
  PACE_TRACE("pace_pthread_mutexattr_getprotocol");

/*
 * Does not support PTHREAD_PRIO_PROTECT for VxWorks
 */
  if (attr == 0)
    return EINVAL;

  *protocol = (*attr)->protocol;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setprioceiling (pace_pthread_mutexattr_t * attr,
                                       int prioceiling)
{
  PACE_TRACE("pace_pthread_mutexattr_setprioceiling");

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
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setprotocol (pace_pthread_mutexattr_t * attr,
                                    int protocol)
{
  PACE_TRACE("pace_pthread_mutexattr_setprotocol");

  return pthread_mutexattr_setprotocol (attr,
                                        protocol);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getpshared (const pace_pthread_mutexattr_t * attr,
                                   int * pshared)
{
  PACE_TRACE("pace_pthread_mutexattr_getpshared");

  /*
   * Only supports PTHREAD_PROCESS_SHARED 
   */
  *pshared = PTHREAD_PROCESS_SHARED;
  return OK;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_init (pace_pthread_mutexattr_t * attr)
{
  PACE_TRACE("pace_pthread_mutexattr_init");

  return pthread_mutexattr_init (attr);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setpshared (pace_pthread_mutexattr_t * attr,
                                   int pshared)
{
  PACE_TRACE("pace_pthread_mutexattr_setpshared");

  return pthread_mutexattr_setpshared (attr,
                                       pshared);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_once (pace_pthread_once_t * once_control,
                   void (*void_routine) (void))
{
  PACE_TRACE("pace_pthread_once");

  return pthread_once (once_control,
                       void_routine);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_pthread_t
pace_pthread_self ()
{
  PACE_TRACE("pace_pthread_self");

  return pthread_self();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setcancelstate (int state,
                             int * oldstate)
{
  PACE_TRACE("pace_pthread_setcancelstate");

  return pthread_setcancelstate (state,
                                 oldstate);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setcanceltype (int type,
                            int * oldtype)
{
  PACE_TRACE("pace_pthread_setcanceltype");

  return pthread_setcanceltype (type,
                                oldtype);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setschedparam (pace_pthread_t thread,
                            int policy,
                            const pace_sched_param * param)
{
  PACE_TRACE("pace_pthread_setschedparam");

  return pthread_setschedparam (thread,
                                policy,
                                param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setspecific (pace_pthread_key_t key,
                          const void * value)
{
  PACE_TRACE("pace_pthread_setspecific");

  return pthread_setspecific (key,
                              value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_sigmask (int how,
                      const pace_sigset_t * set,
                      pace_sigset_t * oset)
{
  PACE_TRACE("pace_pthread_sigmask");

  return pthread_sigmask (how,
                          set,
                          oset);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_testcancel ()
{
  PACE_TRACE("pace_pthread_testcancel");

  /* Do nothing per ACE (i.e., ACE_OS::thr_testcancel). */
  return;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
