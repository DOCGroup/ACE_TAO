// $Id$

// The following configuration file is designed to work for LynxOS,
// version 2.5.0 and later, using the GNU g++ compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"

# if __GNUC_MINOR__ == 7
    // The g++ that's distributed with LynxOS 3.0.0 needs this.
    // It won't hurt with 2.5.0.
#   undef ACE_HAS_TEMPLATE_SPECIALIZATION
# elif __LYNXOS_SDK_VERSION <= 199603L
    // If ACE_HAS_STRING_CLASS is used with LynxOS 3.0.0, some
    // executables, such as IOStream_Test, require linking with
    // libg++.
#   define ACE_HAS_STRING_CLASS
# endif /* __GNUC_MINOR__ == 7 */
#endif /* __GNUG__ */

#if defined (__x86__)
  // PowerPC libraries don't seem to have alloca (), so only use with x86.
# define ACE_HAS_ALLOCA
# define ACE_HAS_ALLOCA_H
# define ACE_HAS_PENTIUM
#elif defined (__powerpc__)
  // fork ()/exec () don't seem to work well on PPC.
# define ACE_LACKS_FORK
  // This doesn't work on LynxOS 3.0.0, because it resets the TimeBaseRegister.
  // # define ACE_HAS_POWERPC_TIMER
#endif /* __x86__ || __powerpc__ */

#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_BROKEN_READV
#define ACE_HAS_BROKEN_SETRLIMIT
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_DIRENT
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GNU_CSTRING_H
#define ACE_HAS_GPERF
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_LYNXOS_SIGNALS
#define ACE_HAS_MSG
#define ACE_HAS_NONCONST_GETBY
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_POLL
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_HAS_SEMUN
#define ACE_HAS_SHM_OPEN
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SOCKIO_H
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STDARG_THR_DEST
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAMS
#define ACE_HAS_STRERROR
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_SYS_SIGLIST
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#define ACE_LACKS_CONST_TIMESPEC_PTR
#define ACE_LACKS_GETHOSTENT
#define ACE_LACKS_GETOPT_PROTO
#define ACE_LACKS_GETPGID
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOME_POSIX_PROTOTYPES
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_UCONTEXT_H
// LynxOS, through 3.0.0, does not support MAP_PRIVATE, so map it to
// MAP_SHARED.
#define ACE_MAP_PRIVATE ACE_MAP_SHARED
#define ACE_PAGE_SIZE 4096
#define ACE_POLL_IS_BROKEN

// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
# define _REENTRANT
#endif

#if ACE_MT_SAFE == 1
  // Platform supports threads.
# define ACE_HAS_PTHREADS
# define ACE_HAS_PTHREADS_DRAFT4
# define ACE_HAS_THREADS
# define ACE_HAS_THREAD_SPECIFIC_STORAGE
  // Without TSS emulation, you'll only have 3 native TSS keys, on
  // LynxOS 3.0.0/ppc.
# define ACE_HAS_TSS_EMULATION
# define ACE_LACKS_NULL_PTHREAD_STATUS
# define ACE_LACKS_SETDETACH
# define ACE_LACKS_THREAD_PROCESS_SCOPING
# define ACE_LACKS_THREAD_STACK_ADDR
  // This gets around Lynx broken macro calls resulting in "::0"
# define _POSIX_THREADS_CALLS
#endif /* ACE_MT_SAFE */

#define ACE_HAS_AIO_CALLS
#define ACE_POSIX_AIOCB_PROACTOR
// AIOCB Proactor works on Lynx. But it is not
// multi-threaded.
// Lynx OS 3.0.0 lacks POSIX call <pthread_sigmask>. So,we cannot use
// SIG Proactor also, with multiple threads. So, let us use the AIOCB
// Proactor. Once <pthreadd_sigmask> is available on Lynx, we can turn
// on SIG Proactor for this platform.
// #define ACE_POSIX_SIG_PROACTOR

// Maximum compensation (10 ms) for early return from timed ::select ().
#if !defined (ACE_TIMER_SKEW)
# define ACE_TIMER_SKEW 10 * 1000
#endif /* ACE_TIMER_SKEW */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
# define ACE_USE_RCSID 0
#endif /* ! ACE_USE_RCSID */

// System include files are not in sys/, this gets rid of warning.
#define __NO_INCLUDE_WARN__

extern "C"
{
  int getopt (int, char *const *, const char *);
  int putenv (const char *);
}

#endif /* ACE_CONFIG_H */
