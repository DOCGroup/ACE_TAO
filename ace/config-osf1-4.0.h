/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the
// Digital UNIX V4.0a and later platforms with either the GNU g++, DEC
// cxx 5.4 and later, Rational RCC (2.4.1) compilers, or KAI 3.3
// compilers.  It is configured to use the IEEE Std 1003.1c-1995,
// POSIX System Application Program Interface, or DCE threads (with
// cxx only); it automatically selects the proper thread interface
// depending on whether the cxx -pthread or -threads option was
// specified.  By 4.0a the version is meant that is called "V4.0 464"
// by uname -a.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if !defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif /* ! ACE_MT_SAFE */

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ! ACE_NTRACE */

// Include unistd.h to define _POSIX_C_SOURCE.
#include <unistd.h>

// Configuration-specific #defines:
// 1) g++ or cxx
// 2) pthreads or DCE threads
#if defined (__GNUG__)
  // g++ with pthreads

# include "ace/config-g++-common.h"

# define ACE_HAS_GNU_CSTRING_H
# define ACE_HAS_REENTRANT_FUNCTIONS
#elif defined (__DECCXX)
# define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
# define ACE_LACKS_LINEBUFFERED_STREAMBUF
# define ACE_LACKS_SIGNED_CHAR
# define DEC_CXX
# if (__DECCXX_VER >= 60090010)
    // DEC CXX 6.0 supports exceptions, etc., by default.  Exceptions
    // are enabled by platform_osf1_4.0.GNU.
#   define ACE_HAS_ANSI_CASTS
#   if !defined (__RTTI)
#     define ACE_LACKS_RTTI
#   endif
#   define ACE_HAS_STDCPP_STL_INCLUDES
#   define ACE_HAS_TEMPLATE_SPECIALIZATION
#   define ACE_HAS_TEMPLATE_TYPEDEFS
#   define ACE_HAS_TYPENAME_KEYWORD
# else  /* __DECCXX_VER < 60090010 */
#   define ACE_LACKS_PRAGMA_ONCE
# endif /* __DECCXX_VER < 60090010 */
#elif defined (__rational__)
# define ACE_HAS_REENTRANT_FUNCTIONS
# define ACE_LACKS_LINEBUFFERED_STREAMBUF
# define ACE_LACKS_SIGNED_CHAR

    // Exceptions are enabled by platform_osf1_4.0_rcc.GNU.
# define ACE_HAS_ANSI_CASTS
# define ACE_HAS_STDCPP_STL_INCLUDES
# define ACE_HAS_TEMPLATE_SPECIALIZATION
# define ACE_HAS_TYPENAME_KEYWORD
#elif defined (__KCC)
# include "ace/config-kcc-common.h"
# error unsupported compiler on Digital Unix
#endif /* ! __GNUG__ && ! __DECCXX && ! __rational__ && !_KCC */

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199506L)
  // cxx with POSIX 1003.1c-1995 threads (pthreads) . . .
# define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
# define ACE_HAS_BROKEN_IF_HEADER
# define ACE_HAS_BROKEN_R_ROUTINES
# define ACE_HAS_PTHREADS
# define ACE_HAS_PTHREADS_STD
# define ACE_HAS_PTHREAD_SIGMASK
# define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
# define ACE_LACKS_T_ERRNO
# define ACE_POLL_IS_BROKEN
# define DIGITAL_UNIX
  // DJT removed this due to some minor issues related to the
  // definitions of timestruc_t and tid_t in procfs.h not sure what
  // functionality is lost?  Platform supports <sys/procfs.h>
  //#define ACE_HAS_PROC_FS
#else /* _POSIX_C_SOURCE < 199506L */
  // cxx with DCE threads . . .
  // This ACE configuration is only supported with cxx; it has not been
  // test with g++.
# define ACE_HAS_BROKEN_MSG_H
# define ACE_HAS_BROKEN_POSIX_TIME
# define ACE_HAS_PTHREADS
# define ACE_HAS_PTHREADS_DRAFT4
# define ACE_HAS_GETPAGESIZE
# define ACE_HAS_PROC_FS
# define ACE_HAS_SETKIND_NP
# define ACE_HAS_THREAD_SELF
# define ACE_LACKS_CONST_TIMESPEC_PTR
# define ACE_LACKS_THREAD_PROCESS_SCOPING
# define ACE_LACKS_PTHREAD_THR_SIGSETMASK
# define ACE_LACKS_PTHREAD_THR_SIGSETMASK
# define ACE_LACKS_READDIR_R
# define ACE_LACKS_SETSCHED
# define ACE_LACKS_SIGNED_CHAR
# define ACE_LACKS_SYSV_MSQ_PROTOS
#endif /* _POSIX_C_SOURCE < 199506L */

// Maximum compensation (10 ms) for early return from timed ::select ().
#if !defined (ACE_TIMER_SKEW)
# define ACE_TIMER_SKEW 10 * 1000
#endif /* ACE_TIMER_SKEW */

#define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
// NOTE: ACE_HAS_64BIT_LONGS is deprecated.  Instead, use ACE_SIZEOF_LONG == 8.
#define ACE_HAS_64BIT_LONGS
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_BROKEN_SETRLIMIT
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_DIRENT
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GPERF
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_LONG_MAP_FAILED
#define ACE_HAS_MSG
#define ACE_HAS_OSF1_GETTIMEOFDAY
#define ACE_HAS_OSF_TIMOD_H
#define ACE_HAS_POLL
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_PRIOCNTL
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAMS
#define ACE_HAS_STRERROR
#define ACE_HAS_STRING_CLASS
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_SVR4_SIGNAL_T
#define ACE_HAS_SYSCALL_H
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_TIUSER_H
#define ACE_HAS_TLI
#define ACE_HAS_TLI_PROTOTYPES
#define ACE_HAS_UALARM
#define ACE_HAS_UCONTEXT_T
#define ACE_LACKS_PRI_T
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_THREAD_STACK_ADDR
#define ACE_PAGE_SIZE 8192

// DJT 6/10/96 All these broken macro's can now be removed with the
// approporiate ordering of the include files. The Platinum release
// now temporarily supports both forms.  Platform's implementation of
// sendmsg() has a non-const msgheader parameter.
#define ACE_HAS_BROKEN_SENDMSG

// As 1MB thread-stack size seems to become standard (at least Solaris and
// NT have it), we should raise the minimum stack size to this level for
// avoiding unpleasant surprises when porting ACE software to Digital UNIX.
// Do not define this smaller than 64KB, because ACE_Log_Msg::log needs that!
// TK, 05 Feb 97
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE (1024 * 1024)
#define ACE_HAS_IDTYPE_T
#endif /* ACE_CONFIG_H */
