// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_H
#define ACE_OS_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Cleanup.h"
#include "ace/Object_Manager_Base.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_dirent.h"
#include "ace/OS_NS_dlfcn.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_math.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_poll.h"
#include "ace/OS_NS_pwd.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stropts.h"
#include "ace/OS_NS_sys_mman.h"
#include "ace/OS_NS_sys_msg.h"
#include "ace/OS_NS_sys_resource.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_sys_shm.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_uio.h"
#include "ace/OS_NS_sys_utsname.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_Thread.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_wchar.h"

// Include the split up ACE_OS classes
#include "ace/OS_Dirent.h"
#include "ace/OS_String.h"
#include "ace/OS_Memory.h"
#include "ace/OS_TLI.h"
#include "ace/OS_Errno.h"

#include "ace/os_include/os_dlfcn.h"
#include "ace/os_include/sys/os_mman.h"
#include "ace/os_include/os_netdb.h"
#include "ace/os_include/sys/os_socket.h"
#include "ace/os_include/net/os_if.h"
#include "ace/os_include/sys/os_sem.h"

#include "ace/Time_Value.h"

class ACE_Timeout_Manager;

// Here are all ACE-specific default constants, needed throughout ACE
// and its applications.  The values can be over written by user
// specific values in config.h files.
#include "ace/Default_Constants.h"

// Here are all ACE-specific global declarations needed throughout
// ACE.
#include "ace/Global_Macros.h"

// include the ACE min()/max() functions.
# include "ace/Min_Max.h"

///////////////////////////////////////////
//                                       //
// NOTE: Please do not add any #includes //
//       before this point.  On VxWorks, //
//       vxWorks.h must be #included     //
//       first!                          //
//                                       //
///////////////////////////////////////////

#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/sys/os_stat.h"
#include "ace/os_include/os_stropts.h"
#include "ace/os_include/os_unistd.h"
#include "ace/os_include/sys/os_wait.h"


# if defined (ACE_PSOS)
#   include /**/ "ace/sys_conf.h" /* system configuration file */
#   include /**/ <pna.h>      /* pNA+ TCP/IP Network Manager calls */
#   if defined (ACE_PSOSIM)
#     include /**/ <psos.h>         /* pSOS+ system calls                */
    /*   include <rpc.h>       pRPC+ Remote Procedure Call Library calls   */
    /*                         are not supported by pSOSim                 */
    /*                                                                     */
    /*   include <phile.h>     pHILE+ file system calls are not supported  */
    /*                         by pSOSim *so*, for the time being, we make */
    /*                         use of UNIX file system headers and then    */
    /*                         when we have time, we wrap UNIX file system */
    /*                         calls w/ pHILE+ wrappers, and modify ACE to */
    /*                         use the wrappers under pSOSim               */
#   else
#     include /**/ <configs.h>   /* includes all pSOS headers */
//    #include /**/ <psos.h>    /* pSOS system calls */
#     include /**/ <phile.h>     /* pHILE+ file system calls */
//    #include /**/ <prepccfg.h>     /* pREPC+ file system calls */
#     if defined (ACE_PSOS_DIAB_MIPS)
#       if !defined (ACE_PSOS_USES_DIAB_SYS_CALLS)
#         include /**/ <prepc.h>
#       endif /* ACE_PSOS_USES_DIAB_SYS_CALLS */
#     endif /* ACE_PSOS_DIAB_MIPS */
#   endif /* defined (ACE_PSOSIM) */
# endif /* defined (ACE_PSOS) **********************************************/

// This needs to go here *first* to avoid problems with AIX.
# if defined (ACE_HAS_PTHREADS)
#   include "ace/os_include/os_pthread.h"
# endif /* ACE_HAS_PTHREADS */

# if defined (ACE_HAS_PROC_FS)
#   include /**/ <sys/procfs.h>
# endif /* ACE_HAS_PROC_FS */

# if defined (ACE_HAS_POSIX_SEM)
#   include "ace/os_include/os_semaphore.h"
# endif /* ACE_HAS_POSIX_SEM */

#include "ace/os_include/sys/os_types.h"
#include "ace/os_include/os_stddef.h"
#if !defined (ACE_LACKS_UNISTD_H)
#  include "ace/os_include/os_unistd.h"
#endif /* ACE_LACKS_UNISTD_H */

// Standard C Library includes
// NOTE: stdarg.h must be #included before stdio.h on LynxOS.
# include "ace/os_include/os_stdarg.h"
# if !defined (ACE_HAS_WINCE)
#   include "ace/os_include/os_assert.h"
#   include "ace/os_include/os_stdio.h"

#   if !defined (ACE_LACKS_NEW_H)
#     if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#       include /**/ <new>
#     else
#       include /**/ <new.h>
#     endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#   endif /* ! ACE_LACKS_NEW_H */

#   if !defined (ACE_PSOS_DIAB_MIPS)  &&  !defined (VXWORKS)
#   define ACE_DONT_INCLUDE_ACE_SIGNAL_H
#     include "ace/os_include/os_signal.h"
#   undef ACE_DONT_INCLUDE_ACE_SIGNAL_H
#   endif /* ! ACE_PSOS_DIAB_MIPS && ! VXWORKS */

#   if ! defined (ACE_PSOS_DIAB_MIPS)
#   include "ace/os_include/os_fcntl.h"
#   endif /* ! ACE_PSOS_DIAB_MIPS */
# endif /* ACE_HAS_WINCE */

# include "ace/os_include/os_limits.h"
# include "ace/os_include/os_ctype.h"
# if ! defined (ACE_PSOS_DIAB_MIPS)
# include "ace/os_include/os_string.h"
# include "ace/os_include/os_stdlib.h"
# endif /* ! ACE_PSOS_DIAB_MIPS */
# include "ace/os_include/os_float.h"

# if defined (ACE_NEEDS_SCHED_H)
#   include "ace/os_include/os_sched.h"
# endif /* ACE_NEEDS_SCHED_H */

#   include "ace/iosfwd.h"

# if !defined (ACE_HAS_WINCE)
#   if ! defined (ACE_PSOS_DIAB_MIPS)
#   include "ace/os_include/os_fcntl.h"
#   endif /* ! ACE_PSOS_DIAB_MIPS */
# endif /* ACE_HAS_WINCE */

# if defined ACE_HAS_BYTESEX_H
#   include /**/ <bytesex.h>
# endif /* ACE_HAS_BYTESEX_H */
# include "ace/Basic_Types.h"

# if defined (ACE_HAS_UTIME)
#   include "ace/os_include/os_utime.h"
# endif /* ACE_HAS_UTIME */

# if defined (ACE_WIN32)

#   if !defined (ACE_HAS_WINCE)
#     include "ace/os_include/sys/os_timeb.h"
#   endif /* ACE_HAS_WINCE */

#   if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#     include "ace/os_include/netinet/os_in.h"  // <ws2tcpip.h>
#   endif /* ACE_HAS_WINSOCK2 */

#   if !defined (ACE_HAS_WINCE)
#     include "ace/os_include/os_time.h"
#     include "ace/os_include/sys/os_stat.h"  // <direct.h>
#     include "ace/os_include/os_unistd.h"  // <process.h>
#   endif /* ACE_HAS_WINCE */

#     include "ace/os_include/os_fcntl.h"

# else /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */

#   if defined (CHORUS)
#     include /**/ <chorus.h>
#     if !defined(CHORUS_4)
#       include /**/ <cx/select.h>
#     else
#       include "ace/os_include/os_stdio.h"
#     endif
#     include "ace/os_include/sys/os_uio.h"
#     include "ace/os_include/os_time.h"
#     include /**/ <stdfileio.h>
#     include /**/ <am/afexec.h>
#     include "ace/os_include/sys/os_types.h"
#     include "ace/os_include/os_signal.h"  // <sys/signal.h>
#     include "ace/os_include/sys/os_wait.h"
#     include "ace/os_include/os_pwd.h"
#     include /**/ <timer/chBench.h>

#   elif defined (CYGWIN32)
#     include "ace/os_include/sys/os_uio.h"
#     include "ace/os_include/os_fcntl.h"  // <sys/file.h>
#     include "ace/os_include/sys/os_time.h"
#     include "ace/os_include/sys/os_resource.h"
#     include "ace/os_include/sys/os_wait.h"
#     include "ace/os_include/os_pwd.h"
#   elif defined (__QNX__)
#     include "ace/os_include/sys/os_uio.h"
#     include "ace/os_include/sys/os_ipc.h"
#     include "ace/os_include:sys/os_time.h"
#     include "ace/os_include/sys/os_wait.h"
#     include "ace/os_include/sys/os_resource.h"
#     include "ace/os_include/os_pwd.h"
      // sets O_NDELAY
#     include /**/ <unix.h>
#     include "ace/os_include/os_limits.h"  // <sys/param.h> /* for NBBY */
#   elif defined(__rtems__)
#     include "ace/os_include/os_fcntl.h"  // <sys/file.h>
#     include "ace/os_include/sys/os_resource.h"
#     include "ace/os_include/sys/os_fcntl.h"
#     include "ace/os_include/sys/os_time.h"
#     include "ace/os_include/sys/os_utsname.h"
#     include "ace/os_include/sys/os_wait.h"
#     include "ace/os_include/os_pwd.h"

#   elif ! defined (VXWORKS) && ! defined (INTEGRITY)
#     include "ace/os_include/sys/os_uio.h"
#     include "ace/os_include/sys/os_ipc.h"
#     if !defined(ACE_LACKS_SYSV_SHMEM)
// No reason to #include this if the platform lacks support for SHMEM
#       include "ace/os_include/sys/os_shm.h"
#     endif /* ACE_LACKS_SYSV_SHMEM */
#     include "ace/os_include/os_fcntl.h"  // <sys/file.h>
#     include "ace/os_include/sys/os_time.h"
#     include "ace/os_include/sys/os_resource.h"
#     include "ace/os_include/sys/os_wait.h"
#     include "ace/os_include/os_pwd.h"
#   endif /* ! VXWORKS */
#   include "ace/os_include/os_stropts.h" // <sys/ioctl.h>

// IRIX5 defines bzero() in this odd file...
#   if defined (ACE_HAS_BSTRING)
#     include /**/ <bstring.h>
#   endif /* ACE_HAS_BSTRING */

// AIX defines bzero() in this odd file...
#   if defined (ACE_HAS_STRINGS)
#     include "ace/os_include/os_strings.h"
#   endif /* ACE_HAS_STRINGS */

#   if defined (ACE_HAS_TERM_IOCTLS)
#     if defined (__QNX__)
#       include "ace/os_include/os_termios.h"
#     else  /* ! __QNX__ */
#       include "ace/os_include/os_termios.h"  // <sys/termios.h>
#     endif /* ! __QNX__ */
#     if defined (HPUX)
#       include /**/ <sys/modem.h>
#     endif /* HPUX */
#   endif /* ACE_HAS_TERM_IOCTLS */

#   if defined (ACE_HAS_AIO_CALLS)
#     include "ace/os_include/os_aio.h"
#   endif /* ACE_HAS_AIO_CALLS */

#     include "ace/os_include/os_limits.h"  // <sys/param.h>

// This is here for ACE_OS::num_processors_online(). On HP-UX, it
// needs sys/param.h (above) and sys/pstat.h. The implementation of the
// num_processors_online() method also uses 'defined (__hpux)' to decide
// whether or not to try the syscall.
#   if defined (__hpux)
#     include /**/ <sys/pstat.h>
#   endif /* __hpux **/

#   if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS) && !defined (VXWORKS)
#     include "ace/os_include/sys/os_un.h"
#   endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#   if defined (ACE_HAS_POLL)
#     include "ace/os_include/os_poll.h"
#   endif /* ACE_HAS_POLL */

#   if defined (ACE_HAS_SELECT_H)
#     include "ace/os_include/sys/os_select.h"
#   endif /* ACE_HAS_SELECT_H */

#     include "ace/os_include/sys/os_msg.h"

#   if defined (ACE_HAS_PRIOCNTL)
#     include /**/ <sys/priocntl.h>
#   endif /* ACE_HAS_PRIOCNTL */

# endif /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */

# if !defined (ACE_WIN32) && !defined (ACE_LACKS_UNIX_SYSLOG)
# include "ace/os_include/os_syslog.h"
# endif /* !defined (ACE_WIN32) && !defined (ACE_LACKS_UNIX_SYSLOG) */

/**
 * @namespace ACE_OS
 *
 * @brief This namespace defines an OS independent programming API that
 *     shields developers from nonportable aspects of writing
 *     efficient system programs on Win32, POSIX and other versions
 *     of UNIX, and various real-time operating systems.
 *
 * This class encapsulates the differences between various OS
 * platforms.  When porting ACE to a new platform, this class is
 * the place to focus on.  Once this file is ported to a new
 * platform, pretty much everything else comes for "free."  See
 * <www.cs.wustl.edu/~schmidt/ACE_wrappers/etc/ACE-porting.html>
 * for instructions on porting ACE.  Please see the README file
 * in this directory for complete information on the meaning of
 * the various macros.
 */
namespace ACE_OS
{


# if defined (ACE_WIN32)
  // = Default Win32 Security Attributes definition.
  LPSECURITY_ATTRIBUTES default_win32_security_attributes (LPSECURITY_ATTRIBUTES);

  // = Win32 OS version determination function.
  /// Return the win32 OSVERSIONINFO structure.
  const OSVERSIONINFO &get_win32_versioninfo (void);

  // = A pair of functions for modifying ACE's Win32 resource usage.
  /// Return the handle of the module containing ACE's resources. By
  /// default, for a DLL build of ACE this is a handle to the ACE DLL
  /// itself, and for a static build it is a handle to the executable.
  HINSTANCE get_win32_resource_module (void);

  /// Allow an application to modify which module contains ACE's
  /// resources. This is mainly useful for a static build of ACE where
  /// the required resources reside somewhere other than the executable.
  void set_win32_resource_module (HINSTANCE);

# endif /* ACE_WIN32 */

  // = A set of wrappers for miscellaneous operations.


};  /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS.i"
# endif /* ACE_HAS_INLINED_OSCALLS */

#if defined (ACE_LEGACY_MODE)
# include "ace/Log_Msg.h"
# include "ace/Thread_Hook.h"
# include "ace/Thread_Adapter.h"
# include "ace/Thread_Exit.h"
# include "ace/Thread_Control.h"
#endif  /* ACE_LEGACY_MODE */

#include /**/ "ace/post.h"
#endif  /* ACE_OS_H */
