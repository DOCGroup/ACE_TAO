// $Id$

#ifndef ACE_CONFIG_FSU_PTHREAD_H
#define ACE_CONFIG_FSU_PTHREAD_H

#define ACE_LACKS_CONST_TIMESPEC_PTR

// Threads
#define ACE_HAS_THREADS
#if !defined(ACE_MT_SAFE)
#define ACE_MT_SAFE 1
#endif

/*
** FSU implements 1003.4a draft 6 threads - the ACE_HAS_FSU_PTHREADS def
** is needed for some peculiarities with this particular implementation.
*/
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_DRAFT6
#define ACE_HAS_FSU_PTHREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_SIGWAIT
#define ACE_HAS_PTHREAD_YIELD_VOID_PTR
#define ACE_HAS_PTHREAD_ATTR_INIT
#define ACE_HAS_PTHREAD_ATTR_DESTROY
#define ACE_LACKS_THREAD_STACK_ADDR
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SETSCHED
#if defined(M_UNIX)
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#endif

#if !defined(ACE_HAS_POSIX_TIME)
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#endif

#include <pthread.h>

#if !defined(PTHREAD_STACK_MIN)
#define PTHREAD_STACK_MIN (1024*10)
#endif

#define ACE_LACKS_THREAD_PROCESS_SCOPING

#undef PTHREAD_INHERIT_SCHED

struct sched_param
{
  int sched_priority;
  int prio;
};

#endif /* ACE_CONFIG_FSU_PTHREAD_H */
