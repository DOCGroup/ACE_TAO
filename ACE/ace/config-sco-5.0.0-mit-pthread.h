/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SCO UNIX
// version 5.0 with MIT pthreads.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#if !defined (__ACE_INLINE__)
#define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
  // This config file has not been tested with ACE_HAS_TEMPLATE_SPECIALIZATION.
  // Maybe it will work?
# undef ACE_HAS_TEMPLATE_SPECIALIZATION
#endif /* __GNUG__ */

// Compiling for SCO.
#if !defined (SCO)
#define SCO
#define _SVID3
#endif /* SCO */

#define ACE_DEFAULT_CLOSE_ALL_HANDLES 0
#define ACE_HAS_SIG_MACROS
// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#if defined (SCO) && !defined (MAXPATHLEN)
#define MAXPATHLEN 1023
#endif /* SCO */

#define ACE_LACKS_PWD_FUNCTIONS
#define ACE_HAS_BIG_FD_SET

#define ACE_LACKS_RLIMIT
#define ACE_LACKS_MADVISE

#define ACE_HAS_REENTRANT_FUNCTIONS

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC
#define ACE_HAS_NONCONST_MSGSND
#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI

// Platform has POSIX terminal interface.
#define ACE_HAS_TERMIOS

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform defines the sig_atomic_t typedef
#define ACE_HAS_SIG_ATOMIC_T

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// ???
// #define ACE_HAS_VOIDPTR_GETTIMEOFDAY
#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Platform uses int for select() rather than fd_set.
#define ACE_HAS_SELECT_H

// Platform has prototypes for ACE_TLI.
#define ACE_HAS_TLI
#define ACE_HAS_TLI_PROTOTYPES
#define ACE_HAS_TIMOD_H
#define ACE_HAS_TIUSER_H
#define ACE_LACKS_T_ERRNO

#define ACE_HAS_GNU_CSTRING_H

#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

#define ACE_HAS_INLINED_OSCALLS

#define ACE_HAS_STRBUF_T
#define ACE_HAS_STREAMS
#define ACE_HAS_IP_MULTICAST

// Threads
#define ACE_HAS_THREADS
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_LACKS_PTHREAD_CANCEL
#define ACE_HAS_SIGWAIT
#define ACE_LACKS_THREAD_PROCESS_SCOPING
//#define ACE_LACKS_THREAD_STACK_ADDR
// If ACE doesn't compile due to the lack of these methods, please
// send email to schmidt@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SETSCHED
#define ACE_HAS_POSIX_TIME

#include <pthread.h>
#include <sys/regset.h>

#define ACE_LACKS_TIMEDWAIT_PROTOTYPES
#define ACE_HAS_RECV_TIMEDWAIT
#define ACE_HAS_RECVFROM_TIMEDWAIT
#define ACE_HAS_RECVMSG_TIMEDWAIT
#define ACE_HAS_SEND_TIMEDWAIT
#define ACE_HAS_SENDTO_TIMEDWAIT
#define ACE_HAS_SENDMSG_TIMEDWAIT
#define ACE_HAS_READ_TIMEDWAIT
#define ACE_HAS_READV_TIMEDWAIT
#define ACE_HAS_WRITE_TIMEDWAIT
#define ACE_HAS_WRITEV_TIMEDWAIT
#define ACE_HAS_DIRENT

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
