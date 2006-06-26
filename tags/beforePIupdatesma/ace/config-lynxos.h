// $Id$

// The following configuration file is designed to work for LynxOS,
// version 3.0.0 and later, using the GNU g++ compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

#if ACE_LYNXOS_MAJOR > 3 || (ACE_LYNXOS_MAJOR == 3 && ACE_LYNXOS_MINOR > 0)
  // LynxOS 3.1.0 or greater needs ipc_1c.h to be included before net/if.h
  // to avoid macro conflict.
# define ACE_NEEDS_IPC_1C_H
#endif /* ACE_LYNXOS_MAJOR */

#if defined (__x86__)
# define ACE_HAS_PENTIUM
#elif defined (__powerpc__)
  // It looks like the default stack size is 15000.
  // ACE's Recursive_Mutex_Test needs more.
# define ACE_NEEDS_HUGE_THREAD_STACKSIZE 65536
  // This doesn't work on LynxOS 3.0.0, because it resets the TimeBaseRegister.
  // # define ACE_HAS_POWERPC_TIMER
#endif /* __x86__ || __powerpc__ */

#define ACE_DEFAULT_BASE_ADDR ((char *) 0)
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_ALLOCA
#define ACE_HAS_ALLOCA_H
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_CHARPTR_SHMAT
#define ACE_HAS_CHARPTR_SHMDT
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CLOCK_SETTIME
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_DIRENT
#define ACE_HAS_GETPAGESIZE
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GNU_CSTRING_H
#define ACE_HAS_GPERF
#define ACE_HAS_ICMP_SUPPORT 1
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_LYNXOS_SIGNALS
#define ACE_HAS_MEMCHR
#define ACE_HAS_MSG
#define ACE_HAS_NONCONST_CLOCK_SETTIME
#define ACE_HAS_NONCONST_GETBY
#define ACE_HAS_NONCONST_MSGSND
#define ACE_HAS_NONCONST_READV
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_NONCONST_SETRLIMIT
#define ACE_HAS_NONCONST_WRITEV
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_REALTIME_SIGNALS
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_HAS_SCANDIR
#define ACE_HAS_SEMUN
#define ACE_HAS_SHM_OPEN
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAMS
#define ACE_HAS_STRERROR
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_SYS_SIGLIST
#define ACE_HAS_SYS_SOCKIO_H
#define ACE_HAS_TERMIOS
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#define ACE_HAS_TYPENAME_KEYWORD
#define ACE_LACKS_CONST_TIMESPEC_PTR
#define ACE_LACKS_GETOPT_PROTOTYPE
#define ACE_LACKS_GETPGID
#define ACE_LACKS_INET_ATON_PROTOTYPE
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MKSTEMP_PROTOTYPE
#define ACE_LACKS_MKTEMP_PROTOTYPE
#define ACE_LACKS_PUTENV_PROTOTYPE
#define ACE_LACKS_REALPATH
#define ACE_LACKS_REGEX_H
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SCANDIR_PROTOTYPE
#define ACE_LACKS_SETEGID
#define ACE_LACKS_SETEUID
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_STRCASECMP_PROTOTYPE
#define ACE_LACKS_STRNCASECMP_PROTOTYPE
#define ACE_LACKS_STRPTIME
#define ACE_LACKS_SWAB_PROTOTYPE
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_WCHAR_H
#define ACE_LACKS_SUSECONDS_T
#define ACE_LACKS_USECONDS_T
#define ACE_MALLOC_ALIGN 8
#define ACE_PAGE_SIZE 4096
#define ACE_SCANDIR_CMP_USES_VOIDPTR

// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
# define _REENTRANT
#endif

#if ACE_MT_SAFE == 1
  // Platform supports threads.
# define ACE_HAS_PTHREADS
# if ACE_LYNXOS_MAJOR > 3 || (ACE_LYNXOS_MAJOR == 3 && ACE_LYNXOS_MINOR > 0)
    /* LynxOS 3.1.0 or greater */
#   define ACE_HAS_PTHREADS_STD
# else  /* LynxOS < 3.1.0 */
#   define ACE_HAS_PTHREADS_DRAFT4
#   define ACE_HAS_STDARG_THR_DEST
#   define ACE_LACKS_MUTEXATTR_PSHARED
    // Without TSS emulation, you'll only have 3 native TSS keys on
    // LynxOS 3.0.0/ppc.
#   define ACE_HAS_TSS_EMULATION
# endif /* LynxOS < 3.1.0 */
# define ACE_HAS_THREADS
# define ACE_HAS_THREAD_SPECIFIC_STORAGE
# define ACE_LACKS_NULL_PTHREAD_STATUS
# define ACE_LACKS_SETDETACH
# define ACE_LACKS_THREAD_PROCESS_SCOPING
# define ACE_LACKS_THREAD_STACK_ADDR
#endif /* ACE_MT_SAFE */

#define ACE_HAS_AIO_CALLS
#define ACE_POSIX_AIOCB_PROACTOR
// AIOCB Proactor works on Lynx. But it is not multi-threaded.
// Lynx OS 3.0.0 lacks POSIX call <pthread_sigmask>. So,we cannot use
// SIG Proactor also, with multiple threads. So, let us use the AIOCB
// Proactor. Once <pthreadd_sigmask> is available on Lynx, we can turn
// on SIG Proactor for this platform.
// #define ACE_POSIX_SIG_PROACTOR

#define ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK 1

#if __GNUC__ == 2  &&  __GNUC_MINOR__ == 9
  // config-g++-common.h defines these incorrectly for LynxOS 3.x
  // with G++ version 2.9-gnupro-98r2
# define ACE_HAS_TEMPLATE_SPECIALIZATION
# define ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION
# undef ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
#endif /* __GNUC__ == 2  &&  __GNUC_MINOR__ == 9 */

#if __GNUC__ < 3
# define ACE_LACKS_NUMERIC_LIMITS
#endif /* __GNUC__ < 3 */

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
# define ACE_USE_RCSID 0
#endif /* ! ACE_USE_RCSID */

// System include files are not in sys/, this gets rid of warning.
#define __NO_INCLUDE_WARN__

#if ACE_LYNXOS_MAJOR > 3 /* LynxOS 4.x */
# define ACE_HAS_POSIX_SEM
  // "changes signedness" error (OS.i and many other files)
# define ACE_HAS_SOCKLEN_T
  // LSOCK.cpp uses a macro from param.h, not included
# define ALIGNBYTES (sizeof(int) - 1)
# define ALIGN(p) (((unsigned)p + ALIGNBYTES) & ~ALIGNBYTES)
#else /* LynxOS 3.x */
  // Note on why ACE_HAS_POSIX_SEM is not #defined:
  // ACE_HAS_POSIX_SEM would cause native LynxOS mutexes and condition
  // variables to be used.  But, they don't appear to be intended to be
  // used between processes.  Without ACE_HAS_POSIX_SEM, ACE uses
  // semaphores for all synchronization.  Those can be used between
  // processes
  //# define ACE_HAS_POSIX_SEM

  // Don't use MAP_FIXED, at least for now.
# define ACE_MAP_FIXED 0
  // LynxOS, through 3.0.0, does not support MAP_PRIVATE,
  // so map it to MAP_SHARED.
# define ACE_MAP_PRIVATE ACE_MAP_SHARED

  // This gets around Lynx broken macro calls resulting in "::0"
# define _POSIX_THREADS_CALLS

# define ACE_HAS_LYNXOS_BROKEN_MMAP
# define ACE_HAS_POLL
# define ACE_LACKS_AUTO_PTR
# define ACE_LACKS_DLFCN_H
# define ACE_LACKS_SETPGID
# define ACE_LACKS_SETREGID
# define ACE_LACKS_SETREUID
# define ACE_LACKS_SETUID
# define ACE_LACKS_SI_ADDR
# define ACE_LACKS_TCP_NODELAY
#endif /* ACE_LYNXOS_MAJOR > 3 */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
