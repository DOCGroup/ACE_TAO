/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Chorus
// platforms using one of these compilers:
//   * GNU g++
//   * GreenHills
// It uses the Chorus POSIX threads interface. 

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

#if !defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#define CHORUS 3.1b
#if defined (linux)
  // This shouldn't be necessary.
# undef linux
#endif /* linux */

// Compiler-specific configuration.

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
#elif defined (ghs)
# define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
# define ACE_HAS_TANDEM_SIGNALS
# define ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA
# define ACE_HAS_TEMPLATE_SPECIALIZATION
# define ACE_LACKS_ACE_IOSTREAM  /* MVME lacks signed and unsigned char */
# define ACE_LACKS_FLOATING_POINT
#else  /* ! __GNUG__ && ! ghs */
# error unsupported compiler for ACE on Chorus
#endif /* ! __GNUG__ && ! ghs */

// OS-specific configuration

# define ACE_NEEDS_WRITEV
# define ACE_NEEDS_READV
# define ACE_HAS_BROKEN_READV

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

#define ACE_LACKS_UNIX_SIGNALS
#define ACE_HAS_IP_MULTICAST
#define ACE_CHORUS_DEFAULT_MIN_STACK_SIZE 0x2000
#define ACE_LACKS_MPROTECT
//#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_ACCESS
#define ACE_LACKS_SBRK
#define ACE_LACKS_GETHOSTENT
#define ACE_LACKS_GETSERVBYNAME
#define ACE_LACKS_GETPGID
#define ACE_LACKS_LONGLONG_T
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_KEY_T
#define ACE_LACKS_EXEC
#define ACE_LACKS_FORK
#define ACE_LACKS_SIGSET
// #define ACE_LACKS_FILELOCKS
#define ACE_LACKS_RLIMIT

// got struct timespec
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_CLOCK_GETTIME
#define ACE_LACKS_STRRECVFD

#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_LONG_MAP_FAILED

// Platforms lacks UNIX domain sockets.
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_UTSNAME_T
#define ACE_HAS_PTHREADS_1003_DOT_1C
//#define ACE_LACKS_SETSCHED

#define ACE_LACKS_MSYNC
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MEMORY_H
#define ACE_LACKS_PARAM_H

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform supports strerror ().
#define ACE_HAS_STRERROR

// Defines the page size of the system.
#define ACE_PAGE_SIZE 4096

// Turns off the tracing feature.
#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// Chorus defines struct msghdr in posix/sys/socket.h
#define ACE_HAS_MSG

// Yes, we do have threads.
#define ACE_HAS_THREADS
#define ACE_HAS_POSIX_SEM
#define ACE_LACKS_NAMED_POSIX_SEM
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif
// And they're even POSIX pthreads
#define ACE_HAS_PTHREADS
#define ACE_LACKS_PTHREAD_CLEANUP
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
#define ACE_LACKS_RWLOCK_T
#define ACE_HAS_SIGWAIT
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_SEMBUF_T

#endif /* ACE_CONFIG_H */
