// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_unistd.h
 *
 *  standard symbolic constants and types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_UNISTD_H
#define ACE_OS_INCLUDE_OS_UNISTD_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"
#include "ace/os_include/os_inttypes.h"

#if defined (__BORLANDC__)
#  include "ace/os_include/os_fcntl.h"
#endif /* __BORLANDC */

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#  include /**/ <process.h>
#  include /**/ <io.h>
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

#if !defined (ACE_LACKS_UNISTD_H)
#  include /**/ <unistd.h>
#endif /* !ACE_LACKS_UNISTD_H */

#if defined (VXWORKS)
// for unlink(), close(), read(), write(), lseek(), chdir(), getcwd(), getwd(), 
// and isatty()
#  include /**/ <ioLib.h>
#endif /* VXWORKS */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if (!defined (_BSD_SOURCE) && \
    !defined (_XOPEN_SOURCE) && !defined (_XOPEN_SOURCE_EXTENDED)) \
    || (defined (_XOPEN_SOURCE) && defined (__GNUC__))

# if defined (ACE_LACKS_SETREUID_PROTOTYPE)
  extern int setreuid (uid_t ruid, uid_t euid);
# endif /* ACE_LACKS_SETREUID_PROTOTYPE */

# if defined (ACE_LACKS_SETREGID_PROTOTYPE)
  extern int setregid (gid_t rgid, gid_t egid);
# endif /* ACE_LACKS_SETREGID_PROTOTYPE */
#endif  /* !_BSD_SOURCE && !_XOPEN_SOURCE && !_XOPEN_SOURCE_EXTENDED
           || _XOPEN_SOURCE && __GNUC__ */

# if !defined (_SC_TIMER_MAX)
#   define _SC_TIMER_MAX 44
# endif /* _SC_TIMER_MAX */

// Default number of <ACE_Event_Handler>s supported by
// <ACE_Timer_Heap>.
# if !defined (ACE_DEFAULT_TIMERS)
#   define ACE_DEFAULT_TIMERS _SC_TIMER_MAX
# endif /* ACE_DEFAULT_TIMERS */

  // for use by access()
# if !defined (R_OK)
#   define R_OK    04      /* Test for Read permission. */
# endif /* R_OK */

# if !defined (W_OK)
#   define W_OK    02      /* Test for Write permission. */
# endif /* W_OK */

# if !defined (X_OK)
#   define X_OK    01      /* Test for eXecute permission. */
# endif /* X_OK */

# if !defined (F_OK)
#   define F_OK    0       /* Test for existence of File. */
# endif /* F_OK */

#if defined (CHORUS)
  int      getgid          __((void));
  int      getuid          __((void));
  char*    getcwd          __((char* buf, size_t size));
  int      pipe            __((int* fildes));
  int      gethostname     __((char*, size_t));
#endif /* CHORUS */

#if defined (ACE_LACKS_UALARM_PROTOTYPE)
   u_int ualarm (u_int usecs, u_int interval);
#endif /* ACE_LACKS_UALARM_PROTOTYPE */

#if defined (ACE_LACKS_PREAD_PROTOTYPE) && (_XOPEN_SOURCE - 0) < 500
   // _XOPEN_SOURCE == 500    Single Unix conformance
   // It seems that _XOPEN_SOURCE == 500 means that the prototypes are
   // already defined in the system headers.
   ssize_t pread (int fd,
                             void *buf,
                             size_t nbytes,
                             off_t offset);

   ssize_t pwrite (int fd,
                              const void *buf,
                              size_t n,
                              off_t offset);
#endif  /* ACE_LACKS_PREAD_PROTOTYPE && (_XOPEN_SOURCE - 0) < 500 */

#if defined (ACE_LACKS_GETPGID_PROTOTYPE) && \
    !defined (_XOPEN_SOURCE) && !defined (_XOPEN_SOURCE_EXTENDED)
   pid_t getpgid (pid_t pid);
#endif  /* ACE_LACKS_GETPGID_PROTOTYPE &&
           !_XOPEN_SOURCE && !_XOPEN_SOURCE_EXTENDED */

#if !defined (_LARGEFILE64_SOURCE)
#  if defined (ACE_LACKS_LSEEK64_PROTOTYPE) && \
      defined (ACE_LACKS_LLSEEK_PROTOTYPE)
#    error Define either ACE_LACKS_LSEEK64_PROTOTYPE or ACE_LACKS_LLSEEK_PROTOTYPE, not both!
#  elif defined (ACE_LACKS_LSEEK64_PROTOTYPE)
     ACE_LOFF_T lseek64 (int fd, ACE_LOFF_T offset, int whence);
#  elif defined (ACE_LACKS_LLSEEK_PROTOTYPE)
     ACE_LOFF_T llseek (int fd, ACE_LOFF_T offset, int whence);
#  endif
#endif  /* _LARGEFILE64_SOURCE */

#if !defined (_SC_AIO_MAX)
#  define _SC_AIO_MAX 1
#endif /* _SC_AIO_MAX */

#if defined (__BORLANDC__)
#  define _chdir chdir
#  undef _access
#  define _access access
#  if (__BORLANDC__ <= 0x540)
#    define _getcwd getcwd
#  endif
#  define _isatty isatty
#endif /* __BORLANDC__ */

#if defined (ACE_PSOS_SNARFS_HEADER_INFO)
   // Header information snarfed from compiler provided header files
   // that are not included because there is already an identically
   // named file provided with pSOS, which does not have this info
   // from compiler supplied header
   int getopt(int, char *const *, const char *);
   int isatty (int h);
#endif /* ACE_PSOS_SNARFS_HEADER_INFO */

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
