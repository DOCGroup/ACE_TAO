// -*- C++ -*-

#ifndef ACE_CONFIG_INTEGRITYSCA_H
#define ACE_CONFIG_INTEGRITYSCA_H

/*
 * This config.h file is for version 11.4.x of the
 * INTEGRITY RTOS with POSIX APIs from Green Hills Software
 * http://www.ghs.com/products/rtos/integrity.html
 *
 */

// This config uses the POSIX calls provided by libraries in INTEGRITY.
// All network utility and socket functions from the GHnet stack
// are also assumed to be available.

#define ACE_INTEGRITY

/* compilation defines */
#define ACE_LACKS_GETPGID
#define ACE_LACKS_SETREUID
#define ACE_LACKS_SETREGID
#define ACE_LACKS_PWD_FUNCTIONS
#define ACE_LACKS_ALPHASORT

#ifndef ACE_HAS_EXCEPTIONS
# define ACE_HAS_EXCEPTIONS
#endif
#define ACE_MT_SAFE 1

#define ACE_HAS_SHM_OPEN

/***** Operating System Defines *****/

/***** ANSI defines *****/
#define ACE_LACKS_TEMPNAM /* believe it or not, this is ANSI C */
#define ACE_HAS_STRERROR
#define ACE_LACKS_SENDMSG

/***** End Stack Defines *****/


/* SCA STUFF */
#if defined(INTEGRITY_VERSION) && (INTEGRITY_VERSION >= 40108)
# define ACE_HAS_SIG_ATOMIC_T
#endif /* INTEGRITY_VERSION */

#define ACE_HAS_SIGINFO_T
#define ACE_LACKS_SIGINFO_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_HAS_SIG_C_FUNC
#define ACE_LACKS_SI_ADDR
#define ACE_HAS_AIO_CALLS

#define ACE_HAS_POSIX_NONBLOCK
#define ACE_HAS_DIRENT

#define ACE_HAS_THREADS
#define ACE_HAS_PTHREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
/***** End Threading Defines *****/

/***** Hardware Defines *****/
#define ACE_PAGE_SIZE 4096
/***** End Hardware Defines *****/

/****** SYSV_IPC STUFF *****/
#define ACE_LACKS_KEY_T

#define ACE_HAS_WCHAR
#define ACE_LACKS_ISCTYPE
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSNICMP
#define ACE_LACKS_WCSDUP
#define ACE_LACKS_WCSTOK
#define ACE_LACKS_ITOW
#define ACE_LACKS_STRPTIME
#define ACE_LACKS_SYSTEM

/****** Posix Defines *****/
#define ACE_LACKS_SUSECONDS_T
#define ACE_LACKS_USECONDS_T
#define ACE_LACKS_WAIT
#define ACE_LACKS_WAITPID
#define ACE_HAS_POSIX_TIME
#define ACE_HAS_POSIX_SEM
#define ACE_HAS_STRDUP_EMULATION
#define ACE_HAS_MSG
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_EXEC
#define ACE_LACKS_FORK
#define ACE_LACKS_MKFIFO
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_MKSTEMP
#define ACE_LACKS_MPROTECT
#define ACE_LACKS_MUTEXATTR_PSHARED
#define ACE_LACKS_PIPE
#define ACE_LACKS_RLIMIT
#define ACE_LACKS_RWLOCK_T
#define ACE_LACKS_SEMBUF_T
#define ACE_LACKS_UNIX_DOMAIN_SOCKETS
#define ACE_LACKS_USER
#define ACE_LACKS_FILE_FCNTL
#define ACE_LACKS_SHARED_MEMORY
#define ACE_LACKS_UMASK
#define ACE_LACKS_SEEK
#define ACE_LACKS_IPC_H
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
#define ACE_LACKS_SETGID
#define ACE_LACKS_SYS_PARAM_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_ISATTY
#define ACE_LACKS_GETOPT
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_TRUNCATE
#define ACE_LACKS_PUTENV
#define ACE_HAS_4_4BSD_SENDMSG_RECVMSG
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CLOCK_SETTIME
#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_WRITEV
#define ACE_LACKS_MALLOC_H /* netbsd's just includes stdlib.h */
#define ACE_LACKS_READV
#define ACE_LACKS_MSYNC
/* below refers to fcntl style locking */
#define ACE_LACKS_FILELOCKS

#define ACE_LACKS_REALPATH
#define ACE_LACKS_SWAB
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

/***** Not tied to standards AFAIK ****/
#define ACE_LACKS_MADVISE /* paging optimization not needed with INTEGRITY */
#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_SEARCH_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_SHM_H

#include <INTEGRITY.h>

typedef void (*__sighandler_t)(int);

#ifdef ppc
#define ACE_HAS_POWERPC_TIMER
#endif

/* MIKEC Addtions */
#define ACE_HAS_NONCONST_SELECT_TIMEVAL
#define ACE_LACKS_READLINK
#define ACE_LACKS_GETPPID
#define NSIG (SIGRTMAX+1)
#define ACE_LACKS_CUSERID
#define ACE_NEEDS_HUGE_THREAD_STACKSIZE 0x5000
#define ACE_DEFAULT_THREAD_PRIORITY 127
#define PRI_FIFO_MIN 1
#define PRI_FIFO_MAX 127
#define ACE_THR_PRI_FIFO_DEF 127
#define PRI_RR_MIN 1
#define PRI_RR_MAX 127
#define ACE_THR_PRI_RR_DEF 127
#define PRI_OTHER_MIN 1
#define PRI_OTHER_MAX 127
#define ACE_THR_PRI_OTHER_DEF 127
#undef ACE_LACKS_UNLINK

#define ACE_HAS_TIMED_MESSAGE_BLOCKS

#define ACE_HAS_VOIDPTR_GETTIMEOFDAY
#define ACE_LACKS_UNIX_SYSLOG
#define ACE_LACKS_TELLDIR
#define ACE_LACKS_SEEKDIR
#define ACE_LACKS_GETHOSTENT
/* end MIKEC Addtions */

#endif /* ACE_CONFIG_INTEGRITYSCA_H */
