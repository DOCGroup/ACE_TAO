/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    fctnl.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_FCNTL_H
# define ACE_OS_INCLUDE_FCNTL_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/os_include/sys/types.h"

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_FCNTL_H)
#   include /**/ <fcntl.h>
# endif /* !ACE_LACKS_FCNTL_H */

// Include additional/alternate headers for certain platfor/compiler combinations.
# if defined (ACE_NEEDS_SYS_FCNTL_H)
#   include /**/ <sys/fcntl.h>
# endif /* ACE_NEEDS_SYS_FCNTL_H) */

// defined Win32 specific macros for UNIX platforms
# if !defined (O_BINARY)
#   define O_BINARY 0
# endif /* O_BINARY */
# if !defined (_O_BINARY)
#   define _O_BINARY O_BINARY
# endif /* _O_BINARY */
# if !defined (O_TEXT)
#   define O_TEXT 0
# endif /* O_TEXT */
# if !defined (_O_TEXT)
#   define _O_TEXT O_TEXT
# endif /* _O_TEXT */
# if !defined (O_RAW)
#   define O_RAW 0
# endif /* O_RAW */
# if !defined (_O_RAW)
#   define _O_RAW O_RAW
# endif /* _O_RAW */

# if !defined (O_NONBLOCK)
#   define O_NONBLOCK  1
# endif /* O_NONBLOCK  */

# if defined (ACE_HAS_POSIX_NONBLOCK)
#   define ACE_NONBLOCK O_NONBLOCK
# else
#   define ACE_NONBLOCK O_NDELAY
# endif /* ACE_HAS_POSIX_NONBLOCK */

# if defined (ACE_LACKS_FILELOCKS)
#   if ! defined (VXWORKS) && ! defined (ACE_PSOS) && ! defined (__rtems__)
// VxWorks defines struct flock in sys/fcntlcom.h.  But it doesn't
// appear to support flock ().  RTEMS defines struct flock but
// currently does not support locking.
struct flock
{
  short l_type;
  short l_whence;
  off_t l_start;
  off_t l_len;          /* len == 0 means until end of file */
  long  l_sysid;
  pid_t l_pid;
  long  l_pad[4];               /* reserve area */
};
#   endif /* ! VXWORKS */
# endif /* ACE_LACKS_FILELOCKS */


struct ace_flock_t
{
# if defined (ACE_WIN32)
  ACE_OVERLAPPED overlapped_;
# else
  struct flock lock_;
# endif /* ACE_WIN32 */

  /// Name of this filelock.
  const ACE_TCHAR *lockname_;

  /// Handle to the underlying file.
  ACE_HANDLE handle_;

# if defined (CHORUS)
  /// This is the mutex that's stored in shared memory.  It can only
  /// be destroyed by the actor that initialized it.
  ACE_mutex_t *process_lock_;
# endif /* CHORUS */
};

#endif /* ACE_OS_INCLUDE_FCNTL_H */
