/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/pthread.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PTHREAD_H
#define PACE_PTHREAD_H

#include "pace/sys/types.h"
#include "pace/signal.h"
#include <pthread.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function pthread_atfork.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.3.
   */
  PACE_INLINE int pace_pthread_atfork (void  (*prepare) (),
                                       void (*parent) (),
                                       void (*child) ());

  /**
     PACE's implementation of the POSIX function pthread_attr_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_destroy (pace_pthread_attr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_attr_getdetachstate.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
                                                    int * detachstate);

  /**
     PACE's implementation of the POSIX function pthread_attr_getinheritsched.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
                                                     int * inheritsched);

  /**
     PACE's implementation of the POSIX function pthread_attr_getschedparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
                                                   struct sched_param * param);

  /**
     PACE's implementation of the POSIX function pthread_attr_getschedpolicy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
                                                    int * policy);

  /**
     PACE's implementation of the POSIX function pthread_attr_getscope.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_getscope (const pace_pthread_attr_t * attr,
                                              int * contentionscope);

  /**
     PACE's implementation of the POSIX function pthread_attr_getstackaddr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
                                                  void ** stackaddr);

  /**
     PACE's implementation of the POSIX function pthread_attr_getstacksize.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
                                                  size_t * stacksize);

  /**
     PACE's implementation of the POSIX function pthread_attr_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_init (pace_pthread_attr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_attr_setdetachstate.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
                                                    int detachstate);

  /**
     PACE's implementation of the POSIX function pthread_attr_setinheritsched.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
                                                     int inheritsched);

  /**
     PACE's implementation of the POSIX function pthread_attr_setschedparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_setschedparam (pace_pthread_attr_t * attr,
                                                   const struct
                                                   sched_param * param);

  /**
     PACE's implementation of the POSIX function pthread_attr_setschedpolicy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
                                                    int policy);

  /**
     PACE's implementation of the POSIX function pthread_attr_setscope.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.1.
   */
  PACE_INLINE int pace_pthread_attr_setscope (pace_pthread_attr_t * attr,
                                              int contentionscope);

  /**
     PACE's implementation of the POSIX function pthread_attr_setstackaddr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
                                                  void * stackaddr);

  /**
     PACE's implementation of the POSIX function pthread_attr_setstacksize.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.1.
   */
  PACE_INLINE int pace_pthread_attr_setstacksize (pace_pthread_attr_t * attr,
                                                  size_t stacksize);

  /**
     PACE's implementation of the POSIX function pthread_cancel.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.1.
   */
  PACE_INLINE int pace_pthread_cancel (pace_pthread_t thread);

  /**
     PACE's implementation of the POSIX function pthread_cleanup_push.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.3.
   */
# define pace_pthread_cleanup_push(routine, arg) \
    pthread_cleanup_push (routine, arg)

  /**
     PACE's implementation of the POSIX function pthread_cleanup_pop.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.3.
   */
# define pace_pthread_cleanup_pop(execute) \
    pthread_cleanup_pop (execute)

# define PACE_PTHREAD_COND_INITIALIZER PTHREAD_COND_INITIALIZER

  /**
     PACE's implementation of the POSIX function pthread_cond_broadcast.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.3.
   */
  PACE_INLINE int pace_pthread_cond_broadcast (pace_pthread_cond_t * cond);

  /**
     PACE's implementation of the POSIX function pthread_cond_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.2.
   */
  PACE_INLINE int pace_pthread_cond_destroy (pace_pthread_cond_t * cond);

  /**
     PACE's implementation of the POSIX function pthread_cond_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.2.
   */
  PACE_INLINE int pace_pthread_cond_init (pace_pthread_cond_t * cond,
                                          const pace_pthread_condattr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_cond_signal.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.3.
   */
  PACE_INLINE int pace_pthread_cond_signal (pace_pthread_cond_t * cond);

  /**
     PACE's implementation of the POSIX function pthread_cond_timedwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.4.
   */
  PACE_INLINE int pace_pthread_cond_timedwait (pthread_cond_t * cond,
                                               pace_pthread_mutex_t * mutex,
                                               const struct timespec * abstime);

  /**
     PACE's implementation of the POSIX function pthread_cond_wait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.4.
   */
  PACE_INLINE int pace_pthread_cond_wait (pace_pthread_cond_t * cond,
                                          pace_pthread_mutex_t * mutex);

  /**
     PACE's implementation of the POSIX function pthread_condattr_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.1.
   */
  PACE_INLINE int pace_pthread_condattr_destroy (pace_pthread_condattr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_condattr_getpshared.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.1.
   */
  PACE_INLINE int pace_pthread_condattr_getpshared (const pace_pthread_condattr_t * attr,
                                                    int * pshared);

  /**
     PACE's implementation of the POSIX function pthread_condattr_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.1.
   */
  PACE_INLINE int pace_pthread_condattr_init (pace_pthread_condattr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_condattr_setpshared.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.4.1.
   */
  PACE_INLINE int pace_pthread_condattr_setpshared (pace_pthread_condattr_t * attr,
                                                    int pshared);

  /**
     PACE's implementation of the POSIX function pthread_create.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.2.
   */
  PACE_INLINE int pace_pthread_create (pace_pthread_t * thread,
                                       const pace_pthread_attr_t * attr,
                                       void * (*start_routine) (void*),
                                       void * arg);

  /**
     PACE's implementation of the POSIX function pthread_detach.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.4.
   */
  PACE_INLINE int pace_pthread_detach (pace_pthread_t thread);

  /**
     PACE's implementation of the POSIX function pthread_equal.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.7.
   */
  PACE_INLINE int pace_pthread_equal (pace_pthread_t t1, pthread_t t2);

  /**
     PACE's implementation of the POSIX function pthread_exit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.5.
   */
  PACE_INLINE void pace_pthread_exit (void * value_ptr);

  /**
     PACE's implementation of the POSIX function pthread_getschedparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.2.5.
   */
  PACE_INLINE int pace_pthread_getschedparam (pace_pthread_t thread,
                                              int * policy,
                                              struct sched_param * param);

  /**
     PACE's implementation of the POSIX function pthread_getspecific.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 17.1.2.
   */
  PACE_INLINE void * pace_pthread_getspecific (pace_pthread_key_t key);

  /**
     PACE's implementation of the POSIX function pthread_join.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.3.
   */
  PACE_INLINE int pace_pthread_join (pace_pthread_t thread, void ** value_ptr);

  /**
     PACE's implementation of the POSIX function pthread_key_create.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 17.1.1.
   */
  PACE_INLINE int pace_pthread_key_create (pace_pthread_key_t * key,
                                           void (*destructor)(void*));

  /**
     PACE's implementation of the POSIX function pthread_key_delete.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 17.1.3.
   */
  PACE_INLINE int pace_pthread_key_delete (pace_pthread_key_t key);

  /**
     PACE's implementation of the POSIX function pthread_kill.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.10.
   */
  PACE_INLINE int pace_pthread_kill (pace_pthread_t thread, int sig);

  # define PACE_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

  /**
     PACE's implementation of the POSIX function pthread_mutex_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.2.
   */
  PACE_INLINE int pace_pthread_mutex_destroy (pace_pthread_mutex_t * mutex);

  /**
     PACE's implementation of the POSIX function pthread_mutex_getprioceiling.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.2.
   */
  PACE_INLINE int pace_pthread_mutex_getprioceiling (pace_pthread_mutex_t * mutex,
                                                     int * prioceiling);

  /**
     PACE's implementation of the POSIX function pthread_mutex_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.2.
   */
  PACE_INLINE int pace_pthread_mutex_init (pace_pthread_mutex_t * mutex,
                                           const pace_pthread_mutexattr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_mutex_lock.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.3.
   */
  PACE_INLINE int pace_pthread_mutex_lock (pace_pthread_mutex_t * mutex);

  /**
     PACE's implementation of the POSIX function pthread_mutex_setprioceiling.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.2.
   */
  PACE_INLINE int pace_pthread_mutex_setprioceiling (pace_pthread_mutex_t * mutex,
                                                     int prioceiling,
                                                     int * old_ceiling);

  /**
     PACE's implementation of the POSIX function pthread_mutex_trylock.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.3.
   */
  PACE_INLINE int pace_pthread_mutex_trylock (pthread_mutex_t * mutex);

  /**
     PACE's implementation of the POSIX function pthread_mutex_unlock.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.3.
   */
  PACE_INLINE int pace_pthread_mutex_unlock (pace_pthread_mutex_t * mutex);

  /**
     PACE's implementation of the POSIX function pthread_mutexattr_destroy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_destroy (pace_pthread_mutexattr_t * sttr);

  /**
     PACE's implementation of the POSIX function
     pthread_mutexattr_getprioceiling.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_getprioceiling (pace_pthread_mutexattr_t * attr,
                                                         int * prioceiling);

  /**
     PACE's implementation of the POSIX function pthread_mutexattr_getprotocol.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_getprotocol
                  (const pace_pthread_mutexattr_t * attr, int * protocol);

  /**
     PACE's implementation of the POSIX function
     pthread_mutexattr_setprioceiling.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_setprioceiling (pace_pthread_mutexattr_t * attr,
                                                         int prioceiling);

  /**
     PACE's implementation of the POSIX function pthread_mutexattr_setprotocol.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.6.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_setprotocol (pace_pthread_mutexattr_t * attr,
                                                      int protocol);
  /**
     PACE's implementation of the POSIX function pthread_mutexattr_getpshared.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.1.
   */
  PACE_INLINE int
  pace_pthread_mutexattr_getpshared (const pace_pthread_mutexattr_t * attr,
                                     int * pshared);

  /**
     PACE's implementation of the POSIX function pthread_mutexattr_init.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_init (pace_pthread_mutexattr_t * attr);

  /**
     PACE's implementation of the POSIX function pthread_mutexattr_setpshared.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 11.3.1.
   */
  PACE_INLINE int pace_pthread_mutexattr_setpshared (pace_pthread_mutexattr_t * attr,
                                                     int pshared);

  /**
     PACE's implementation of the POSIX function pthread_once.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.8.
   */
  PACE_INLINE int pace_pthread_once (pace_pthread_once_t * once_control,
                                     void (*void_routine) ());

# define PACE_PTHREAD_ONCE_INIT PTHREAD_ONCE_INIT

  /**
     PACE's implementation of the POSIX function pthread_self.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 16.2.6.
   */
  PACE_INLINE pace_pthread_t pace_pthread_self ();

  /**
     PACE's implementation of the POSIX function pthread_setcancelstate.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.2.
   */
  PACE_INLINE int pace_pthread_setcancelstate (int state, int * oldstate);

  /**
     PACE's implementation of the POSIX function pthread_setcanceltype.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.2.
   */
  PACE_INLINE int pace_pthread_setcanceltype (int type, int * oldtype);

  /**
     PACE's implementation of the POSIX function pthread_setschedparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.5.2.
   */
  PACE_INLINE int pace_pthread_setschedparam (pace_pthread_t thread,
                                              int policy,
                                              const struct sched_param * param);

  /**
     PACE's implementation of the POSIX function pthread_setspecific.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 17.1.2.
   */
  PACE_INLINE int pace_pthread_setspecific (pace_pthread_key_t key, const void * value);

  /**
     PACE's implementation of the POSIX function pthread_sigmask.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.5.
   */
  PACE_INLINE int pace_pthread_sigmask (int how,
                                        const sigset_t * set,
                                        sigset_t * oset);

  /**
     PACE's implementation of the POSIX function pthread_testcancel.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 18.2.2.
   */
  PACE_INLINE void pace_pthread_testcancel ();

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/pthread.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_PTHREAD_H */
