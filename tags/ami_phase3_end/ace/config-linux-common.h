/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all Linux platforms.  It automatically determines the CPU
// architecture, compiler (g++ or egcs), and libc (libc5 or glibc),
// and configures based on those.

#ifndef ACE_LINUX_COMMON_H
#define ACE_LINUX_COMMON_H

#define ACE_HAS_BYTESEX_H

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Needed to make some prototypes visible.
#if ! defined (_GNU_SOURCE)
#define _GNU_SOURCE
#endif /* ! _GNU_SOURCE */

// Needed to differentiate between libc 5 and libc 6 (aka glibc).
// It's there on all libc 5 systems I checked.
#include <features.h>


// First the machine specific part

#if defined (__alpha)
  // This is necessary on Alphas with glib 2.0.7-13.
# define ACE_POLL_IS_BROKEN
#endif /* ! __alpha */

// Then glibc/libc5 specific parts

#if defined(__GLIBC__)
# define ACE_HAS_BROKEN_SETRLIMIT
# define ACE_HAS_RUSAGE_WHO_ENUM enum __rusage_who
# define ACE_HAS_RLIMIT_RESOURCE_ENUM enum __rlimit_resource
# define ACE_HAS_SOCKLEN_T
# define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

  // To avoid the strangeness with Linux's ::select (), which modifies
  // its timeout argument, use ::poll () instead.
# define ACE_HAS_POLL

# if !defined (__USE_XOPEN_EXTENDED)
#   include <unistd.h>
    // unistd.h only declares getpgid () ifdef __USE_XOPEN_EXTENDED.
    extern "C" __pid_t getpgid __P ((__pid_t __pid));
# endif /* ! __USE_XOPEN_EXTENDED */

  // NOTE:  the following defines are necessary with glibc 2.0 (0.961212-5)
  //        on Alpha.  I assume that they're necessary on Intel as well,
  //        but that may depend on the version of glibc that is used.
//# define ACE_HAS_DLFCN_H_BROKEN_EXTERN_C
# define ACE_HAS_VOIDPTR_SOCKOPT
# define ACE_LACKS_SYSTIME_H
  // The strtok_r declaration is protected in string.h.
  extern "C" char *strtok_r __P ((char *__s, __const char *__delim,
                                  char **__save_ptr));
  // NOTE:  end of glibc 2.0 (0.961212-5)-specific configuration.

# if __GLIBC__ > 1 && __GLIBC_MINOR__ >= 1
    // These were suggested by Robert Hanzlik <robi@codalan.cz> to get
    // ACE to compile on Linux using glibc 2.1 and libg++/gcc 2.8.
#   undef ACE_HAS_BYTESEX_H
#   define ACE_HAS_SIGINFO_T
#   define ACE_LACKS_SIGINFO_H
#   define ACE_HAS_UCONTEXT_T
# endif /* __GLIBC__ 2.1+ */
#else  /* ! __GLIBC__ */
    // Fixes a problem with some non-glibc versions of Linux...
#   define ACE_LACKS_MADVISE
#   define ACE_LACKS_MSG_ACCRIGHTS
#endif /* ! __GLIBC__ */


// Then the compiler specific parts

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#elif defined (__KCC)
# include "ace/config-kcc-common.h"
#else  /* ! __GNUG__ && ! __KCC */
# error unsupported compiler in ace/config-linux-common.h
#endif /* ! __GNUG__ && ! __KCC */


// Completely common part :-)

// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT

# define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)

// Compiler/platform supports alloca().
#define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GETRUSAGE_PROTO

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// ONLY define this if you have config'd multicast into a 2.0.34 or
// prior kernel.  It is enabled by default in 2.0.35 kernels.
#if !defined (ACE_HAS_IP_MULTICAST)
# define ACE_HAS_IP_MULTICAST
#endif /* ! ACE_HAS_IP_MULTICAST */

#define ACE_HAS_BIG_FD_SET

#define ACE_HAS_STRING_CLASS

// Linux defines struct msghdr in /usr/include/socket.h
#define ACE_HAS_MSG

// Linux "improved" the interface to select() so that it modifies
// the struct timeval to reflect the amount of time not slept
// (see NOTES in Linux's select(2) man page).
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

#define ACE_HAS_TERM_IOCTLS

#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65535

#define ACE_DEFAULT_SELECT_REACTOR_SIZE 256

#define ACE_HAS_GETPAGESIZE 1

// Platform lacks POSIX prototypes for certain System V functions
// like shared memory and message queues.
#define ACE_LACKS_SOME_POSIX_PROTOTYPES

// Platform defines struct timespec but not timespec_t
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_STRRECVFD

//#define ACE_LACKS_MSYNC
#define ACE_HAS_PROCFS

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

#define ACE_HAS_POSIX_TIME

#define ACE_HAS_GPERF

#define ACE_HAS_DIRENT

# define ACE_UINT64_FORMAT_SPECIFIER "%Lu"

#if !defined (ACE_TIMER_SKEW)
# define ACE_TIMER_SKEW 10 * 1000
#endif /* ACE_TIMER_SKEW */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_LINUX_COMMON_H */
