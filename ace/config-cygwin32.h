/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for CygWin
// platforms using GNU C++.

#ifndef ACE_CONFIG_CYGWIN32_H
#define ACE_CONFIG_CYGWIN32_H

#include "ace/pre.h"

#if !defined (ACE_MT_SAFE)
#define ACE_MT_SAFE 1
#endif

#define CYGWIN32

// We trust this file will get included before <sys/types.h>
#if !defined(FD_SETSIZE)
# define FD_SETSIZE 1024
#endif

#if !defined (ACE_IOV_MAX)
# define ACE_IOV_MAX 64
#endif /* ACE_IOV_MAX */

// Define custom export macros for export/import of symbols from/of dll's
#define ACE_HAS_CUSTOM_EXPORT_MACROS
#define ACE_Proper_Export_Flag __declspec (dllexport)
#define ACE_Proper_Import_Flag __declspec (dllimport)
#define ACE_EXPORT_SINGLETON_DECLARATION(T) template class __declspec (dllexport) T
#define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#define ACE_IMPORT_SINGLETON_DECLARATION(T) extern template class T
#define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) extern template class SINGLETON_TYPE <CLASS, LOCK>;

#define ACE_LACKS_SYSV_MSG_H
#define ACE_HAS_SIG_MACROS
#define ACE_LACKS_SYSTIME_H
#define ACE_LACKS_SYSV_SHMEM

#define ACE_HAS_BROKEN_MAP_FAILED

#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SENDMSG
#define ACE_LACKS_RECVMSG
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_FCNTL

#define ACE_HAS_SELECT_H

#define ACE_LACKS_PRAGMA_ONCE

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Needed to differentiate between libc 5 and libc 6 (aka glibc).
// It's there on all libc 5 systems I checked.
#include /**/ <features.h>

// config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
// this must appear before its #include.
#define ACE_HAS_STRING_CLASS

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#else
# error unsupported compiler in ace/config-cygwin32.h
#endif /* __GNUG__ */

#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T
#define ACE_HAS_MSG
#define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_GETHOSTENT
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_LACKS_GETPGID_PROTOTYPE
#define ACE_LACKS_UNIX_SIGNALS
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Compiler/platform supports alloca().
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you want ACE to use it.
//#define ACE_HAS_ALLOCA

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#if !defined(ACE_HAS_IP_MULTICAST)
# define ACE_HAS_IP_MULTICAST
#endif /* #if ! defined(ACE_HAS_IP_MULTICAST) */

#define ACE_HAS_BIG_FD_SET

#define ACE_HAS_TERM_IOCTLS

#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65535

#define ACE_DEFAULT_SELECT_REACTOR_SIZE 256

#define ACE_HAS_GETPAGESIZE

#define ACE_LACKS_STRRECVFD

#define ACE_LACKS_MADVISE

#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

#define ACE_HAS_GPERF

#define ACE_HAS_DIRENT
#define ACE_HAS_STRPTIME

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_LACKS_MKFIFO

// Cygwin has no siginfo.h
#define ACE_LACKS_SIGINFO_H
// Cygwin has no ucontext.h
#define ACE_LACKS_UCONTEXT_H

#define ACE_HAS_AUTOMATIC_INIT_FINI

#if ACE_MT_SAFE
// Yes, we do have threads.
#  define ACE_HAS_THREADS
// And they're even POSIX pthreads (LinuxThreads implementation)
#  define ACE_HAS_PTHREADS

// Compiler/platform has thread-specific storage
#   define ACE_HAS_THREAD_SPECIFIC_STORAGE

#  if !defined (ACE_HAS_PTHREADS_UNIX98_EXT)
#    define ACE_LACKS_RWLOCK_T
#  endif  /* !ACE_HAS_PTHREADS_UNIX98_EXT */

// ... and the final standard even!
#  define ACE_HAS_PTHREADS_STD
// Cygwin (see pthread.h): Not supported or implemented.
#  define ACE_LACKS_THREAD_STACK_ADDR
#  define ACE_LACKS_SETSCHED
#  define ACE_LACKS_SETDETACH
#  define ACE_LACKS_PTHREAD_CANCEL
#endif  /* ACE_MT_SAFE */

#include "ace/post.h"

#endif /* ACE_CONFIG_CYGWIN32_H */
