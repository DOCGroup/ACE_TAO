/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#include <sys/types.h>

#ifndef PACE_SYS_TYPES_H
#define PACE_SYS_TYPES_H

#include <wtypes.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /* Wrap a handle. */
# define PACE_HANDLE int
# define PACE_INVALID_HANDLE -1

#ifndef PACE_DEV_T
#define PACE_DEV_T
  typedef dev_t pace_dev_t;
#endif /* PACE_DEV_T */

#ifndef PACE_GID_T
#define PACE_GID_T
  typedef int gid_t;
  typedef gid_t pace_gid_t;
#endif /* PACE_GID_T */

#ifndef PACE_INO_T
#define PACE_INO_T
  typedef ino_t pace_ino_t;
#endif /* PACE_INO_T */

#ifndef PACE_MODE_T
#define PACE_MODE_T
  typedef int mode_t;
  typedef mode_t pace_mode_t;
#endif

#ifndef PACE_NLINK_T
#define PACE_NLINK_T
  typedef DWORD nlink_t;
  typedef nlink_t pace_nlink_t;
#endif

#ifndef PACE_OFF_T
#define PACE_OFF_T
  typedef off_t pace_off_t;
#endif

#ifndef PACE_PID_T
#define PACE_PID_T
  typedef long pid_t;
  typedef pid_t pace_pid_t;
#endif /* PACE_PID_T */

#ifndef PACE_PTHREAD_ATTR_T
#define PACE_PTHREAD_ATTR_T
  typedef int pthread_attr_t;           /* temporary */
  typedef pthread_attr_t pace_pthread_attr_t;
#endif /* PACE_PTHREAD_ATTR_T */

#ifndef PACE_PTHREAD_CONDATTR_T
#define PACE_PTHREAD_CONDATTR_T
  typedef int pthread_condattr_t;        /* temporary */
  typedef pthread_condattr_t pace_pthread_condattr_t;
#endif /* PACE_PTHREAD_CONDATTR_T */

#ifndef PACE_PTHREAD_COND_T
#define PACE_PTHREAD_COND_T
  typedef int pthread_cond_t;            /* temporary */
  typedef pthread_cond_t pace_pthread_cond_t;
#endif /* PACE_PTHREAD_COND_T */

#ifndef PACE_PTHREAD_KEY_T
#define PACE_PTHREAD_KEY_T
  typedef int pthread_key_t;             /* temporary */
  typedef pthread_key_t pace_pthread_key_t;
#endif /* PACE_PTHREAD_KEY_T */

#ifndef PACE_PTHREAD_MUTEXATTR_T
#define PACE_PTHREAD_MUTEXATTR_T
  typedef int pthread_mutexattr_t;       /* temporary */
  typedef pthread_mutexattr_t pace_pthread_mutexattr_t;
#endif /* PACE_PTHREAD_MUTEXATTR_T */

#ifndef PACE_PTHREAD_MUTEX_T
#define PACE_PTHREAD_MUTEX_T
  typedef int pthread_mutex_t;           /* temporary */
  typedef pthread_mutex_t pace_pthread_mutex_t;
#endif /* PACE_PTHREAD_MUTEX_T */

#ifndef PACE_PTHREAD_ONCE_T
#define PACE_PTHREAD_ONCE_T
  typedef int pthread_once_t;            /* temporary */
  typedef pthread_once_t pace_pthread_once_t;
#endif /* PACE_PTHREAD_ONCE_T */

#ifndef PACE_PTHREAD_T
#define PACE_PTHREAD_T
  typedef DWORD pace_pthread_t;
  typedef HANDLE pace_hpthread_t;
#endif /* PACE_PTHREAD_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_SSIZE_T
#define PACE_SSIZE_T
  typedef long ssize_t;
  typedef ssize_t pace_ssize_t;
#endif /* PACE_SSIZE_T */

#ifndef PACE_UID_T
#define PACE_UID_T
  typedef int uid_t;
  typedef uid_t pace_uid_t;
#endif /* PACE_UID_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H */




