/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// 5.2/5.3 platforms using one of these compilers:
// 1) The GNU/Cygnus g++ compiler that is shipped with Tornado 1.0.1.
// 2) The Green Hills 1.8.8 (not 1.8.7!!!!) and 1.8.9 compilers.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

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

  // On g++/VxWorks, iostream.h defines a static instance (yes, instance)
  // of the Iostream_init class.  That causes all files that #include it
  // to put in the global constructor/destructor hooks.  For files that
  // don't have any static instances of non-class (built-in) types, the
  // hooks refer to the file name, e.g., "foo.cpp".  That file name gets
  // embedded in a variable name by munch.  The output from munch won't
  // compile, though, because of the period!  So, let g++/VxWorks users
  // include iostream.h only where they need it.
# define ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION
#elif defined (ghs)
  // Processor type, if necessary.  Green Hills defines "ppc".
# if defined (ppc)
#   define ACE_HAS_POWERPC_TIMER
# endif /* ppc */

# if defined (i386) || defined (__i386__)
    // If running an Intel, assume that it's a Pentium so that
    // ACE_OS::gethrtime () can use the RDTSC instruction.  If
    // running a 486 or lower, be sure to comment this out.
    // (If not running an Intel CPU, this #define will not be seen
    //  because of the i386 protection, so it can be ignored.)
#   define ACE_HAS_PENTIUM
# endif /* i386 */

# if defined (__STANDARD_CXX)
   // Green Hills 1.8.9, but not 1.8.8.
#   define ACE_HAS_STANDARD_CPP_LIBRARY 1
# endif /* __STANDARD_CXX */

# define ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA
# define ACE_HAS_WCHAR_TYPEDEFS_CHAR
# define ACE_LACKS_LONGLONG_T
# define ACE_LACKS_UNISTD_H
#else  /* ! __GNUG__ && ! ghs */
# error unsupported compiler on VxWorks
#endif /* ! __GNUG__ && ! ghs */

// OS-specific configuration

#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 32768
#define ACE_DEFAULT_THREAD_KEYS 16
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
#define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_RECURSIVE_MUTEXES
#define ACE_HAS_SIGINFO_T
#define ACE_HAS_SIGWAIT
#define ACE_HAS_SIG_ATOMIC_T
#define ACE_HAS_STRERROR
#define ACE_HAS_THREADS
#define ACE_HAS_TSS_EMULATION
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
#define ACE_LACKS_LINEBUFFERED_STREAMBUF
#define ACE_LACKS_LSTAT
#define ACE_LACKS_MADVISE
#define ACE_LACKS_MALLOC_H
#define ACE_LACKS_MEMORY_H
#define ACE_LACKS_MKTEMP
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
#define ACE_LACKS_SIGNED_CHAR
#define ACE_LACKS_SI_ADDR
#define ACE_LACKS_SOCKETPAIR
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_SYSCALL
#define ACE_LACKS_SYSV_MSG_H
#define ACE_LACKS_SYSV_SHMEM
#define ACE_LACKS_SYS_NERR
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_TIMESPEC_T
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_UNIX_SIGNALS
#define ACE_LACKS_UTSNAME_T
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif
#define ACE_LACKS_SYSTIME_H
#define ACE_PAGE_SIZE 4096
#define ACE_THR_PRI_FIFO_DEF 101

#if !defined (ACE_NTRACE)
# define ACE_NTRACE 1
#endif /* ACE_NTRACE */

#define ACE_HAS_STRDUP_EMULATION
#define ACE_LACKS_MKFIFO

// By default, don't include RCS Id strings in object code.
#if !defined (ACE_USE_RCSID)
#define ACE_USE_RCSID 0
#endif /* #if !defined (ACE_USE_RCSID) */

#endif /* ACE_CONFIG_H */
