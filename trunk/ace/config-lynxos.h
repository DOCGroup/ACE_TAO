// $Id$

// The following configuration file is designed to work for LynxOS 2.50
// platforms using the GNU g++ compiler (version 2.7.2 or later).

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// System include files are not in sys/, this gets rid of warning.
#define __NO_INCLUDE_WARN__

#define ACE_HAS_STRING_CLASS
#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_HAS_UALARM
#define ACE_LACKS_UALARM_PROTOTYPE

// ACE_HAS_EXCEPTIONS requires -fhandle-exceptions, but that gives
// g++ 2.7.2 fits:  it spits out all kinds of warnings that it doesn't
// without that option (and that are just wrong), and runs out of
// virtual memory when trying to compile ace/Log_Msg.cpp.
// So until -fhandle-exceptions gets fixed, we can't use this with g++ . . .
// #define ACE_HAS_EXCEPTIONS

// G++ doesn't support template typedefs fully (yet).
// #define ACE_HAS_TEMPLATE_TYPEDEFS

// Must specialize templates due to G++'s lame parameterized type
// support...
#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

#define ACE_HAS_LYNXOS_SIGNALS

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

// Platform supports reentrant functions (i.e., all the POSIX *_r functions).
//#define ACE_HAS_REENTRANT_FUNCTIONS

// Compiler's template mechanism must see source code (i.e., .C files).
#define ACE_TEMPLATES_REQUIRE_SOURCE

// Denotes that GNU has cstring.h as standard
// which redefines memchr()
#define ACE_HAS_GNU_CSTRING_H

// Compiler doesn't support static data member templates.
#define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform supports SunOS high resolution timers.
//#define ACE_HAS_HI_RES_TIMER

// ACE_HAS_CLOCK_GETTIME requires linking with -lposix4.
#define ACE_HAS_CLOCK_GETTIME

// Platform passes timezone into gettimeofday call
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Platform does not have const anything
#define ACE_LACKS_CONST_TIMESPEC_PTR
#define ACE_HAS_NONCONST_GETBY
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_BROKEN_READV
#define ACE_HAS_BROKEN_SETRLIMIT
#define ACE_LACKS_POSIX_PROTO_FOR_SOME_FUNCS

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform has union semun defined
#define ACE_HAS_SEMUN

// Compiler/platform supports the "long long" datatype.
#define ACE_HAS_LONGLONG_T

// Compiler/platform supports alloca()
#define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

// Sockets may be called in multi-threaded programs.
#define ACE_HAS_MT_SAFE_SOCKETS

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_TIMESPEC_T

#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_MADVISE
#define ACE_LACKS_GETPGID

// in man pages, but no prototype in headers
#define ACE_LACKS_MKTEMP

// Platform supports the rusage struct.
#define ACE_HAS_GETRUSAGE

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_UCONTEXT_H
//#define ACE_HAS_UCONTEXT_T
//Platform has si_addr, but its not supported currently
#define ACE_LACKS_SI_ADDR


// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SOCKIO_H

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST

/* Turn off the following seven defines if you want to disable threading. */
// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
# define _REENTRANT
#endif

// Platform supports threads.
#define ACE_HAS_THREADS

//#define ACE_HAS_PTHREADS
#define ACE_HAS_DCETHREADS
#define ACE_HAS_DCE_DRAFT4_THREADS
#define ACE_HAS_PTHREADS_1003_DOT_1C
#define ACE_HAS_PTHREAD_GETSPECIFIC_DATAPTR
#define ACE_DOES_NOT_HAVE_SETKIND_NP
#define ACE_LACKS_THREAD_STACK_ADDR
#define ACE_LACKS_SETDETACH
#define ACE_LACKS_THREAD_PROCESS_SCOPING
// This gets around Lynx broken macro calls resulting in "::0"
#define _POSIX_THREADS_CALLS

// Platform lack reader/writer locks
#define ACE_LACKS_RWLOCK_T


// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

#define ACE_HAS_SIGWAIT
#define ACE_LACKS_CONDATTR_PSHARED


// Use the poll() event demultiplexor rather than select().
//#define ACE_USE_POLL

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_MALLOC_ALIGN 8

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#endif /* ACE_CONFIG_H */
