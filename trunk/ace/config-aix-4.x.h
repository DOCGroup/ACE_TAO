/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for OS
// platforms running AIX 4.x using the IBM C++ compiler (xlC) or
// g++/egcs.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#if defined (__xlC__) || (__IBMCPP__)
   // AIX xlC, IBM C/C++, and Visual Age C++ compilers
   //********************************************************************
   //
   // Compiler-related definitions.  These are set for C Set ++ V3
#  define ACE_HAS_EXCEPTIONS
   // Compiler supports the ssize_t typedef.
#  define ACE_HAS_SSIZE_T

// When using the preprocessor, Ignore info msg; invalid #pragma
#  if defined (__IBMCPP__) && (__IBMCPP__ < 400)  // IBM C++ 3.6
#    define ACE_CC_PREPROCESSOR_ARGS "-E -qflag=w:w"
#  endif /* (__IBMCPP__) && (__IBMCPP__ < 400) */

   // Keep an eye on this as the compiler and standards converge...
#  define ACE_LACKS_LINEBUFFERED_STREAMBUF
#  define ACE_LACKS_PRAGMA_ONCE

   // C Set++ 3.1 and IBM C/C++ 3.6
#  if defined (__xlC__)
#    define ACE_LACKS_PLACEMENT_OPERATOR_DELETE
#    define ACE_TEMPLATES_REQUIRE_PRAGMA
     // If compiling without thread support, turn off ACE's thread capability.
#    if !defined (_THREAD_SAFE)
#      define ACE_HAS_THREADS 0
#    endif /* _THREAD_SAFE */
#  endif

   // These are for Visual Age C++ only
#  if defined (__IBMCPP__) && (__IBMCPP__ >= 400)
#    define ACE_TEMPLATES_REQUIRE_SOURCE
#    define ACE_HAS_STD_TEMPLATE_SPECIALIZATION
#    define ACE_HAS_TYPENAME_KEYWORD
#    undef WIFEXITED
#    undef WEXITSTATUS
#  endif /* __IBMCPP__ */

#elif defined (__GNUG__)
  // config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
  // this must appear before its #include.
# define ACE_HAS_STRING_CLASS

# include "ace/config-g++-common.h"
  // Denotes that GNU has cstring.h as standard, to redefine memchr().
# define ACE_HAS_GNU_CSTRING_H
# define ACE_HAS_SSIZE_T

# if !defined (ACE_MT_SAFE) || ACE_MT_SAFE != 0
    // ACE_MT_SAFE is #defined below, for all compilers.
#   if !defined (_REENTRANT)
#     define _REENTRANT
#   endif /* _REENTRANT */
# endif /* !ACE_MT_SAFE */

#else  /* ! __xlC__ && ! __GNUG__ */
# error unsupported compiler in ace/config-aix-4.x.h
#endif /* ! __xlC__ && ! __GNUG__ */


// Compiling for AIX.
#ifndef AIX
#  define AIX
#endif /* AIX */

// Use BSD 4.4 socket definitions for pre-AIX 4.2.  The _BSD setting also
// controls the data type used for waitpid(), wait(), and wait3().
#if (ACE_AIX_MINOR_VERS < 2)
#  define _BSD 44
#  define ACE_HAS_UNION_WAIT
#endif /* ACE_AIX_MINOR_VERS < 3 */

// This environment requires this thing, pre-AIX 4.3
#if (ACE_AIX_MINOR_VERS < 3)
#  define _BSD_INCLUDES
#endif /* ACE_AIX_MINOR_VERS < 3 */

#define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)

#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

// ACE_HAS_AIX_BROKEN_SOCKET_HEADER may be required if your OS patches are
// not up to date.  On up-to-date 4.2+ systems, it should not be required.
// 4.2+ has 4.4 BSD sendmsg/recvmsg
#if (ACE_AIX_MINOR_VERS < 2)
#  define ACE_HAS_AIX_BROKEN_SOCKET_HEADER
#else
#  define ACE_HAS_SIZET_SOCKET_LEN
#  define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#endif /* ACE_AIX_MINOR_VERS < 2 */

#define ACE_HAS_AIX_HI_RES_TIMER
#define ACE_HAS_ALLOCA

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS
#if (ACE_AIX_MINOR_VERS < 3)
#  define ACE_HAS_CHARPTR_DL
#endif /* ACE_AIX_MINOR_VERS < 3 */

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// OS has readdir and friends.
#define ACE_HAS_DIRENT

// OS supports the getrusage() system call
#define ACE_HAS_GETRUSAGE

#define ACE_HAS_GPERF

#define ACE_HAS_H_ERRNO

#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_MSG
#if (ACE_AIX_MINOR_VERS < 2)
#  define ACE_LACKS_MSG_ACCRIGHTS
#  define ACE_LACKS_RLIMIT
#endif

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

#define ACE_HAS_POSIX_TIME
// ... but needs to include another header for it on 4.2+
#if (ACE_AIX_MINOR_VERS >= 2)
#  define ACE_HAS_BROKEN_POSIX_TIME
#endif /* ACE_AIX_MINOR_VERS > 2 */
// ... and needs another typedef
#define ACE_LACKS_TIMESPEC_T
#define ACE_HAS_SELECT_H

#define ACE_HAS_REENTRANT_FUNCTIONS

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T
#if (ACE_AIX_MINOR_VERS >= 2)
#  define ACE_HAS_SIGINFO_T
#  define ACE_LACKS_SIGINFO_H
#endif /* ACE_AIX_MINOR_VERS >=2 */
#define ACE_HAS_SIGWAIT
#if (ACE_AIX_MINOR_VERS > 3)
#  define ACE_HAS_SIGTIMEDWAIT
#endif /* ACE_AIX_MINOR_VERS > 3 */
#define ACE_HAS_SIN_LEN
#define ACE_HAS_STRBUF_T

// Compiler supports stropts.h
#define ACE_HAS_STREAMS
// #define ACE_HAS_STREAM_PIPES

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// AIX bzero()
#define ACE_HAS_STRINGS

#define ACE_HAS_STRUCT_NETDB_DATA

// Dynamic linking is in good shape on newer OS/patch levels.  If you have
// trouble with the dynamic linking parts of ACE, and can't patch your OS
// up to latest levels, comment this out.
#define ACE_HAS_SVR4_DYNAMIC_LINKING
// This is tightly related to dynamic linking...
#define ACE_HAS_AUTOMATIC_INIT_FINI

#define ACE_HAS_SVR4_GETTIMEOFDAY

#define ACE_HAS_SYSV_IPC
#define ACE_HAS_TIMOD_H
#define ACE_HAS_TIUSER_H
#define ACE_HAS_TLI
#define ACE_HAS_BROKEN_T_ERROR
#define ACE_HAS_TLI_PROTOTYPES
#define ACE_TLI_TCP_DEVICE  "/dev/xti/tcp"

#define ACE_HAS_UALARM

#if (ACE_AIX_MINOR_VERS >= 2)
#  define ACE_HAS_UCONTEXT_T
#endif /* ACE_AIX_MINOR_VERS >= 2 */

#define ACE_HAS_UTIME

// Platform has XPG4 wide character type and functions
#define ACE_HAS_XPG4_MULTIBYTE_CHAR

#define ACE_LACKS_TCP_H

// AIX uses LIBPATH to search for libraries
#define ACE_LD_SEARCH_PATH "LIBPATH"

#define ACE_NEEDS_DEV_IO_CONVERSION

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

//**************************************************************
//
// Threads related definitions.
//
// The threads on AIX are generally POSIX P1003.1c (ACE_HAS_PTHREADS).
// However, there is also a kernel thread ID (tid_t) that is used in
// ACE_Log_Msg (printing the thread ID).  The tid_t is not the same as
// pthread_t, and can't derive one from the other - thread_self() gets
// the tid_t (kernel thread ID) if called from a thread.
// Thanks very much to Chris Lahey for straightening this out.

// Unless threads are specifically turned off, build with them enabled.
#if !defined (ACE_HAS_THREADS)
#  define ACE_HAS_THREADS 1
#endif

#if (ACE_HAS_THREADS != 0)
#  if !defined (ACE_MT_SAFE)
#    define ACE_MT_SAFE 1
#  endif

#  define ACE_HAS_PTHREADS
// 4.3 and up has 1003.1c standard; 4.2 has draft 7
#  if (ACE_AIX_MINOR_VERS >= 3)
#    define ACE_HAS_PTHREADS_STD
#    define ACE_HAS_PTHREADS_UNIX98_EXT
// ACE_LACKS_SETSCHED should not be needed, but the OS.* doesn't fine tune
// this enough - it's too overloaded. Fix after ACE 5 is done.
#    define ACE_LACKS_SETSCHED
#  else
#    define ACE_HAS_PTHREADS_DRAFT7
#    define ACE_LACKS_RWLOCK_T
#    define ACE_LACKS_THREAD_STACK_ADDR
// If ACE doesn't compile due to the lack of these methods, please
// send email to ace-users@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED
#    define ACE_LACKS_SETSCHED
#  endif /* ACE_AIX_MINOR_VERS >= 3 */

#  define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#  define ACE_HAS_SIGTHREADMASK
#  define ACE_HAS_THREAD_SPECIFIC_STORAGE

#  define ACE_LACKS_THREAD_PROCESS_SCOPING
#else
#  undef ACE_HAS_THREADS
#endif /* ACE_HAS_THREADS != 0 */

#define ACE_MALLOC_ALIGN 8

// By default, tracing code is not compiled.  To compile it in, cause
// ACE_NTRACE to not be defined, and rebuild ACE.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
