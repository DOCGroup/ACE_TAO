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

#ifndef PACE_SYS_TYPES_H_WIN32
#define PACE_SYS_TYPES_H_WIN32

#include <sys/types.h>
#if defined (__BORLANDC__)
# include <windows.h>
#else /* __BORLANDC__ */
# include <wtypes.h>
# include <windef.h>
#endif /* __BORLANDC__ */

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_TCHAR
#define PACE_TCHAR 1
  typedef char pace_tchar;
#endif /* PACE_TCHAR */

#ifndef PACE_HANDLE
#define PACE_HANDLE HANDLE
#endif /* PACE_HANDLE */

#ifndef PACE_INVALID_HANDLE
#define PACE_INVALID_HANDLE (HANDLE)-1
#endif /* PACE_INVALID_HANDLE */

#ifndef PACE_DEV_T
#define PACE_DEV_T 1
  typedef dev_t pace_dev_t;
#endif /* PACE_DEV_T */

#ifndef PACE_GID_T
#define PACE_GID_T 1
  typedef int pace_gid_t;
#endif /* PACE_GID_T */

#ifndef PACE_INO_T
#define PACE_INO_T 1
  typedef ino_t pace_ino_t;
#endif /* PACE_INO_T */

#ifndef PACE_MODE_T
#define PACE_MODE_T 1
  typedef int pace_mode_t;
#endif /* PACE_MODE_T */

#ifndef PACE_NLINK_T
#define PACE_NLINK_T 1
  typedef DWORD pace_nlink_t;
#endif /* PACE_NLINK_T */

#ifndef PACE_OFF_T
#define PACE_OFF_T 1
  typedef off_t pace_off_t;
#endif /* PACE_OFF_T */

#ifndef PACE_PID_T
#define PACE_PID_T 1
  typedef long pace_pid_t;
#endif /* PACE_PID_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T 1
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_SSIZE_T
#define PACE_SSIZE_T 1
  typedef long pace_ssize_t;
#endif /* PACE_SSIZE_T */

#ifndef PACE_UID_T
#define PACE_UID_T 1
  typedef int pace_uid_t;
#endif /* PACE_UID_T */

#include "pace/win32/pthreadtypes.h"

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TYPES_H_WIN32 */
