/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/pthread.h
 *
 * = AUTHOR
 *    Joe Hoffert. The *VAST* majority of the pthread code for VxWorks
 *    has been supplied by Hughes Network Systems via Braeton Taylor.
 *
 * ============================================================================ */

#ifndef PACE_PTHREAD_H_VXWORKS
#define PACE_PTHREAD_H_VXWORKS

#include <taskLib.h>
#include <taskHookLib.h>
#include <intLib.h>
#include <errnoLib.h>
#include "pace/sys/types.h"
#include "pace/signal.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_PTHREAD_CANCELED PTHREAD_CANCELED
#define PACE_PTHREAD_CANCEL_AYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS
#define PACE_PTHREAD_CANCEL_DEFERRED PTHREAD_CANCEL_DEFERRED
#define PACE_PTHREAD_CACEL_DISABLE PTHREAD_CANCEL_DISABLE
#define PACE_PTHREAD_CANCEL_ENABLE PTHREAD_CANCEL_ENABLE
#define PACE_PTHREAD_COND_INITIALIZER PTHREAD_COND_INITIALIZER
#define PACE_PTHREAD_CREATE_DETACHED PTHREAD_CREATE_DETACHED
#define PACE_PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_JOINABLE
#define PACE_PTHREAD_EXPLICIT_SCHED PTHREAD_EXPLICIT_SCHED
#define PACE_PTHREAD_INHERIT_SCHED PTHREAD_INHERIT_SCHED
#define PACE_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define PACE_PTHREAD_ONCE_INIT PTHREAD_ONCE_INIT
#define PACE_PTHREAD_PRIO_INHERIT PTHREAD_PRIO_INHERIT
#define PACE_PTHREAD_PRIO_NONE PTHREAD_PRIO_NONE
#define PACE_PTHREAD_PRIO_PROTECT PTHREAD_PRIO_PROTECT
#define PACE_PTHREAD_PROCESS_PRIVATE PTHREAD_PROCESS_PRIVATE
#define PACE_PTHREAD_PROCESS_SHARED PTHREAD_PROCESS_SHARED
#define PACE_PTHREAD_SCOPE_PROCESS PTHREAD_SCOPE_PROCESS
#define PACE_PTHREAD_SCOPE_SYSTEM PTHREAD_SCOPE_SYSTEM

#define PTHREAD_MUTEX_INITIALIZER 0
#define PTHREAD_COND_INITIALIZER 0

#ifndef PACE_SCHED_PARAM
#define PACE_SCHED_PARAM
  typedef struct sched_param pace_sched_param;
#endif /* PACE_SCHED_PARAM */

#if defined (PACE_HAS_CPLUSPLUS)
# ifndef PACE_ATFORK_PF
# define PACE_ATFORK_PF
  typedef void (*pace_atfork_pf) (void);
# endif /* PACE_ATFORK_PF */

# ifndef PACE_KEYCREATE_PF
# define PACE_KEYCREATE_PF
  typedef void (*pace_keycreate_pf) (void*);
# endif /* PACE_KEYCREATE_PF */

# ifndef PACE_ONCE_PF
# define PACE_ONCE_PF
  typedef void (*pace_once_pf) (void);
# endif /* PACE_ONCE_PF */

# ifndef PACE_CREATE_PF
# define PACE_CREATE_PF
  typedef void* (*pace_create_pf) (void*);
# endif /* PACE_CREATE_PF */

#endif /* PACE_HAS_CPLUPLUS */

  /*
   * VxWorks helper functions
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  extern pace_pthread_attr_t pthread_attr_default;

  STATUS pacevx_vxworks_init();
  void pacevx_pthread_proc_exit(pace_pthread_t pthread, void *value_ptr);
  int pacevx_pthread_verify(pace_pthread_t pthread);
  void pacevx_pthread_run_cleanup (WIND_TCB *pTcb);
  int pacevx_pthread_queue_add(pace_pthread_t pthread);
  int pacevx_pthread_queue_remove(pace_pthread_t pthread);
  void pacevx_pthread_cleanup_popall(pace_pthread_t thread);
  void pacevx_pthread_destructor_thread(pace_pthread_t pthread);
  int pacevx_pthread_key_validate(pace_pthread_key_t key);
  void pacevx_pthread_destructor_key(pace_pthread_key_t key, void * arg);

  int pthread_cond_timedwait (pace_pthread_cond_t * cond,
                              pace_pthread_mutex_t * mutex,
                              const pace_timespec * abstime);
  int pthread_cond_wait (pace_pthread_cond_t * cond,
                         pace_pthread_mutex_t * mutex);
# if defined (PACE_HAS_CPLUSPLUS)
  int pthread_create (pace_pthread_t * thread,
                      const pace_pthread_attr_t * attr,
                      pace_create_pf start_routine,
                      void * arg);
  int pthread_key_create (pace_pthread_key_t * key,
                          pace_keycreate_pf destructor);
  int pthread_once (pace_pthread_once_t * once_control,
                    pace_once_pf init_routine);
# else /* ! PACE_HAS_CPLUSPLUS */
  int pthread_create (pace_pthread_t * thread,
                      const pace_pthread_attr_t * attr,
                      void * (*start_routine) (void*),
                      void * arg);
  int pthread_key_create (pace_pthread_key_t * key,
                          void (*destructor)(void*));
  int pthread_once (pace_pthread_once_t * once_control,
                    void (*init_routine) (void));
# endif /*! PACE_HAS_CPLUSPLUS */
  int pthread_detach (pace_pthread_t thread);
  int pthread_join (pace_pthread_t thread, void ** value_ptr);
  int pthread_key_delete (pace_pthread_key_t key);
  int pthread_attr_init (pace_pthread_attr_t * attr);
  int pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
                                   int detachstate);
  int pthread_attr_setschedparam (pace_pthread_attr_t * attr,
                                  const pace_sched_param * param);
  int pthread_cancel (pace_pthread_t thread);
  int pthread_getschedparam (pace_pthread_t thread,
                             int * policy,
                             pace_sched_param * param);
  void * pthread_getspecific (pace_pthread_key_t key);
  int pthread_mutex_init (pace_pthread_mutex_t * mutex,
                          const pace_pthread_mutexattr_t * attr);
  int pthread_mutex_lock (pace_pthread_mutex_t * mutex);
  int pthread_mutex_trylock (pace_pthread_mutex_t * mutex);
  int pthread_mutex_unlock (pace_pthread_mutex_t * mutex);
  int pthread_mutexattr_setprotocol (pace_pthread_mutexattr_t * attr,
                                     int protocol);
  int pthread_mutexattr_init (pace_pthread_mutexattr_t * attr);
  int pthread_mutexattr_setpshared (pace_pthread_mutexattr_t * attr,
                                    int pshared);
  int pthread_setcancelstate (int state, int * oldstate);
  int pthread_setcanceltype (int type, int * oldtype);
  int pthread_setschedparam (pace_pthread_t thread,
                             int policy,
                             const pace_sched_param * param);
  int pthread_setspecific (pace_pthread_key_t key,
                           const void * value);
  int pthread_sigmask (int how,
                       const sigset_t * set,
                       sigset_t * oset);
  pace_pthread_t pthread_self ();
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_PTHREAD_H_VXWORKS */
