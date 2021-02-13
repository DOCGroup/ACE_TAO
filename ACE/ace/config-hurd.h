// The following configuration file is designed to work for GNU/Hurd
// platforms using GNU C++.
#ifndef ACE_CONFIG_HURD_H
#define ACE_CONFIG_HURD_H
#include /**/ "ace/pre.h"

#if !defined (ACE_MT_SAFE)
#  define ACE_MT_SAFE 1
#endif

#define ACE_PLATFORM_CONFIG config-hurd.h
#define ACE_LACKS_SYS_SYSCTL_H

/*
? #define ACE_USES_FIFO_SEM
          ACE_HAS_IP_MULTICAST
          ACE_LACKS_PERFECT_MULTICAST_FILTERING
*/

// Needed to differentiate between libc 5 and libc 6 (aka glibc).
#include <features.h>

#if (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500)
#  define ACE_HAS_PTHREADS_UNIX98_EXT
#endif /* _XOPEN_SOURCE - 0 >= 500 */

# include "ace/config-posix.h"

// AIO support pulls in the rt library, which pulls in the pthread
// library.  Disable AIO in single-threaded builds.
#if defined (ACE_HAS_THREADS)
#  define ACE_HAS_CLOCK_GETTIME
#  define ACE_HAS_CLOCK_SETTIME
#else
#  undef ACE_HAS_AIO_CALLS
#endif

// Then glibc/libc5 specific parts

#if defined(__GLIBC__)
# define ACE_HAS_SOCKLEN_T
# define ACE_HAS_4_4BSD_SENDMSG_RECVMSG

  // glibc defines both of these, used in OS_String.
# if defined (_GNU_SOURCE)
#   define ACE_HAS_STRNLEN
#   define ACE_HAS_WCSNLEN
# endif

  // To avoid the strangeness with Linux's ::select (), which modifies
  // its timeout argument, use ::poll () instead.
# define ACE_HAS_POLL

# define ACE_HAS_SIGINFO_T
# define ACE_LACKS_SIGINFO_H
# define ACE_HAS_UCONTEXT_T
# define ACE_HAS_SIGTIMEDWAIT

#endif /* __GLIBC__ */

#define ACE_HAS_LSEEK64
//#define ACE_LACKS_LSEEK64_PROTOTYPE

#define ACE_HAS_P_READ_WRITE
// Use ACE's alternate cuserid() implementation since the use of the
// system cuserid() is discouraged.
#define ACE_HAS_ALT_CUSERID

#if (__GLIBC__  > 2)  || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 3)
# define ACE_HAS_ISASTREAM_PROTOTYPE
# define ACE_HAS_PTHREAD_SIGMASK_PROTOTYPE
# define ACE_HAS_CPU_SET_T
#endif /* __GLIBC__ > 2 || __GLIBC__ === 2 && __GLIBC_MINOR__ >= 3) */

#if (__GLIBC__  > 2)  || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 30)
#  define ACE_LACKS_SYS_SYSCTL_H
#endif /* __GLIBC__ > 2 || __GLIBC__ === 2 && __GLIBC_MINOR__ >= 30) */

// Then the compiler specific parts

#if defined (__GNUG__)
  // config-g++-common.h undef's ACE_HAS_STRING_CLASS with -frepo, so
  // this must appear before its #include.
# define ACE_HAS_STRING_CLASS
# include "ace/config-g++-common.h"
#elif defined (__GNUC__)
/**
 * GNU C compiler.
 *
 * We need to recognize the GNU C compiler since TAO has at least one
 * C source header and file
 * (TAO/orbsvcs/orbsvcs/SSLIOP/params_dup.{h,c}) that may indirectly
 * include this
 */
#else  /* ! __GNUG__ */
#  ifdef __cplusplus  /* Let it slide for C compilers. */
#    error unsupported compiler in ace/config-hurd.h
#  endif  /* __cplusplus */
#endif /* ! __GNUG__*/

// Completely common part :-)

// Platform/compiler has the sigwait(2) prototype
#define ACE_HAS_SIGWAIT

#define ACE_HAS_SIGSUSPEND

#define ACE_HAS_UALARM

#define ACE_HAS_STRSIGNAL

#define ACE_HAS_XPG4_MULTIBYTE_CHAR
#define ACE_HAS_VFWPRINTF

#define ACE_LACKS_ITOW
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSNICMP
#define ACE_LACKS_ISWASCII

#define ACE_HAS_3_PARAM_WCSTOK

#if !defined (ACE_DEFAULT_BASE_ADDR)
#  define ACE_DEFAULT_BASE_ADDR ((char *) 0x80000000)
#endif /* ! ACE_DEFAULT_BASE_ADDR */

#define ACE_HAS_ALLOCA

// Compiler/platform has <alloca.h>
#define ACE_HAS_ALLOCA_H
#define ACE_HAS_SYS_SYSINFO_H

// Compiler/platform has the getrusage() system call.
#define ACE_HAS_GETRUSAGE
#define ACE_HAS_GETRUSAGE_PROTOTYPE

#define ACE_HAS_BYTESWAP_H
#define ACE_HAS_BSWAP_16
#define ACE_HAS_BSWAP_32

#if defined (__GNUC__)
#  define ACE_HAS_BSWAP_64
#endif

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// ONLY define this if you have config'd multicast into a 2.0.34 or
// prior kernel.  It is enabled by default in 2.0.35 kernels.
#if !defined (ACE_HAS_IP_MULTICAST)
# define ACE_HAS_IP_MULTICAST
#endif /* ! ACE_HAS_IP_MULTICAST */

// At least for IPv4, Linux lacks perfect filtering.
#if !defined ACE_LACKS_PERFECT_MULTICAST_FILTERING
# define ACE_LACKS_PERFECT_MULTICAST_FILTERING 1
#endif /* ACE_LACKS_PERFECT_MULTICAST_FILTERING */

#define ACE_HAS_BIG_FD_SET

// Linux defines struct msghdr in /usr/include/socket.h
#define ACE_HAS_MSG

// Linux "improved" the interface to select() so that it modifies
// the struct timeval to reflect the amount of time not slept
// (see NOTES in Linux's select(2) man page).
#define ACE_HAS_NONCONST_SELECT_TIMEVAL

#define ACE_DEFAULT_MAX_SOCKET_BUFSIZ 65535

#define ACE_CDR_IMPLEMENT_WITH_NATIVE_DOUBLE 1

#define ACE_HAS_GETPAGESIZE 1

// Platform defines struct timespec but not timespec_t
#define ACE_LACKS_TIMESPEC_T

// A conflict appears when including both <ucontext.h> and
// <sys/procfs.h> with recent glibc headers.
//#define ACE_HAS_PROC_FS

// Platform supports System V IPC (most versions of UNIX, but not Win32)
#define ACE_HAS_SYSV_IPC

// Compiler/platform contains the <sys/syscall.h> file.
#define ACE_HAS_SYS_SYSCALL_H

// Platform/compiler supports global timezone variable.
#define ACE_HAS_TIMEZONE

#define ACE_HAS_TIMEZONE_GETTIMEOFDAY

// Compiler supports the ssize_t typedef.
#define ACE_HAS_SSIZE_T

// Compiler/platform defines the sig_atomic_t typedef.
#define ACE_HAS_SIG_ATOMIC_T

// Compiler/platform defines a union semun for SysV shared memory.
#define ACE_HAS_SEMUN

#define ACE_HAS_POSIX_TIME

#define ACE_HAS_GPERF

#define ACE_HAS_DIRENT

#define ACE_SIZEOF_WCHAR 4

#define ACE_LACKS_GETIPNODEBYADDR
#define ACE_LACKS_GETIPNODEBYNAME
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_STRRECVFD

// Platform has POSIX terminal interface.
#define ACE_HAS_TERMIOS

// Linux implements sendfile().
#define ACE_HAS_SENDFILE 1

#define ACE_HAS_VOIDPTR_MMAP

#define ACE_HAS_ICMP_SUPPORT 1

#define ACE_HAS_VASPRINTF

// According to man pages Linux uses different (compared to UNIX systems) types
// for setting IP_MULTICAST_TTL and IPV6_MULTICAST_LOOP / IP_MULTICAST_LOOP
// in setsockopt/getsockopt.
#define ACE_HAS_IP_MULTICAST_TTL_AS_INT 1
#define ACE_HAS_IPV6_MULTICAST_LOOP_AS_BOOL 1
#define ACE_HAS_IP_MULTICAST_LOOP_AS_INT 1

#define ACE_HAS_GETIFADDRS

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE
#define ACE_HAS_RECURSIVE_MUTEXES
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#define ACE_HAS_REENTRANT_FUNCTIONS

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_HURD_H */
