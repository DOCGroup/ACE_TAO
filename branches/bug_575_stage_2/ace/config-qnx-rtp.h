// $Id$

// The following configuration file is designed to work for QNX RTP
// GNU C++ and the POSIX (pthread) threads package. You can get QNX
// RTP at http://get.qnx.com

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#define _POSIX_C_SOURCE 199506
#define _QNX_SOURCE

// These constants are in i386-nto/include/limits.h, but egcs
// picks up its own limits.h instead:
#define _POSIX_NAME_MAX     14      /*  Max bytes in a filename             */
#define _POSIX_PATH_MAX     256     /*  Num. bytes in pathname (excl. NULL) */

// gcc can do inline
#if __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 8)
# if !defined (__ACE_INLINE__)
#   define __ACE_INLINE__
# endif /* ! __ACE_INLINE__ */
#endif

#if defined(__OPTIMIZE__)
# if defined(__X86__)
    // string.h can't be used by ACE with __OPTIMIZE__.
#   undef __OPTIMIZE__
#   include <string.h>
#   define __OPTIMIZE__
# endif /* __X86__ */
#endif /* __OPTIMIZE__ */

#include "ace/config-g++-common.h"

// The following defines the Neutrino compiler.
// gcc should know to call g++ as necessary
#ifdef __GNUC__
# define ACE_CC_NAME "gcc"
#else
# define ACE_CC_NAME "QNX-RTP compiler ??"
#endif

// /usr/nto/include/float.h defines
//  FLT_MAX_EXP 127
//  DBL_MAX_EXP 1023
//  ace expects 128 & 1024 respectively
//  to set the following macros in ace/Basic_Types.h
//  These macros are:
#define ACE_SIZEOF_DOUBLE   8
#define ACE_SIZEOF_FLOAT    4

/////////////////////////////////////////////////////////////////
//    Definition of the features that are available.
//
//                ACE_HAS Section
/////////////////////////////////////////////////////////////////

#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_ALLOCA
#define ACE_HAS_ALLOCA_H
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_DIRENT
#define ACE_HAS_GETPAGESIZE
// Enable gperf, this is a hosted configuration.
#define ACE_HAS_GPERF
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
//#define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#define ACE_HAS_INLINED_OSCALLS
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_MSG
#define ACE_HAS_MT_SAFE_MKTIME
#define ACE_HAS_MUTEX_TIMEOUTS
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_HAS_PTHREAD_SIGMASK
#define ACE_HAS_P_READ_WRITE
#define ACE_HAS_REENTRANT_FUNCTIONS
#define ACE_HAS_SELECT_H
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGISMEMBER_BUG
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SIG_MACROS
#define ACE_HAS_SIN_LEN
#define ACE_HAS_SIZET_SOCKET_LEN
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRERROR
#define ACE_HAS_STRINGS
#define ACE_HAS_SVR4_GETTIMEOFDAY
#define ACE_HAS_TERM_IOCTLS
#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_THR_C_DEST
#define ACE_HAS_THR_C_FUNC
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#define ACE_HAS_UALARM
#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_VOIDPTR_MMAP
#define ACE_HAS_VOIDPTR_SOCKOPT

/////////////////////////////////////////////////////////////////
//    Definition of the features that are not available.
//
//                ACE_LACKS Section
/////////////////////////////////////////////////////////////////
#define ACE_LACKS_CMSG_DATA_MEMBER
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_CONST_TIMESPEC_PTR
#define ACE_LACKS_LINEBUFFERED_STREAMBUF
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MSGBUF_T
#define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_NAMED_POSIX_SEM
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_RPC_H
#define ACE_LACKS_RTTI
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SBRK
#define ACE_LACKS_SEEKDIR
#define ACE_LACKS_SOCKET_BUFSIZ
#define ACE_LACKS_SOCKETPAIR     // Even if the QNX RTP docs says that socket pair are
                                 // available, there is actually no implementation of
				 // soket-pairs.
#define ACE_LACKS_SOME_POSIX_PROTOTYPES
#define ACE_LACKS_STREAM_MODULES
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_SYSTIME_H
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_SYSV_SHMEM
//#define ACE_LACKS_TCP_NODELAY  // Based on the  QNX RTP documentation, this option seems to
                                 // to be supported.
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_T_ERRNO
#define ACE_LACKS_UALARM_PROTOTYPE
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_U_LONGLONG_T

#define ACE_LACKS_RLIMIT         // QNX rlimit syscalls don't work properly with ACE.

#define ACE_MT_SAFE 1
#define ACE_NEEDS_FUNC_DEFINITIONS
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 64000
#define ACE_TEMPLATES_REQUIRE_SOURCE
#define ACE_THR_PRI_FIFO_DEF 10
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#define ACE_HAS_SIGTIMEDWAIT
#define ACE_HAS_SIGSUSPEND

#define ACE_HAS_BROKEN_PREALLOCATED_OBJECTS_AFTER_FORK 1

#define ACE_SIZEOF_WCHAR 4

// Not really, but the prototype returns wchar_t instead of wchar_t *
#define ACE_LACKS_WCSSTR

// No prototypes
#define ACE_LACKS_ITOW
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSNICMP
#define ACE_LACKS_WCSDUP

// And these have prototypes but no implementation
#define ACE_LACKS_WCSLEN
#define ACE_LACKS_WCSNCMP
#define ACE_LACKS_WCSCPY
#define ACE_LACKS_WCSNCPY
#define ACE_LACKS_TOWLOWER
#define ACE_LACKS_WCSCMP
#define ACE_LACKS_WCSCAT
#define ACE_LACKS_WCSNCAT
#define ACE_LACKS_WCSSPN
#define ACE_LACKS_WCSCHR
#define ACE_LACKS_WCSPBRK
#define ACE_LACKS_WCSRCHR

#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
