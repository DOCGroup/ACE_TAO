/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 10.x.  It includes all of the ACE information
// needed for HP-UX 10.x itself.  The compiler-specific information is in
// config-hpux-10.x-<compiler>.h - they include this file.

#ifndef ACE_CONFIG_H
#error "You need to use a compiler-specific .h file - they include this file"
#endif

// Compiling for HPUX.
#if !defined (HPUX)
#define HPUX
#endif /* HPUX */
#define HPUX_10

#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#include "ace/pre.h"
#endif

// Some things are different for 10.10 vs. 10.20 vs. 10.30
// If the version number wasn't set up by the compiler command line,
// set up as if it was 10.20.
#if !defined (HPUX_VERS)
#define HPUX_VERS 1020
#endif

#if (HPUX_VERS < 1020)                  // 10.10
#  define ACE_HAS_BROKEN_MMAP_H
#  define ACE_LACKS_T_ERRNO
#  define ACE_LACKS_TIMESPEC_T
#elif (HPUX_VERS < 1030)                // 10.20

   // Platform supports reentrant functions (all the POSIX *_r functions).
#  define ACE_HAS_REENTRANT_FUNCTIONS
   // But this one is not like other platforms
#  define ACE_CTIME_R_RETURNS_INT
   // And _REENTRANT must be set, even if not using threads.
#  if !defined (_REENTRANT)
#    define _REENTRANT
#  endif /* _REENTRANT */

#else                                   // 10.30
// Don't know yet... probably will be 10.20 but with some different thread
// settings.
#endif /* HPUX_VERS tests */

#include /**/ <sys/stdsyms.h>
#include /**/ <sched.h>         /*  pthread.h doesn't include this */

extern int h_errno;     /* This isn't declared in a header file on HP-UX */

// HP-UX is a POSIX-compliant system - see what's available.
#include "ace/config-posix.h"


////////////////////////////////////////////////////////////////////////////
//
// General OS information - see README for more details on what they mean
//
///////////////////////////////////////////////////////////////////////////

// HP/UX needs to have these addresses in a special range.
#define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H
// But doesn't have a prototype for syscall()
#define ACE_LACKS_SYSCALL

// Platform supports POSIX 1.b clock_gettime ()
#define ACE_HAS_CLOCK_GETTIME

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform has Dirent iterator functions.
#define ACE_HAS_DIRENT

// Platform supports getpagesize() call
#define ACE_HAS_GETPAGESIZE
// But we define this just to be safe
#define ACE_PAGE_SIZE 4096

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// select's timeval arg is non-const
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Platform supports the POSIX struct timespec type
#define ACE_HAS_POSIX_TIME

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals
#define ACE_HAS_SIGINFO_T

// Platform doesn't detect a signal out of range unless it's way out of range.
#define ACE_HAS_SIGISMEMBER_BUG

// Platform supports ucontext_t (which is used in the extended signal API).
#define ACE_HAS_UCONTEXT_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// SunOS 4 style prototype for gettimeofday
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// HP/UX has an undefined syscall for GETRUSAGE...
#define ACE_HAS_SYSCALL_GETRUSAGE
// Note, this only works if the flag is set above!
#define ACE_HAS_GETRUSAGE

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

#define ACE_HAS_UALARM

// Platform has XPG4 wide character support
#define ACE_HAS_XPG4_MULTIBYTE_CHAR

// Platform lacks readers/writer locks.
#define ACE_LACKS_RWLOCK_T

// Shared library path/search components
#define ACE_DLL_SUFFIX      ".sl"
#define ACE_LD_SEARCH_PATH  "SHLIB_PATH"

//////////////////////////////////////////////////////////////////////////
//
// STREAMS information
//
//////////////////////////////////////////////////////////////////////////

// Platform supports STREAMS
#define ACE_HAS_STREAMS
// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T
// But the putmsg signature doesn't have it as const...
#define ACE_LACKS_CONST_STRBUF_PTR

// Platform supports STREAM pipes
// This is possible, but not by default - need to rebuild the kernel to
// get them enabled - see pipe(2) and "STREAMS/UX for the HP 9000"
// #define ACE_HAS_STREAM_PIPES

/////////////////////////////////////////////////////////////////////////
//
// TLI information
//
////////////////////////////////////////////////////////////////////////

// Platform supports ACE_TLI, including SVR4 facilities.
#define ACE_HAS_TLI

// t_error's arg is char *, not const char *
#define ACE_HAS_BROKEN_T_ERROR
// ACE_HAS_SVR4_TLI should work on HP-UX, but doesn't yet.  Riverace
// problem ID P27.
//#define ACE_HAS_SVR4_TLI
// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H
// But it has _terrno() outside the extern "C" stuff.
#define ACE_HAS_TIUSER_H_BROKEN_EXTERN_C
// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES
// Platform uses a TCP TLI device other than /dev/tcp.  Uses XTI only.
#define ACE_TLI_TCP_DEVICE  "/dev/inet_cots"

/////////////////////////////////////////////////////////////////////////
//
// Threads information.
// Threads definitions are controlled by the threads setting in the
// include/makeinclude/platform_hpux_aCC.GNU file - see that for details.
// If you build with threads support, the DCE Core subset must be installed
// from the core OS CD.
//
////////////////////////////////////////////////////////////////////////

#ifdef ACE_HAS_THREADS
#  if !defined (ACE_MT_SAFE)
        #define ACE_MT_SAFE 1
#  endif

#  define ACE_HAS_PTHREADS
#  define ACE_HAS_PTHREADS_DRAFT4
// POSIX real-time semaphore definitions are in the header files, and it
// will compile and link with this in place, but will not run.  HP says
// the functions are not implemented.
//#  define ACE_HAS_POSIX_SEM

#  define ACE_HAS_THREAD_SPECIFIC_STORAGE

// They forgot a const in the prototype of pthread_cond_timedwait
#  define ACE_LACKS_CONST_TIMESPEC_PTR

// Platform lacks pthread_thr_sigsetmask
#  define ACE_LACKS_PTHREAD_THR_SIGSETMASK

// Platform has no implementation of pthread_condattr_setpshared()
#  define ACE_LACKS_CONDATTR_PSHARED

// Platform lacks pthread_attr_setdetachstate()
#  define ACE_LACKS_SETDETACH

// Platform lacks pthread_attr_setscope
#  define ACE_LACKS_THREAD_PROCESS_SCOPING

// Platform lacks pthread_attr_setstackaddr
#  define ACE_LACKS_THREAD_STACK_ADDR

// If this is not turned on, the CMA wrappers will redefine a bunch of
// system calls with wrappers - one being select() and it only defines
// select with int arguments (not fd_set).  So, as long as _CMA_NOWRAPPERS_
// is set, the regular fd_set arg types are used for select().
// Without threads being compiled in, the fd_set/int thing is not an issue.
#  define _CMA_NOWRAPPERS_

#else
// If threading is disabled, then timespec_t does not get defined.
#  ifndef ACE_LACKS_TIMESPEC_T
#    define ACE_LACKS_TIMESPEC_T
#  endif
#endif /* ACE_HAS_THREADS */

// Manually tweaking malloc paddings.
#define ACE_MALLOC_PADDING 16
#define ACE_MALLOC_ALIGN 8
#define ACE_CONTROL_BLOCK_ALIGN_LONGS 0
#define ACE_PI_CONTROL_BLOCK_ALIGN_LONGS 2

// Turns off the tracing feature.
// To build with tracing enabled, make sure ACE_NTRACE is not defined
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#include "ace/post.h"
#endif /* ACE_NTRACE */
