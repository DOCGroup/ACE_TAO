/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/types.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#include <sys/types.h>
#include <pthread.h>

#ifndef PACE_SYS_TYPES_H_POSIX
#define PACE_SYS_TYPES_H_POSIX

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

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
  typedef pthread_attr_t pace_pthread_attr_t;
#endif /* PACE_PTHREAD_ATTR_T */

#ifndef PACE_PTHREAD_CONDATTR_T
#define PACE_PTHREAD_CONDATTR_T
  typedef pthread_condattr_t pace_pthread_condattr_t;
#endif /* PACE_PTHREAD_CONDATTR_T */

#ifndef PACE_PTHREAD_COND_T
#define PACE_PTHREAD_COND_T
  typedef pthread_cond_t pace_pthread_cond_t;
#endif /* PACE_PTHREAD_COND_T */

#ifndef PACE_PTHREAD_KEY_T
#define PACE_PTHREAD_KEY_T
  typedef pthread_key_t pace_pthread_key_t;
#endif /* PACE_PTHREAD_KEY_T */

#ifndef PACE_PTHREAD_MUTEXATTR_T
#define PACE_PTHREAD_MUTEXATTR_T
  typedef pthread_mutexattr_t pace_pthread_mutexattr_t;
#endif /* PACE_PTHREAD_MUTEXATTR_T */

#ifndef PACE_PTHREAD_MUTEX_T
#define PACE_PTHREAD_MUTEX_T
  typedef pthread_mutex_t pace_pthread_mutex_t;
#endif /* PACE_PTHREAD_MUTEX_T */

#ifndef PACE_PTHREAD_ONCE_T
#define PACE_PTHREAD_ONCE_T
  typedef pthread_once_t pace_pthread_once_t;
#endif /* PACE_PTHREAD_ONCE_T */

#ifndef PACE_PTHREAD_T
#define PACE_PTHREAD_T
  typedef pthread_t pace_pthread_t;
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

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H_POSIX */
