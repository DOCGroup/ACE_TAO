/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for DG/UX
// 4.x platforms using the Green Hills Multi C++ compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#define ACE_DGUX

# define ACE_CONFIG_INCLUDE_GHS_COMMON
# include "ace/config-ghs-common.h"

// Static objects do not get initialized correctly, so this is needed.
#define ACE_HAS_NONSTATIC_OBJECT_MANAGER

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

// Platform supports reentrant functions (i.e., all the POSIX *_r functions).
//#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform has terminal ioctl flags like TCGETS and TCSETS.
#define ACE_HAS_TERM_IOCTLS

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

// Green Hills doesn't give a preprocessor symbol for long double
#define ACE_SIZEOF_LONG_DOUBLE 8

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
#define ACE_HAS_SOCKIO_H

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
#define ACE_HAS_SVR4_DYNAMIC_LINKING
// ... but redefines the entrypoint names with a leading underscore
#define dlopen  _dlopen
#define dlclose _dlclose
#define dlerror _dlerror
#define dlsym   _dlsym
// ... and uses 'char *' rather than 'const char *'
#define ACE_HAS_CHARPTR_DL

// DG/UX uses the same gettimeofday() prototype as OSF/1.
#define ACE_HAS_OSF1_GETTIMEOFDAY

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports SVR4 ACE_TLI (in particular, T_GETNAME stuff)...
#define ACE_HAS_SVR4_TLI

// Compiler/platform supports sys_siglist array.
// On DG/UX this is only done if the /usr/include/sys/_int_dg_features.h
// file determines it should be turned on.  It will take some work to
// figure out the magic options to turn it on reliably, so it'll have to
// be delayed for now.
//#define _TARGETTING_DGUXELF 1
//#define _IX86_DG 1
//#define _DGUX_TARGET 1
//#define ACE_HAS_SYS_SIGLIST

/* Turn off the following four defines if you want to disable threading. */
// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

// Platform supports threads.
#define ACE_HAS_THREADS
#define ACE_HAS_PTHREADS
// DG/UX claims to implement draft 10 of the pthreads standard (which became
// (with editorial change only) the final standard.  To get the behavior, some
// further macros need to be defined which are specific to DG/UX.
// _POSIX4A_DRAFT10_SOURCE turns on the draft 10 variant of threads.
// _POSIX4A_DRAFT_SOURCE turns on sched_yield().
#define _POSIX4A_DRAFT10_SOURCE
#define ACE_HAS_PTHREADS_STD
#define _POSIX4_DRAFT_SOURCE
// Well, here are some from the standard they don't have...
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_HAS_SIGWAIT
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_THREAD_PROCESS_SCOPING
// The default thread stacksize on DG/UX 4.x is 4096.  This needs to be bumped
// up to do anything real in ACE.
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 65536

// Need to #include <sched.h>
#define ACE_NEEDS_SCHED_H

// Compiler/platform has thread-specific storage
//
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Platform supports ACE_TLI timod STREAMS module.
#define ACE_HAS_TIMOD_H

// Platform supports ACE_TLI tiuser header.
#define ACE_HAS_TIUSER_H

// Platform provides ACE_TLI function prototypes.
#define ACE_HAS_TLI_PROTOTYPES

// Platform supports ACE_TLI.
#define ACE_HAS_TLI

// Platform doesn't have read-write locks natively.
#define ACE_LACKS_RWLOCK_T

// Platform doesn't have timespec_t data type.
#define ACE_LACKS_TIMESPEC_T

// Use the poll() event demultiplexor rather than select().
//#define ACE_USE_POLL

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
#define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_HAS_UCONTEXT_T
#define ACE_LACKS_SYSTIME_H
#define ACE_HAS_NONCONST_GETBY
#define ACE_LACKS_MADVISE

#if !defined (IP_ADD_MEMBERSHIP)
#define IP_ADD_MEMBERSHIP 0x13
#endif

#if !defined (IP_DROP_MEMBERSHIP)
# define IP_DROP_MEMBERSHIP 0x14
#endif

// Header files lack t_errno for ACE_TLI.
#define ACE_LACKS_T_ERRNO

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
