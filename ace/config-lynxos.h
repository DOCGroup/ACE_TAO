// $Id$

// The following configuration file is designed to work for LynxOS 2.50
// platforms using the GNU g++ compiler (version 2.7.2 or later).

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
  // PowerPC libraries don't seem to have alloca ()
# define ACE_HAS_ALLOCA
# define ACE_HAS_ALLOCA_H
# define ACE_HAS_PENTIUM
#elif defined (__powerpc__)
  // This doesn't work on LynxOS 3.0.0, because it resets the TimeBaseRegister.
  // # define ACE_HAS_POWERPC_TIMER
#endif /* __x86__ || __powerpc__ */

// System include files are not in sys/, this gets rid of warning.
#define __NO_INCLUDE_WARN__

// Maximum compensation (10 ms) for early return from timed ::select ().
#if !defined (ACE_TIMER_SKEW)
# define ACE_TIMER_SKEW 10 * 1000
#endif /* ACE_TIMER_SKEW */

#define ACE_LACKS_GETOPT_PROTO
#define ACE_HAS_SHM_OPEN
// LynxOS 2.5.0 does not support MAP_PRIVATE, so map it to MAP_SHARED
#define ACE_MAP_PRIVATE ACE_MAP_SHARED
#define ACE_HAS_STDARG_THR_DEST

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

#define ACE_HAS_LYNXOS_SIGNALS

#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

// Denotes that GNU has cstring.h as standard, which redefines memchr()
#define ACE_HAS_GNU_CSTRING_H

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// ACE_HAS_CLOCK_GETTIME requires linking with -lposix4.
#define ACE_HAS_CLOCK_GETTIME

// Platform passes timezone into gettimeofday call
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Platform does not have const anything
#define ACE_LACKS_CONST_TIMESPEC_PTR
#define ACE_HAS_NONCONST_GETBY
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_BROKEN_READV
#define ACE_HAS_BROKEN_SETRLIMIT
#define ACE_LACKS_SOME_POSIX_PROTOTYPES

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform has union semun defined
#define ACE_HAS_SEMUN

// Platform contains <poll.h>.
#define ACE_HAS_POLL
#define ACE_POLL_IS_BROKEN

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_MADVISE
#define ACE_LACKS_GETPGID

// in man pages, but no prototype in headers
#define ACE_LACKS_MKTEMP

// Platform supports the rusage struct.
#define ACE_HAS_GETRUSAGE

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_UCONTEXT_H
//#define ACE_HAS_UCONTEXT_T
//Platform has si_addr, but its not supported currently
#define ACE_LACKS_SI_ADDR

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST

/* Turn off the following seven defines if you want to disable threading. */
// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
# define _REENTRANT
#endif

// Platform supports threads.
#define ACE_HAS_THREADS

#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_DRAFT4
#define ACE_LACKS_NULL_PTHREAD_STATUS
#define ACE_LACKS_THREAD_STACK_ADDR
#define ACE_LACKS_SETDETACH
#define ACE_LACKS_THREAD_PROCESS_SCOPING
// This gets around Lynx broken macro calls resulting in "::0"
#define _POSIX_THREADS_CALLS

// Platform lack reader/writer locks
#define ACE_LACKS_RWLOCK_T

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Without TSS emulation, you'll only have 3 native TSS keys, on
// LynxOS 3.0.0/ppc.
#define ACE_HAS_TSS_EMULATION

#define ACE_HAS_SIGWAIT
#define ACE_HAS_DIRENT

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_MALLOC_ALIGN 8

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#define ACE_LACKS_GETHOSTENT

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
# define ACE_USE_RCSID 0
#endif /* ! ACE_USE_RCSID */

extern "C"
{
  int getopt (int, char *const *, const char *);
  int putenv (const char *);
}

// = Proactor stuff

// Aio works on lynx
#define ACE_HAS_AIO_CALLS

// AIOCB Proactor works on Lynx. But it is not
// multi-threaded.
// Lynx OS 3.0.0 lacks POSIX call <pthread_sigmask>. So,we cannot use
// SIG Proactor also, with multiple threads. So, let us use the AIOCB
// Proactor. Once <pthreadd_sigmask> is available on Lynx, we can turn
// on SIG Proactor for this platform.
// #define ACE_POSIX_SIG_PROACTOR
#define ACE_POSIX_AIOCB_PROACTOR 
#endif /* ACE_CONFIG_H */
