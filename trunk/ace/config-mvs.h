/* -*- C++ -*- */
// $Id$

// Config file for MVS with OpenEdition

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// The following #defines are hacks to get around things
// that seem to be missing or different in MVS land
#define MAXPATHLEN 1024          /* sys/param.h not on MVS */
#define NSIG 44                  /* missing from Signal.h */
#define MAXHOSTNAMELEN 256       /* missing form netdb.h */
#define howmany __howmany        /* MVS uses different names than most others */
#define fd_mask __fd_mask
#define MAXNAMLEN  __DIR_NAME_MAX
#define ERRMAX __sys_nerr
#if defined (log)                /* log is a macro in math.h */
#undef log			 /* conflicts with log function in ACE */
#endif /* log */

// See the README file in this directory 
// for a description of the following ACE_ macros

#if __COMPILER_VER__ >= 0x21020000   /* OS/390 r2 or higher */
  #define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#endif
#define ACE_HAS_BROKEN_CTIME
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_GETPAGESIZE
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_LIMITED_RUSAGE_T
#define ACE_HAS_MSG
#define ACE_HAS_ONEARG_SIGWAIT
#define ACE_HAS_POLL
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP
#define ACE_HAS_PTHREAD_DSTATE_PTR
#define ACE_HAS_PTHREAD_EQUAL
#define ACE_HAS_PTHREAD_GETSPECIFIC_DATAPTR
#define ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP
#define ACE_HAS_PTHREAD_T
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_SIG_C_FUNC
#define ACE_HAS_SIN_LEN
#define ACE_HAS_SIZET_SOCKET_LEN
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRERROR
#define ACE_HAS_STRINGS
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_THR_C_DEST
#define ACE_HAS_THR_C_FUNC
#define ACE_HAS_TID_T
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#define ACE_HAS_UTIME
#define ACE_HAS_VOIDPTR_MMAP
#define ACE_HAS_VOIDPTR_SOCKOPT
#define ACE_HAS_YIELD_VOID_PTR
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_IOSTREAM_FX
#define ACE_LACKS_KEYDELETE
#define ACE_LACKS_LINEBUFFERED_STREAMBUF
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MSGBUF_T
#define ACE_LACKS_PARAM_H
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SETSCHED
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYS_NERR
#define ACE_LACKS_TCP_H
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_LACKS_THREAD_STACK_ADDR
#define ACE_LACKS_UCONTEXT_H
#define ACE_MT_SAFE
#define ACE_NEEDS_DEV_IO_CONVERSION
#define ACE_NEEDS_SYSTIME_H
#define ACE_TEMPLATES_REQUIRE_SOURCE

#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */

