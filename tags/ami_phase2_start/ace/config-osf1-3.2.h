/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for OSF1 3.2
// platforms with the DEC 5.1 C++ compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#define ACE_LACKS_SETSCHED
#define ACE_LACKS_RWLOCK_T
// DF: this platform uses Digital's CXX compiler
#define DEC_CXX

// DF: DEC's CXX supports explicit template specialization.
#define ACE_HAS_TEMPLATE_SPECIALIZATION

// DF: 3.2 has getpgid but no prototype defined anywhere.  So we cheat
// and declare it here.
extern "C" pid_t getpgid (pid_t);

// DF: ACE_HAS_STRING_CLASS seems the right thing to do...
#define ACE_HAS_STRING_CLASS

// DF: Seems apropriate since this is a new compiler...
#if !defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#define ACE_HAS_BROKEN_MSG_H
#define ACE_LACKS_SYSV_MSQ_PROTOS

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Platform supports <sys/procfs.h>
#define ACE_HAS_PROC_FS

#define ACE_HAS_UALARM

// If ACE doesn't compile due to the lack of these methods, please
// send email to schmidt@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED

// Platform lacks support for stack address information
#define ACE_LACKS_THREAD_STACK_ADDR

// Platform lacks thread process scoping
#define ACE_LACKS_THREAD_PROCESS_SCOPING

// Platform has non-POSIX setkind and other functions.
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_HAS_SETKIND_NP

// Platform supports POSIX 1.b clock_gettime ()
#define ACE_HAS_CLOCK_GETTIME

// Platform defines MAP_FAILED as a long constant.
#define ACE_HAS_LONG_MAP_FAILED

// Platform's implementation of sendmsg() has a non-const msgheader parameter.
#define ACE_HAS_BROKEN_SENDMSG

// Platform's implementation of writev() has a non-const iovec parameter.
#define ACE_HAS_BROKEN_WRITEV

// Platform's implementation of setlrmit() has a non-const rlimit parameter.
#define ACE_HAS_BROKEN_SETRLIMIT

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform correctly calls init()/fini().
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

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

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME

// Platform defines struct timespec in <sys/timers.h>
#define ACE_HAS_BROKEN_POSIX_TIME

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

#define ACE_LACKS_PTHREAD_THR_SIGSETMASK

// ACE supports POSIX Pthreads. OSF/1 3.2 has draft 4
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_DRAFT4
#define ACE_HAS_THREAD_SELF

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Added 6/13/95, 1 line
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

// Compiler/platform has ssize_t.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Platform has 64bit longs and 32bit ints...
// NOTE: ACE_HAS_64BIT_LONGS is deprecated.  Instead, use ACE_SIZEOF_LONG == 8.
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

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform supports ACE_TLI timod STREAMS module.
// #define ACE_HAS_TIMOD_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#define ACE_NEEDS_DEV_IO_CONVERSION

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Defines the page size of the system.
#define ACE_PAGE_SIZE 8192
#define ACE_HAS_GETPAGESIZE

#endif /* ACE_CONFIG_H */
