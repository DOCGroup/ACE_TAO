/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pthread.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_PTHREAD_H
#define PACE_PTHREAD_H


#include "pace/defines.h"
#include "pace/sys/types.h"
#include "pace/signal.h"
#include <pthread.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_pthread_atfork (void  (*prepare) (),
                                       void (*parent) (),
                                       void (*child) ());

  PACE_INLINE int pace_pthread_attr_destroy (pthread_attr_t * attr);

  PACE_INLINE int pace_pthread_attr_getdetachstate (const pthread_attr_t * attr,
                                                    int * detachstate);

  PACE_INLINE int pace_pthread_attr_getinheritsched (const pthread_attr_t * attr,
                                                     int * inheritsched);

  PACE_INLINE int pace_pthread_attr_getschedparam (const pthread_attr_t * attr,
                                                   struct sched_param * param);

  PACE_INLINE int pace_pthread_attr_getschedpolicy (const pthread_attr_t * attr,
                                                    int * policy);

  PACE_INLINE int pace_pthread_attr_getscope (const pthread_attr_t * attr,
                                              int * contentionscope);

  PACE_INLINE int pace_pthread_attr_getstackaddr (const pthread_attr_t * attr,
                                                  void ** stackaddr);

  PACE_INLINE int pace_pthread_attr_getstacksize (const pthread_attr_t * attr,
                                                  size_t * stacksize);

  PACE_INLINE int pace_pthread_attr_init (pthread_attr_t * attr);

  PACE_INLINE int pace_pthread_attr_setdetachstate (pthread_attr_t * attr,
                                                    int detachstate);

  PACE_INLINE int pace_pthread_attr_setinheritsched (pthread_attr_t * attr,
                                                     int inheritsched);

  PACE_INLINE int pace_pthread_attr_setschedparam (pthread_attr_t * attr,
                                                   const struct
                                                   sched_param * param);

  PACE_INLINE int pace_pthread_attr_setschedpolicy (pthread_attr_t * attr,
                                                    int policy);

  PACE_INLINE int pace_pthread_attr_setscope (pthread_attr_t * attr,
                                              int contentionscope);

  PACE_INLINE int pace_pthread_attr_setstackaddr (pthread_attr_t * attr,
                                                  void * stackaddr);

  PACE_INLINE int pace_pthread_attr_setstacksize (pthread_attr_t * attr,
                                                  size_t stacksize);

  PACE_INLINE int pace_pthread_cancel (pthread_t thread);

  PACE_INLINE void pace_cleanup_pop (int execute);

  PACE_INLINE void pace_pthread_cleanup_push (void (*routine)(void*), void * arg);

  # define PACE_PTHREAD_COND_INITIALIZER PTHREAD_COND_INITIALIZER

  PACE_INLINE int pace_pthread_cond_broadcast (pthread_cond_t * cond);

  PACE_INLINE int pace_pthread_cond_destroy (pthread_cond_t * cond);

  PACE_INLINE int pace_pthread_cond_init (pthread_cond_t * cond,
                                          const pthread_condattr_t * attr);

  PACE_INLINE int pace_pthread_cond_signal (pthread_cond_t * cond);

  PACE_INLINE int pace_pthread_cond_timedwait (pthread_cond_t * cond,
                                               pthread_mutex_t * mutex,
                                               const struct timespec * abstime);

  PACE_INLINE int pace_pthread_cond_wait (pthread_cond_t * cond,
                                          pthread_mutex_t * mutex);

  PACE_INLINE int pace_pthread_condattr_destroy (pthread_condattr_t * attr);

  PACE_INLINE int pace_pthread_condattr_getpshared (const pthread_condattr_t * attr,
                                                    int * pshared);

  PACE_INLINE int pace_pthead_condattr_init (pthread_condattr_t * attr);

  PACE_INLINE int pace_pthread_condattr_setpshared (pthread_condattr_t * attr,
                                                    int pshared);

  PACE_INLINE int pace_pthread_create (pthread_t * thread,
                                       const pthread_attr_t * attr,
                                       void * (*start_routine) (void*),
                                       void * arg);

  PACE_INLINE int pace_pthread_detach (pthread_t thread);

  PACE_INLINE int pace_pthread_equal (pthread_t t1, pthread_t t2);

  PACE_INLINE void pace_pthread_exit (void * value_ptr);

  PACE_INLINE int pace_pthread_getschedparam (pthread_t thread,
                                              int * policy,
                                              struct sched_param * param);

  PACE_INLINE void * pace_pthread_getspecific (pthread_key_t key);

  PACE_INLINE int pace_pthread_join (pthread_t thread, void ** value_ptr);

  PACE_INLINE int pace_pthread_key_create (pthread_key_t * key,
                                           void (*destructor)(void*));

  PACE_INLINE int pace_pthread_key_delete (pthread_key_t key);

  PACE_INLINE int pace_pthread_kill (pthread_t thread, int sig);

  # define PACE_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

  PACE_INLINE int pace_pthread_mutex_destroy (pthread_mutex_t * mutex);

  PACE_INLINE int pace_pthread_mutex_getprioceiling (pthread_mutex_t * mutex,
                                                     int * prioceiling);

  PACE_INLINE int pace_pthread_mutex_init (pthread_mutex_t * mutex,
                                           const pthread_mutexattr_t * attr);

  PACE_INLINE int pace_pthread_mutex_lock (pthread_mutex_t * mutex);

  PACE_INLINE int pace_pthread_mutex_setprioceiling (pthread_mutex_t * mutex,
                                                     int prioceiling,
                                                     int * old_ceiling);

  PACE_INLINE int pace_pthread_mutex_trylock (pthread_mutex_t * mutex);

  PACE_INLINE int pace_pthread_mutex_unlock (pthread_mutex_t * mutex);

  PACE_INLINE int pace_pthread_mutexattr_destroy (pthread_mutexattr_t * sttr);

  PACE_INLINE int pace_pthread_mutexattr_getprioceiling (pthread_mutexattr_t * attr,
                                                         int * prioceiling);

  PACE_INLINE int pace_pthread_mutexattr_getprotocol
                  (const pthread_mutexattr_t * attr, int * protocol);

  PACE_INLINE int pace_pthread_mutexattr_setprioceiling (pthread_mutexattr_t * attr,
                                                         int prioceiling);

  PACE_INLINE int pace_pthread_mutexattr_setprotocol (pthread_mutexattr_t * attr,
                                                      int protocol);
  PACE_INLINE int
  pace_pthread_mutexattr_getpshared (const pthread_mutexattr_t * attr,
                                     int * pshared);

  PACE_INLINE int pace_pthread_mutexattr_init (pthread_mutexattr_t * attr);

  PACE_INLINE int pace_pthread_mutexattr_setpshared (pthread_mutexattr_t * attr,
                                                     int pshared);

  PACE_INLINE int pace_pthread_once (pthread_once_t * once_control,
                                     void (*void_routine) ());

  # define PACE_PTHREAD_ONCE_INIT PTHREAD_ONCE_INIT

  PACE_INLINE pthread_t pace_pthread_self ();

  PACE_INLINE int pace_pthread_setcancelstate (int state, int * oldstate);

  PACE_INLINE int pace_pthread_setcanceltype (int type, int * oldtype);

  PACE_INLINE int pace_pthread_setschedparam (pthread_t thread,
                                              int policy,
                                              const struct sched_param * param);

  PACE_INLINE int pace_pthread_setspecific (pthread_key_t key, const void * value);

  PACE_INLINE int pace_pthread_sigmask (int how,
                                        const sigset_t * set,
                                        sigset_t * oset);

  PACE_INLINE void pace_pthread_testcancel ();

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/pthread.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_PTHREAD_H */
