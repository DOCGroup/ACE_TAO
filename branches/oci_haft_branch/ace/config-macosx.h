/* -*- C++ -*- */
// $Id$

// This configuration file is designed to work with the MacOS X operating system.

#ifndef ACE_CONFIG_MACOSX_H
#define ACE_CONFIG_MACOSX_H

#if ! defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#endif /* __GNUG__ */

#undef ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

#define ACE_SIZE_T_FORMAT_SPECIFIER ACE_LIB_TEXT ("%lu")

#if defined (ACE_HAS_PENTIUM)
# undef ACE_HAS_PENTIUM
#endif /* ACE_HAS_PENTIUM */

#if !defined (_THREAD_SAFE)
#define _THREAD_SAFE
#endif /* _THREAD_SAFE */

#define ACE_HAS_GPERF
#define ACE_HAS_POSIX_SEM

//#define ACE_HAS_SVR4_TLI

#define ACE_HAS_MEMCHR

#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_MSG_H
//
// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Compiler/platform supports SVR4 signal typedef
#define ACE_HAS_SVR4_SIGNAL_T

//Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT

//Platform supports sigsuspend()
#define ACE_HAS_SIGSUSPEND

//Platform/compiler has macros for sig{empty,fill,add,del}set (e.g., SCO and FreeBSD)
#define ACE_HAS_SIG_MACROS

//#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_LACKS_GETPGID
#define ACE_LACKS_RWLOCK_T

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_HAS_NONCONST_SELECT_TIMEVAL

//?
#define ACE_LACKS_SIGSET

#define ACE_NEEDS_SCHED_H

// Use of <malloc.h> is deprecated.
#define ACE_LACKS_MALLOC_H

#define ACE_HAS_ALT_CUSERID

// Platform supports POSIX timers via struct timespec.
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_UALARM

// Platform defines struct timespec but not timespec_t
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_STRRECVFD

#define ACE_HAS_SIN_LEN

#define ACE_HAS_ANSI_CASTS

// Platform supports System V IPC (most versions of UNIX, but not Win32)
//#define ACE_HAS_SYSV_IPC

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform supports alloca().
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Explicit dynamic linking permits "lazy" symbol resolution
//#define ACE_HAS_RTLD_LAZY_V

// platform supports POSIX O_NONBLOCK semantics
#define ACE_HAS_POSIX_NONBLOCK

// platform supports IP multicast
#define ACE_HAS_IP_MULTICAST
#define ACE_LACKS_PERFECT_MULTICAST_FILTERING 1

//platform supports snprintf ()
#define ACE_HAS_SNPRINTF

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Defines the page size of the system.
#define ACE_HAS_GETPAGESIZE

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports SVR4 gettimeofday() prototype
#define ACE_HAS_SUNOS4_GETTIMEOFDAY
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_LACKS_SYSV_MSG_H
#define ACE_HAS_MSG
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_NONCONST_MSGSND

// Thread specific settings
// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif /* ! ACE_MT_SAFE */
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_HAS_PTHREAD_KILL
#define ACE_HAS_PTHREAD_CANCEL
#define ACE_HAS_PTHREAD_SIGMASK
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_DIRENT
#define ACE_LACKS_SYSTIME_H

#define ACE_LACKS_SETSCHED
//#define ACE_HAS_RECURSIVE_MUTEXES

#define ACE_HAS_TERM_IOCTLS
#define TCGETS TIOCGETA
#define TCSETS TIOCSETA

#define ACE_HAS_SEMUN
#define ACE_HAS_SIGINFO_T 
#define ACE_LACKS_SIGINFO_H 
#define ACE_HAS_UCONTEXT_T 
#define ACE_HAS_GETIFADDRS 
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES 
#define ACE_LACKS_UNNAMED_SEMAPHORE

// dlcompat package (not part of base Darwin) is needed for dlopen().
// You may download directly from sourceforge and install or use fink
// Fink installer puts libraries in /sw/lib and headers in /sw/include
// In order to install dlcompat do the following:
//   - download fink from http://fink.sf.net
//   - type:
//        fink install dlcompat
// as of Dec 2002, if you use fink you will need to uncomment the next line
//#define ACE_NEEDS_DL_UNDERSCORE
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_LD_SEARCH_PATH ACE_LIB_TEXT ("DYLD_LIBRARY_PATH")
#define ACE_DLL_SUFFIX ACE_LIB_TEXT (".dylib")
#define ACE_LACKS_DLCLOSE

// gperf seems to need this
#define ACE_HAS_NONSTATIC_OBJECT_MANAGER

#if defined(__APPLE_CC__) && (__APPLE_CC__ < 1173)
#error "Compiler must be upgraded, see http://developer.apple.com"
#endif /* __APPLE_CC__ */

#endif /* ACE_CONFIG_MACOSX_H */
