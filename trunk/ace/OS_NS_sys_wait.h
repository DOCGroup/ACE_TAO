// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_sys_wait.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_SYS_WAIT_H
# define ACE_OS_NS_SYS_WAIT_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_wait.h"

namespace ACE_OS {

  /// Calls OS <::wait> function, so it's only portable to UNIX/POSIX
  /// platforms.
  pid_t wait (int * = 0);

  /**
   * Calls <::WaitForSingleObject> on Win32 and <ACE::waitpid>
   * otherwise.  Returns the passed in <pid_t> on success and -1 on
   * failure.
   * On Win32, <pid> is ignored if the <handle> is not equal to 0.
   * Passing the process <handle> is prefer on Win32 because using
   * <pid> to wait on the project doesn't always work correctly
   * if the waited process has already terminated.
   */
  pid_t wait (pid_t pid,
              ACE_exitcode *status,
              int wait_options = 0,
              ACE_HANDLE handle = 0);

  /**
   * Calls <::waitpid> on UNIX/POSIX platforms and <::await> on
   * Chorus.  Does not work on Vxworks, or pSoS.
   * On Win32, <pid> is ignored if the <handle> is not equal to 0.
   * Passing the process <handle> is prefer on Win32 because using
   * <pid> to wait on the project doesn't always work correctly
   * if the waited process has already terminated.
   */
  pid_t waitpid (pid_t pid,
                 ACE_exitcode *status = 0,
                 int wait_options = 0,
                 ACE_HANDLE handle = 0);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_sys_wait.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_SYS_WAIT_H */
