/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Motorola
// 88k SVR4 platforms using pthreads from Florida State (ACE_HAS_FSU_PTHREADS).

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
  // This config file has not been tested with ACE_HAS_TEMPLATE_SPECIALIZATION.
  // Maybe it will work?
# undef ACE_HAS_TEMPLATE_SPECIALIZATION
#endif /* __GNUG__ */

#if !defined (m88k)
#define m88k
#endif

extern "C" void pthread_init();

#define PTHREAD_STACK_MIN 1024

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#if !defined (IP_ADD_MEMBERSHIP)
#define IP_ADD_MEMBERSHIP 0x13
#endif  /*  m88k */

#if !defined (IP_DROP_MEMBERSHIP)
#define IP_DROP_MEMBERSHIP 0x14
#endif  /*  m88k */

struct sched_param
{
  int sched_priority;
  int prio;
};

// This seems to be necessary for m88k.
struct ip_mreq
{
  struct in_addr imr_multiaddr; // IP multicast address of the group
  struct in_addr imr_interface; // local IP address of the interface
};

#if !defined (ACE_HAS_FSU_PTHREADS)
# define ACE_HAS_FSU_PTHREADS
#endif
#if !defined (ACE_HAS_PTHREADS_DRAFT6)
# define ACE_HAS_PTHREADS_DRAFT6
#endif

// Added for compilation on the m88k
#if defined (m88k)
# define ACE_LACKS_T_ERRNO
# define ACE_LACKS_MADVISE
# define ACE_HAS_GNU_CSTRING_H
#endif  /* m88k */

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Sun has the wrong prototype for sendmsg.
#define ACE_HAS_NONCONST_SENDMSG

// The SunOS 5.x version of rand_r is inconsistent with the header files...
#define ACE_HAS_BROKEN_RANDR

// Platform supports system configuration information.
#define ACE_HAS_SYSINFO

// Platform supports the POSIX regular expression library.
#define ACE_HAS_REGEX

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYS_SYSCALL_H

#if !defined (ACE_HAS_FSU_PTHREADS)
// Platform supports reentrant functions (i.e., all the POSIX *_r functions).
#define ACE_HAS_REENTRANT_FUNCTIONS
#endif  /* ACE_HAS_FSU_PTHREADS */

// Platform has POSIX terminal interface.
#define ACE_HAS_TERMIOS 

// Compiler/platform correctly calls init()/fini() for shared libraries.
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

#if !defined (m88k)
// Compiler/platform supports SunOS high resolution timers.
# define ACE_HAS_HI_RES_TIMER
#endif  /* m88k */

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

// Compiler/platform supports alloca()
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA

#if !defined (m88k)
// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H
#endif  /* m88k */

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME

// Platform supports the /proc file system.
#define ACE_HAS_PROC_FS

#if !defined (m88k)
// Platform supports the prusage_t struct.
#define ACE_HAS_PRUSAGE_T
#endif  /* m88k */

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

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
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Compiler/platform supports SVR4 gettimeofday() prototype.
#define ACE_HAS_SVR4_GETTIMEOFDAY

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_SVR4_SIGNAL_T

// Compiler/platform supports SVR4 ACE_TLI (in particular, T_GETNAME stuff)...
#define ACE_HAS_SVR4_TLI

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

#if !defined (m88k)
// Compiler/platform supports sys_siglist array.
#define ACE_HAS_SYS_SIGLIST
#endif  /* m88k */

/* Turn off the following five defines if you want to disable threading. */
// Compile using multi-thread libraries.
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#define ACE_HAS_PTHREADS
#define ACE_LACKS_RWLOCK_T

// Platform supports threads.
#define ACE_HAS_THREADS

#if defined (ACE_HAS_FSU_PTHREADS)
#define ACE_LACKS_THREAD_STACK_ADDR
#endif  /*  ACE_HAS_FSU_PTHREADS */

// Compiler/platform has thread-specific storage
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

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
