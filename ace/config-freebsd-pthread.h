/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for FreeBSD
// platforms with GNU C++ and the POSIX (pthread) threads package.

// Notice that the threaded version of ACE is only supported for -current.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include <osreldate.h>
// Make sure we source in the OS version.

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if (__FreeBSD_version < 220000)
#error Threads are not supported.
#endif /* __FreeBSD_version < 220000 */

#define ACE_SIZEOF_LONG_DOUBLE 12

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

// Platform specific directives
// gcc defines __FreeBSD__ automatically for us.
#if !defined (_THREAD_SAFE)
#define _THREAD_SAFE
#endif /* _THREAD_SAFE */

#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_LACKS_GETPGID
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_READDIR_R
#define ACE_HAS_SIG_MACROS
// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_CHARPTR_DL
//#define ACE_USES_ASM_SYMBOL_IN_DLSYM
#define ACE_LACKS_SIGSET

// Use of <malloc.h> is deprecated.
#define ACE_LACKS_MALLOC_H

// sched.h still not fully support on FreeBSD ?
// this is taken from /usr/src/lib/libc_r/uthread/pthread-private.h
enum schedparam_policy {
        SCHED_RR,
        SCHED_IO,
        SCHED_FIFO,
        SCHED_OTHER
};

// This won't be necessary after it is fixed in the system include headers.
extern "C" { char * cuserid (char *s); }

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

#if (__FreeBSD_version >= 300000)
#define ACE_HAS_SIGINFO_T
#endif /* __FreeBSD_version >= 300000 */

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_SI_ADDR

// Compiler/platform supports SVR4 signal typedef
#define ACE_HAS_SVR4_SIGNAL_T

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

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_HAS_MSG
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_NONCONST_MSGSND

// Thread specific settings
// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif /* ! ACE_MT_SAFE */
#define ACE_HAS_PTHREADS
#define ACE_LACKS_SETSCHED
#define ACE_LACKS_PTHREAD_CANCEL
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_DIRENT

#define ACE_HAS_SIGWAIT

#endif /* ACE_CONFIG_H */
