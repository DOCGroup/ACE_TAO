/* -*- C -*- */
// $Id$

/* The following configuration file is designed to work for RTEMS
   platforms using GNU C.
*/

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

// begin of linux-common.h

/* #define ACE_HAS_BYTESEX_H */

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Needed to make some prototypes visible.
// #if ! defined (_GNU_SOURCE)
// #define _GNU_SOURCE
// #endif /* ! _GNU_SOURCE */

// First the machine specific part
//   There are no known port specific issues with the RTEMS port of ACE.
//   XXX Pentium and PowerPC have high res timer support in ACE.

// Then the compiler specific parts
#if defined (__GNUG__)
  // config-g-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
  // this must appear before its #include.
# define ACE_HAS_STRING_CLASS
# include "ace/config-g-common.h"
#elif defined (__KCC)
# define ACE_HAS_STRING_CLASS
# include "ace/config-kcc-common.h"
#elif defined (__DECCXX)
# define ACE_CONFIG_INCLUDE_CXX_COMMON
# include "ace/config-cxx-common.h"
#else  /* ! __GNUG__ && ! __KCC && !__DECCXX */
# error unsupported compiler in ace/config-linux-common.h
#endif /* ! __GNUG__ && ! __KCC */

// Completely common part :-)

#define ACE_HAS_NONSTATIC_OBJECT_MANAGER
# if !defined (ACE_MAIN)
#   define ACE_MAIN ace_main
# endif /* ! ACE_MAIN */

// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads (MIT implementation)
#define ACE_HAS_PTHREADS
// ... and the final standard even!
#define ACE_HAS_PTHREADS_STD
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// XXX thread defines go here
#define ACE_MT_SAFE 1
#define ACE_PAGE_SIZE 4096
#define ACE_HAS_ALT_CUSERID
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_CLOCK_GETTIME
/* #define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES */
#define ACE_HAS_DIRENT
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
/* #define ACE_HAS_LLSEEK */
#define ACE_HAS_MEMCHR
#define ACE_HAS_MSG
#define ACE_HAS_MT_SAFE_MKTIME
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_PROCESS_ENUM
#define ACE_HAS_REENTRANT_FUNCTIONS
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGSUSPEND
#define ACE_HAS_SSIZE_T
/* #define ACE_HAS_STANDARD_CPP_LIBRARY */
#define ACE_HAS_STRERROR
#define ACE_HAS_SUNOS4_GETTIMEOFDAY
#define ACE_HAS_SYS_ERRLIST
#define ACE_HAS_SYS_FILIO_H
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#define ACE_LACKS_DIFFTIME
#define ACE_LACKS_EXEC
#define ACE_LACKS_FILELOCKS
#define ACE_LACKS_FORK
#define ACE_LACKS_GETOPT_PROTO
#define ACE_LACKS_GETPGID
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MKFIFO
#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MSYNC
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
/* #define ACE_LACKS_POSIX_PROTOTYPES */
  // ... for System V functions like shared memory and message queues
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_READLINK
#define ACE_HAS_BROKEN_READV
#define ACE_LACKS_READV
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RLIMIT_PROTOTYPE
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SBRK
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETREUID_PROTOTYPE
#define ACE_LACKS_SETREGID
#define ACE_LACKS_SETREGID_PROTOTYPE
#define ACE_LACKS_SOME_POSIX_PROTOTYPES
  // ... for System V functions like shared memory and message queues
#define ACE_LACKS_NATIVE_STRPTIME
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_SYS_NERR
#define ACE_LACKS_UALARM_PROTOTYPE
#define ACE_LACKS_UCONTEXT_H
#define ACE_HAS_BROKEN_WRITEV
#define ACE_LACKS_WRITEV
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 65536
#define ACE_NEEDS_SCHED_H
#define ACE_HAS_POSIX_NONBLOCK

/*  What should these be set to?.

ACE_TLI_TCP_DEVICE
ACE_HAS_RECVFROM_TIMEDWAIT
ACE_HAS_SEND_TIMEDWAIT
ACE_HAS_SENDTO_TIMEDWAIT
ACE_HAS_IP_MULTICAST
ACE_HAS_NONCONST_SELECT_TIMEVAL
ACE_HAS_WCHAR_TYPEDEFS_CHAR
ACE_HAS_WCHAR_TYPEDEFS_USHORT
ACE_HAS_SIGNAL_SAFE_OS_CALLS
ACE_HAS_SIZET_SOCKET_LEN
ACE_HAS_SOCKADDR_MSG_NAME
ACE_HAS_SOCKLEN_T
ACE_HAS_STRBUF_T
ACE_HAS_SYS_SIGLIST
ACE_HAS_TERM_IOCTLS
ACE_HAS_THREAD_SAFE_ACCEPT
ACE_LACKS_COND_TIMEDWAIT_RESET
ACE_LACKS_MSG_ACCRIGHTS
ACE_LACKS_NETDB_REENTRANT_FUNCTIONS

Why don't we have alloca.h?
 */

#include /**/ <pthread.h>

extern "C"
{
  int getopt (int, char *const *, const char *);
}

#endif /* ACE_CONFIG_H */
