/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// 5.2/5.3 platforms using one of these compilers:
// 1) The GNU/Cygnus g++ compiler that is shipped with Tornado 1.0.1.
// 2) The Green Hills 1.8.8 (not 1.8.7!!!!) and 1.8.9 compilers.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#if ! defined (VXWORKS)
# define VXWORKS
#endif /* ! VXWORKS */

#if ! defined (__ACE_INLINE__)
# define __ACE_INLINE__
#endif /* ! __ACE_INLINE__ */

// Compiler-specific configuration.
#if defined (__GNUG__)
# include "ace/config-g++-common.h"
# undef ACE_HAS_TEMPLATE_SPECIALIZATION

# define ACE_LACKS_IOSTREAM_FX
# if !defined (ACE_MAIN)
#   define ACE_MAIN ace_main
# endif /* ! ACE_MAIN */

  // Even though the documentation suggests that g++/VxWorks 5.3.1
  // (Tornado 1.0.1) supports long long, Wind River tech support says
  // that it doesn't.  It causes undefined symbols for math functions.
# define ACE_LACKS_LONGLONG_T

# define ACE_LACKS_LINEBUFFERED_STREAMBUF
# define ACE_LACKS_SIGNED_CHAR

# if defined (ACE_VXWORKS) && ACE_VXWORKS >= 0x542
#   define ACE_LACKS_CLEARERR
# endif /* ACE_VXWORKS >= 0x542 */

#elif defined (ghs)
  // Processor type, if necessary.  Green Hills defines "ppc".
# if defined (ppc)
#   define ACE_HAS_POWERPC_TIMER
#   define ACE_LACKS_CLEARERR
# elif defined (i386) || defined (__i386__)
    // If running an Intel, assume that it's a Pentium so that
    // ACE_OS::gethrtime () can use the RDTSC instruction.  If
    // running a 486 or lower, be sure to comment this out.
    // (If not running an Intel CPU, this #define will not be seen
    //  because of the i386 protection, so it can be ignored.)
#   define ACE_HAS_PENTIUM
# endif /* ppc || i386 */

# define ACE_CONFIG_INCLUDE_GHS_COMMON
# include "ace/config-ghs-common.h"

# define ACE_LACKS_UNISTD_H
# define ACE_LACKS_IOSTREAM_TOTALLY

// Short-circuit the include of <arpa/inet.h>
// Green Hills has a problem with multiply defined functions
// with different parameters.
# define __INCineth

#elif defined (__DCPLUSPLUS__)
  // Diab 4.2a or later.
# if !defined (ACE_LACKS_PRAGMA_ONCE)
    // We define it with a -D with make depend.
#   define ACE_LACKS_PRAGMA_ONCE
# endif /* ! ACE_LACKS_PRAGMA_ONCE */

  // Diab doesn't support VxWorks' iostream libraries.
# define ACE_LACKS_IOSTREAM_TOTALLY

  // #include <new.h> causes strange compilation errors in
  // the system header files.
# define ACE_LACKS_NEW_H

#else  /* ! __GNUG__ && ! ghs */
# error unsupported compiler on VxWorks
#endif /* ! __GNUG__ && ! ghs */

// OS-specific configuration

#define ACE_LACKS_UNIX_SYSLOG
#define ACE_HAS_MUTEX_TIMEOUTS
#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 32768
#define ACE_DEFAULT_THREAD_KEYS 16
#define ACE_HAS_BROKEN_ACCEPT_ADDR
#define ACE_HAS_BROKEN_SENDMSG
#define ACE_HAS_BROKEN_WRITEV
#define ACE_HAS_CHARPTR_SOCKOPT
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_DIRENT
#define ACE_HAS_DLL 0
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT
#define ACE_HAS_MSG
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_RECURSIVE_MUTEXES
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_STRDUP_EMULATION
#define ACE_HAS_STRERROR
#define ACE_HAS_THREADS
#define ACE_HAS_TSS_EMULATION
#define ACE_HAS_STRPTIME
#define ACE_LACKS_ACCESS
#define ACE_LACKS_COND_T
#define ACE_LACKS_EXEC
#define ACE_LACKS_FCNTL
#define ACE_LACKS_FILELOCKS
#define ACE_LACKS_FORK
#define ACE_LACKS_FSYNC
#define ACE_LACKS_GETHOSTENT
#define ACE_LACKS_GETSERVBYNAME
#define ACE_LACKS_KEY_T
#define ACE_LACKS_LSTAT
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MEMORY_H
#define ACE_LACKS_MKFIFO
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MKSTEMP
#define ACE_LACKS_MMAP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MSYNC
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_PARAM_H
#define ACE_LACKS_PWD_FUNCTIONS
#define ACE_LACKS_READDIR_R
#define ACE_LACKS_READLINK
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SBRK
#define ACE_LACKS_SEEKDIR
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_SYSTIME_H
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_SYS_NERR
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_TEMPNAM
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_UNIX_SIGNALS
#define ACE_LACKS_UTSNAME_T
#define ACE_LACKS_NATIVE_STRPTIME
#define ACE_PAGE_SIZE 4096
#define ACE_THR_PRI_FIFO_DEF 101
#define ACE_THR_PRI_OTHER_DEF ACE_THR_PRI_FIFO_DEF
#define ACE_HAS_SIGTIMEDWAIT
#define ACE_HAS_SIGSUSPEND
#if !defined (ACE_VXWORKS_SPARE)
# define ACE_VXWORKS_SPARE spare4
#endif /* ! ACE_VXWORKS_SPARE */

#define ACE_LACKS_STDINT_H
#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_UNISTD_H
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_SYS_TIME_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_SYS_UIO_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_MSG_H

// Not sure if these should always be defined.
#define ACE_LACKS_SYS_UN_H
#define ACE_LACKS_PTHREAD_H

// Some string things
#define ACE_LACKS_WCSCAT
#define ACE_LACKS_WCSCHR
#define ACE_LACKS_WCSCMP
#define ACE_LACKS_WCSCPY
#define ACE_LACKS_WCSCSPN
#define ACE_LACKS_WCSLEN
#define ACE_LACKS_WCSNCAT
#define ACE_LACKS_WCSNCMP
#define ACE_LACKS_WCSNCPY
#define ACE_LACKS_WCSPBRK
#define ACE_LACKS_WCSRCHR
#define ACE_LACKS_WCSSPN
#define ACE_LACKS_WCSSTR
#define ACE_LACKS_WCSTOK
#define ACE_LACKS_TOWLOWER
#define ACE_LACKS_ITOW
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSNICMP
#define ACE_LACKS_WCSTOD
#define ACE_LACKS_WCSTOL
#define ACE_LACKS_WCSTOUL
#define ACE_LACKS_WCSDUP

#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif

#if !defined (ACE_NEEDS_HUGE_THREAD_STACKSIZE)
# define ACE_NEEDS_HUGE_THREAD_STACKSIZE 65536
#endif /* ACE_NEEDS_HUGE_THREAD_STACKSIZE */

#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
#define ACE_USE_RCSID 0
#endif /* #if !defined (ACE_USE_RCSID) */

#include "ace/post.h"
#endif /* ACE_CONFIG_H */
