// -*- C++ -*-
//
// $Id$

#ifndef ACE_CONFIG_NSK_H
#define ACE_CONFIG_NSK_H
#include /**/ "ace/pre.h"


// The following configuration file contains defines for Tandem NSK
// platform, MIPS processor, version 2 C++ compiler.


//=========================================================================
// Tandem NSK specific parts
//=========================================================================


// Disable pthread renaming of symbols such as "open" and "close"
#define _CMA_NOWRAPPERS_ 1

// Get Handle_Set.cpp to generate correct bit operations for NSK platform
#define ACE_TANDEM_NSK_BIT_ORDER

// Use facilities provided by T1248 version of pthreads.
// (If not defined, will use old version of pthreads.)
#define ACE_TANDEM_T1248_PTHREADS

// Use all available T1248 thread aware wrapper functions for providing
// non-blocking I/O.
//#define ACE_TANDEM_T1248_PTHREADS_ALL_IO_WRAPPERS


// Need this include here because some symbols defined by pthreads
// (e.g. timespec_t) are needed before spthread.h is normally included
// by ACE
#ifdef ACE_TANDEM_T1248_PTHREADS
#include <spthread.h>
#else
#include "pthread.h"
#include "dce/cma_dispatch_coop.h"
#endif

// The following #defines are hacks to get around things
// that seem to be missing or different in Tandem land
#define NSIG 32                 // missing from Signal.h
                                // note: on nsk TNS/R there is room in
                                // sigset_t for 128 signals but those
                                // above 31 are not valid.
typedef long    fd_mask;        // should be in select.h but no such file
#define NBBY 8                  // must be consistent with value in sys/types.h
#define NFDBITS         (sizeof (fd_mask) * NBBY)       /* bits per mask */
#define MAXNAMLEN  248          // missing from dirent.h
#define ERRMAX 4218             // from errno.h

// Following seems to be missing from G06.20 version of standard
// pthreads includes (it appeared in older version of standard pthreads)
// (SCHED_FIFO (aka cma_c_sched_fifo) used in Dynamic_Priority_Test)
#ifdef ACE_TANDEM_T1248_PTHREADS
typedef enum CMA_T_SCHED_POLICY {
    cma_c_sched_fifo = 0,
    cma_c_sched_rr = 1,
    cma_c_sched_throughput = 2,
    cma_c_sched_background = 3,
    cma_c_sched_ada_low = 4
    }				cma_t_sched_policy;
#endif


//=========================================================================
// Platform specific parts
//=========================================================================

// Platform lacks getpwnam_r() methods (e.g., SGI 6.2).
#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS

// Platform/compiler lacks {get,set}rlimit() function
#define ACE_LACKS_RLIMIT

// The platform doesn't have mmap(2)
#define ACE_LACKS_MMAP

// Platform lacks streambuf "linebuffered ()".  [C++ iostream]
#define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Use native implementation of memchr.
#define ACE_HAS_MEMCHR

// Platform supports recvmsg and sendmsg
#define ACE_HAS_MSG

// Platform has ACE_HAS_4_4BSD_SENDMSG_RECVMSG but its cmsghdr
// structure does not contain an 'unsigned char cmsg_data[0]' member.
#define ACE_LACKS_CMSG_DATA_MEMBER

//  Platform defines ACE_HAS_MSG, but lacks msg_accrights{,len}.
#define ACE_LACKS_MSG_ACCRIGHTS

// Platform supports sigsuspend()
#define ACE_HAS_SIGSUSPEND

// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// OS/compiler uses size_t * rather than int * for socket lengths
#define ACE_HAS_SIZET_SOCKET_LEN

// OS/compiler uses void * arg 4 setsockopt() rather than const char *
#define ACE_HAS_VOIDPTR_SOCKOPT

// The platform doesn't have mprotect(2)
#define ACE_LACKS_MPROTECT

// Platform lacks struct msgbuf
#define ACE_LACKS_MSGBUF_T

// Platform lacks msync()
#define ACE_LACKS_MSYNC

// Platform does not support reentrant netdb functions (getprotobyname_r,
// getprotobynumber_r, gethostbyaddr_r, gethostbyname_r, getservbyname_r).
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS     Platform does not support

// Platform lacks madvise()
#define ACE_LACKS_MADVISE

// Platform lacks pri_t
#define ACE_LACKS_PRI_T

// Platform lacks a working sbrk()
#define ACE_LACKS_SBRK

// Platform doesn't have syscall() prototype
#define ACE_LACKS_SYSCALL

// Platform lacks the inet_aton() function.
#define ACE_LACKS_INET_ATON

// Compiler/platform has Dirent iterator functions
#define ACE_HAS_DIRENT

// Platform uses ACE_HAS_DIRENT but does not have readdir_r()
#define ACE_LACKS_READDIR_R

// Platform supports getpagesize() call (otherwise,
// ACE_PAGE_SIZE must be defined)
#define ACE_HAS_GETPAGESIZE

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform's select() uses non-const timeval*
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

// Platform supports POSIX O_NONBLOCK semantics
#define ACE_HAS_POSIX_NONBLOCK

// Platform lacks named POSIX semaphores
#define ACE_LACKS_NAMED_POSIX_SEM

// Platform has support for multi-byte character support compliant
// with the XPG4 Worldwide Portability Interface wide-character
// classification.
#define ACE_HAS_XPG4_MULTIBYTE_CHAR

// Platform supports the POSIX regular expression library.
// [Note Tandem NSK platform does have regular expresson support but it
// does not follow the assumptions made by ACE.  To use it would need
// to make some ACE modifications.]
//#define ACE_HAS_REGEX

// Compiler/platform supports strerror ()
#define ACE_HAS_STRERROR

// Platform doesn't have truncate()
#define ACE_LACKS_TRUNCATE

// Platform lacks readers/writer locks.
#define ACE_LACKS_RWLOCK_T

// Compiler's 'new' throws exception on failure (ANSI C++ behavior).
#define ACE_NEW_THROWS_EXCEPTIONS

// Optimize ACE_Handle_Set::count_bits for select() operations (common
// case)
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Platform lacks setreuid()
#define ACE_LACKS_SETREUID

// Platform lacks setregid()
#define ACE_LACKS_SETREGID

// Platforms/compiler lacks the sys_nerr variable
#define ACE_LACKS_SYS_NERR

// Compile using multi-thread libraries
#define ACE_MT_SAFE 1



// Platform supports System V IPC
#define ACE_HAS_SYSV_IPC

// Platform lacks the socketpair() call
// [Needed due to failure of Pipe_Test.  even though nsk platform
// has socketpair(), Pipe.cpp tries to set socket buf size but this
// is not allowed for AF_UNIX protocol on nsk.]
#define ACE_LACKS_SOCKET_BUFSIZ

// Platform lacks the socketpair() call
#define ACE_LACKS_SOCKETPAIR

//=========================================================================
// Threads specific parts
//=========================================================================

// Platform supports threads
#define ACE_HAS_THREADS

// Platform supports POSIX Pthreads, of one form or another.  This
// macro says the platform has a pthreads variety - should also define
// one of the below to say which one.  Also may need some
// ACE_HAS_... thing for extensions.
#define ACE_HAS_PTHREADS

// Platform's 'Pthreads' is .4a draft 4
#ifndef ACE_TANDEM_T1248_PTHREADS
#  define ACE_HAS_PTHREADS_DRAFT4
#  define ACE_LACKS_CONST_TIMESPEC_PTR
extern int cma_sigwait  (sigset_t *);
#endif

// Platform supports POSIX.1c-1995 threads
// (This is the final standard Pthreads).
#ifdef ACE_TANDEM_T1248_PTHREADS
#define ACE_HAS_PTHREADS_STD
#endif

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform has no implementation of pthread_condattr_setpshared(),
// even though it supports pthreads!
#define ACE_LACKS_CONDATTR_PSHARED

// pthread_cond_timedwait does *not* reset the time argument when the
// lock is acquired.
#define ACE_LACKS_COND_TIMEDWAIT_RESET

// Platform lacks pthread_attr_setsched()
#define ACE_LACKS_SETSCHED

// Platform has pthread_mutexattr_setkind_np().
#define ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP

// Platform lacks pthread_mutexattr_setpshared().
#define ACE_LACKS_MUTEXATTR_PSHARED

// Platform lacks pthread_attr_setscope()
#define ACE_LACKS_THREAD_PROCESS_SCOPING

// Platform lacks pthread_attr_setstackaddr
#define ACE_LACKS_THREAD_STACK_ADDR

// Defining ACE_HAS_UCONTEXT_T since G06.21 version of spthreads has
// a definition for it.
#ifdef ACE_TANDEM_T1248_PTHREADS
#define ACE_HAS_UCONTEXT_T
#endif

//=========================================================================
// Include file characteristics
//=========================================================================

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Platform lacks malloc.h
#define ACE_LACKS_MALLOC_H

// <time.h> doesn't automatically #include /**/ <sys/time.h>
#define ACE_LACKS_SYSTIME_H

// Platform lacks the siginfo.h include file
#define ACE_LACKS_SIGINFO_H

// Platform doesn't define struct strrecvfd.
#define ACE_LACKS_STRRECVFD

// Platform lacks the ucontext.h file
#define ACE_LACKS_UCONTEXT_H

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Platform lacks POSIX prototypes for certain System V functions like
// shared memory and message queues.
#define ACE_LACKS_SOME_POSIX_PROTOTYPES

// Platform supports the POSIX struct timespec type
#define ACE_HAS_POSIX_TIME

// Platform/compiler supports timezone * as second parameter to gettimeofday()
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Platform has <strings.h> (which contains bzero() prototype)
#define ACE_HAS_STRINGS 1


// OS/compiler omits the const from the iovec parameter in the
// writev() prototype.
#define ACE_HAS_BROKEN_WRITEV

// Platform lacks <stdint.h>
#define ACE_LACKS_STDINT_H

// Platform lacks <inttypes.h>
#define ACE_LACKS_INTTYPES_H

// Platform lacks <sys/select.h>
#define ACE_LACKS_SYS_SELECT_H

// Platform lacks <dlfcn.h>
#define ACE_LACKS_DLFCN_H

// Platform lacks <semaphore.h>
#define ACE_LACKS_SEMAPHORE_H

// Platform lacks <poll.h>
#define ACE_LACKS_POLL_H

//=========================================================================
// Compiler specific parts
//=========================================================================

// Compiler supports C++ exception handling
#define ACE_HAS_EXCEPTIONS

// Compiler/platform has correctly prototyped header files
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler supports mutable.
#define ACE_HAS_MUTABLE_KEYWORD

// Compiler/platform does not support the unsigned long long datatype.
#define ACE_LACKS_LONGLONG_T

// Compiler supports the ssize_t typedef
#define ACE_HAS_SSIZE_T

// Platform/compiler supports Standard C++ Library
#define ACE_HAS_STANDARD_CPP_LIBRARY 0

// Compiler support explicit constructors (for type conversions).
#define ACE_HAS_EXPLICIT_KEYWORD

// Compiler uses the template<> syntax
#define ACE_HAS_STD_TEMPLATE_SPECIALIZATION
#define ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION
#define ACE_HAS_STD_TEMPLATE_CLASS_MEMBER_SPECIALIZATION

// Compiler's template mechanism must see source code (i.e.,
// .cpp files).
#define ACE_TEMPLATES_REQUIRE_SOURCE

// Compiler implements template specialization
#define ACE_HAS_TEMPLATE_SPECIALIZATION

// Compiler implements templates that support typedefs inside
// of classes used as formal arguments to a template class.
#define ACE_HAS_TEMPLATE_TYPEDEFS

// Platform/Compiler supports a String class
//#define ACE_HAS_STRING_CLASS
//# define ACE_HAS_STDCPP_STL_INCLUDES

// Platform has its standard c++ library in the namespace std.
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

// Compiler doesn't support static data member templates
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Platform lacks "signed char" type (broken!)
// Following will not be needed if use standard c library (G06.20 and later)
#define ACE_LACKS_SIGNED_CHAR

//=========================================================================
// Build options
//=========================================================================

// Use in-line functions by default
//#if ! defined (__ACE_INLINE__)
//# define __ACE_INLINE__
//#endif /* ! __ACE_INLINE__ */

// Disable the inclusion of RCS ids in the generated code.
#define ACE_USE_RCSID 0

// For debugging problems in os calls (but this doesn't work too well
// since output is not interleaved properly with output from ACE_TRACE
//# define ACE_OS_TRACE(X) ::printf(X)

// Uncomment the following if timed message blocks are needed (e.g.
// for Dynamic_Priority_Test.  Otherwise leave this disabled because
// enabling it adds overhead to message blocks and timed message blocks
// are "rarely used."
//#define ACE_HAS_TIMED_MESSAGE_BLOCKS

// Uncomment the following if tokens library is needed.
//#define ACE_HAS_TOKENS_LIBRARY

#endif /* ACE_CONFIG_NSK_H */
