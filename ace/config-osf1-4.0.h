/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the
// Digital UNIX V4.0a platforms with the DEC 5.4 C++ compiler.  It is
// configured to use the IEEE Std 1003.1c-1995, POSIX System
// Application Program Interface.  By 4.0a the version is meant that
// is called "V4.0 464" by uname -a.
     
#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// DJT removed this due to some minor issues related to the
// definitions of timestruc_t and tid_t in procfs.h not sure what
// functionality is lost?  Platform supports <sys/procfs.h>
//#define ACE_HAS_PROC_FS
 
// Platform defines MAP_FAILED as a long constant.
#define ACE_HAS_LONG_MAP_FAILED
 
// DJT 6/10/96 All these broken macro's can now be removed with the
// approporiate ordering of the include files. The Platinum release
// now temporarily supports both forms.  Platform's implementation of
// sendmsg() has a non-const msgheader parameter.
#define ACE_HAS_BROKEN_SENDMSG

// Platform's implementation of writev() has a non-const iovec
// parameter.
#define ACE_HAS_BROKEN_WRITEV

// Platform's implementation of setlrmit() has a non-const rlimit
// parameter.
#define ACE_HAS_BROKEN_SETRLIMIT

// Platform supports System V IPC (most versions of UNIX, but not
// Win32)
#define ACE_HAS_SYSV_IPC			

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform correctly calls init()/fini().
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// ACE has no mktemp().
//#define ACE_LACKS_MKTEMP

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform supports C++ headers
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Platform supports the OSF ACE_TLI timod STREAMS module.
#define ACE_HAS_OSF_TIMOD_H

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME

// Platform defines struct timespec in <sys/timers.h>
// #define ACE_HAS_BROKEN_POSIX_TIME 
// DJT added 6/5/96
// #define ACE_HAS_SVR4_TIME
//#define ACE_HAS_SVR4_TIME

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// DJT modified 6/5/96 
// ACE supports POSIX Pthreads.
//#define ACE_HAS_DCETHREADS
#define ACE_HAS_PTHREADS

#define ACE_LACKS_RWLOCK_T
// DJT 6/6/96 added
// IEEE Std 1003.1c-1995, POSIX System Application Program Interface
#define ACE_HAS_PTHREADS_1003_DOT_1C

// Explicit dynamic linking permits "lazy" symbol resolution
#define ACE_HAS_RTLD_LAZY_V

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Added 6/13/95, 1 line
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

// DJT added 6/7/96
#define ACE_HAS_SIGWAIT 

// Compiler/platform has ssize_t.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Platform has 64bit longs and 32bit ints...
#define ACE_HAS_64BIT_LONGS

// Platform supports STREAM pipes.
// #define ACE_HAS_STREAM_PIPES

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Platform support OSF1 gettimeofday 
#define ACE_HAS_OSF1_GETTIMEOFDAY

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform has strerror().
#define ACE_HAS_STRERROR

// ACE supports threads.
#define ACE_HAS_THREADS

// DJT modified 6/5/96
// Platform defines the tid_t structure.
//#define ACE_HAS_TID_T

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform supports ACE_TLI timod STREAMS module.
// #define ACE_HAS_TIMOD_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// DJT added 6/6/96
#define ACE_LACKS_CONDATTR_PSHARED

// DJT added 6/7/96
#define ACE_LACKS_THREAD_STACK_ADDR

// DJT added 7/10/96
#define ACE_LACKS_THREAD_PROCESS_SCOPING

// Compile using multi-thread libraries.
#define ACE_MT_SAFE

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// DJT modified 6/5/96
// Defines the page size of the system.
//#define ACE_PAGE_SIZE 4096
// This should really be set with the getpagesize() function since
// this is machine configurable on Digital UNIX, etc..
#define ACE_PAGE_SIZE 8192

// DJT added 6/6/96
// uses ctime_r & asctime_r with only two parameters vs. three
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

#define ACE_HAS_BROKEN_IF_HEADER
//#define ACE_HAS_REENTRANT_FUNCTIONS
#define DIGITAL_UNIX
#define ACE_LACKS_T_ERRNO
#define ACE_HAS_BROKEN_T_ERRNO
#define ACE_HAS_BROKEN_R_ROUTINES
// We need a larger per-thread stack size in order to run ACE_Log_Msg::log
// TK, 11 Nov 96
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 65536
#endif /* ACE_CONFIG_H */
