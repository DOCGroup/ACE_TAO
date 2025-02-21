// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_unistd.h
 *
 *  standard symbolic constants and types
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_UNISTD_H
#define ACE_OS_INCLUDE_OS_UNISTD_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"
#include "ace/os_include/os_inttypes.h"

#if defined (ACE_HAS_PROCESS_H)
#  include /**/ <process.h>
#endif /* ACE_HAS_PROCESS_H */

#if defined (ACE_HAS_IO_H)
#  include /**/ <io.h>
#endif /* ACE_HAS_IO_H */

#if !defined (ACE_LACKS_UNISTD_H)
#  include /**/ <unistd.h>
#endif /* !ACE_LACKS_UNISTD_H */

#if defined (ACE_VXWORKS)
#  if !defined (__RTP__)
    // for unlink(), close(), read(), write(), lseek(), chdir(), getcwd(),
   // getwd(), and isatty()
   #  include /**/ <ioLib.h>
#  endif
// for gethostname()
#  include /**/ <hostLib.h>
#endif /* ACE_VXWORKS */

#ifdef ACE_MQX
#  if !defined (STDIN_FILENO) && defined (_LLIO_STDIN)
#    define STDIN_FILENO (_LLIO_STDIN)
#  endif
#  if !defined (STDOUT_FILENO) && defined (_LLIO_STDOUT)
#    define STDOUT_FILENO (_LLIO_STDOUT)
#  endif
#  if !defined (STDERR_FILENO) && defined (_LLIO_STDERR)
#    define STDERR_FILENO (_LLIO_STDERR)
#  endif
#endif

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_WIN32)
// The following are #defines and #includes that are specific to
// WIN32.
#    define ACE_STDIN GetStdHandle (STD_INPUT_HANDLE)
#    define ACE_STDOUT GetStdHandle (STD_OUTPUT_HANDLE)
#    define ACE_STDERR GetStdHandle (STD_ERROR_HANDLE)
// The following are #defines and #includes that are specific to UNIX.
#else /* !ACE_WIN32 */
#  if defined (STDIN_FILENO)
#    define ACE_STDIN STDIN_FILENO
#  else
#    define ACE_STDIN 0
#  endif
#  if defined (STDOUT_FILENO)
#    define ACE_STDOUT STDOUT_FILENO
#  else
#    define ACE_STDOUT 1
#  endif
#  if defined (STDERR_FILENO)
#    define ACE_STDERR STDERR_FILENO
#  else
#    define ACE_STDERR 2
#  endif
#endif /* ACE_WIN32 */

  // for use by access()
# if !defined (R_OK)
#   define R_OK    04      /* Test for Read permission. */
# endif /* R_OK */

# if !defined (W_OK)
#   define W_OK    02      /* Test for Write permission. */
# endif /* W_OK */

# if !defined (X_OK)
#   if defined (ACE_WIN32)
      /* Windows has no test for X_OK - use R_OK instead */
#     define X_OK    R_OK      /* Test for eXecute permission. */
#   else  /* ACE_WIN32 */
#     define X_OK    01      /* Test for eXecute permission. */
#   endif /* ACE_WIN32 */
# endif /* X_OK */

# if !defined (F_OK)
#   define F_OK    0       /* Test for existence of File. */
# endif /* F_OK */

#if defined (ACE_LACKS_GETPGID_PROTOTYPE) && \
    !defined (_XOPEN_SOURCE) && !defined (_XOPEN_SOURCE_EXTENDED)
   pid_t getpgid (pid_t pid);
#endif  /* ACE_LACKS_GETPGID_PROTOTYPE &&
           !_XOPEN_SOURCE && !_XOPEN_SOURCE_EXTENDED */

#if defined (__BORLANDC__)
#  define _isatty isatty
#endif /* __BORLANDC__ */

# if defined (ACE_LACKS_TIMEDWAIT_PROTOTYPES)

  ssize_t read_timedwait (ACE_HANDLE handle,
                          char *buf,
                          size_t n,
                          struct timespec *timeout);

  ssize_t write_timedwait (ACE_HANDLE handle,
                           const void *buf,
                           size_t n,
                           struct timespec *timeout);

# endif /* ACE_LACKS_TIMEDWAIT_PROTOTYPES */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_UNISTD_H */
