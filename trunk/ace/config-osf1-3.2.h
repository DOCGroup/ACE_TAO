/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for OSF1 3.2
// platforms with the DEC 5.1 C++ compiler.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#define ACE_HAS_PTHREAD_MUTEXATTR_CREATE	
#define ACE_HAS_PTHREAD_MUTEXATTR_DELETE	

#define ACE_HAS_BROKEN_MSG_H
#define ACE_LACKS_SYSV_MSQ_PROTOS

// Platform supports <sys/procfs.h>
#define ACE_HAS_PROC_FS

// Platform supports tid_t
#define ACE_HAS_TID_T
 
// Platform lacks support for shared condition variables
#define ACE_LACKS_CONDATTR_PSHARED

// Platform lacks support for stack address information
#define ACE_LACKS_THREAD_STACK_ADDR

// Platform lacks thread process scoping
#define ACE_LACKS_THREAD_PROCESS_SCOPING

// Platform has non-POSIX setkind and other functions.
#define ACE_HAS_SETKIND_NP
#define ACE_HAS_PTHREAD_T

// Platform defines MAP_FAILED as a long constant.
#define ACE_HAS_LONG_MAP_FAILED
 
// Platform's implementation of sendmsg() has a non-const msgheader parameter.
#define ACE_HAS_BROKEN_SENDMSG
 
// Platform's implementation of writev() has a non-const iovec parameter.
#define ACE_HAS_BROKEN_WRITEV
 
// Platform's implementation of setlrmit() has a non-const rlimit parameter.
#define ACE_HAS_BROKEN_SETRLIMIT
 
// Compiler has integer overflow problem with bit-shift operations.
#define ACE_HAS_BROKEN_BITSHIFT

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

// ACE has no mktemp().
#define ACE_LACKS_MKTEMP

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
#define ACE_HAS_BROKEN_POSIX_TIME 
// #define ACE_HAS_SVR4_TIME

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// ACE supports POSIX Pthreads.
#define ACE_HAS_DCETHREADS

// Explicit dynamic linking permits "lazy" symbol resolution
#define ACE_HAS_RTLD_LAZY_V

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
#define ACE_MT_SAFE

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#endif /* ACE_CONFIG_H */
