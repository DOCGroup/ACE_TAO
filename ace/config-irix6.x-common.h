/* -*- C++ -*- */
//
// $Id$
//
// This file contains the common configuration options for both
// SGI/MIPSPro C++ and g++ under IRIX 6.X
//
#ifndef ACE_CONFIG_IRIX6X_COMMON_H

#if !(defined(ACE_CONFIG_H) || defined(ACE_CONFIG_IRIX6X_NTHR_H))
#error "This file may only be included via config.h or config-irix6.x-nothreads.h"
#endif

#if (defined(ACE_CONFIG_H) && defined(ACE_CONFIG_IRIX6X_NTHR_H))
#error "May only be included via config.h *OR* config-irix6.x-nothreads.h, not both!"
#endif

// The Irix 6.x float.h doesn't allow us to distinguish between a
// double and a long double.  So, we have to hard-code this.  Thanks
// to Bob Laferriere <laferrie@gsao.med.ge.com> for figuring it out.
#if defined (_MIPS_SIM)             /* 6.X System */
# include <sgidefs.h>
# if defined (__GNUC__) /* GNU Compiler reports long doubles as 8 bytes */
#   define ACE_SIZEOF_LONG_DOUBLE 8
# elif defined (_MIPS_SIM_NABI32) && (_MIPS_SIM == _MIPS_SIM_NABI32)
#   define ACE_SIZEOF_LONG_DOUBLE 16
# elif defined (_MIPS_SIM_ABI32) && (_MIPS_SIM == _MIPS_SIM_ABI32)
#   define ACE_SIZEOF_LONG_DOUBLE 8
# elif defined (_MIPS_SIM_ABI64) && (_MIPS_SIM == _MIPS_SIM_ABI64)
#   define ACE_SIZEOF_LONG_DOUBLE 16
# elif !defined (ACE_SIZEOF_LONG_DOUBLE)
#   define ACE_SIZEOF_LONG_DOUBLE 8
# endif
#else
# define ACE_SIZEOF_LONG_DOUBLE 8   /* 5.3 System */
#endif

// petern, Next part of it:

// Platform supports getpagesize() call.
#define ACE_HAS_GETPAGESIZE

// Platform has no implementation of pthread_condattr_setpshared(),
// even though it supports pthreads! (like Irix 6.2)
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_MUTEXATTR_PSHARED

// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIGTIMEDWAIT
#define ACE_HAS_SIGSUSPEND

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Platform requires void * for mmap().
#define ACE_HAS_VOIDPTR_MMAP

// Platform supports recvmsg and sendmsg.
#define ACE_HAS_MSG

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYSCALL_H

// Compiler/platform supports alloca()
#define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H

// Irix needs to define bzero() in this odd file <bstring.h>
#define ACE_HAS_BSTRING

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE

// Platform supports POSIX O_NONBLOCK semantics.
#define ACE_HAS_POSIX_NONBLOCK

// Compiler/platform has correctly prototyped header files.
#define ACE_HAS_CPLUSPLUS_HEADERS

// Platform contains <poll.h>.
#define ACE_HAS_POLL

// No multi-threading so use poll() call
// - for easier debugging, if nothing else
// #define ACE_USE_POLL

// Platform supports the /proc file system.
#define ACE_HAS_PROC_FS

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Platform supports SVR4 extended signals.
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_UCONTEXT_T

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Platform supports STREAMS.
#define ACE_HAS_STREAMS

// Platform supports STREAM pipes (note that this is disabled by
// default, see the manual page on pipe(2) to find out how to enable
// it).
// #define ACE_HAS_STREAM_PIPES

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Compiler/platform supports struct strbuf.
#define ACE_HAS_STRBUF_T

// Compiler/platform supports SVR4 dynamic linking semantics.
#define ACE_HAS_SVR4_DYNAMIC_LINKING

// Platform provides <sys/filio.h> header.
#define ACE_HAS_SYS_FILIO_H

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

// Platform supports IP multicast
#define ACE_HAS_IP_MULTICAST

#define ACE_HAS_STRPTIME

//**************************************************************
// Not so sure how next lines should look like

// Platform supports POSIX timers via timestruc_t.
#define ACE_HAS_POSIX_TIME
#define ACE_LACKS_SYSTIME_H

//**************************************************************

// IRIX 6.4 and below do not support reentrant netdb functions
// (getprotobyname_r, getprotobynumber_r, gethostbyaddr_r,
// gethostbyname_r, getservbyname_r).
#if ACE_IRIX_VERS <= 64 && !defined (ACE_HAS_NETDB_REENTRANT_FUNCTIONS)
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#endif /* ACE_HAS_NETDB_REENTRANT_FUNCTIONS */

#define ACE_HAS_DIRENT
// Unless the thread enabled version is used the readdir_r interface
// does not get defined in IRIX 6.2
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_RWLOCK_T

#define ACE_HAS_GPERF

#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_BROKEN_DGRAM_SENDV

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_LACKS_PLACEMENT_OPERATOR_DELETE
#define ACE_PI_CONTROL_BLOCK_ALIGN_LONGS 2

// Enables use of POSIX termios struct
#define ACE_USES_NEW_TERMIOS

// IRIX 6.5 supports AIO
#define ACE_HAS_AIO_CALLS
#define ACE_POSIX_AIOCB_PROACTOR

#endif /* ACE_CONFIG_IRIX6X_COMMON_H */
