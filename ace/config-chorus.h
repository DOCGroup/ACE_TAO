/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Chorus
// platforms using one of these compilers:
//   * GNU g++
//   * GreenHills
// It uses the Chorus POSIX threads interface.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

// Define CHORUS to the correct version number
#ifdef CHORUS_4
# define CHORUS 4.0
#else
# define CHORUS 3.1b
#endif

#if defined (linux)
  // This shouldn't be necessary.
# undef linux
#endif /* linux */

// Compiler-specific configuration.

#if defined (__GNUG__)
# include "ace/config-g++-common.h"
# undef ACE_HAS_ANSI_CASTS
# define ACE_LACKS_CHAR_STAR_RIGHT_SHIFTS
#elif defined (ghs)
# define ACE_CONFIG_INCLUDE_GHS_COMMON
# include "ace/config-ghs-common.h"

# define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
# define ACE_HAS_TANDEM_SIGNALS
# define ACE_HAS_TEMPLATE_SPECIALIZATION
# define ACE_LACKS_ACE_IOSTREAM  /* MVME lacks signed and unsigned char */
# define ACE_LACKS_FLOATING_POINT
#else  /* ! __GNUG__ && ! ghs */
# error unsupported compiler for ACE on Chorus
#endif /* ! __GNUG__ && ! ghs */

// OS-specific configuration

// Chorus cannot grow shared memory, so this is the default size for a
// local name space
#define ACE_CHORUS_LOCAL_NAME_SPACE_T_SIZE 128000
// Used in OS.i to map an actor id into a KnCap.
#define ACE_CHORUS_MAX_ACTORS 64

// This is not needed when compiling for PPC.  It is necessary
// for certain methods in ACE.cpp.
#if defined(sparc) && !defined(__unix)
# define __unix
#endif

#if defined(CHORUS_4)
# define ACE_CHORUS_DEFAULT_MIN_STACK_SIZE 0x8000
# define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
# define ACE_HAS_TIMEZONE_GETTIMEOFDAY
#else
# define ACE_CHORUS_DEFAULT_MIN_STACK_SIZE 0x2000
# define ACE_LACKS_KEY_T
# define ACE_LACKS_ACCESS
# define ACE_LACKS_FSYNC
# define ACE_LACKS_GETSERVBYNAME
# define ACE_LACKS_MKFIFO
# define ACE_LACKS_READLINK
# define ACE_LACKS_TRUNCATE
# define ACE_LACKS_LONGLONG_T
#endif

#define ACE_HAS_BROKEN_READV
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_DIRENT
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_IP_MULTICAST
#define ACE_HAS_LONG_MAP_FAILED
#define ACE_HAS_MSG
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SSIZE_T
#define ACE_HAS_STRDUP_EMULATION
#define ACE_HAS_STRERROR
#define ACE_HAS_TSS_EMULATION
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_FORK
#define ACE_LACKS_GETHOSTENT
#define ACE_LACKS_GETPGID
#define ACE_LACKS_SETPGID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MEMORY_H
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MSYNC
#define ACE_LACKS_NAMED_POSIX_SEM
#define ACE_LACKS_PARAM_H
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_READV
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SBRK
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SIGSET
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_UNIX_SIGNALS
#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_WRITEV
#define ACE_PAGE_SIZE 4096

// Chorus has syslog, but doesn't have setlogmask()
#define ACE_LACKS_SETLOGMASK

// Yes, we do have threads.
#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif
// And they're even POSIX pthreads
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_LACKS_PTHREAD_CANCEL
#define ACE_LACKS_PTHREAD_CLEANUP
#define ACE_LACKS_PTHREAD_SIGMASK
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK
// Chorus 4.0 supports POSIX Threads 1003.1b/1003.1c
#if !defined(CHORUS_4)
# define ACE_HAS_PTHREAD_PROCESS_ENUM
#endif


#if !defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
# define ACE_USE_RCSID 0
#endif /* #if !defined (ACE_USE_RCSID) */

// Needed to wait for "processes" to exit.
#include <am/await.h>

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
