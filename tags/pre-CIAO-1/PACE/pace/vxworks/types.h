/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/types.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#include <sys/types.h>
#include <sched.h>
#include <semLib.h>

#ifndef PACE_SYS_TYPES_H_VXWORKS
#define PACE_SYS_TYPES_H_VXWORKS

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PTHREAD_MAX_PRIORITY 255
#define PTHREAD_MIN_PRIORITY 0

#define MAX_CLEAN_RTN 20
#define MAX_ONCE_ENTRY 100

#define JOIN_NORMAL 0
#define JOIN_PENDING 1
#define JOIN_TERMINATED 2
#define JOIN_DETATCHED 3

/* This needs to be pretty big for VxWorks according to ACE */
#define PTHREAD_DEFAULT_STACK_SIZE 64000

/* detach */
#define PTHREAD_CREATE_DETACHED         0x40    /* = THR_DETACHED */
#define PTHREAD_CREATE_JOINABLE         0
/* scope */
#define PTHREAD_SCOPE_SYSTEM            0x01    /* = THR_BOUND */
#define PTHREAD_SCOPE_PROCESS           0

/* inherit */
#define PTHREAD_INHERIT_SCHED           1
#define PTHREAD_EXPLICIT_SCHED          0

/*
 * Value of process-shared attribute
 */
#define PTHREAD_PROCESS_SHARED          1       /* = USYNC_PROCESS */
#define PTHREAD_PROCESS_PRIVATE         0       /* = USYNC_THREAD */
#define DEFAULT_TYPE                    PTHREAD_PROCESS_PRIVATE

#define PTHREAD_MUTEX_DEFAULT     0x1
#define PTHREAD_MUTEX_ERRORCHECK  0x2
#define PTHREAD_MUTEX_NORMAL      0x3
#define PTHREAD_MUTEX_RECURSIVE   0x4

#define PTHREAD_PRIO_INHERIT      0x10
#define PTHREAD_PRIO_NONE         0x20
#define PTHREAD_PRIO_PROTECT      0x30

/*
 * macros - default initializers defined as in synch.h
 * Any change here should be reflected in synch.h.
 */


/* cancellation type and state */
#define PTHREAD_CANCEL_ENABLE           0x00
#define PTHREAD_CANCEL_DISABLE          0x01
#define PTHREAD_CANCEL_DEFERRED         0x00
#define PTHREAD_CANCEL_ASYNCHRONOUS     0x02
#define PTHREAD_CANCELED                -19

#define PTHREAD_MAXNAME 100


  /* For future upgradeability to wchar */
#ifndef PACE_TCHAR
#define PACE_TCHAR
  typedef char pace_tchar;
#endif /* PACE_TCHAR */

#ifndef PACE_HANDLE
  typedef int PACE_HANDLE;
#endif /* PACE_HANDLE */

#ifndef PACE_INVALID_HANDLE
#define PACE_INVALID_HANDLE -1
#endif /* PACE_INVALID_HANDLE */

#ifndef PACE_DEV_T
#define PACE_DEV_T
  typedef dev_t pace_dev_t;
#endif /* PACE_DEV_T */

#ifndef PACE_GID_T
#define PACE_GID_T
  typedef gid_t pace_gid_t;
#endif /* PACE_GID_T */

#ifndef PACE_INO_T
#define PACE_INO_T
  typedef ino_t pace_ino_t;
#endif /* PACE_INO_T */

#ifndef PACE_MODE_T
#define PACE_MODE_T
  typedef mode_t pace_mode_t;
#endif /* PACE_MODE_T */

#ifndef PACE_NLINK_T
#define PACE_NLINK_T
  typedef nlink_t pace_nlink_t;
#endif /* PACE_NLINK_T */

#ifndef PACE_OFF_T
#define PACE_OFF_T
  typedef off_t pace_off_t;
#endif /* PACE_OFF_T */

#ifndef PACE_PID_T
#define PACE_PID_T
  typedef pid_t pace_pid_t;
#endif /* PACE_PID_T */

#ifndef PACE_PTHREAD_ATTR_T
#define PACE_PTHREAD_ATTR_T

  typedef struct _PTHREAD_ATTR
  {
    int stacksize;          
    char name[PTHREAD_MAXNAME];      /*the name */
    struct sched_param schedule;           /* priority */
    int scope;              /* options  */
    int policy;             /* schedule policy */
    int dstate;             /* detached state */
  } * pace_pthread_attr_t;
#endif /* PACE_PTHREAD_ATTR_T */

#ifndef PACE_PTHREAD_CONDATTR_T
#define PACE_PTHREAD_CONDATTR_T
  typedef int pace_pthread_condattr_t;
#endif /* PACE_PTHREAD_CONDATTR_T */

#ifndef PACE_PTHREAD_COND_T
#define PACE_PTHREAD_COND_T
  typedef SEM_ID pace_pthread_cond_t;
#endif /* PACE_PTHREAD_COND_T */

#ifndef PACE_PTHREAD_KEY_T
#define PACE_PTHREAD_KEY_T
  typedef int pace_pthread_key_t;
#endif /* PACE_PTHREAD_KEY_T */

#ifndef PACE_PTHREAD_MUTEXATTR_T
#define PACE_PTHREAD_MUTEXATTR_T

  typedef struct _PTHREAD_MUX_ATTR
  {
    int protocol;   /* priority inheritance */
    int shared;     /* system or process */
    int type;       /* mutex type */
  } * pace_pthread_mutexattr_t;
#endif /* PACE_PTHREAD_MUTEXATTR_T */

#ifndef PACE_PTHREAD_MUTEX_T
#define PACE_PTHREAD_MUTEX_T
  typedef SEM_ID pace_pthread_mutex_t;
#endif /* PACE_PTHREAD_MUTEX_T */

#ifndef PACE_PTHREAD_ONCE_T
#define PACE_PTHREAD_ONCE_T
  typedef int pace_pthread_once_t;
# define PTHREAD_ONCE_INIT 0
#endif /* PACE_PTHREAD_ONCE_T */

#ifndef PACE_PTHREAD_T
#define PACE_PTHREAD_T
# define PTHREAD_KEYS_MAX 100
# define _USER_SPARE4 spare4
  typedef struct _FUNC_HANDLE 
  {
    void (*routine)(void *);
    void * args;
  } funcHandle;

  typedef struct _ONCE_FUNC
  {
    int once_ctl;
    void (*init_routine)(void);
  } pthread_once_e;

  typedef struct _PTHREAD_T
  {
    unsigned int tid;
    funcHandle cleanupRtn[MAX_CLEAN_RTN];
    int rtnCount;
    pthread_once_e onceList[MAX_ONCE_ENTRY];
    int onceCount;     /* once variable count */
    int stateflag;     /* cancel state */
    int canceltype;    /* cancel type */
    int detachflag;    /* detached state */
    int joinstate;     /* join state */
    SEM_ID joinSem;    /* join semaphore */
    void * joinvalue;  /* pass value */
    struct _PTHREAD_T * jointhread;    /* the calling thread */
    void * keyvaluelist[PTHREAD_KEYS_MAX];
  } * pace_pthread_t;
#endif /* PACE_PTHREAD_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_SSIZE_T
#define PACE_SSIZE_T
  typedef ssize_t pace_ssize_t;
#endif /* PACE_SSIZE_T */

#ifndef PACE_UID_T
#define PACE_UID_T
  typedef uid_t pace_uid_t;
#endif /* PACE_UID_T */

#ifndef PACE_PTHREAD_QUEUE_T
#define PACE_PTHREAD_QUEUE_T
  struct _PTHREAD_QUEUE_ENTRY
  {
    int status;
    pace_pthread_t pthread;
    struct _PTHREAD_QUEUE_ENTRY * next;
  };

  typedef struct _PTHREAD_QUEUE_ENTRY pthread_q_entry;

# define PTHEAD_QUEUE_MAX_LEN 100

  struct _PTHREAD_KEY_ENTRY
  {
    int index;
    int valid;
    void (*destructor)(void*);
  };

  typedef struct _PTHREAD_KEY_ENTRY pthread_key_entry;
#endif /* PACE_PTHREAD_QUEUE_T */


#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H_VXWORKS */
