/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 11.x.  It includes all of the ACE information
// needed for HP-UX 11.x itself.  The compiler-specific information is in
// config-hpux-11.x-<compiler>.h - they include this file.

#ifndef ACE_CONFIG_H
#error "You need to use a compiler-specific .h file - they include this file"
#endif

// Compiling for HPUX.
#if !defined (HPUX)
#define HPUX
#endif /* HPUX */
#define HPUX_11

#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#endif

#include /**/ <sys/stdsyms.h>

////////////////////////////////////////////////////////////////////////////
//
// General OS information - see README for more details on what they mean
//
///////////////////////////////////////////////////////////////////////////

// HP/UX needs to have these addresses in a special range.
// If this is on a 64-bit model, the default is to use 64-bit addressing.
// It can also be set so that the mapped region is shareable with 32-bit
// programs.  To enable the 32/64 sharing, comment out the first definition
// of ACE_DEFAULT_BASE_ADDR and uncomment the two lines after it.
// Note - there's a compiler bug on aC++ A.03.04 in 64-bit mode which prevents
// these from working as-is.  So, there's some hackery in Naming_Context.cpp
// and Memory_Pool.cpp which works around it.  It requires the
// ACE_DEFAULT_BASE_ADDRL definition below - make sure it has the same
// value as what you use for ACE_DEFAULT_BASE_ADDR.  This is allegedly fixed
// in A.03.10 on the June Applications CD.
#if defined (__LP64__)
#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x0000001100000000)
//#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
//#  define ACE_OS_EXTRA_MMAP_FLAGS MAP_ADDR32

#  define ACE_DEFAULT_BASE_ADDRL (0x0000001100000000)
//#  define ACE_DEFAULT_BASE_ADDRL (0x80000000)
#else
#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
#endif  /* __LP64__ */

// Platform can do async I/O (aio_*)
#define ACE_HAS_AIO_CALLS
// ... but seems to require this in order to keep from hanging.  Needs some
// investigation, maybe with HP.  John Mulhern determined this value
// empirically.  YMMV.  If it does vary, set it up in your own config.h which
// then includes the ACE-supplied config.
#if !defined (ACE_INFINITE)
#  define ACE_INFINITE 10000000
#endif

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

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Platform supports the POSIX struct timespec type
#define ACE_HAS_POSIX_TIME

// Platform supports reentrant functions (all the POSIX *_r functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// HP-UX 11 has reentrant netdb functions.  The catch is that the old
// functions (gethostbyname, etc.) are thread-safe and the _r versions are
// not used and will be removed at some point.  So, define things so
// the _r versions are not used.  This will slow things down a bit due to
// the extra mutex lock in the ACE_NETDBCALL_RETURN macro, and will be fixed
// in the future (problem ID P64).
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals
#define ACE_HAS_SIGINFO_T

// Platform doesn't detect a signal out of range unless it's way out of range.
#define ACE_HAS_SIGISMEMBER_BUG

#define ACE_HAS_UALARM

// Platform supports ucontext_t (which is used in the extended signal API).
#define ACE_HAS_UCONTEXT_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// SunOS 4 style prototype for gettimeofday
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// Platform supports SVR4 dynamic linking semantics, in 64-bit mode only.
// When used, this requires -ldl on the ACE library link line.
#ifdef __LP64__
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#endif

// HP/UX has an undefined syscall for GETRUSAGE...
#define ACE_HAS_SYSCALL_GETRUSAGE
// Note, this only works if the flag is set above!
#define ACE_HAS_GETRUSAGE

// Platform has the sigwait function in a header file
#define ACE_HAS_SIGWAIT

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// accept() is thread-safe
#define ACE_HAS_THREAD_SAFE_ACCEPT

// Platform has XPG4 wide character support
#define ACE_HAS_XPG4_MULTIBYTE_CHAR

// Platform lacks readers/writer locks.
// For now... it does have pthread read/write locks but the code in OS.{h i}
// doesn't know how to do pthread read/write locks.
#define ACE_LACKS_RWLOCK_T

// Platform lacks a typedef for timespec_t, but has struct timespec
#define ACE_LACKS_TIMESPEC_T

// Platform needs a timer skew value.  It *may* vary by processor, but this
// one works.  You can override it in your config.h file if needed.
// It's in units of microseconds.  This value is 10 msec.
#if !defined (ACE_TIMER_SKEW)
#  define ACE_TIMER_SKEW 10 * 1000
#endif /* ACE_TIMER_SKEW */

// Shared library name/path components
#define ACE_DLL_SUFFIX  ".sl"
#if defined (__LP64__)
#  define ACE_LD_SEARCH_PATH "LD_LIBRARY_PATH"
#else
#  define ACE_LD_SEARCH_PATH "SHLIB_PATH"
#endif  /* __LP64__ */

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
// Well, it really does, but it depends on preprocessor defines.
#define ACE_LACKS_CONST_STRBUF_PTR

// Platform supports STREAM pipes
// This is possible, but not by default - need to rebuild the kernel to
// get them enabled - see pipe(2) and "STREAMS/UX for the HP 9000"
// #define ACE_HAS_STREAM_PIPES

/////////////////////////////////////////////////////////////////////////
//
// TLI/XTI information
//
////////////////////////////////////////////////////////////////////////

// Platform supports XTI (includes TLI), including SVR4 facilities.
#define ACE_HAS_TLI
// ACE_HAS_SVR4_TLI should work on HP-UX, but doesn't yet.  Riverace
// problem ID P27.
//#define ACE_HAS_SVR4_TLI
// Platform uses <xti.h>, not tiuser.h
#define ACE_HAS_XTI
// But it has _terrno() outside the extern "C" stuff.
#define ACE_HAS_TIUSER_H_BROKEN_EXTERN_C
// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES
// HP-UX 11.00 (at least at initial releases) has some busted macro defs
#define ACE_HAS_BROKEN_XTI_MACROS

/////////////////////////////////////////////////////////////////////////
//
// Threads information.
//
// Use of threads is controlled by the 'threads' argument to make.  See
// include/makeinclude/platform_hpux_aCC.GNU for details.
//
////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_THREADS)

#  if !defined (ACE_MT_SAFE)
#    define ACE_MT_SAFE 1
#  endif

#  define ACE_HAS_PTHREADS
#  define ACE_HAS_PTHREADS_STD
#  define ACE_HAS_PTHREAD_SIGMASK

#  define ACE_HAS_THREAD_SPECIFIC_STORAGE
#endif /* ACE_HAS_THREADS */

#define ACE_HAS_POSIX_SEM

// Turns off the tracing feature.
// To build with tracing enabled, make sure ACE_NTRACE is not defined
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */
