/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for HP
// platforms running HP/UX 10.x and using the HP/aCC compiler.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// Compiling for HPUX.
#if !defined (HPUX)
#define HPUX
#endif /* HPUX */

#if __cplusplus < 199707L
#define ACE_HAS_BROKEN_HPUX_TEMPLATES
#endif /* __cplusplus < 199707L */

#define _HPUX_SOURCE
#include /**/ <sys/stdsyms.h>
#include /**/ <sched.h> /*  pthread.h doesn't include this */

#define ACE_LACKS_STREAM_MODULES
#define ACE_HAS_BROKEN_CONVERSIONS

// They forgot a const in the prototype of putmsg and putpmsg...
#define ACE_LACKS_CONST_STRBUF_PTR

// They forgot a const in the prototype of const_timewait...
#define ACE_LACKS_CONST_TIMESPEC_PTR

#define ACE_HAS_EXCEPTIONS

#define ACE_HAS_TYPENAME_KEYWORD
#define ACE_HAS_TEMPLATE_TYPEDEFS
#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_LACKS_SYSCALL
#define ACE_HAS_POSIX_TIME

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Fixes a problem with HP/UX not wrapping the mmap(2) header files
// with extern "C".
#define ACE_HAS_BROKEN_MMAP_H

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Header files lack t_errno for ACE_TLI.
#define ACE_LACKS_T_ERRNO

// Compiler/platform supports poll().
#define ACE_HAS_POLL

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// #define ACE_HAS_THREADS
// #define ACE_HAS_PTHREADS
// #define ACE_MT_SAFE
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_HAS_SETKIND_NP
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SETDETACH                     /*  new */
#define ACE_LACKS_KEYDELETE                     /*  new */
#define ACE_LACKS_THREAD_PROCESS_SCOPING
#define ACE_LACKS_THREAD_STACK_ADDR

// Compiler/platform has thread-specific storage
// #define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE

// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform supports the tid_t type (e.g., AIX and Irix 6.2)
#define ACE_HAS_TID_T

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Platform supports POSIX 1.b clock_gettime ()
#define ACE_HAS_CLOCK_GETTIME

// iostream header lacks ipfx (), isfx (), etc., declarations
#define ACE_LACKS_IOSTREAM_FX

// streambuf class lacks linebuffered() method.
#define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAM_PIPES
#define ACE_HAS_STREAMS

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// ???
#define ACE_HAS_SUNOS4_GETTIMEOFDAY

// HP/UX has an undefined syscall for GETRUSAGE...
#define ACE_HAS_SYSCALL_GETRUSAGE

// Note, this only works if the flag is set above!
#define ACE_HAS_GETRUSAGE

// Platform uses int for select() rather than fd_set.
// #define ACE_SELECT_USES_INT

// Platform has the XLI version of ACE_TLI.
// #define ACE_HAS_XLI

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

#define ACE_NEEDS_DEV_IO_CONVERSION

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#endif /* ACE_CONFIG_H */
