/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/pthread.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PTHREAD_H_WIN32
#define PACE_PTHREAD_H_WIN32

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

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_create (pace_pthread_t * thread,
		      const pace_pthread_attr_t * attr,
		      void * (*start_routine) (void*),
		      void * arg);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  pace_pthread_t pthread_self ();
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  /* Returns -1 if invalid pointer to param is passed in! */
  int pthread_getschedparam (pace_pthread_t thread,
			     int * policy,
			     pace_sched_param * param);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_setschedparam (pace_pthread_t thread,
			     int policy,
			     const pace_sched_param * param);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  /* Returns -1 if attr is already valid! */
  int pthread_attr_init (pace_pthread_attr_t * attr);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  /* Returns -1 if attr is invalid! */
  int pthread_attr_destroy (pace_pthread_attr_t * attr);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getdetachstate (const pace_pthread_attr_t * attr,
				   int * detachstate);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setdetachstate (pace_pthread_attr_t * attr,
				   int detachstate);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getinheritsched (const pace_pthread_attr_t * attr,
				    int * inheritsched);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setinheritsched (pace_pthread_attr_t * attr,
				    int inheritsched);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getschedparam (const pace_pthread_attr_t * attr,
				  pace_sched_param * param);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setschedparam (pace_pthread_attr_t * attr,
				  const pace_sched_param * param);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getschedpolicy (const pace_pthread_attr_t * attr,
				   int * policy);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setschedpolicy (pace_pthread_attr_t * attr,
				   int policy);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getscope (const pace_pthread_attr_t * attr,
			     int * contentionscope);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setscope (pace_pthread_attr_t * attr,
			     int contentionscope);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getstackaddr (const pace_pthread_attr_t * attr,
				 void ** stackaddr);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setstackaddr (pace_pthread_attr_t * attr,
				 void * stackaddr);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_getstacksize (const pace_pthread_attr_t * attr,
				 size_t * stacksize);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pthread_attr_setstacksize (pace_pthread_attr_t * attr,
				 size_t stacksize);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_PTHREAD_H_WIN32 */
