/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pthreadtypes.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

/* Keep pthread types defined here as they just clutter up sys/types.h

 * Do NOT include this file directly!! Please include "sys/types.h". */

#ifndef PACE_SYS_PTHREADTYPES_H_WIN32
#define PACE_SYS_PTHREADTYPES_H_WIN32

#include "pace/win32/schedtypes.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_PTHREAD_ATTR_T
#define PACE_PTHREAD_ATTR_T 1
  typedef struct pthread_attr_t
  {
    int init_;
    int detach_state_;
    int policy_;
    pace_sched_param sparam_;
    int inherit_sched_;
    int contention_scope_;
    pace_size_t guard_size_;
    int stackaddr_set_ ;
    void * stack_addr_;
    pace_size_t stack_size_;
  } pace_pthread_attr_t;
#endif /* PACE_PTHREAD_ATTR_T */

#ifndef PACE_PTHREAD_COND_T
#define PACE_PTHREAD_COND_T 1
  typedef struct pthread_cond_t
  {
#if 0
    /* Concurrent access protection. */
    struct _pthread_fastlock __c_lock;

    /* Pool of threads waiting. */
    _pthread_descr __c_waiting;
#endif
    int a;

  } pace_pthread_cond_t;

#endif /* PACE_PTHREAD_COND_T */

#ifndef PACE_PTHREAD_CONDATTR_T
#define PACE_PTHREAD_CONDATTR_T 1
  typedef struct pthread_condattr_t
  {
    int not_used_;
  } pace_pthread_condattr_t;
#endif /* PACE_PTHREAD_CONDATTR_T */

#ifndef PACE_PTHREAD_KEY_T
#define PACE_PTHREAD_KEY_T 1
  typedef unsigned int pthread_key_t;
  typedef pthread_key_t pace_pthread_key_t;
#endif /* PACE_PTHREAD_KEY_T */

#ifndef PACE_PTHREAD_MUTEX_T
#define PACE_PTHREAD_MUTEX_T 1
  typedef struct pthread_mutex_t
  {
    /* Depth of recursive locking */
    int recursive_count_;

#if 0
    /* Owner thread (if recursive or errcheck) */
    _pthread_descr __m_owner;
#endif

    /* Mutex kind: fast, recursive or errcheck */
    int type_;

    union
    {
       HANDLE proc_mutex_;
       CRITICAL_SECTION thr_mutex_;
    };

#if 0
    /* Underlying fast lock */
    struct _pthread_fastlock __m_lock;
#endif

  } pace_pthread_mutex_t;
#endif /* PACE_PTHREAD_MUTEX_T */

#ifndef PACE_PTHREAD_MUTEXATTR_T
#define PACE_PTHREAD_MUTEXATTR_T
  typedef struct pthread_mutexattr_t
  {
    int mutex_kind_;
  } pace_pthread_mutexattr_t;
#endif /* PACE_PTHREAD_MUTEXATTR_T */

#ifndef PACE_PTHREAD_ONCE
#define PACE_PTHREAD_ONCE

  typedef int pthread_once_t;
  typedef pthread_once_t pace_pthread_once_t;

#endif /* PACE_PTHREAD_ONCE */

#ifndef PACE_PTHREAD_T
#define PACE_PTHREAD_T
  typedef HANDLE pace_pthread_t;
  typedef DWORD pace_idpthread_t;
#endif /* PACE_PTHREAD_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_PTHREADTYPES_H_WIN32 */
