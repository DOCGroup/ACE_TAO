/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/pthread.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/unistd.h"

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
int
pace_pthread_atfork (pace_atfork_pf prepare,
                     pace_atfork_pf parent,
                     pace_atfork_pf child)
{
  return pthread_atfork (prepare, parent, child);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
int
pace_pthread_atfork (void (*prepare) (),
                     void (*parent) (),
                     void (*child) ())
{
  return pthread_atfork (prepare, parent, child);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_destroy (pace_pthread_attr_t * attr)
{
  return pthread_attr_destroy (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
                                  int * detachstate)
{
  return pthread_attr_getdetachstate (attr, detachstate);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
                                   int * inheritsched)
{
#if PACE_LYNXOS == 0x300
  return pthread_attr_getinheritsched (PACE_NONCONST_ARG_CAST (pace_pthread_attr_t *) attr);
#else
  return pthread_attr_getinheritsched (PACE_NONCONST_ARG_CAST (pace_pthread_attr_t *) attr,
                                       inheritsched);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
                                 pace_sched_param * param)
{
  return pthread_attr_getschedparam (attr, param);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
                                  int * policy)
{
  return pthread_attr_getschedpolicy (attr, policy);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getscope (const pace_pthread_attr_t * attr,
                            int * contentionscope)
{
  return pthread_attr_getscope (attr, contentionscope);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
                                void ** stackaddr)
{
  return pthread_attr_getstackaddr (attr, stackaddr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
                                size_t * stacksize)
{
#if PACE_LYNXOS == 0x300
  return pthread_attr_getstacksize (PACE_NONCONST_ARG_CAST (pace_pthread_attr_t *) attr);
#else
  return pthread_attr_getstacksize (PACE_NONCONST_ARG_CAST (pace_pthread_attr_t *) attr,
                                    stacksize);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_init (pace_pthread_attr_t * attr)
{
  return pthread_attr_init (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
                                  int detachstate)
{
  return pthread_attr_setdetachstate (attr, detachstate);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
                                   int inheritsched)
{
  return pthread_attr_setinheritsched (attr, inheritsched);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedparam (pace_pthread_attr_t * attr,
                                 const pace_sched_param * param)
{
  return pthread_attr_setschedparam (attr, param);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
                                  int policy)
{
  return pthread_attr_setschedpolicy (attr, policy);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setscope (pace_pthread_attr_t * attr,
                            int contentionscope)
{
  return pthread_attr_setscope (attr, contentionscope);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
                                void * stackaddr)
{
  return pthread_attr_setstackaddr (attr, stackaddr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_attr_setstacksize (pace_pthread_attr_t * attr,
                                size_t stacksize)
{
  return pthread_attr_setstacksize (attr, stacksize);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cancel (pace_pthread_t thread)
{
  return pthread_cancel (thread);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_broadcast (pace_pthread_cond_t * cond)
{
  return pthread_cond_broadcast (cond);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_destroy (pace_pthread_cond_t * cond)
{
  return pthread_cond_destroy (cond);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_init (pace_pthread_cond_t * cond,
                        const pace_pthread_condattr_t * attr)
{
#if PACE_LYNXOS == 0x300
  return pthread_cond_init (cond, *attr);
#else
  return pthread_cond_init (cond, attr);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_signal (pace_pthread_cond_t * cond)
{
  return pthread_cond_signal (cond);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_timedwait (pthread_cond_t * cond,
                             pace_pthread_mutex_t * mutex,
                             const pace_timespec * abstime)
{
  return pthread_cond_timedwait (cond, mutex, PACE_NONCONST_ARG_CAST (struct timespec *) abstime);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_cond_wait (pace_pthread_cond_t * cond,
                        pace_pthread_mutex_t * mutex)
{
  return pthread_cond_wait (cond, mutex);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_destroy (pace_pthread_condattr_t * attr)
{
  return pthread_condattr_destroy (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_getpshared (const pace_pthread_condattr_t * attr,
                                  int * pshared)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (pshared);
  return -1;
#else
  return pthread_condattr_getpshared (PACE_NONCONST_ARG_CAST (pace_pthread_condattr_t *) attr, pshared);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_init (pace_pthread_condattr_t * attr)
{
  return pthread_condattr_init (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_condattr_setpshared (pace_pthread_condattr_t * attr,
                                 int pshared)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (pshared);
  return -1;
#else
  return pthread_condattr_setpshared (attr, pshared);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
int
pace_pthread_create (pace_pthread_t * thread,
                     const pace_pthread_attr_t * attr,
                     pace_create_pf start_routine,
                     void * arg)
{
  return pthread_create (thread, attr, start_routine, arg);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
int
pace_pthread_create (pace_pthread_t * thread,
                     const pace_pthread_attr_t * attr,
                     void * (*start_routine) (void*),
                     void * arg)
{
#if PACE_LYNXOS == 0x300
  return pthread_create (thread, *attr, start_routine, arg);
#else
  return pthread_create (thread, attr, start_routine, arg);
#endif /* PACE_LYNXOS != 0x300 */
}
# endif /*! PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_detach (pace_pthread_t thread)
{
#if PACE_LYNXOS == 0x300
  return pthread_detach (&thread);
#else
  return pthread_detach (thread);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_equal (pace_pthread_t t1, pthread_t t2)
{
  return pthread_equal (t1, t2);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_exit (void * value_ptr)
{
  pthread_exit (value_ptr);
  return;
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_getschedparam (pace_pthread_t thread,
                            int * policy,
                            pace_sched_param * param)
{
  return pthread_getschedparam (thread, policy, param);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_pthread_getspecific (pace_pthread_key_t key)
{
#if PACE_LYNXOS == 0x300
  void *value;
  if (pthread_getspecific (key, &value) != 0)
    return 0;

  return value;
#else
  return pthread_getspecific (key);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_join (pace_pthread_t thread, void ** value_ptr)
{
  return pthread_join (thread, value_ptr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
int
pace_pthread_key_create (pace_pthread_key_t * key,
                         pace_keycreate_pf destructor)
{
  return pthread_key_create (key, destructor);
}
#else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
int
pace_pthread_key_create (pace_pthread_key_t * key,
                         void (*destructor)(void*))
{
  return pthread_key_create (key, destructor);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_key_delete (pace_pthread_key_t key)
{
  return pthread_key_delete (key);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_kill (pace_pthread_t thread, int sig)
{
  return pthread_kill (thread, sig);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_destroy (pace_pthread_mutex_t * mutex)
{
  return pthread_mutex_destroy (mutex);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_getprioceiling (pace_pthread_mutex_t * mutex,
                                   int * prioceiling)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (mutex);
  PACE_UNUSED_ARG (prioceiling);
  return -1;
#elif PACE_LYNXOS
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#else  /* ! PACE_LYNXOS */
  return pthread_mutex_getprioceiling (mutex, prioceiling);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_init (pace_pthread_mutex_t * mutex,
                         const pace_pthread_mutexattr_t * attr)
{
#if PACE_LYNXOS == 0x300
  return pthread_mutex_init (mutex, *attr);
#else
  return pthread_mutex_init (mutex, attr);
#endif /* PACE_LYNXOS != 0x300 */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_lock (pace_pthread_mutex_t * mutex)
{
  return pthread_mutex_lock (mutex);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_setprioceiling (pace_pthread_mutex_t * mutex,
                                   int prioceiling,
                                   int * old_ceiling)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (mutex);
  PACE_UNUSED_ARG (prioceiling);
  PACE_UNUSED_ARG (old_ceiling);
  return -1;
#elif PACE_LYNXOS
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#else  /* ! PACE_LYNXOS */
  return pthread_mutex_setprioceiling (mutex, prioceiling, old_ceiling);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_trylock (pthread_mutex_t * mutex)
{
  return pthread_mutex_trylock (mutex);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutex_unlock (pace_pthread_mutex_t * mutex)
{
  return pthread_mutex_unlock (mutex);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_destroy (pace_pthread_mutexattr_t * attr)
{
  return pthread_mutexattr_destroy (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprioceiling (pace_pthread_mutexattr_t * attr,
                                       int * prioceiling)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (prioceiling);
  return -1;
#else
  return pthread_mutexattr_getprioceiling (attr, prioceiling);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getprotocol (const pace_pthread_mutexattr_t * attr,
                                    int * protocol)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (protocol);
  return -1;
#else
  return pthread_mutexattr_getprotocol (PACE_NONCONST_ARG_CAST (pace_pthread_mutexattr_t *) attr, protocol);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setprioceiling (pace_pthread_mutexattr_t * attr,
                                       int prioceiling)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (prioceiling);
  return -1;
#else
  return pthread_mutexattr_setprioceiling (attr, prioceiling);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setprotocol (pace_pthread_mutexattr_t * attr,
                                    int protocol)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (protocol);
  return -1;
#else
  return pthread_mutexattr_setprotocol (attr, protocol);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_getpshared (const pace_pthread_mutexattr_t * attr,
                                   int * pshared)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (pshared);
  return -1;
#else
  return pthread_mutexattr_getpshared (PACE_NONCONST_ARG_CAST (pace_pthread_mutexattr_t *) attr, pshared);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_init (pace_pthread_mutexattr_t * attr)
{
  return pthread_mutexattr_init (attr);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_mutexattr_setpshared (pace_pthread_mutexattr_t * attr,
                                   int pshared)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (attr);
  PACE_UNUSED_ARG (pshared);
  return -1;
#else
  return pthread_mutexattr_setpshared (attr, pshared);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
# if defined (PACE_HAS_CPLUSPLUS)
PACE_INLINE
int
pace_pthread_once (pace_pthread_once_t * once_control,
                   pace_once_pf void_routine)
{
  return pthread_once (once_control, void_routine);
}
# else /* ! PACE_HAS_CPLUSPLUS */
PACE_INLINE
int
pace_pthread_once (pace_pthread_once_t * once_control,
                   void (*void_routine) (void))
{
  return pthread_once (once_control, void_routine);
}
# endif /* PACE_HAS_CPLUSPLUS */
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_pthread_t
pace_pthread_self ()
{
  return pthread_self ();
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setcancelstate (int state, int * oldstate)
{
  return pthread_setcancelstate (state, oldstate);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setcanceltype (int type, int * oldtype)
{
  return pthread_setcanceltype (type, oldtype);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setschedparam (pace_pthread_t thread,
                            int policy,
                            const pace_sched_param * param)
{
  return pthread_setschedparam (thread, policy, param);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_setspecific (pace_pthread_key_t key, const void * value)
{
  return pthread_setspecific (key, PACE_NONCONST_ARG_CAST (void *) value);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_pthread_sigmask (int how, const sigset_t * set,
                      sigset_t * oset)
{
  return pthread_sigmask (how, set, oset);
}
#endif /* PACE_HAS_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
pace_pthread_testcancel ()
{
  pthread_testcancel ();
  return;
}
#endif /* PACE_HAS_NONUOF_FUNCS */
