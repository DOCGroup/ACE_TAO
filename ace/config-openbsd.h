/* -*- C++ -*- */
// $Id$

// ***** This configuration file is still under testing. *****

// The following configuration file is designed to work for OpenBSD
// platforms using GNU g++.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#define ACE_SIZEOF_LONG_DOUBLE 12

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

#define ACE_HAS_GPERF

// Platform specific directives
#define ACE_LACKS_GETPGID
#define ACE_LACKS_SETPGID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_RWLOCK_T
#define ACE_HAS_SIG_MACROS
#define ACE_HAS_CHARPTR_DL
#define ACE_HAS_DIRENT
#define ACE_USES_ASM_SYMBOL_IN_DLSYM

#define ACE_LACKS_UCONTEXT_H


// OpenBSD has sigwait defined
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIGINFO_T

#define ACE_HAS_SOCKLEN_T

#define ACE_HAS_POLL

// Use of <malloc.h> is deprecated.
#define ACE_LACKS_MALLOC_H

// NetBSD appears to have a sigset_t type.
// #define ACE_LACKS_SIGSET

// Platform supports POSIX timers via struct timespec.
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_UALARM

// Platform defines struct timespec but not timespec_t
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_SYSTIME_H

#define ACE_LACKS_STRRECVFD

#define ACE_HAS_SIN_LEN

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// OpenBSD supports the getifaddrs interface
#define ACE_HAS_GETIFADDRS

// Compiler/platform supports SVR4 signal typedef
#define ACE_HAS_SVR4_SIGNAL_T
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform supports alloca().
#define ACE_HAS_ALLOCA

// Compiler/platform supports SVR4 dynamic linking semantics..
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Explicit dynamic linking permits "lazy" symbol resolution
#define ACE_HAS_RTLD_LAZY_V

// platform supports POSIX O_NONBLOCK semantics
#define ACE_HAS_POSIX_NONBLOCK

// platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Compiler/platform has <alloca.h>
//#define ACE_HAS_ALLOCA_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Compiler/platform supports sys_siglist array.
// *** This refers to (_sys_siglist) instead of (sys_siglist)
// #define ACE_HAS_SYS_SIGLIST

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports SVR4 gettimeofday() prototype
#define ACE_HAS_SUNOS4_GETTIMEOFDAY
// #define ACE_HAS_TIMEZONE_GETTIMEOFDAY

//#define ACE_HAS_SIG_C_FUNC

// OpenBSD's dlsym call segfaults when passed an invalid handle.
// It seems as if most other OSs detect this and just report an
// error.
#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_HAS_MSG
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_NONCONST_MSGSND
#define ACE_HAS_ALT_CUSERID

#define ACE_HAS_SIGWAIT

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

#define ACE_HAS_TERM_IOCTLS
#define ACE_USES_NEW_TERMIOS_STRUCT
#define TCGETS TIOCGETA
#define TCSETS TIOCSETA

// OpenBSD actually has the clearerr call, but it causes a
// bogus compiler syntax error.
#define ACE_LACKS_CLEARERR

#define ACE_HAS_MKSTEMP

// OpenBSD supports IPv6 by default, but ACE IPv6 code
// has compile errors.
//#define ACE_HAS_IPV6

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
