// $Id$

// The following configuration file is designed to work for LynxOS,
// version 2.5.0 and later, using the GNU g++ compiler.

// Note on why ACE_HAS_POSIX_SEM is not #defined:
// ACE_HAS_POSIX_SEM would cause native LynxOS mutexes and condition
// variables to be used.  But, they don't appear to be intended to be
// used between processes.  Without ACE_HAS_POSIX_SEM, ACE uses
// semaphores for all synchronization.  Those can be used between
// processes

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# if __GNUC_MINOR__ == 7

#   include "ace/config-g++-common.h"

    // The g++ that's distributed with LynxOS 3.0.0 needs this.
    // It won't hurt with 2.5.0.
#   undef ACE_HAS_TEMPLATE_SPECIALIZATION
# elif __LYNXOS_SDK_VERSION <= 199603L
    /* LynxOS <= 2.5.0 */
    // config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
    // this must appear before its #include.

    // If ACE_HAS_STRING_CLASS is used with LynxOS 3.0.0, some
    // executables, such as IOStream_Test, require linking with
    // libg++.
#   define ACE_HAS_STRING_CLASS

#   include "ace/config-g++-common.h"
# endif /* __GNUC_MINOR__ == 7 */
#endif /* __GNUG__ */

#if defined (__x86__)
  // PowerPC libraries don't seem to have alloca (), so only use with x86.
# define ACE_HAS_ALLOCA
# define ACE_HAS_ALLOCA_H
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
#define ACE_LACKS_SETPGID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOME_POSIX_PROTOTYPES
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_MKSTEMP
#define ACE_MALLOC_ALIGN 8
#define ACE_HAS_TYPENAME_KEYWORD
// Don't use MAP_FIXED, at least for now.
#define ACE_MAP_FIXED 0
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
# include <unistd.h>
# if _POSIX_VERSION >= 199506L
    // LynxOS 3.1.0 or greater
#   define ACE_HAS_PTHREADS_STD
    // Though there's a pthread_sigmask man page, there isn't a
    // declaration in a system header file.
#   include <signal.h>
    extern "C" int pthread_sigmask (int, const sigset_t *, sigset_t *);
# else  /* LynxOS < 3.1.0 */
#   define ACE_HAS_PTHREADS_DRAFT4
#   define ACE_HAS_STDARG_THR_DEST
    // Without TSS emulation, you'll only have 3 native TSS keys, on
    // LynxOS 3.0.0/ppc.
#   define ACE_HAS_TSS_EMULATION
# endif /* LynxOS < 3.1.0 */
# define ACE_HAS_THREADS
# define ACE_HAS_THREAD_SPECIFIC_STORAGE
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

#define ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK 1

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

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
