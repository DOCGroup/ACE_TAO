/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for DG/UX
// 4.11 platforms using the EPC compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#define ACE_DGUX

// Platform requires (struct sockaddr *) for msg_name field of struct
// msghdr.
#define ACE_HAS_SOCKADDR_MSG_NAME

// Platform lacks strcasecmp().
#define ACE_LACKS_STRCASECMP

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Platform supports system configuration information.
#define ACE_HAS_SYSINFO

// Platform supports the POSIX regular expression library.
#define ACE_HAS_REGEX

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
// #define ACE_HAS_SYS_SYSCALL_H

// Platform supports reentrant functions (i.e., all the POSIX *_r functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform has POSIX terminal interface.
#define ACE_HAS_TERMIOS

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Compiler/platform supports SunOS high resolution timers.
// #define ACE_HAS_HI_RES_TIMER

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Compiler/platform supports alloca()
// #define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
// #define ACE_HAS_ALLOCA_H

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME

// Platform supports the /proc file system.
#define ACE_HAS_PROC_FS

// Platform supports the prusage_t struct.
// #define ACE_HAS_PRUSAGE_T
#define ACE_HAS_GETRUSAGE

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T

// Compiler/platform provides the sockio.h file.
#define ACE_HAS_SYS_SOCKIO_H

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Platform supports STREAM pipes.
#define ACE_HAS_STREAM_PIPES

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Compiler/platform supports SVR4 dynamic linking semantics.
// #define ACE_HAS_SVR4_DYNAMIC_LINKING

// Compiler/platform supports SVR4 gettimeofday() prototype.
// #define ACE_HAS_SVR4_GETTIMEOFDAY

// DG/UX uses the same gettimeofday() prototype as OSF/1.
#define ACE_HAS_OSF1_GETTIMEOFDAY

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports SVR4 ACE_TLI (in particular, T_GETNAME stuff)...
#define ACE_HAS_SVR4_TLI

// Platform provides <sys/filio.h> header.
// #define ACE_HAS_SYS_FILIO_H

// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST

/* Turn off the following four defines if you want to disable threading. */
// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 0
#endif

// Platform supports threads.
// #define ACE_HAS_THREADS

// Platform supports POSIX pthreads *and* Solaris threads!
// #define ACE_HAS_STHREADS
// #define ACE_HAS_PTHREADS
#define ACE_HAS_SIGWAIT
// If ACE doesn't compile due to the lack of these methods, please
// send email to schmidt@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED

// Compiler/platform has thread-specific storage
//
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Reactor detects deadlock
// #define ACE_REACTOR_HAS_DEADLOCK_DETECTION

// Platform supports ACE_TLI timod STREAMS module.
#define ACE_HAS_TIMOD_H

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// Use the poll() event demultiplexor rather than select().
//#define ACE_USE_POLL

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// #define _USING_POSIX4A_DRAFT6
#define _POSIX_SOURCE
#define _DGUX_SOURCE
// #define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#define ACE_HAS_UCONTEXT_T
#define ACE_HAS_NONCONST_GETBY
#define ACE_LACKS_MADVISE

#if !defined (IP_ADD_MEMBERSHIP)
# define IP_ADD_MEMBERSHIP 0x13
#endif

#if !defined (IP_DROP_MEMBERSHIP)
# define IP_DROP_MEMBERSHIP 0x14
#endif

// Header files lack t_errno for ACE_TLI.
#define ACE_LACKS_T_ERRNO

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_IDTYPE_T

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
